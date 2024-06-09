//
// Created by kosta on 11.8.23..
//

#ifndef PROJ_BASE_SEMAFOR_HPP
#define PROJ_BASE_SEMAFOR_HPP

#endif //PROJ_BASE_SEMAFOR_HPP
#include "../h/scheduler.hpp"
#include "../h/list.hpp"
#include "../h/PCB.hpp"
class semafor{
private:
    void unblock();
    void block();
    int val;
    int closed = false;
    List<PCB> blocked_list;
public:
    static int create_sem(semafor **,unsigned init);
    semafor(unsigned init);
    ~semafor(){
        close();
    }
    int get_val() const{
        return val;
    }
    void set_val(int v){
        val = v;
    }
    void dec_val(){
        val--;
    }
    void inc_val(){
        val++;
    }

    int signal();
    int wait();
    void close();
};