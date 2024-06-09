.extern interrupt
.extern console_handler
.extern time_handler
.extern trap
.align 4
.global trap
.type trap,@function
trap:
j ecall
j timer
sret
sret
sret
sret
sret
sret
j console
j console
ecall:
//save context
addi sp,sp,-256//rezervisemo mesto za cuvanje 32 registara(8B * 32 = 256B)
.irp index 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
sd x\index,\index *8(sp)//npr sd x4,4*8(sp)
.endr
call interrupt
sd x10,(sp)
//restore context
.irp index 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
ld x\index,\index *8(sp)//npr ld x4,4*8(sp)
.endr
ld x10,(sp)
addi sp,sp,256//oslobadjam stek

sret//upisuje u pc iz sepc ne iz ra!
timer:
addi sp,sp,-256//rezervisemo mesto za cuvanje 32 registara(8B * 32 = 256B)
.irp index 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
sd x\index,\index *8(sp)//npr sd x4,4*8(sp)
.endr
call time_handler
sd x10,(sp)
//restore context
.irp index 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
ld x\index,\index *8(sp)//npr ld x4,4*8(sp)
.endr
ld x10,(sp)
addi sp,sp,256//oslobadjam stek

sret//upisuje u pc iz sepc ne iz ra!
console:
//save context
addi sp,sp,-256//rezervisemo mesto za cuvanje 32 registara(8B * 32 = 256B)
.irp index 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
sd x\index,\index *8(sp)//npr sd x4,4*8(sp)
.endr
call console_handler
sd x10,(sp)
//restore context
.irp index 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
ld x\index,\index *8(sp)//npr ld x4,4*8(sp)
.endr
ld x10,(sp)
addi sp,sp,256//oslobadjam stek

sret//upisuje u pc iz sepc ne iz ra!