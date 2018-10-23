
#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

void _e_undef(void);
void _e_svc(void);
void _e_pref_abort(void);
void _e_data_abort(void);
void _e_irq(void);
void _e_fiq(void);

#endif /* _EXCEPTION_H_ */
