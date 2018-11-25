#ifndef _MEM_H_
#define _MEM_H_
#define _USE_MISC

#include <stddef.h>
#include  <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <stdbool.h>

#define BLOCK_MIN_SIZE 16
#define  HEAP_START  ((void*)0x04040000)

#pragma pack(push, 1)
typedef struct mem  {
    struct mem* next;
    size_t capacity;
    bool is_free;
} mem;
#pragma pack(pop)

void* _malloc( size_t query );

void _free( mem* block);

void* heap_init( size_t initial_size );

#endif