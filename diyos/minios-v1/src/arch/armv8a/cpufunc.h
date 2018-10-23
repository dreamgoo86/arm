
#ifndef _CPUFUNC_H_
#define _CPUFUNC_H_

#include <types64.h>

static __inline register_t arch_cntfrq_get(void)
{
	register_t reg;

	__asm __volatile("mrs %0,CNTFRQ_EL0" : "=r" (reg));

	return reg;
}

static __inline uint64_t arch_cntpct_get(void)
{
	uint64_t reg;

	__asm __volatile("MRS %0, CNTPCT_EL0" : "=r" (reg));

	return reg;
}

static inline uint64_t arch_cntvct_get(void)
{
	uint64_t reg;

	__asm __volatile("MRS %0, CNTVCT_EL0" : "=r" (reg));
	return (reg);
}

static __inline void arch_cntp_ctl_set(register_t reg)
{
	__asm __volatile("MSR CNTP_CTL_EL0, %0" :: "r" (reg));
}

static __inline void arch_cntv_ctl_set(register_t reg)
{
	__asm __volatile("MSR CNTV_CTL_EL0, %0" :: "r" (reg));
}

static __inline void arch_cntp_tval_set(register_t reg)
{
	__asm __volatile("msr CNTP_TVAL_EL0, %0" :: "r" (reg));
}

static __inline register_t arch_cntp_tval_get(void)
{
	register_t reg;

	__asm __volatile("mrs %0,CNTP_TVAL_EL0" : "=r" (reg));

	return reg;
}

static __inline void arch_cntv_tval_set(register_t reg)
{
	__asm __volatile("msr CNTV_TVAL_EL0, %0" :: "r" (reg));
}

static __inline register_t arch_cntp_ctl_get(void)
{
	register_t reg;

	__asm __volatile("mrs %0,CNTP_CTL_EL0" : "=r" (reg));

	return reg;
}

static __inline register_t arch_cntv_ctl_get(void)
{
	register_t reg;

	__asm __volatile("mrs %0, CNTV_CTL_EL0" : "=r" (reg));

	return reg;
}

static __inline register_t arch_cntkctl_get(void)
{
	register_t reg;

	__asm __volatile("mrs  %0, CNTKCTL_EL1" : "=r" (reg));

	return reg;
}

static __inline void arch_cntkctl_set(register_t reg)
{
	__asm __volatile("msr CNTKCTL_EL1, %0" :: "r" (reg));
}

static __inline void enable_irq(void)
{
	__asm __volatile("msr daifclr,#2" ::);
}

static __inline void enable_fiq(void)
{
	__asm __volatile("msr daifclr,#1" ::);
}

static __inline void disable_irq(void)
{
	__asm __volatile("msr daifset,#2" ::);
}

static __inline void disable_fiq(void)
{
	__asm __volatile("msr daifset,#1" ::);
}

static __inline register_t arch_mpidr_get(void)
{
	register_t reg;

	__asm __volatile("MRS %0,MPIDR_EL1" : "=r" (reg));

	return reg;
}


static __inline void syscall_pause()
{
	__asm __volatile("svc #0x0");
}


static __inline void syscall_alarm(uint32_t s)
{
	__asm __volatile("mov x2,%0"::"r"(s):);
	__asm __volatile("svc #0x1");
}

static __inline void syscall_wakeup(long pid)
{
	__asm __volatile("mov x2,%0"::"r"(pid):);
	__asm __volatile("svc #0x2");
}

static __inline void syscall_yield()
{
	__asm __volatile("svc #0x3");
}

static __inline void syscall_no_sched()
{
	__asm __volatile("svc #0x4");
}

static __inline void syscall_re_sched()
{
	__asm __volatile("svc #0x5");
}


static __inline void dsb()
{
	__asm __volatile("dsb sy");
}

static __inline void isb()
{
	__asm __volatile("isb");
}

static __inline void barrier()
{
	__asm __volatile("dsb sy");
	__asm __volatile("isb");
}


#endif /* _CPUFUNC_H_ */
