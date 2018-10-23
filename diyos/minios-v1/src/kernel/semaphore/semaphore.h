#ifndef __SEMAPHORE_H__
#define __SEMAPHORE_H__

typedef struct sem_t{
	long flag ;//  =0   not init    =1    init
	long value;
} sem_t;

int sem_init(sem_t *sem, int pshared, unsigned int value);
int sem_getvalue(sem_t *sem, int *valp);
int sem_wait(sem_t *sem);
int sem_post(sem_t *sem);
int sem_destroy(sem_t *sem);
#endif