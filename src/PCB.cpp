//
// Created by kosta on 3.8.23.
//
#include "../h/syscall_c.hpp"
#include "../h/PCB.hpp"
#include "../h/riscv.hpp"
#include "../h/list.hpp"
extern "C" void save_context();
extern "C" void restore_context();
void inline unblock_all(){
    if(PCB::running->is_someone_waiting()) {
        List<PCB>::Node *iter = PCB::running->waitList.get_head();
        while (iter != nullptr) {
            PCB* pcb = (iter->data);
            pcb->set_blocked(false);
            Scheduler::put(pcb);
            iter = iter->next;
        }
        PCB::running->set_someone_waiting(false);
    }
}
PCB* PCB:: running = nullptr;
void PCB::set_finished() {
finished = true;
}
bool PCB:: is_finished () {
return this->finished;
}
PCB:: PCB(void (*f)(void* arg), void* a) {
    task = f;
    args = a;
    if (this->task != nullptr)stack_start = new uint64[DEFAULT_STACK_SIZE];
    else stack_start = nullptr;
    finished = false;
    if (this->task != nullptr)context.ra = (uint64) &thread_wrapper;
    else context.ra = 0;
    if (this->task != nullptr) context.sp = (uint64) &stack_start[1024];
    else context.sp = 0;
    if(task != nullptr) Scheduler::put(this);

}
void PCB::yield(){ //menja nit uz cuvanje konteksta
    save_context(); //save current context
    change_context();
    restore_context();//restore new context
}
void PCB::change_context() { //menja kontekst procesora
    PCB* old = running;
    PCB* curr;
    uint64 t0;
    __asm__ ("mv %0,ra":"=r"(t0));
    old->context.ra = t0;
    __asm__ ("mv %0,sp":"=r"(t0));
    old->context.sp = t0;
    if(!old->is_finished() && !old->is_blocked()){
        Scheduler::put(old);
    }
    running = Scheduler::get();
    curr = running;
    //context switch
    t0 = running->context.sp;
    __asm__ ("mv sp,%0"::"r"(t0));
    t0 = curr->context.ra;
    __asm__ ("mv ra,%0"::"r"(t0));
    __asm__ volatile("sd ra,24(sp)");//podmetnem svoj ra na stek,ne radi bez ovoga
}
void PCB::thread_wrapper() {
    Riscv::popSppSpie();
    if(running->task != nullptr) {
        running->task(running->args);
    }
    unblock_all();
    thread_exit();
}

int PCB::create_thread(PCB** handle,void (*f)(void *), void *a){
    if(handle == nullptr) return -1;
    *handle = new PCB(f, a);
    return 0;
}

void PCB::die() {
    this->set_finished();
    thread_dispatch();
}
void PCB::add_to_waitList(PCB* pcb) {
    this->waitList.addLast(pcb);
    pcb->set_blocked(true);
    this->set_someone_waiting(true);
    thread_dispatch();
}

PCB::~PCB() {
    delete[] stack_start;
}
