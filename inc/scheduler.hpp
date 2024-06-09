//
// Created by kosta on 2.8.23..
//

#ifndef PROJ_BASE_SCHEDULER_HPP
#define PROJ_BASE_SCHEDULER_HPP


#include "../h/list.hpp"
class PCB;
class Scheduler{
public:
    static List<PCB> ReadyList;
    static void put(PCB* thread ); //dodaje na kraj liste
    static PCB* get(); // vraca prvi spreman
};
#endif //PROJ_BASE_SCHEDULER_HPP