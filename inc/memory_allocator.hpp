//
// Created by kosta on 14.5.23..
//
#include "../lib/hw.h"
#ifndef PROJ_BASE_MEMORY_ALLOCATOR_HPP
#define PROJ_BASE_MEMORY_ALLOCATOR_HPP
typedef struct{
    size_t size;
    char* next_fragment;
    char* prev_fragment;
}header;
class memory_allocator{
private:
    char* free_fragments_head;
    char* allocated_fragments_head;
public:
    memory_allocator();
    static memory_allocator allocator;
    void* mem_alloc(size_t num_of_blocks);
    int free(void* addr);
};
#endif //PROJ_BASE_MEMORY_ALLOCATOR_HPP

