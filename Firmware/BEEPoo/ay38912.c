//
// BEEPoo - Standalone AY-3-8912 player with ATMEGA32U4 with Arduino bootloader
//
// Copyright (c) 2014 SmallRoomLabs / Mats Engstrom - mats.engstrom@gmail.com
// 
// Hardware and Firmware released under the MIT License (MIT) than can be
// found in full in the file named LICENCE in the git repository.
//
// https://github.com/SmallRoomLabs/BEEPoo
//

#include "Arduino.h"
#include <stdint.h>

#include "ay38912.h"

#define BC1_LOW      PORTB &= B10111111
#define BC1_HIGH     PORTB |= B01000000

#define DIR_LOW      PORTE &= B11111011
#define DIR_HIGH     PORTE |= B00000100

static uint8_t ayPort;


//
//
//
void AYinactive(void) {
  DIR_LOW;
  __asm__("nop\n\t");
  BC1_LOW;
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
}  


//
//
//
void AYlatch(void) {
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  BC1_HIGH;
  __asm__("nop\n\t");
  DIR_HIGH;
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
}


//
//
//
void AYwrite(void) {
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  BC1_LOW;
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  DIR_HIGH;
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
  __asm__("nop\n\t");
}


//
// Write "value" to the register in the AY-3-8912 pointed to by "address"
//
void AyRegister(uint8_t address, uint8_t value) {
  AYinactive();
  PORTD=address;
  AYlatch();
  AYinactive();
  AYwrite();
  PORTD=value;
  AYinactive();
}


//
// Set the value of the AY-3-8912 PORT A
//
void AyPort(uint8_t value) {
    ayPort=value;
    AyRegister(AY_PORTA, value);
}


//
//
//
void AyPortBit0(uint8_t bitNo) {
  ayPort&=(~(1<<bitNo));
  AyRegister(AY_PORTA, ayPort);
}


//
//
//
void AyPortBit1(uint8_t bitNo) {
  ayPort|=(1<<bitNo);
  AyRegister(AY_PORTA, ayPort);
}
