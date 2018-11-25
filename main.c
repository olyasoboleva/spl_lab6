#include <stdio.h>
#include "mem_debug.h"

int main() {
    heap_init(1024);

    FILE* f = fopen("heap_states.txt","w+");
    memalloc_debug_heap(f,HEAP_START);
    fprintf(f,"----------\n");

    char* test1 = (char*)_malloc(sizeof(char)*2048);
    char* test2 = (char*)_malloc(sizeof(char)*32);
    char* test3 = (char*)_malloc(sizeof(char)*4);
    memalloc_debug_heap(f,HEAP_START);
    fprintf(f,"----------\n");

    _free(test1);
    _free(test2);
    _free(test3);
    memalloc_debug_heap(f,HEAP_START);
    fprintf(f,"----------\n");
    fclose(f);
    return 0;
}