
#ifndef _UART_H_
#define _UART_H_

#include <types.h>

/*
 * Low-level UART interface.
 */

void uart_init(int, int, int, int);

void uart_putc(int, void*);

#define UART_BASE 0x70100000
#define UART_STS1 0x7010000c

#endif /* _UART_H_ */
