#ifndef TIMER4_H_
#define TIMER4_H_

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

void InitTimer4(unsigned long freq);
void SetTimer4Duty(unsigned int duty);
void Timer4Start(void);
  
#ifdef __cplusplus
}
#endif

#endif


