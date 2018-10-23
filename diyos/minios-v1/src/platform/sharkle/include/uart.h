
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

#define _UART_TXD 0x0000    //ctrl0 bit14=0  8bits   bit14=1  32bits
#define _UART_RXD 0x0004  //8bits  read from fifo
#define _UART_STS0 0x0008
#define _UART_STS1 0x000c
#define _UART_IEN 0x0010  //bit0  rxf_full_int_en     bit1 TXF_empty_int_en
#define _UART_ICLR 0x0014
#define _UART_CTRL0 0x0018
#define _UART_CTRL1 0x001c
#define _UART_CTRL2 0x0020   //config tx fifo[15:8] and  rx fifo[6:0]
#define _UART_CKD0 0x0024
#define _UART_STS2 0x002c
#define _UART_DSPWAIT 0x0030   //config uart function ,such as  dma transfer  tx_dma   bit5  rx_dma bit4
#define _UART_SFCTL 0x0034
#define _UART_XCMD 0x0038
#define _UART_ESCAPE 0x003c
#define _UART_IP_VERSION 0x0040


#define uart_dma_id   16

#endif /* _UART_H_ */
