
	.global _e_undef
	.global _e_svc
	.global _e_pref_abort
	.global _e_data_abort
	.global _e_irq
	.global _e_fiq
	.global ret_from_svc
	.extern current_tss



_e_undef:
	bl .

_e_svc:
	cpsid i  //disable irq
	/**save contex*/
	ldr sp,=current_tss
	ldr sp,[sp]
	add sp,sp,#68
	stmfd sp!,{r0-r12,r14}
	mrs r0,spsr
	stmfd sp!,{r0}
	mov r0,sp
	mrs r1,cpsr
	msr cpsr_c,#0xdf //switch to system mode
	stmfd r0!,{r13,r14}
	msr cpsr,r1 //back
	msr cpsr_c,#0xd3//switch to svc mode
	ldr sp, =kernstack_top @Set kernel in system mode

	ldr r0,[lr,#-4]
	bic r0,#0xff000000
	mov r1,r2
	bl syscall_handler

ret_from_svc:
	cpsid i  //disable irq
	mrs r1,cpsr
	msr cpsr_c,#0xdf //switch to system mode
	ldr r0,=current_tss
	ldr r0,[r0]
	ldmfd r0!,{r13,r14}
	msr cpsr,r1  //back to svc mode
	ldr r1,[r0] //cpsr
	msr spsr,r1
	add r0,r0,#4
	mov r13,r0
	ldmfd sp!,{r0-r12,r14}
	subs pc,r14,#0

	b .	

_e_pref_abort:
	bl .

_e_data_abort:
	ldr r0, =abt_text
	mov r1, lr
	mrc p15, 0, r2, c6, c0, 0
	mrs r3, cpsr
	bl kprintf
	bl .



_e_irq:
	cpsid i  //disable irq
	/**save contex*/
	ldr sp,=current_tss
	ldr sp,[sp]
	add sp,sp,#68
	sub r14,r14,#4
	stmfd sp!,{r0-r12,r14}
	mrs r0,spsr
	stmfd sp!,{r0}
	mov r0,sp
	mrs r1,cpsr
	msr cpsr_c,#0xdf //switch to system mode
	stmfd r0!,{r13,r14}
	msr cpsr,r1 //back
	msr cpsr_c,#0xd3//switch to svc mode
	ldr sp, =kernstack_top @Set kernel in system mode

	bl irq_handler		   @ IRQ handler in irq mode
	cmp r0,#0		
	bne ret_from_svc		@not systick interrupt

    bl do_timer
	bl do_signal
	b ret_from_svc
	b .


_e_fiq:
	b .
	ldr r0, =fiq_text
	mov r1, lr 
	bl kprintf
	bl .

syscall_text:
	.asciz "[svc] Syscall %d used!\n"
undef_text:
	.asciz "[und] Undefined exception %p\n"
irq_text:
	.asciz "[irq] IRQ exception core0=0x%x lr=0x%x cpsr=0x%x\n"
fiq_text:
	.asciz "[fiq] FIQ exception %p\n"
pref_abort:
	.asciz "pref abort %p\n"
abt_text:
	.asciz "[abt] data abort lr=0x%x dfar=0x%x cpsr=0x%x\n"
