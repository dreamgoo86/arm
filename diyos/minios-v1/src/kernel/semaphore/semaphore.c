#include <cpufunc.h>
#include <thread.h>
#include <subr.h>
#include <platform.h>
#include <signal.h>
#include <semaphore.h>


//sem_t semaphore[MAXTHREADS];//多进程之间使用的，主要应用在包含文件系统的os。当前先不用

//其中sem是要初始化的信号量，
//pshared表示此信号量是在进程间共享还是线程间共享，value是信号量的初始值。
//int sem_init(sem_t *sem, int pshared, unsigned int value)
int sem_init(sem_t *sem, int pshared, unsigned int value)
{
	syscall_no_sched();
	sem->flag = 1;
	sem->value = value;
	syscall_re_sched();
	return 0;
}

//获取信号量的值
int sem_getvalue(sem_t *sem, int *valp)
{
	if(!sem->flag)
		return -1;
	
	*valp = sem->value;
	
	return 0;
}

//等待信号量，如果信号量的值大于0,将信号量的值减1,立即返回。
//如果信号量的值为0,则线程阻塞。相当于P操作。成功返回0,失败返回-1。
int sem_wait(sem_t *sem)
{
	if(!sem->flag)
		return -1;
	
	while(sem->value <= 0)
	{
		sys_yield();
	}		
	
	syscall_no_sched();
	sem->value --;
	syscall_re_sched();
	return 0;
}

//释放信号量，让信号量的值加1。相当于V操作。
int sem_post(sem_t *sem)
{
	if(!sem->flag)
		return -1;
	
	syscall_no_sched();
	sem->value ++;
	syscall_re_sched();
	
	return 0;
}
//其中sem是要销毁的信号量。
//只有用sem_init初始化的信号量才能用sem_destroy销毁。
int sem_destroy(sem_t *sem)
{
	if(!sem->flag)
		return -1;
	
	syscall_no_sched();
	sem->flag = 0;
	sem->value = 0;
	syscall_re_sched();
	return 0;
}