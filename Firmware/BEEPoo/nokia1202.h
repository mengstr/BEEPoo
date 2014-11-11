#ifndef NOKIA1202_H_
#define NOKIA1202_H_

#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

#define LCD_C           0         // Command mode for LCD
#define LCD_D           1         // Data mode LCD

#define LCD_CE_LOW      PORTE &= B10111111
#define LCD_CE_HIGH     PORTE |= B01000000

#define LCD_CLK_LOW     PORTB &= B11111101
#define LCD_CLK_HIGH    PORTB |= B00000010

#define LCD_DI_LOW      PORTB &= B11111011
#define LCD_DI_HIGH     PORTB |= B00000100

#define TOGGLECLK      __asm__("nop\n\t"); LCD_CLK_HIGH; __asm__("nop\n\t"); LCD_CLK_LOW; __asm__("nop\n\t");


void LcdInit(void);
void LcdXY(uint8_t x, uint8_t y);
void LcdClear(void);
void LcdFill(void);
void LcdCharacter(char ch);
void LcdString(char *string);

#ifdef __cplusplus
}
#endif

#endif

