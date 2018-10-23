#include <cpufunc.h>
#include <thread.h>
#include <subr.h>
#include <platform.h>
#include <signal.h>



//config signal action function
int sys_signal(int signum, long handler, long restorer)
{
	struct sigaction tmp;

	if (signum<1 || signum>32 || signum==SIGKILL)
		return -1;
	tmp.sa_handler = (void (*)(int)) handler;
	tmp.sa_flags = SA_ONESHOT | SA_NOMASK;
	tmp.sa_restorer = (void (*)(void)) restorer;
	handler = (long) current_task->sigaction[signum-1].sa_handler;
	current_task->sigaction[signum-1] = tmp;
	return handler;
}

//Execute current thread’s signal function
void do_signal()
{
	if(!current_task->signal) //not 
        return ;
    int i =0;

    for(i=0;i<32;i++)
    {
       if(current_task->signal&(1<<i)){
		   if(current_task->sigaction[i].sa_flags)
           		current_task->sigaction[i].sa_handler(i+1);
           current_task->signal &= (~(1<<i));
       }
        
    }

}


//send signal to other thread
int sys_send_signal(long pid,int signum)
{
	task[pid].signal |= (1<<(signum-1));
}



void sigstop_handler(long pid)
{

}

