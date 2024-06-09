//
// Created by kosta on 11.5.23..
//

#ifndef PROJ_BASE_SYSCALL_C_HPP
#define PROJ_BASE_SYSCALL_C_HPP


#include "../h/PCB.hpp"
class sem;
typedef PCB* thread_t;
typedef unsigned long size_t;
typedef unsigned long time_t;
typedef sem* sem_t;
void* mem_alloc(size_t size);
int mem_free(void*);
int thread_create(thread_t* handle,void(*)(void*),void* arg);
int thread_exit();
void thread_dispatch();
void thread_join(thread_t handle);
int sem_open(sem_t* handle,unsigned init);
int sem_close(sem_t handle);
int sem_wait(sem_t id);
int sem_signal(sem_t id);
int time_sleep(time_t);
void putc(char c);
char getc();
#endif //PROJ_BASE_SYSCALL_C_HPP