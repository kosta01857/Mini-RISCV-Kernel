#include "../h/syscall_c.hpp"
#include "../h/riscv.hpp"
#include "../h/memory_allocator.hpp"
#include "../h/syscall_cpp.hpp"
extern "C" void trap();
extern void userMain();

void jmp_usermain(void*){
    userMain();
}
int main(){
    memory_allocator mem;
    memory_allocator::allocator = mem;
   __asm__ volatile("csrw stvec,%0"::"r"((uint64)&trap|1)); //upisujem adresu asm fajla prekidne rutine
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);
    PCB *main;
    thread_create(&main,nullptr,nullptr);
    PCB::running = main;
    PCB* usermain;
    thread_create(&usermain,jmp_usermain,nullptr);
    thread_join(usermain);
    delete usermain;
    delete main;
    return 0;
}