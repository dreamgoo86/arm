#include <subr.h>
#include <types.h>
#include <uart.h>

void uart_init(int baudrate, int databits, int stopbits,int parity)
{
	REG32(UART_BASE+0x24 ) = 0xe2;
//	kprintf("Initialising PrimeCell UART\n");
}


void  __attribute__((optimize("-O0")))   uart_putc(int c, void *arg)
{
	while(REG32(UART_STS1)&0x0001ff00);
	REG32(UART_BASE ) = c;
	#ifdef AARCH32
	asm volatile("dmb");
	#endif
	#ifdef AARCH64
	asm volatile("dmb sy");
	#endif
	asm volatile("isb");
}

void  uart_puts(const char *s)
{
	while (*s){
		while(REG32(UART_STS1)&0x0001ff00);
		REG32(UART_BASE ) = *s++;
		#ifdef AARCH32
		asm volatile("dmb");
		#endif
		#ifdef AARCH64
		asm volatile("dmb sy");
		#endif
		asm volatile("isb");
	}
}

int  uart_gets(char * buf)
{
	int cnt = 0;
	
	while(!(REG32(UART_STS1)&0x000000ff));//wait until receive data

rev:
	while((REG32(UART_STS1)&0x000000ff))
	{
		buf[cnt++] = (char)REG32(UART_BASE + 4);
		#ifdef AARCH32
		asm volatile("dmb");
		#endif
		#ifdef AARCH64
		asm volatile("dmb sy");
		#endif
		asm volatile("isb");
	}
	
	if(!(REG32(UART_BASE + 8)&0x2000))//time out status
		goto rev;
	
	REG32(UART_BASE + 0x14) |= 0x2000;//time out int clear
	
	return cnt;
}


void uart_dma_enable(int isTrue)
{
	if(isTrue)
		REG32(UART_BASE+_UART_CTRL1) |= BIT_15;
	else
		REG32(UART_BASE+_UART_CTRL1) &= (~BIT_15);
}

void uart_fifo_config(int txf,int rxf)
{
	REG32(UART_BASE+_UART_CTRL2) = (txf<<8)|(rxf&0x7f);
}
//0 disable   1  rxf  2 txf  3 all
void uart_dma_mode_sel(int mod)
{
	if(mod)
		REG32(UART_BASE+_UART_DSPWAIT) |= (mod<<4);
	else
		REG32(UART_BASE+_UART_DSPWAIT) &= ~(mod<<4);
}


int get_tx_fifo_num()
{
	unsigned int num = REG32(UART_BASE+_UART_STS1)&(0x1ff<<8);
	return num>>8;
}

int get_rx_fifo_num()
{
	unsigned int num = REG32(UART_BASE+_UART_STS1)&(0x7f);
	return num;
}

void rx_fifo_full_interrupt_enable()
{
	REG32(UART_BASE+_UART_IEN) |= 1;
}

void tx_fifo_empty_interrupt_enable()
{
	REG32(UART_BASE+_UART_IEN) |= 2;
}

void rx_fifo_full_interrupt_disable()
{
	REG32(UART_BASE+_UART_IEN) &= ~1;
}

void tx_fifo_empty_interrupt_disable()
{
	REG32(UART_BASE+_UART_IEN) &= ~2;
}

void uart_timeout_interrupt_enable()
{
	REG32(UART_BASE+_UART_IEN) |= 1<<13;
}
void uart_timeout_interrupt_disable()
{
	REG32(UART_BASE+_UART_IEN) &= ~(1<<13);
}

void uart_receive_timeout_config(int num)
{
	unsigned int value = REG32(UART_BASE+_UART_CTRL1);
	value = value &(~(0x1f<<9));
	value = value | ((num&0x1f)<<9);
	REG32(UART_BASE+_UART_CTRL1) = value ;
}

void uart_timeout_interrupt_clr()
{
	REG32(UART_BASE+_UART_ICLR) |= (1<<13);
}