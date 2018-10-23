#ifndef __SHAREMEM_H__
#define __SHAREMEM_H__
typedef struct sharemem{
	void * data;
	int size;
	int flag;
	int key;
}sharemem;

int sharemem_create(int key,sharemem * shmem,int size);
int sharemem_delete(int key,sharemem * shmem);
int sharemem_read(int key,sharemem * shmem,void * data,int size);
int sharemem_write(int key,sharemem * shmem,void * data,int size);

#endif