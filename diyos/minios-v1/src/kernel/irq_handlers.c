#include <types.h>
#include <subr.h>
#include <cpufunc.h>
#include <platform.h>

//uint64_t sys_cycle = 0;
//uint64_t jiffies = 0;

int irq_handler()
{
//	sys_cycle++;
//	kprintf("lp=0x%x\n",(uint32_t)sys_cycle);
	if(arch_cntp_ctl_get()&BIT_2){
		arch_cntp_tval_set(SYSTICK);
		arch_cntp_ctl_set(1);
		return 0;//systick interrupt
	}

	kprintf("interrupt\n");
	return 1;
}
