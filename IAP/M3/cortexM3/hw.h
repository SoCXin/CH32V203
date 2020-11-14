#ifndef _HW_H
#define _HW_H

#ifdef __cplusplus
extern "C" {
#endif

unsigned long hw_interrupt_disable(void);
void hw_interrupt_enable(unsigned long level);
	
#ifdef __cplusplus
}
#endif

#endif
