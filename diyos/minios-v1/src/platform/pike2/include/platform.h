
#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#define ARCH_TIMER_FREQ 26000000
#define ARCH_MS_FREQ 26000
#define SYSTICK 260000
#define HZ 100

/* Prototypes */
void apcpu_ts_init();
void intc_enable(int irq);
void intc_init();
void platform_init();


#endif /* */
