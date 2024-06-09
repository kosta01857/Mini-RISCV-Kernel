.global save_context
.type save_context,@function
save_context:
addi sp,sp,-256//rezervisemo mesto za cuvanje 32 registara(8B * 32 = 256B)
.irp index 3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
sd x\index,\index *8(sp)//npr sd x4,4*8(sp)
.endr
ret

.global restore_context
.type restore_context,@function
restore_context:
.irp index 3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
ld x\index,\index *8(sp)//npr ld x4,4*8(sp)
.endr
addi sp,sp,256//oslobadjam stek
ret