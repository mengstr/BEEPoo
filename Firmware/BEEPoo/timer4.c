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

#include "timer4.h"

#define TIMER4_RESOLUTION 1023UL
#define PLL_FREQ 48000000UL
 
static unsigned long pwmPeriod = 0; 

//
//
//
void Timer4Start(void) {
  TCCR4A |= _BV(COM4A1);
}


//
//
//
void Timer4Stop(void)  {
  TCCR4A &= ~(_BV(COM4A1));
}


//
//
//
void Timer4SetPeriod(unsigned long freq)  {
  unsigned long cycles = PLL_FREQ / 2 / freq;
  unsigned char clockSelectBits = 0;
 
  if (cycles < TIMER4_RESOLUTION) {
    clockSelectBits = _BV(CS40);
    pwmPeriod = cycles;
  } else if (cycles < TIMER4_RESOLUTION * 2) {
    clockSelectBits = _BV(CS41);
    pwmPeriod = cycles / 2;
  } else if (cycles < TIMER4_RESOLUTION * 4) {
    clockSelectBits = _BV(CS41) | _BV(CS40);
    pwmPeriod = cycles / 4;
  } else if (cycles < TIMER4_RESOLUTION * 8) {
    clockSelectBits = _BV(CS42);
    pwmPeriod = cycles / 8;
  } else if (cycles < TIMER4_RESOLUTION * 16) {
    clockSelectBits = _BV(CS42) | _BV(CS40);
    pwmPeriod = cycles / 16;
  } else if (cycles < TIMER4_RESOLUTION * 32) {
    clockSelectBits = _BV(CS42) | _BV(CS41);
    pwmPeriod = cycles / 32;
  } else if (cycles < TIMER4_RESOLUTION * 64) {
    clockSelectBits = _BV(CS42) | _BV(CS41) | _BV(CS40);
    pwmPeriod = cycles / 64;
  } else if (cycles < TIMER4_RESOLUTION * 128) {
    clockSelectBits = _BV(CS43);
    pwmPeriod = cycles / 128;
  } else if (cycles < TIMER4_RESOLUTION * 256) {
    clockSelectBits = _BV(CS43) | _BV(CS40);
    pwmPeriod = cycles / 256;
  } else if (cycles < TIMER4_RESOLUTION * 512) {
    clockSelectBits = _BV(CS43) | _BV(CS41);
    pwmPeriod = cycles / 512;
  } else if (cycles < TIMER4_RESOLUTION * 1024) {
    clockSelectBits = _BV(CS43) | _BV(CS41) | _BV(CS40);
    pwmPeriod = cycles / 1024;
  } else if (cycles < TIMER4_RESOLUTION * 2048) {
    clockSelectBits = _BV(CS43) | _BV(CS42);
    pwmPeriod = cycles / 2048;
  } else if (cycles < TIMER4_RESOLUTION * 4096) {
    clockSelectBits = _BV(CS43) | _BV(CS42) | _BV(CS40);
    pwmPeriod = cycles / 4096;
  } else if (cycles < TIMER4_RESOLUTION * 8192) {
    clockSelectBits = _BV(CS43) | _BV(CS42) | _BV(CS41);
    pwmPeriod = cycles / 8192;
  } else if (cycles < TIMER4_RESOLUTION * 16384) {        
    clockSelectBits = _BV(CS43) | _BV(CS42) | _BV(CS41) | _BV(CS40);        
    pwmPeriod = cycles / 16384;     
  }  
    // else           
    //clockSelectBits = _BV(CS43) | _BV(CS42) | _BV(CS41) | _BV(CS40);        
    //  pwmPeriod = TIMER4_RESOLUTION - 1; 
  TCCR4B = clockSelectBits;             
  TC4H = pwmPeriod >> 8; 
  OCR4C = pwmPeriod; 
}


//
//
//
void InitTimer4(unsigned long freq) {
   // Init the internal PLL */
   PLLFRQ = _BV(PDIV2);
   PLLCSR = _BV(PLLE);
   while(!(PLLCSR & _BV(PLOCK)));
   PLLFRQ |= _BV(PLLTM0); /* PCK 48MHz */
 
   TCCR4A = (1<<PWM4A);  
   TCCR4E = (1<<ENHC4); 
   TCCR4D = (1<<WGM40); //set it to phase and frequency correct mode    
   TCCR4C = 0;     
   Timer4SetPeriod(freq);    
} 


//
//
//
void SetTimer4Duty(unsigned int duty) {    
   unsigned long dutyCycle = pwmPeriod;     
   dutyCycle *= duty;   
   dutyCycle >>= 9;
   TC4H = (dutyCycle) >> 8; 
   OCR4A = (dutyCycle) & 255; 
}

