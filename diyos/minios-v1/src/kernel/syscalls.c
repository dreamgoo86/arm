
#include <types.h>
#include <cpufunc.h>
#include <syscalls.h>
#include <subr.h>
#include <thread.h>
#include <platform.h>
#include <signal.h>


extern int sys_pause(void);
extern int sys_alarm(long seconds);
extern int sys_nice(long increment);
extern void sys_yield();
extern void sys_wakeup(long pid);
extern int sys_signal(int signum, long handler, long restorer);
extern int sys_send_signal(long pid,int signum);



int syscall_handler(int syscall,void * data)
{
	switch(syscall){
		case 0:
			sys_pause();
			break;
		case 1:
			sys_alarm((uint32_t)data);
			break;
		case 2:
			syscall_wakeup((long)data);
			break;
		case 3:
			sys_yield();
			break;
		case 4:
			disable_irq();
			break;
		case 5:
			enable_irq();
			break;
		default:
			break;
	}
	return 0;
}
