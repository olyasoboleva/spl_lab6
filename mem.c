#include "mem.h"

void merge_blocks(mem*);
mem* find_free_block(size_t);
mem* map_more_pages(mem*, size_t);
void* _malloc(size_t);
void _free( mem*);
void* heap_init(size_t);

static mem* heap_start = NULL;

void merge_blocks(mem* first_block){
    mem* block = heap_start;
    mem* second_block = first_block->next;
    if (second_block!=NULL && (mem*)((void*)first_block+first_block->capacity+ sizeof(mem)==second_block)){
        if (second_block->is_free) {
            first_block->next = second_block->next;
            first_block->capacity += second_block->capacity + sizeof(mem);
        }
    }

    for (; block->next!=NULL && block->next!=first_block ; block = block->next);
    if (block->next!=NULL && (mem*)((void*)block+block->capacity+ sizeof(mem)==first_block && block->is_free)){
        block->next = first_block->next;
        block->capacity += first_block->capacity + sizeof(mem);
    }
}

mem* find_free_block( size_t query ){
    mem* new_block = NULL;
    mem* block = heap_start;
    mem* last_block;
    while (block!=NULL) {
        if (block->is_free && query < block->capacity) {
            size_t remain_mem = block->capacity - query;
            if (remain_mem >= sizeof(mem) + BLOCK_MIN_SIZE) {
                new_block = (mem *) ((void *) block + sizeof(mem) + query);
                new_block->is_free = true;
                new_block->capacity = remain_mem - sizeof(mem);
                new_block->next = block->next;
                remain_mem = 0;
            }
            block->is_free = false;
            block->capacity = query + remain_mem;
            if (new_block != NULL) {
                block->next = new_block;
            }
            return block;
        }
        last_block = block;
        block = block->next;
    }
    return map_more_pages(last_block, query);
}

mem* map_more_pages(mem* last_block, size_t query){
    bool merge=false;
    mem* heap_end = last_block+ sizeof(mem)+last_block->capacity;
    mem* new_page = (mem*)mmap((void*)heap_end, query, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE | MAP_FIXED, 0, 0);
    if (new_page==MAP_FAILED){
        new_page = (mem*)mmap(NULL, query, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);
        if (new_page==MAP_FAILED) return NULL;
    } else merge = true;
    last_block->next = new_page;
    if (merge) {
        merge_blocks(last_block);
        return last_block;
    }
    new_page->capacity = query;
    new_page->is_free = false;
    new_page->next = NULL;
    return new_page;
}

void* _malloc( size_t query ){
    if (query<BLOCK_MIN_SIZE) query = BLOCK_MIN_SIZE;
    if (heap_start==NULL) heap_init(query);
    mem* free_block = find_free_block(query);

    if (free_block==NULL) return NULL;
    return (void*)(free_block+ sizeof(mem));
}

void  _free( mem* data){
    mem* block = data- sizeof(mem);
    block->is_free = true;
    merge_blocks(block);
}

void* heap_init( size_t initial_size ){
    if (initial_size<sizeof(mem)) return NULL;
    heap_start = (mem*)mmap(HEAP_START, initial_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, 0, 0);
    if (heap_start==NULL) return NULL;

    heap_start->next = NULL;
    heap_start->is_free = true;
    heap_start->capacity = initial_size;

    return heap_start;
}
