#include <types.h>
#include <subr.h>
#include <cpufunc.h>
#include <syscalls.h>
#include <thread.h>
#include <platform.h>
#include <dma.h>


void kern_boot(void);
void clearbss(void);
void user_entry(void);


static void minios_startup_msg(void)
{
//	kprintf("miniOS ARMv7-%s startup ...\n", MINIOS_VERSION);
}
extern void ret_from_svc();

void kernel_boot(void)
{
	minios_startup_msg(); //print start msg

	sched_init();

	user_entry();

	platform_init();
//	enable_irq();

	arch_cntp_tval_set(SYSTICK);
	arch_cntp_ctl_set(1);

	#ifdef AARCH32
	ret_from_svc();//idle thread
	#endif
		#ifdef AARCH64
		ret_from_svc_idle();//idle thread
		#endif
	/* Loop forever */
	for (;;)
	{
		;
	}
}
