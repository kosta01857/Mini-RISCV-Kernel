#include "../h/riscv.hpp"
void to_user(){
    __asm__ volatile("csrw sepc,ra");
    __asm__ volatile ("csrc sstatus ,%0"::"r"((uint64)256));
    __asm__ volatile ("ld s0,8(sp)");
    __asm__ volatile("addi sp,sp,16 ");
    __asm__ volatile("sret");
}
void Riscv::popSppSpie() {
    __asm__ volatile("csrw sepc,ra");
    __asm__ volatile ("csrc sstatus ,%0"::"r"((uint64)256));
    __asm__ volatile("sret");
}