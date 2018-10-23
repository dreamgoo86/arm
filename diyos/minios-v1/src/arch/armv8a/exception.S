
	.global _e_undef
	.global _e_svc
	.global _e_pref_abort
	.global _e_data_abort
	.global _e_irq
	.global _e_fiq
	.global config_vectors
	.global ret_from_svc
	.global ret_from_svc_idle
	.global new__vectors
	.extern current_tss
	.extern do_timer
	
	
	//initilize vectors
config_vectors:
	ldr x0, =new__vectors
	MSR	    VBAR_EL3, x0

	ldr x0, =new__vectors_el2
	MSR	    VBAR_EL2, x0

	ldr x0, =new__vectors_el1
	MSR	    VBAR_EL1, x0
	ret

/**********************************************/
	.align 11
new__vectors:
	b .
	.align 7
	B  .			//Current Exception level with SP_EL0
	.align 7
	B  .
	.align 7
	B .

	.align 7
	B .
	.align 7
	B .		//Current Exception level with SP_ELx,x>0
	.align 7
	B .
	.align 7
	B .
	
	.align 7
	B .
	.align 7
	B .		    //Current Exception level < target level using AArch64
	.align 7
	B .
	.align 7
	B .
	
	.align 7
	B .
	.align 7
	B .			    //Current Exception level < target level using AArch32
	.align 7
	B .
	.align 7
	B .
/**********************************************/
	.align 11
new__vectors_el2:
	b .
	.align 7
	B .			//Current Exception level with SP_EL0
	.align 7
	B .
	.align 7
	B .
	.align 7
	B .
	.align 7
	B .			//Current Exception level with SP_ELx,x>0
	.align 7
	B .
	.align 7
	B .
	.align 7
	B .
	.align 7
	B .			    //Current Exception level < target level using AArch64
	.align 7
	B .
	.align 7
	B .
	.align 7
	B .
	.align 7
	B .			    //Current Exception level < target level using AArch32
	.align 7
	B .
	.align 7
	B .
	.align 7
/**********************************************/
	.align 11
new__vectors_el1:
	b .
	.align 7
	B .		//Current Exception level with SP_EL0
	.align 7
	B .	
	.align 7
	B .
/********************same exception level *******************/	
	.align 7
	B _e_svc_idle
	.align 7
	B _e_irq_idle				//Current Exception level with SP_ELx,x>0
	.align 7
	B _e_irq_idle
	.align 7
	B .
/********************diff exception*******************/	
	.align 7
//	b .
	B _e_svc
	.align 7
//	b .
	B _e_irq				    //Current Exception level < target level using AArch64
	.align 7
	B _e_irq	
	.align 7
	B .
/**********************************************/	
	.align 7
	B .
	.align 7
	B .			    //Current Exception level < target level using AArch32
	.align 7
	B .
	.align 7
	B .
/*******************************************/	
//user thread to kernel	
	.align 7
_e_svc:
	msr daifset,#2  //disable irq
	msr daifset,#1 //disable fiq
	msr spsel,xzr  //use sp_el0
	
	/**save contex*/
	sub sp,sp,#0x100   //generic registers &ELR store in stack
	stp x0,x1,[sp]   
	stp x2,x3,[sp,#0x10]
	stp x4,x5,[sp,#0x20]
	stp x6,x7,[sp,#0x30]
	stp x8,x9,[sp,#0x40]
	stp x10,x11,[sp,#0x50]
	stp x12,x13,[sp,#0x60]
	stp x14,x15,[sp,#0x70]
	stp x16,x17,[sp,#0x80]
	stp x18,x19,[sp,#0x90]
	stp x20,x21,[sp,#0xa0]
	stp x22,x23,[sp,#0xb0]
	stp x24,x25,[sp,#0xc0]
	stp x26,x27,[sp,#0xd0]
	stp x28,x29,[sp,#0xe0]
	mrs x0,elr_el1      //pc
	stp x30,x0,[sp,#0xf0]
	
	ldr x0,=current_tss
	ldr w0,[x0]   //get point to tss[x]
	mov x1,sp
	mrs x3,spsr_el1
	stp x1,x3,[x0]
	
	msr spsel,#1  //use sp_elx
	ldr x0,=kernstack_top
	mov sp,x0
	
	mrs x0,elr_el1
	
	ldr w0,[x0,#-4]
	bic w0,w0,#0xff000001
	lsr w0,w0,#5
	mov x1,x2
	bl syscall_handler
	

ret_from_svc:
	msr daifset,#2  //disable irq
	msr daifset,#1 //disable fiq
	msr spsel,xzr  //use sp_el0

	ldr x0,=current_tss
	ldr w0,[x0]   //get point to tss[x]
	ldp x1,x3,[x0]
	msr spsr_el1,x3
	//check the level that want to switch
	and x3,x3,#0x1f
	cmp x3,#0   // ==0,  level0
	beq config_stack
	msr spsel,#1  //!= 0  >level 0
config_stack:	
	mov sp,x1
	
	ldp x30,x0,[sp,#0xf0]
	msr elr_el1,x0  
	ldp x0,x1,[sp]   
	ldp x2,x3,[sp,#0x10]
	ldp x4,x5,[sp,#0x20]
	ldp x6,x7,[sp,#0x30]
	ldp x8,x9,[sp,#0x40]
	ldp x10,x11,[sp,#0x50]
	ldp x12,x13,[sp,#0x60]
	ldp x14,x15,[sp,#0x70]
	ldp x16,x17,[sp,#0x80]
	ldp x18,x19,[sp,#0x90]
	ldp x20,x21,[sp,#0xa0]
	ldp x22,x23,[sp,#0xb0]
	ldp x24,x25,[sp,#0xc0]
	ldp x26,x27,[sp,#0xd0]
	ldp x28,x29,[sp,#0xe0]
		
	add sp,sp,#0x100

	msr spsel,#1  //use sp_elx

	eret
	b .	


_e_irq:
	msr daifset,#2  //disable irq
	msr daifset,#1 //disable fiq
	msr spsel,xzr  //use sp_el0
	
	sub sp,sp,#0x100   //generic registers &ELR store in stack
	stp x0,x1,[sp]   
	stp x2,x3,[sp,#0x10]
	stp x4,x5,[sp,#0x20]
	stp x6,x7,[sp,#0x30]
	stp x8,x9,[sp,#0x40]
	stp x10,x11,[sp,#0x50]
	stp x12,x13,[sp,#0x60]
	stp x14,x15,[sp,#0x70]
	stp x16,x17,[sp,#0x80]
	stp x18,x19,[sp,#0x90]
	stp x20,x21,[sp,#0xa0]
	stp x22,x23,[sp,#0xb0]
	stp x24,x25,[sp,#0xc0]
	stp x26,x27,[sp,#0xd0]
	stp x28,x29,[sp,#0xe0]
	mrs x0,elr_el1      //pc
	stp x30,x0,[sp,#0xf0]
	
	ldr x0,=current_tss
	ldr w0,[x0]   //get point to tss[x]
	mov x1,sp
	mrs x3,spsr_el1
	stp x1,x3,[x0]
	
	msr spsel,#1  //use sp_elx
	ldr x0,=kernstack_top
	mov sp,x0

	bl irq_handler		   
	cmp x0,#0		
	bne ret_from_svc		

    bl do_timer
	bl do_signal
	b ret_from_svc
	b .
	
/********************************************/
//idle thread to kernel
	.align 7
_e_svc_idle:
	msr daifset,#2  //disable irq
	msr daifset,#1 //disable fiq
	
	/**save contex*/
	sub sp,sp,#0x100   //generic registers &ELR store in stack
	stp x0,x1,[sp]   
	stp x2,x3,[sp,#0x10]
	stp x4,x5,[sp,#0x20]
	stp x6,x7,[sp,#0x30]
	stp x8,x9,[sp,#0x40]
	stp x10,x11,[sp,#0x50]
	stp x12,x13,[sp,#0x60]
	stp x14,x15,[sp,#0x70]
	stp x16,x17,[sp,#0x80]
	stp x18,x19,[sp,#0x90]
	stp x20,x21,[sp,#0xa0]
	stp x22,x23,[sp,#0xb0]
	stp x24,x25,[sp,#0xc0]
	stp x26,x27,[sp,#0xd0]
	stp x28,x29,[sp,#0xe0]
	mrs x0,elr_el1      //pc
	stp x30,x0,[sp,#0xf0]
	
	ldr x0,=current_tss
	ldr w0,[x0]   //get point to tss[x]
	mov x1,sp
	mrs x3,spsr_el1
	stp x1,x3,[x0]
	
	ldr x0,=kernstack_top
	mov sp,x0
	
	mrs x0,elr_el1
	
	ldr w0,[x0,#-4]
	bic w0,w0,#0xff000001
	lsr w0,w0,#5
	mov x1,x2
	bl syscall_handler

ret_from_svc_idle:
	msr daifset,#2  //disable irq
	msr daifset,#1 //disable fiq
	msr spsel,#1  //use sp_elx

	ldr x0,=current_tss
	ldr w0,[x0]   //get point to tss[x]
	ldp x1,x3,[x0]
	msr spsr_el1,x3
	mov sp,x1
	
	ldp x30,x0,[sp,#0xf0]
	msr elr_el1,x0  
	ldp x0,x1,[sp]   
	ldp x2,x3,[sp,#0x10]
	ldp x4,x5,[sp,#0x20]
	ldp x6,x7,[sp,#0x30]
	ldp x8,x9,[sp,#0x40]
	ldp x10,x11,[sp,#0x50]
	ldp x12,x13,[sp,#0x60]
	ldp x14,x15,[sp,#0x70]
	ldp x16,x17,[sp,#0x80]
	ldp x18,x19,[sp,#0x90]
	ldp x20,x21,[sp,#0xa0]
	ldp x22,x23,[sp,#0xb0]
	ldp x24,x25,[sp,#0xc0]
	ldp x26,x27,[sp,#0xd0]
	ldp x28,x29,[sp,#0xe0]
		
	add sp,sp,#0x100
	eret
	b .	
	
	
_e_irq_idle:
	msr daifset,#2  //disable irq
	msr daifset,#1 //disable fiq
	
	sub sp,sp,#0x100   //generic registers &ELR store in stack
	stp x0,x1,[sp]   
	stp x2,x3,[sp,#0x10]
	stp x4,x5,[sp,#0x20]
	stp x6,x7,[sp,#0x30]
	stp x8,x9,[sp,#0x40]
	stp x10,x11,[sp,#0x50]
	stp x12,x13,[sp,#0x60]
	stp x14,x15,[sp,#0x70]
	stp x16,x17,[sp,#0x80]
	stp x18,x19,[sp,#0x90]
	stp x20,x21,[sp,#0xa0]
	stp x22,x23,[sp,#0xb0]
	stp x24,x25,[sp,#0xc0]
	stp x26,x27,[sp,#0xd0]
	stp x28,x29,[sp,#0xe0]
	mrs x0,elr_el1      //pc
	stp x30,x0,[sp,#0xf0]
	
	ldr x0,=current_tss
	ldr w0,[x0]   //get point to tss[x]
	mov x1,sp
	mrs x3,spsr_el1
	stp x1,x3,[x0]
	
	ldr x0,=kernstack_top
	mov sp,x0

	bl irq_handler		   // IRQ handler in irq mode
	cmp x0,#0		
	bne ret_from_svc_idle		//not systick interrupt

    bl do_timer
	bl do_signal
	b ret_from_svc_idle
	b .
	