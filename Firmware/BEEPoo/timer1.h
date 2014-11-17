#ifndef TIMER1_H_
#define TIMER1_H_

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

extern volatile uint16_t timer1Tick;

void InitTimer1(uint16_t _reloadValue);
void IrqTimer1(void);
  

#ifdef __cplusplus
}
#endif

#endif


