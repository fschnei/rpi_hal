

// ARM cpu modes bits[4:0] in cpsr (current programm status register)
.equ    CPSR_MODE_USER,         0x10
.equ    CPSR_MODE_FIQ,          0x11
.equ    CPSR_MODE_IRQ,          0x12
.equ    CPSR_MODE_SVR,          0x13
.equ    CPSR_MODE_ABORT,        0x17
.equ    CPSR_MODE_UNDEFINED,    0x1B
.equ    CPSR_MODE_SYSTEM,       0x1F

// ARM interrupt enable bits[7:6] in cpsr
.equ    CPSR_IRQ_BLOCK,       0x80
.equ    CPSR_FIQ_BLOCK,       0x40



// this disables irq
.globl hal_interrupt_initIVT
hal_interrupt_initIVT:
	// push register to stack
	push {r0-r9}

	// disable irq
	mrs     r0, cpsr
	orr     r0, r0, #CPSR_IRQ_BLOCK
	msr     cpsr_c, r0

	// copy interrupt vector table to 0x0000
	// get address of interrupt vector table
	adr     r0, interrupt_vector_table
	// get destination pointer (new start address of ivt)
	mov     r1, #0x0000
	// copy 1st part of ivt
	ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
	stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}
	// copy 2nd part of ivt
	ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
	stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}


	// set irq stack pointer
    // get current programm status register
	mrs     r0, cpsr
	// clear mode field (TODO: should I go to CPSR_MODE_SVR ?!)
	bic r1, r0, #0x1f
	// set mode filed to interrupt mode
	orr r1,r1, #CPSR_MODE_IRQ
	// write cpsr (with mode filed: interrupt mode) back to hardware
	msr     cpsr_c, r1
	// set stack pointer for interrupt mode
	mov sp, #0x21000000
	// go back to previous mode by write the old value to current programm status register
	msr     cpsr_c, r0

	// pop register back from stack
	pop {r0-r9}
	// go back to caller
	mov pc, lr




// interrupt vector table (must be copied to 0x0000)
interrupt_vector_table:
    ldr pc, _reset_h
    ldr pc, _undefined_instruction_vector_h
    ldr pc, _software_interrupt_vector_h
    ldr pc, _prefetch_abort_vector_h
    ldr pc, _data_abort_vector_h
    ldr pc, _unused_handler_h
    ldr pc, _interrupt_vector_h
    ldr pc, _fast_interrupt_vector_h

_reset_h:                           .word   _reset_		// main?
_undefined_instruction_vector_h:    .word   undefined_instruction_vector
_software_interrupt_vector_h:       .word   software_interrupt_vector
_prefetch_abort_vector_h:           .word   prefetch_abort_vector
_data_abort_vector_h:               .word   data_abort_vector
_unused_handler_h:                  .word   unused_vector
_interrupt_vector_h:                .word   interrupt_vector
_fast_interrupt_vector_h:           .word   fast_interrupt_vector



undefined_instruction_vector:
	b undefined_instruction_vector

software_interrupt_vector:
	b software_interrupt_vector

prefetch_abort_vector:
	b prefetch_abort_vector

data_abort_vector:
	b data_abort_vector

unused_vector:
	b unused_vector

//interrupt_vector:
//	b interrupt_vector

fast_interrupt_vector:
	b fast_interrupt_vector




