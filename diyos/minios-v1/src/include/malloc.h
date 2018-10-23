#ifndef __MALLOC_H__
#define __MALLOC_H__
#include <types.h>
//typedef int size_t;
void *malloc(size_t size);
void *calloc(size_t number , size_t size);
void free(void *p);
void *realloc(void *p, size_t size);

#endif