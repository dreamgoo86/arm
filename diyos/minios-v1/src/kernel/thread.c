#include <cpufunc.h>
#include <thread.h>
#include <subr.h>
#include <platform.h>
#include <signal.h>

#include <malloc.h>
#include <rand.h>

static __attribute__((optimize("-O0"))) void delay(uint32_t usec)
{
       int counts;
       for (; usec > 0; usec--)
               for (counts = 200; counts > 0; counts--)
                       ;
}

uint32_t idle_cnt = 0;

void sys_idle()
{
	while(1)
	{
		idle_cnt++;	
		kprintf("idle %u \n",idle_cnt);
		delay(1000);
	}
}

void task_1()
{
	int i = 0;
	while(1)
	{
		kprintf("task1\n");
		delay(100);
		i++;
		if(i>100){
			i=0;
			syscall_alarm(10);
		}
			
	}
}

void task_2()
{
	int i =0;
	while(1)
	{
		kprintf("task2\n");
		delay(100);
		i++;
		if(i>100)
			syscall_pause();
	}
}
/*
double float_test(int value)
{
	double f_v = value/3.0;
	return f_v;
}
*/

void task_3()
{
	int i =0;
	rand_seed(123);
	int cnt;
	while(1)
	{
		//kprintf("malloc function\n");
		delay(10);
		//cnt = rand()%100+1;
	//	kprintf("malloc %d byte\n",1000);
		delay(10);
		void * test = malloc(1000);
		if(test)
		{
			kprintf("malloc 1000 success\n");
			delay(10);
		}
		else
		{
			kprintf("malloc failed\n");
			delay(10);
			break;
		}
		
		free(test);
		
		
		delay(100);
	}
	
	syscall_pause();
}


void task_init(long pid,uint32_t addr)
{
	task[pid].pid = pid; 
	task[pid].priority = 4;
	task[pid].counter = 4;
	task[pid].state = TASK_RUNNING;
	#ifdef AARCH32
	task[pid].start_code = addr;
	tss[pid].CPSR	= 0x50;
	tss[pid].R13 = task[pid].start_stack;
	tss[pid].PC = addr;
	task[pid].start_stack = (unsigned long)(&stack[pid+1]);
	#endif
	#ifdef AARCH64
	task[pid].start_code = addr;
	tss[pid].CPSR	= 0x0;//EL0
	task[pid].start_stack = (unsigned long)(&stack[pid+1]) - 0x100;
	tss[pid].sp = task[pid].start_stack ;
	REG64(tss[pid].sp+0xf8) = addr;
	#endif
}


/*****************************************************************************
* sched init function
*****************************************************************************/
void sched_init(void)
{
	int i = 0;

    jiffies = 0;
    current_tss = NULL;
    current_task = NULL;

    //init pcb
    for(i=0; i < MAXTHREADS; i++){
       // thread_tss[i].R0 = 0;
       task[i].state = TASK_UNALLOCATE;
       task[i].counter = -1;
       task[i].priority = -1;
       task[i].signal = 0;
       task[i].exit_code = 0;
       task[i].start_code = 0;
       task[i].end_code = 0;
       task[i].end_data = 0;
       task[i].start_stack = 0;
       task[i].pid = -1;
       task[i].used_math = 0;
       task[i].tss = NULL;
    }

	task[0].pid = 0; 
	task[0].priority = 4;
	task[0].counter = 4;
	task[0].state = TASK_RUNNING;
	#ifdef AARCH32
	task[0].start_code = (uint32_t)sys_idle;
	tss[0].CPSR	= 0x5f;
	tss[0].R13 = task[0].start_stack;
	tss[0].PC = (uint32_t)sys_idle;
	task[0].start_stack = (uint32_t)&stack[1];
	#endif
	#ifdef AARCH64
	task[0].start_code = (uint32_t)((uint64_t)sys_idle);
	task[0].start_stack = (uint32_t)((uint64_t)&stack[1] - 0x100);
	tss[0].CPSR	= 0x5;//el1
	tss[0].sp = task[0].start_stack;
	REG64(tss[0].sp+0xf8) = (uint64_t)sys_idle;
	#endif

    //init idle & creat idle thread
    current_task = &task[0];
	current_tss = &tss[0];
	
	#ifdef AARCH32
	task_init(1,(uint32_t)task_1);
	task_init(2,(uint32_t)task_2);
	#endif
	
	#ifdef AARCH64
	task_init(1,(uint32_t)((uint64_t)task_1));
	task_init(2,(uint32_t)((uint64_t)task_2));
	#endif
//	task_init(3,(uint32_t)task_3);

}



void schedule(void)
{
	int i,next,c;
	struct task_struct * p;

/* check alarm, wake up any interruptible tasks that have got a signal */
//task[0] not signal check
	for(p = &LAST_TASK ; p > &FIRST_TASK ; --(p))
		if (p) {
			if ((p)->alarm && (p)->alarm < jiffies) {
					(p)->signal |= (1<<(SIGALRM-1));
					(p)->alarm = 0;
				}
			if ((p)->signal && (p)->state==TASK_INTERRUPTIBLE)
				(p)->state=TASK_RUNNING;
		}

/* this is the scheduler proper: */

	while (1) {
		c = -1;
		next = 0;
		i = NR_TASKS;
		p = &task[NR_TASKS];
		while (--i) {
			if (! --(p))
				continue;
			if ((p)->state == TASK_RUNNING && (p)->counter > c)
				c = (p)->counter, next = i;
		}
		if (c) break;
		for(p = &LAST_TASK ; p > &FIRST_TASK ; --(p))
			if (p)
				(p)->counter = ((p)->counter >> 1) +
						(p)->priority;
	}

    if(-1 == c) //not running,call idle
        next = 0;

//	switch_to(next);//switch to task pid=next
	current_task = &task[next];\
    current_tss= &tss[next];\
}


void do_timer(long cpl)
{
    jiffies ++;
//	if (cpl)
//		current->utime++;
//	else
//		current->stime++;
	if(current_task->pid != 0)
		if ((--current_task->counter)>0) return;
//	current->counter=0;
//	if (!cpl) return;
	schedule();
}


void panic(const char * s)
{
	kprintf("Kernel panic: %s\n\r",s);

	for(;;);
}


int sys_nice(long increment)
{
	if (current_task->priority-increment>0)
		current_task->priority -= increment;
	return 0;
}


int sys_alarm(long seconds)
{
	int old = current_task->alarm;
	old = old/HZ;
	current_task->alarm = (seconds>0)?(jiffies+HZ*seconds):0;
	current_task->state = TASK_INTERRUPTIBLE;
	schedule();
	return (old);
}


int sys_pause(void)
{
	current_task->state = TASK_INTERRUPTIBLE;
	schedule();
	return 0;
}

//skip one schedule
void sleep_on(struct task_struct *p)
{
	struct task_struct *tmp;

	if (!p)
		return;
	if (current_task == &(task[0]))
		panic("task[0] trying to sleep");
	tmp = p;
	p = current_task;
	current_task->state = TASK_UNINTERRUPTIBLE;
	schedule();
	if (tmp)
		tmp->state = TASK_RUNNING;
}

void wake_up(struct task_struct *p)
{
	if (p) {
		(*p).state = TASK_RUNNING;
	}
}

void sys_yield()
{
	sleep_on(current_task);
}


void sys_wakeup(long pid)
{
	wake_up(&task[pid]);
}