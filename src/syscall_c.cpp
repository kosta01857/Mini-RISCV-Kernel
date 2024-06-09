//
// Created by kosta on 11.5.23.
//
#include "../h/syscall_c.hpp"
uint64 inline get_res(){
    uint64 ret;
    __asm__ volatile("mv %0,a0":"=r"(ret));
    return ret;
}
void inline ecall(){
    __asm__ volatile("ecall");
}
void inline op_code(uint64 code){
    __asm__ volatile("mv a0,%0"::"r"(code)); //op kod?
}
uint64 inline calc_blocks(size_t size){
    if(size >= MEM_BLOCK_SIZE){
        size = size/MEM_BLOCK_SIZE;
        size+=2;
    }
    else{
        size = 2;
    }
    return size;
}
void* mem_alloc(size_t size){
    size = calc_blocks(size);
    //prenesi argumente
    __asm__ volatile("mv a1,%0"::"r"(size)); //broj bajtova
    op_code(1);
    ecall();
    //upisi arg u promenljivu
    uint64 rez = get_res();
    if(rez == 0) return nullptr;
    return (void*)rez;
}
int mem_free(void*){
    //prenesi argumente
    __asm__ volatile("add a1,a0,x0"); //pointer
    op_code(2);
    ecall();
    uint64 rez = get_res();
    return (int)rez;
}
int thread_create(PCB** handle, void(*start_routine)(void*), void* args){//start routine je f-ja
    __asm__ ("mv a6,a0");
    __asm__ ("mv a2,%0":"=r"(args));
    __asm__ ("mv a1,%0":"=r"(start_routine));
    op_code(11);
    ecall();
    uint64 rez = get_res();
    return (int)rez;
}

void thread_dispatch(){
    op_code(13);
    ecall();
}
int thread_exit(){
    op_code(12);
    ecall();
    return 0;
}
    void putc(char c){
        __asm__ volatile("add a1,a0,x0"); //char
        op_code(42);
        ecall();
    }
    char getc(){
        op_code(41);
        ecall();
        uint64 rez = get_res();
        return (char)rez;
    }
void thread_join(thread_t handle){
    __asm__ volatile("add a1,a0,x0"); //pcb pointer
    op_code(14);
    ecall();
}


int sem_wait(sem_t id){
    __asm__ volatile("add a1,a0,x0"); //sem pointer
    op_code(23);
    ecall();
    uint64 rez = get_res();
    return (int)rez;
}
int sem_signal(sem_t id){
    __asm__ volatile("add a1,a0,x0"); //sem pointer
    op_code(24);
    ecall();
    uint64 rez = get_res();
    return (int)rez;
}
int sem_open(sem_t* handle,unsigned init){
    __asm__("mv a2,a0");
    __asm__ ("mv a1,%0":"=r"(init));
    op_code(21);
    ecall();
    uint64 rez = get_res();
    return (int)rez;
}
int sem_close(sem_t handle){
    __asm__ volatile("mv a1,a0"); //sem pointer
    op_code(22);
    ecall();
    return 0;
}
int time_sleep(time_t){
    return 0;
}