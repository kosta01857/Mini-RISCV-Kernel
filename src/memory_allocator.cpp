//
// Created by kosta on 14.5.23.
//
#include "../h/memory_allocator.hpp"
memory_allocator memory_allocator::allocator;
memory_allocator:: memory_allocator():free_fragments_head((char*)HEAP_START_ADDR),allocated_fragments_head(nullptr){
        header* curr_head = (header*)free_fragments_head;
        curr_head->next_fragment = nullptr;
        curr_head->prev_fragment = nullptr;
        int cnt = ((char*)HEAP_END_ADDR - (char*)HEAP_START_ADDR + 1)
                /MEM_BLOCK_SIZE;
        curr_head->size = cnt;
    }
void* memory_allocator:: mem_alloc(size_t num_of_blocks) {
    header* curr = (header*) free_fragments_head;
    while (curr) {
        if (curr->size >= num_of_blocks) {
            header* prev = (header *) curr->prev_fragment;
            header* next = (header *) curr->next_fragment;
            if (curr->size > num_of_blocks) {
                //smanji fragment
                header* new_header = (header *) ((char *) curr + MEM_BLOCK_SIZE * num_of_blocks);
                new_header->size = curr->size - num_of_blocks;
                new_header->next_fragment = curr->next_fragment;
                new_header->prev_fragment = curr->prev_fragment;
                if(prev)prev->next_fragment = (char*)new_header;
                if (next)next->prev_fragment = (char *)new_header;
                if ((char *) curr == free_fragments_head) free_fragments_head = (char *) new_header;
            } else {//izbaci fragment skroz
                if (prev)prev->next_fragment = (char *) next;
                if (next)next->prev_fragment = (char *) prev;
                if((char*) curr == free_fragments_head){
                    if(next)free_fragments_head = (char*)next;
                    else free_fragments_head = nullptr;
                }
            }
            //update curr header and insert in alloc list
            curr->size = num_of_blocks;
            curr->next_fragment = allocated_fragments_head;
            curr->prev_fragment = nullptr;
            header *alloc_head = (header *)allocated_fragments_head;
            if(alloc_head)alloc_head->prev_fragment = (char *) curr;
            allocated_fragments_head = (char *) curr;
                return (void*)((char*) curr + sizeof(header));
        }
        curr = (header *) curr->next_fragment;
    }
    return nullptr;
}
int memory_allocator:: free(void* addr){
        header* curr = (header*)allocated_fragments_head;
        addr = (header*)((char*)(addr) - sizeof(header));
        while(curr){
            if((char*)curr == (char*)addr){
                //delete from alloc list
                if((char*)curr==allocated_fragments_head){
                    allocated_fragments_head = curr->next_fragment;
                    header* alloc_head = (header*)allocated_fragments_head;
                    if(alloc_head)alloc_head->prev_fragment = nullptr;
                }
                else{
                    header* next = (header*)curr->next_fragment;
                    header* prev = (header*)curr->prev_fragment;
                    if(next)next->prev_fragment = (char*)prev;
                    prev->next_fragment = (char*)next;
                }
                curr->next_fragment = nullptr;
                curr->prev_fragment = nullptr;
                //insert in free frag list
                header* iter = (header*)free_fragments_head;
                header* prev = nullptr;
                while(iter){
                    if((char*)curr < (char*)iter){
                        if((char*)iter == free_fragments_head){
                            curr->next_fragment = (char*)iter;
                            iter->prev_fragment = (char*)curr;
                            free_fragments_head = (char*)curr;
                        }
                        else{
                            curr->prev_fragment = (char*)prev;
                            curr->next_fragment = (char*)iter;
                            iter->prev_fragment = (char*)curr;
                            prev->next_fragment = (char*)curr;
                        }
                        break;
                    }
                    prev = iter;
                    iter = (header*)iter->next_fragment;
                }
                if(!iter){
                    prev->next_fragment = (char*)curr;
                    curr->prev_fragment = (char*)prev;
                }
                //try to merge
                prev = (header*)curr->prev_fragment;
                if(prev && (char*)prev + prev->size*MEM_BLOCK_SIZE == (char*)curr){
                    prev->next_fragment = curr->next_fragment;
                    header* next = (header*)curr->next_fragment;
                    if(next) next->prev_fragment = (char*)prev;
                    prev->size = prev->size+curr->size;
                    curr = prev;
                }
                header* next = (header*)curr->next_fragment;
                if(next && (char*)curr+curr->size*MEM_BLOCK_SIZE == (char*)next){
                    curr->next_fragment = next->next_fragment;
                    header* new_next = (header*)next->next_fragment;
                    if(new_next)new_next->prev_fragment = (char*)curr;
                    curr->size = curr->size + next->size;
                }
                return 0;
            }
            curr = (header*)curr->next_fragment;
        }return -1;

    }


