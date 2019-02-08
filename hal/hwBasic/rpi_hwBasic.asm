

.global hal_hwBasic_GetSP
hal_hwBasic_GetSP:
    // get stack pointer value
    str     sp, [sp]
    // save value to ro (return register)
    ldr     r0, [sp]
    // return
    mov     pc, lr


.globl hal_hwBasic_GetCPSR
hal_hwBasic_GetCPSR:
	mrs     r0, cpsr
	mov     pc, lr


.globl hal_hwBasic_PutMem
hal_hwBasic_PutMem:
    str r1,[r0]
    bx lr

.globl hal_hwBasic_GetMem
hal_hwBasic_GetMem:
    ldr r0,[r0]
	bx lr

