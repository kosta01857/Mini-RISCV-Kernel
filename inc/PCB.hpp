//
// Created by kosta on 3.8.23.
//

#ifndef PROJ_BASE_PCB_HPP
#define PROJ_BASE_PCB_HPP


#include "../lib/hw.h"
#include "../h/scheduler.hpp"
class PCB{
public:
    void die();
    static void thread_wrapper();
    ~PCB();
    static int create_thread(PCB** handle,void (*f)(void*), void* a);
    bool is_finished ();
    void set_finished();
    static void yield();
    static void change_context();
    static PCB* running;
    using Task = void(*)(void*); // pokazivac na funkc koju nit izvrsava
    explicit PCB(void (*f)(void* arg), void* a);
    void add_to_waitList(PCB*);
    List<PCB> waitList;
    bool is_blocked() const{
        return im_blocked;
    }
    void set_blocked(bool b){
        im_blocked = b;
    }
    bool is_someone_waiting() const{
        return some_waiting;
    }
    void set_someone_waiting(bool b){
        some_waiting = b;
    }
private:
    bool im_blocked = false;
    bool some_waiting = false;
    Task task;
    void* args;
    uint64* stack_start; //pocetak steka niti
    bool finished;
    struct Context{
        uint64 sp; //gde je trenutan stack pointer
        uint64 ra; // na koju adr se vraca nit kada dobije procesor
        //svi ostali reg se cuvaju na steku
    };
    Context context;
};
#endif //PROJ_BASE_PCB_HPP