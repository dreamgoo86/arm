#ifndef _THREAD_H_
#define _THREAD_H_

#define MAXTHREADS	32
#define STACKSIZE	1024*8

#define TASK_UNALLOCATE			-1	// PCB unallocate
#define TASK_RUNNING			0	// thread running
#define TASK_INTERRUPTIBLE		1	// thread pending can interrupt
#define TASK_UNINTERRUPTIBLE	2	// thread pending can not interrupt
#define TASK_ZOMBIE				3	// thread zombie
#define TASK_STOPPED			4	// thread stop

#ifdef AARCH32
typedef struct tss_struct {
	long	R13;	
	long	R14;
	long	CPSR;
	long	R0;		
	long	R1;
	long	R2;		
	long	R3;
	long	R4;		
	long	R5;
	long	R6;
	long	R7;
	long	R8;
	long	R9;
	long	R10;
	long	R11;
	long	R12;
	long	PC;				
} tss_struct;
#endif
#ifdef AARCH64
typedef struct tss_struct {
	long	long	sp;	
	long	long	CPSR;			
} tss_struct;
#endif

struct sigaction {
	void (*sa_handler)(int);
	int sa_flags;
	void (*sa_restorer)(void);
};


typedef struct task_struct {
/* these are hardcoded - don't touch */
	long state;	/* -1 unrunnable, 0 runnable, >0 stopped */
	long counter;  //运行时间片
	long priority; //运行优先数
	long signal; //信号，位图，每bit代表一种信号
	struct sigaction sigaction[32];  //信号执行属性结构
//	long blocked;	/* bitmap of masked signals*/
/* various fields */
	int exit_code;  //退出码，父进程会来取
	unsigned long start_code,
	end_code,
	end_data,
	brk,//总长度
	start_stack;//堆栈段地址
	long pid,father;
	long alarm; //报警定时器值
	unsigned long used_math;//是否使用协处理器
/* tss for this task */
	struct tss_struct *tss; //任务从执行中被切换出时tss_struct结构保存当前处理器的所有寄存器值。当任务又被cpu重新执行时，cpu就会利用这些值恢复到任务被切换出时的状态
} task_struct;



__attribute__((aligned(0x10))) unsigned char stack[MAXTHREADS][STACKSIZE];//define the stack array for thread
__attribute__((aligned(0x10))) tss_struct tss[MAXTHREADS];
__attribute__((aligned(0x10))) task_struct task[MAXTHREADS];


#define LAST_TASK task[MAXTHREADS - 1]
#define FIRST_TASK task[0]
#define NR_TASKS MAXTHREADS


__attribute__((aligned(0x10))) tss_struct * current_tss;
__attribute__((aligned(0x10))) task_struct * current_task;
uint64_t jiffies;









#endif /* _THREAD_H_ */
