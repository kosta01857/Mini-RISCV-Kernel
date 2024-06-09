//
// Created by kosta on 11.8.23.
//
#include "../h/semafor.hpp"

void semafor::unblock() {
        PCB* pcb = blocked_list.removeFirst();
        pcb->set_blocked(false);
        Scheduler::put(pcb);
}

void semafor::block() {
    blocked_list.addLast(PCB::running);
    PCB::running->set_blocked(true);
    PCB::yield();
}

int semafor::signal() {
    if(closed) return -1;
    inc_val();
    if(val<=0)unblock();
    return 0;
}

int semafor::wait() {
    if(closed) return -1;
    dec_val();
    if(val < 0)block();
    if (closed == true) return -1;
    return 0;
}

semafor::semafor(unsigned int init) {
    this->val = (int)init;
}

int semafor::create_sem(semafor** handle,unsigned int init) {
        if (handle == nullptr) return - 1;
        *handle =  new semafor(init);
        return 0;
}

void semafor::close() {
    if(closed)return;
    List<PCB>::Node* curr = blocked_list.get_head();
    while(curr != nullptr){
        PCB* pcb = curr->data;
        pcb->set_blocked(false);
        Scheduler::put(pcb);
        curr = curr->next;
    }
    closed = true;
    delete this;
}

