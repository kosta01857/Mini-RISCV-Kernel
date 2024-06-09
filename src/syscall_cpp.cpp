//
// Created by kosta on 15.5.23..
//
#include "../h/syscall_cpp.hpp"
void *operator new(uint64 n){
    void*a = mem_alloc(n);
    return a;
}
void *operator new[](uint64 n){
    return mem_alloc(n);
}
void operator delete(void* p) noexcept{
    //memory_allocator::allocator.free(p);
    mem_free(p);
}
void operator delete[](void* p) noexcept{
    //memory_allocator::allocator.free(p);
    mem_free(p);
}
void Console:: putc(char cpp) {
    ::putc(cpp);
}
char Console:: getc() {
    return ::getc();
}
Semaphore::Semaphore (unsigned init){
    myHandle = nullptr;
    sem_open(&myHandle,init);
}
Semaphore:: ~Semaphore (){
    sem_close(myHandle);
}
int Semaphore:: wait (){
    return sem_wait(myHandle);
}
int Semaphore:: signal (){
    return sem_signal(myHandle);
}
Thread:: Thread (void (*body)(void*), void* arg){
    myHandle = nullptr;
    this->body = body;
    this->arg = arg;

}
Thread:: ~Thread (){
    delete myHandle;
}
int Thread:: start (){
    int a = thread_create(&this->myHandle,this->body,this->arg);
    return a;
}
void Thread:: join(){
    thread_join(myHandle);
}
void Thread:: dispatch (){
    thread_dispatch();
}
int Thread:: sleep (time_t){
    return 0;
}
Thread:: Thread (){
    this->body = run_wrapper;
    this->arg = this;
}
void Thread:: run () {
}

void Thread::run_wrapper(void* thr) {
    ((Thread*) thr)->run();
}
