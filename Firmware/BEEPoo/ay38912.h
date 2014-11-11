#ifndef AY38912_H_
#define AY38912_H_

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

#define AY_CH_A_FINETUNE    0
#define AY_CH_A_COARSETUNE  1
#define AY_CH_B_FINETUNE    2
#define AY_CH_B_COARSETUNE  3
#define AY_CH_C_FINETUNE    4
#define AY_CH_C_COARSETUNE  5
#define AY_NOISE_PERIOD     6
#define AY_ENABLE           7
#define AY_CH_A_AMPLITUDE   8
#define AY_CH_B_AMPLITUDE   9
#define AY_CH_C_AMPLITUDE   10
#define AY_ENVELOPE_FINE    11
#define AY_ENVELOPE_COARSE  12
#define AY_ENVELOPE_SHAPE   13
#define AY_PORTA            14
#define AY_PORTB            15

void AyRegister(uint8_t address, uint8_t value);
void AyPort(uint8_t value);
void AyPortBit0(uint8_t bitNo);
void AyPortBit1(uint8_t bitNo);

  
#ifdef __cplusplus
}
#endif

#endif


