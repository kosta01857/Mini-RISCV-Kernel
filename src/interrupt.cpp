//
// Created by kosta on 15.5.23.
//
#include "../h/memory_allocator.hpp"
#include "../lib/mem.h"
#include "../h/interrupt.hpp"
#include "../h/riscv.hpp"
#include "../h/PCB.hpp"
#include "../lib/console.h"
#include "../h/semafor.hpp"
uint64 inline read_opcode(){
    uint64 t;
    __asm__ volatile("mv %0,a0":"=r"(t));//read op_kod
    return t;
}
void inline insert_retval(uint64 rez){
    __asm__ volatile("mv a0,%0"::"r"(rez));
}
void interrupt() {
    uint64 volatile status = Riscv::r_sstatus();
    uint64 volatile cause = Riscv::r_scause();
    uint64 volatile pc = Riscv::r_sepc();
    if (cause == 0x8000000000000001UL) { //da li je softverski prekid?
    volatile uint64 arg = 1 << 1;
    __asm__ volatile("csrc sip,%0"::"r"(arg)); //clear sip reg da vise ne budi interrupt bit na 1
    } else if (cause == 0x8000000000000009UL) { //konzola

        console_handler();
    }
    else if (cause == 0x0000000000000009UL ||cause == 0x0000000000000008UL ) {//ecall
        uint64 volatile val = read_opcode();
        switch (val) {
            default:{
                break;
            }
            case 1: {
            uint64 volatile s;
            __asm__ volatile("mv %0,a1":"=r"(s));//read arg
            void *rez = memory_allocator::allocator.mem_alloc((size_t) s);
                insert_retval((uint64)rez);
            break;
            }
            case 2: {
            uint64 volatile s;
            __asm__ volatile("mv %0,a1":"=r"(s));//read arg
            int rez = memory_allocator::allocator.free((void *) s);
            insert_retval((uint64)rez);
            break;
            }
            case 11:{
            volatile uint64 adr;
            volatile uint64 args;
            volatile uint64 pcb;
            __asm__ ("mv %0,a6":"=r"(pcb));//read arg
            __asm__ ("mv %0,a2":"=r"(args));//read arg
            __asm__ ("mv %0,a1":"=r"(adr));//read arg
            int rez = PCB::create_thread((PCB**)pcb,(void(*)(void*))adr, (void*)args);
            insert_retval((uint64)rez);
            break;
            }
            case 13:{
            PCB::yield();
            break;
            }
            case 12:{
                PCB::running->die();
                break;
            }
            case 42:{
                uint64 volatile s;
                __asm__ volatile("mv %0,a1":"=r"(s));//read arg
                __putc((char)s);
                break;
            }
                case 41:{
                    char c = __getc();
                    insert_retval((uint64)c);
                    break;
                }
                case 14:{
                    uint64 volatile s;
                    __asm__ volatile("mv %0,a1":"=r"(s));//read arg
                    ((PCB*)s)->add_to_waitList(PCB::running);
                    break;
                }
                case 23:{
                    uint64 volatile s;
                    __asm__ volatile("mv %0,a1":"=r"(s));//read arg
                    int rez = ((semafor*)s)->wait();
                    insert_retval((uint64)rez);
                    break;
                }
                case 24:{
                    uint64 volatile s;
                    __asm__ volatile("mv %0,a1":"=r"(s));//read arg
                    int rez = ((semafor*)s)->signal();
                    insert_retval((uint64)rez);
                    break;
                }
                case 21:{
                    uint64 volatile handle;
                    uint64 volatile s;
                    __asm__ volatile("mv %0,a2":"=r"(handle));//read arg
                    __asm__ volatile("mv %0,a1":"=r"(s));//read arg
                    int ret = semafor::create_sem((semafor**) handle,s);
                    insert_retval((uint64)ret);
                    break;
                }
                case 22:{
                    uint64 volatile s;
                    __asm__ volatile("mv %0,a1":"=r"(s));//read arg
                    ((semafor*)s)->close();
                    break;
                }
        }

        pc+=4;
        Riscv::w_sepc(pc);
        Riscv::w_sstatus(status);
    }
    }
extern "C"{
    void time_handler(){
        volatile uint64 arg = 1 << 1;
        __asm__ volatile("csrc sip,%0"::"r"(arg));
    }
}