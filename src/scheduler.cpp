//
// Created by kosta on 2.8.23.
//
#include "../h/scheduler.hpp"
List<PCB> Scheduler::ReadyList;
void Scheduler::put(PCB* thread){
    ReadyList.addLast(thread);
}
PCB* Scheduler::get(){
    return ReadyList.removeFirst();

}