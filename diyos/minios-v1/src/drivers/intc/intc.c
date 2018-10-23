#include <types.h>
#include <platform.h>
#include <intc.h>

void intc_enable(int irq)
{
	REG32(INTC3_BASE+0x8) |= (BIT_14|BIT_15|BIT_16|BIT_17|BIT_18|BIT_19|BIT_20|BIT_21);
}

void intc_irq_enable(int irq)
{
	REG32(INTC0_BASE+0x08) |= (1<<irq);
//	char data[10];
//	ksprintf(data, "intc%d",0);

}

void intc_irq_disable(int irq)
{
	REG32(INTC0_BASE+0x0c) |= (1<<irq);
//	char data[10];
//	ksprintf(data, "intc%d",0);

}




