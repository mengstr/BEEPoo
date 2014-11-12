#ifndef ROTARYENC_H
#define	ROTARYENC_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>

//#define HALFSTEP
#define DIR_NONE 0x0    // No complete step yet.
#define DIR_CW 0x10     // Clockwise step.
#define DIR_CCW 0x20    // Anti-clockwise step.

extern volatile int16_t rotaryValue;

void IrqRotaryEncoder(void);

#ifdef	__cplusplus
}
#endif

#endif	/* ROTARYENC_H */
