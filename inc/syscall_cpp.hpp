//
// Created by kosta on 9.8.23..
//

#ifndef PROJ_BASE_SYSCALL_CPP_HPP
#define PROJ_BASE_SYSCALL_CPP_HPP
#include "../h/syscall_c.hpp"
#include "../lib/mem.h"
void *operator new(uint64 n);
void *operator new[](uint64 n);
void operator delete(void* p) noexcept;
void operator delete[](void* p) noexcept;
class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread ();
    int start ();
    void join();
    static void dispatch ();
    static int sleep (time_t);
protected:
    Thread ();
    virtual void run ();
private:
    void static run_wrapper(void*);
    thread_t myHandle;
    void (*body)(void*); void* arg;
};
class Semaphore {
public:
    Semaphore (unsigned init = 1);
    virtual ~Semaphore ();
    int wait ();
    int signal ();
private:
    sem_t myHandle;
};
class Console {
    //CXXFLAGS += -fno-use-cxa-atexit
public:
    static char getc();

    static void putc(char);
};
#endif //PROJ_BASE_SYSCALL_CPP_HPP
