#ifndef LAB6_MEM_DEBUG_H
#define LAB6_MEM_DEBUG_H
#define DEBUG_FIRST_BYTES 4

#include <stdio.h>
#include "mem.h"

void memalloc_debug_struct_info( FILE* f, mem const* const address );
void memalloc_debug_heap( FILE* f, mem const* ptr );

#endif //LAB6_MEM_DEBUG_H
