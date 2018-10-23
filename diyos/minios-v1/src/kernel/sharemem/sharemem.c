#include <cpufunc.h>
#include <thread.h>
#include <subr.h>
#include <platform.h>
#include <signal.h>
#include <sharemem.h>


int sharemem_create(int key,sharemem * shmem,int size)
{
	shmem->data = malloc(size);
	if(!shmem->data)
		return -1;
	shmem->size = size;
	shmem->key = key;
	shmem->flag = 0;
	
	return 0;
}

int sharemem_delete(int key,sharemem * shmem)
{
	if(shmem->key == key){
		free(shmem->data)
		shmem->data = NULL;
		shmem->size = 0;
		shmem->key = 0;
		shmem->flag = 0;
		return 0;
	}else{
		return -1;
	}
}

int sharemem_read(int key,sharemem * shmem,void * data,int size)
{
	if(! shmem->key == key)
		return -1;
	
	if(shmem->flag)
		return -1;
	
	syscall_no_sched();
	shmem->flag = 1;
	syscall_re_sched();
	
	memcpy(data,shmem->data,size);
	shmem->flag = 0;
	
	return 0;
}


int sharemem_write(int key,sharemem * shmem,void * data,int size)
{
	if(shmem->flag)
		return -1;
	
	if(! shmem->key == key)
		return -1;
	
	syscall_no_sched();
	shmem->flag = 1;
	syscall_re_sched();
	
	memcpy(shmem->data,data,size);
	shmem->flag = 0;
	
	return 0;
}