
#ifndef _INTC_H_
#define _INTC_H_

#define INTC0_BASE 0x71400000
#define INTC1_BASE 0x71500000
#define INTC2_BASE 0x71600000
#define INTC3_BASE 0x71700000

void intc_enable(int irq);
void intc_irq_enable(int irq);
void intc_irq_disable(int irq);
#endif 
