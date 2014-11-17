#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "Arduino.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BUTTON_LEFT       1
#define BUTTON_UP         2
#define BUTTON_OK        16
#define BUTTON_RIGHT     32
#define BUTTON_DOWN      64
#define BUTTON_ROTARY   128

#define BUTTON_ALL     (1|2|16|32|64|128)

#define BUTTONS ((PINF&BUTTON_ALL)^BUTTON_ALL)  

#ifdef __cplusplus
}
#endif

#endif


