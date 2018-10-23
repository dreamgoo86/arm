
	.section .text.boot, "x"
	.global _reset
	.global _gouser

_reset:
	b _startup	/* 0x00 Reset */
	b _e_undef	/* 0x04 Undefined instruction */
	b _e_svc	/* 0x08 Supervisor call */
	b _e_pref_abort	/* 0x0c Prefetch abort */
	b _e_data_abort	/* 0x10 Data abort */
	b .		/* 0x14 Unused */
	b _e_irq	/* 0x18 IRQ Interrupt */
	b _e_fiq	/* 0x1c FIQ Interrupt */

_startup:
	cpsid if	@ Disable IRQ/FIQ

	/*
	 * B4.1.130 SCTLR, System Control register
	 *
	 * Vectors Bit
	 * SCTLR.V = 0 Low Exceptions Vector, can remap.
	 * SCTLR.V = 1 High Exceptions Vector, can't remap.
	 */
	mrc p15, 0, r0, c1, c0, 0	@ Read SCTLR into R0
	bic r0, #(1 << 13)		@ Set SCTLR.V to 0
	bic r0, #(1 << 12)		@ Set SCTLR.I to 0
	bic r0, #(1 << 11)		@ Set SCTLR.Z to 0
	bic r0, #(1 << 2)		@ Set SCTLR.C to 0
	mcr p15, 0, r0, c1, c0, 0	@ Write SCTLR

	/*
	 * B4.1.156 VBAR, Vector Base Address Register
	 *
	 * Vector Base Address at 31:5
	 * UNK/SBZP 4:0
	*/
	ldr r0, =_reset    
	mcr p15, 0, r0, c12, c0, 0   @config VBAR


	/* config all mode  stack*/

	msr cpsr_c, #0xd2	   @ Switch to irq mode
	ldr sp, =irqstack_top	   @ Load stack top at init

	msr cpsr_c, #0xd1	   @ Switch to fiq mode
	ldr sp, =fiqstack_top	   @ Load stack top at init

	msr cpsr_c, #0xd7	   @ Switch to abt mode
	ldr sp, =abtstack_top

	msr cpsr_c, #0xdb	   @ Switch to Undefined mode
	ldr sp, =undstack_top	   @ Load stack top at init

	msr cpsr_c, #0xdf	   @ Switch to System mode
	ldr sp, =usrstack_top	   @ Load stack top at init

	/* Set kernel stack pointer */
	msr cpsr_c, #0xd3	   @ Switch to svc mode
	ldr sp, =kernstack_top


	/*clear bss */
	ldr r0,=__bss_start
	ldr r1,=__bss_end
	mov r2,#0

clear:
	str r2,[r0]
	add r0,r0,#4
	cmp r0,r1
	bne clear


	bl kernel_boot


.global run_idle
run_idle:
	msr cpsr_c,#0x5f
	mov sp,r0
	msr cpsr,r1
	mov r14,r2
	subs pc,r14,#0
	b .


sctlr_text:
	.asciz "sctlr=0x%x\n"
