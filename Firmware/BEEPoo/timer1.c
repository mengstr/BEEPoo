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
// Part of the timer4 code generation the 1MHz clock to the AY-3-8912 chip is copied from
// http://harizanov.com/2013/04/crazy-high-frequency-pwm-with-atmega32u4/
//

#include "Arduino.h"
#include <stdint.h>

#include "timer1.h"

volatile uint16_t timer1Tick;
static uint16_t reloadValue; 

//
//
//
void InitTimer1(uint16_t _reloadValue) {
  reloadValue=_reloadValue;

  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = reloadValue;
  TCCR1B |= (1 << CS12);    // 256 prescaler 
  TIMSK1 |= (1 << TOIE1);   // enable timer overflow interrupt
}

//
//
//
void IrqTimer1(void) {
  TCNT1 = reloadValue;
  timer1Tick++;
}
