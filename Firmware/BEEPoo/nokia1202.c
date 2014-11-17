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

#include "nokia1202.h"

//
// A 5x8 pixel font for the Nokia 1202 LCD
//
static const uint8_t font5px[][5] ={
    {0x00, 0x00, 0x00, 0x00, 0x00} // 20
   ,{0x00, 0x00, 0x5f, 0x00, 0x00} // 21 !
   ,{0x00, 0x07, 0x00, 0x07, 0x00} // 22 "
   ,{0x14, 0x7f, 0x14, 0x7f, 0x14} // 23 #
   ,{0x24, 0x2a, 0x7f, 0x2a, 0x12} // 24 $
   ,{0x23, 0x13, 0x08, 0x64, 0x62} // 25 %
   ,{0x36, 0x49, 0x55, 0x22, 0x50} // 26 &
   ,{0x00, 0x05, 0x03, 0x00, 0x00} // 27 '
   ,{0x00, 0x1c, 0x22, 0x41, 0x00} // 28 (
   ,{0x00, 0x41, 0x22, 0x1c, 0x00} // 29 )
   ,{0x14, 0x08, 0x3e, 0x08, 0x14} // 2a *
   ,{0x08, 0x08, 0x3e, 0x08, 0x08} // 2b +
   ,{0x00, 0x50, 0x30, 0x00, 0x00} // 2c ,
   ,{0x08, 0x08, 0x08, 0x08, 0x08} // 2d -
   ,{0x00, 0x60, 0x60, 0x00, 0x00} // 2e .
   ,{0x20, 0x10, 0x08, 0x04, 0x02} // 2f /
   ,{0x3e, 0x51, 0x49, 0x45, 0x3e} // 30 0
   ,{0x00, 0x42, 0x7f, 0x40, 0x00} // 31 1
   ,{0x42, 0x61, 0x51, 0x49, 0x46} // 32 2
   ,{0x21, 0x41, 0x45, 0x4b, 0x31} // 33 3
   ,{0x18, 0x14, 0x12, 0x7f, 0x10} // 34 4
   ,{0x27, 0x45, 0x45, 0x45, 0x39} // 35 5
   ,{0x3c, 0x4a, 0x49, 0x49, 0x30} // 36 6
   ,{0x01, 0x71, 0x09, 0x05, 0x03} // 37 7
   ,{0x36, 0x49, 0x49, 0x49, 0x36} // 38 8
   ,{0x06, 0x49, 0x49, 0x29, 0x1e} // 39 9
   ,{0x00, 0x36, 0x36, 0x00, 0x00} // 3a :
   ,{0x00, 0x56, 0x36, 0x00, 0x00} // 3b ;
   ,{0x08, 0x14, 0x22, 0x41, 0x00} // 3c <
   ,{0x14, 0x14, 0x14, 0x14, 0x14} // 3d =
   ,{0x00, 0x41, 0x22, 0x14, 0x08} // 3e >
   ,{0x02, 0x01, 0x51, 0x09, 0x06} // 3f ?
   ,{0x32, 0x49, 0x79, 0x41, 0x3e} // 40 @
   ,{0x7e, 0x11, 0x11, 0x11, 0x7e} // 41 A
   ,{0x7f, 0x49, 0x49, 0x49, 0x36} // 42 B
   ,{0x3e, 0x41, 0x41, 0x41, 0x22} // 43 C
   ,{0x7f, 0x41, 0x41, 0x22, 0x1c} // 44 D
   ,{0x7f, 0x49, 0x49, 0x49, 0x41} // 45 E
   ,{0x7f, 0x09, 0x09, 0x09, 0x01} // 46 F
   ,{0x3e, 0x41, 0x49, 0x49, 0x7a} // 47 G
   ,{0x7f, 0x08, 0x08, 0x08, 0x7f} // 48 H
   ,{0x00, 0x41, 0x7f, 0x41, 0x00} // 49 I
   ,{0x20, 0x40, 0x41, 0x3f, 0x01} // 4a J
   ,{0x7f, 0x08, 0x14, 0x22, 0x41} // 4b K
   ,{0x7f, 0x40, 0x40, 0x40, 0x40} // 4c L
   ,{0x7f, 0x02, 0x0c, 0x02, 0x7f} // 4d M
   ,{0x7f, 0x04, 0x08, 0x10, 0x7f} // 4e N
   ,{0x3e, 0x41, 0x41, 0x41, 0x3e} // 4f O
   ,{0x7f, 0x09, 0x09, 0x09, 0x06} // 50 P
   ,{0x3e, 0x41, 0x51, 0x21, 0x5e} // 51 Q
   ,{0x7f, 0x09, 0x19, 0x29, 0x46} // 52 R
   ,{0x46, 0x49, 0x49, 0x49, 0x31} // 53 S
   ,{0x01, 0x01, 0x7f, 0x01, 0x01} // 54 T
   ,{0x3f, 0x40, 0x40, 0x40, 0x3f} // 55 U
   ,{0x1f, 0x20, 0x40, 0x20, 0x1f} // 56 V
   ,{0x3f, 0x40, 0x38, 0x40, 0x3f} // 57 W
   ,{0x63, 0x14, 0x08, 0x14, 0x63} // 58 X
   ,{0x07, 0x08, 0x70, 0x08, 0x07} // 59 Y
   ,{0x61, 0x51, 0x49, 0x45, 0x43} // 5a Z
   ,{0x00, 0x7f, 0x41, 0x41, 0x00} // 5b [
   ,{0x02, 0x04, 0x08, 0x10, 0x20} // 5c ･
   ,{0x00, 0x41, 0x41, 0x7f, 0x00} // 5d ]
   ,{0x04, 0x02, 0x01, 0x02, 0x04} // 5e ^
   ,{0x40, 0x40, 0x40, 0x40, 0x40} // 5f _
   ,{0x00, 0x01, 0x02, 0x04, 0x00} // 60 `
   ,{0x20, 0x54, 0x54, 0x54, 0x78} // 61 a
   ,{0x7f, 0x48, 0x44, 0x44, 0x38} // 62 b
   ,{0x38, 0x44, 0x44, 0x44, 0x20} // 63 c
   ,{0x38, 0x44, 0x44, 0x48, 0x7f} // 64 d
   ,{0x38, 0x54, 0x54, 0x54, 0x18} // 65 e
   ,{0x08, 0x7e, 0x09, 0x01, 0x02} // 66 f
   ,{0x0c, 0x52, 0x52, 0x52, 0x3e} // 67 g
   ,{0x7f, 0x08, 0x04, 0x04, 0x78} // 68 h
   ,{0x00, 0x44, 0x7d, 0x40, 0x00} // 69 i
   ,{0x20, 0x40, 0x44, 0x3d, 0x00} // 6a j
   ,{0x7f, 0x10, 0x28, 0x44, 0x00} // 6b k
   ,{0x00, 0x41, 0x7f, 0x40, 0x00} // 6c l
   ,{0x7c, 0x04, 0x18, 0x04, 0x78} // 6d m
   ,{0x7c, 0x08, 0x04, 0x04, 0x78} // 6e n
   ,{0x38, 0x44, 0x44, 0x44, 0x38} // 6f o
   ,{0x7c, 0x14, 0x14, 0x14, 0x08} // 70 p
   ,{0x08, 0x14, 0x14, 0x18, 0x7c} // 71 q
   ,{0x7c, 0x08, 0x04, 0x04, 0x08} // 72 r
   ,{0x48, 0x54, 0x54, 0x54, 0x20} // 73 s
   ,{0x04, 0x3f, 0x44, 0x40, 0x20} // 74 t
   ,{0x3c, 0x40, 0x40, 0x20, 0x7c} // 75 u
   ,{0x1c, 0x20, 0x40, 0x20, 0x1c} // 76 v
   ,{0x3c, 0x40, 0x30, 0x40, 0x3c} // 77 w
   ,{0x44, 0x28, 0x10, 0x28, 0x44} // 78 x
   ,{0x0c, 0x50, 0x50, 0x50, 0x3c} // 79 y
   ,{0x44, 0x64, 0x54, 0x4c, 0x44} // 7a z
   ,{0x00, 0x08, 0x36, 0x41, 0x00} // 7b {
   ,{0x00, 0x00, 0x7f, 0x00, 0x00} // 7c |
   ,{0x00, 0x41, 0x36, 0x08, 0x00} // 7d }
   ,{0x10, 0x08, 0x08, 0x10, 0x08} // 7e ?
   ,{0x00, 0x7F, 0x3E, 0x1C, 0x08} // 7f ?
};



//
// Send a byte to the display, either as a command or data depending on the "cd" flag
//
void LcdSend(uint8_t cd, uint8_t data) {
  LCD_CE_LOW;                                   // Enable LCD ~CE
  if (cd==0) LCD_DI_LOW; else LCD_DI_HIGH;      // Command/Data-bit
  TOGGLECLK;
  SpiEnable(1);
  SpiSend(data);
  SpiDisable();
  LCD_CE_HIGH;                                  // Disable LCD ~CE
}



//
// Set the cursor position for LcdCharacter()/LcdString()
// The x coordinate is in pixels, the y coordinate is the line
// ranging from 0 to 7
//
void LcdXY(uint8_t x, uint8_t y) {
  LcdSend(LCD_C,0xB0 | (y & 0x0F));
  LcdSend(LCD_C,0x10 | ((x >> 4) & 0x07));
  LcdSend(LCD_C,0x00 | (x & 0x0F));
}

//
//
//
void LcdClear(void){
  uint16_t i;
  LcdXY(0,0);
  for (i=0; i<96*9 ; i++) {
    LcdSend(LCD_D, 0x00);
  }
}


//
//
//
void LcdFill(void) {
  uint16_t i;
  LcdXY(0,0);
  for (i=0; i<96*9 ; i++) {
    LcdSend(LCD_D, 0xFF);
  }
}


//
// Print a single character on the display
//
void LcdCharacter(char ch) {
  uint16_t index;

  LcdSend(LCD_D, 0x00);
  for (index=0; index<5; index++) {
    LcdSend(LCD_D, font5px[ch-0x20][index]);
  }
}


//
// Print a null-terminated string on the display
//
void LcdString(char *string){
  while (*string) {
    LcdCharacter(*string++);
  }
}


//
// Print an uint16_t as a string with 0-padding
//
void LcdPrintUint16(uint16_t value) {
  static uint16_t powers[]={10,100,1000,10000,100000,1000000,10000000,100000000,1000000000};
  uint16_t power; 
  char digit;
  char i;

  for (i=4; i>=0; i--) {
    digit='0';
    power=powers[i];
    while (value >= power) {
      value -= power;
      digit++;
    }
    LcdCharacter(digit);
  }
  LcdCharacter('0'+value);
}

//
// Send all the required initialization commands to the display
//
void LcdInit(void) {
  LcdSend(LCD_C, 0xE2);  // Software Reset
  delay(1);
  LcdSend(LCD_C, 0x3D);  // Charge pump ON
  LcdSend(LCD_C, 0x01);  // Charge pump=4
  LcdSend(LCD_C, 0xA4);  // Display all points = OFF
  LcdSend(LCD_C, 0x2F);  // Booster=ON, Voltage Reg=ON, Voltage Follower=ON
  LcdSend(LCD_C, 0xAF);  // Display ON
  LcdSend(LCD_C, 0xA6);  // Normal display
  LcdSend(LCD_C, 0xC0);  // Normal screen up/down
  LcdSend(LCD_C, 0xA0);  // Normal screen left/right
  LcdClear();
}

