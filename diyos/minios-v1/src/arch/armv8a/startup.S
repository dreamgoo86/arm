
	.section .text.boot, "x"
	.global _start
	.global _reset
	.global _gouser
	.extern config_vectors
	
_reset:
_start:
	mrs x0,sctlr_el3
	bic x0,x0,#1  //disable mmu
	bic x0,x0,#(1<<2) //disable dcache&icache
	bic x0,x0,#(1<<12)
	bic x0,x0,#(1<<25) //litter endian
	msr sctlr_el3,x0
	
	//init reg
	mov x0,#0
	mov x1,#0
	mov x2,#0
	mov x3,#0
	mov x4,#0
	mov x5,#0
	mov x6,#0
	mov x7,#0
	mov x8,#0
	mov x9,#0
	mov x10,#0
	mov x11,#0
	mov x12,#0
	mov x13,#0
	mov x14,#0
	mov x15,#0
	mov x16,#0
	mov x17,#0
	mov x18,#0
	mov x19,#0
	mov x20,#0
	mov x21,#0
	mov x22,#0
	mov x23,#0
	mov x24,#0
	mov x25,#0
	mov x26,#0
	mov x27,#0
	mov x28,#0
	mov x29,#0
	mov x30,#0
	
	//enable phyiscal interrupt in el3
	//all exception will route to EL3
//	mrs     x1, scr_el3
//	orr     x1, x1, #(0x1 << 1) 		//I:IRQ Enable
//	orr     x1, x1, #(0x1 << 2) 		//F:FIQ Enable
//	orr     x1, x1, #(0x1 << 3) 		//EA:abort Enable
//	orr     x1, x1, #(0x1 << 8) 		//HVC Enable
//	orr     x1, x1, #(0x1 << 10) 		//execute state control EL3,EL2 is aarch64
//	msr    scr_el3, x1


	//all exception route to EL1
	mrs     x1, scr_el3
	bic     x1, x1, #(0x1 << 0) 		//NS: secure mode
	bic     x1, x1, #(0x1 << 1) 		//I:IRQ Enable
	bic     x1, x1, #(0x1 << 2) 		//F:FIQ Enable
	bic     x1, x1, #(0x1 << 3) 		//EA:abort Enable
	orr     x1, x1, #(0x1 << 8) 		//HVC Enable
	orr     x1, x1, #(0x1 << 10) 		//execute state control EL3,EL2 is aarch64
	msr    scr_el3, x1
	
	//execute state control EL2   //EL1 is aarch64
	mrs     x1, hcr_el2
	orr     x1, x1, #(0x1 << 31)
	msr     hcr_el2, x1
	
	ldr x0,=usrstack_top
	msr sp_el0,x0
	ldr x0,=kernstack_top
	msr sp_el1,x0
	ldr x0,=secstack_top
	//msr spsel,xzr  //use sp_el0
	msr spsel,#1  //use sp_el3
	mov sp,x0
	
	
	
	bl config_vectors   //config vectors
	
	
	/*clear bss */
	ldr x0,=__bss_start
	ldr x1,=__bss_end
	mov x2,#0
	cmp x0,x1
	beq boot
	
clear:
	str x2,[x0]
	add x0,x0,#8
	cmp x0,x1
	bne clear

boot:
	//enable float registers
	mrs x0,cpacr_el1
	orr x0,x0,#(3<<20)
	msr cpacr_el1,x0
	
	//config stack
	//switch to kernel   EL1
	mov x0,#5    //EL1h
	msr spsr_el3,x0
	ldr x0,=kernel_addr
	msr elr_el3,x0
	eret   
kernel_addr:
	msr daifset,#2  //disable irq
	msr daifset,#1 //disable fiq	
	bl kernel_boot
	b .

.global run_idle
run_idle:

	b .


sctlr_text:
	.asciz "sctlr=0x%x\n"
