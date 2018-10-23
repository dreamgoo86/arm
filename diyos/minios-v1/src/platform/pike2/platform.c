#include <types.h>
#include <subr.h>
#include <platform.h>
#include <uart.h>
#include <intc.h>
#include <cpufunc.h>
#include <dma.h>

void intc_init()
{
	REG32(0x71300000) += (BIT_19|BIT_20|BIT_21|BIT_22);
}

void apcpu_ts_init()
{
	REG32(0x402e0000) |= BIT_28|BIT_29;//Enable TS0
	REG32(0x40400000 + 0x20) = ARCH_TIMER_FREQ;  // ts0 frequency
	REG32(0x40400000 + 0x08) = 0x0;
	REG32(0x40400000 + 0x0C) = 0x0;
	REG32(0x40400000 + 0x00) = 0x1;
}



void  twpllclk_init_768M()
{
	REG32(0x20e00054)  = 0;
	int i =0;
	for(i=0;i<100;i++);
	REG32(0x20e00054)  = 2;
}


void platform_init()
{
	twpllclk_init_768M();
	/* Initialize UART */
	uart_init(115200, 8, 1, 0);

	apcpu_ts_init();
	asm volatile("dsb sy");
	asm volatile("isb");
	intc_init();
	intc_enable(0);

	//


}

uint64_t arch_timer_tick()
{
	return arch_cntvct_get();
}



