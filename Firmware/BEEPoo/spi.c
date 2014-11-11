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
#include "spi.h"

// SPI clock modes
#define SPI_MODE_0 0x00 /* Sample (Rising) Setup (Falling) CPOL=0, CPHA=0 */
#define SPI_MODE_1 0x01 /* Setup (Rising) Sample (Falling) CPOL=0, CPHA=1 */
#define SPI_MODE_2 0x02 /* Sample (Falling) Setup (Rising) CPOL=1, CPHA=0 */
#define SPI_MODE_3 0x03 /* Setup (Falling) Sample (Rising) CPOL=1, CPHA=1 */

// data direction
#define SPI_LSB 1 /* send least significant bit (bit 0) first */
#define SPI_MSB 0 /* send most significant bit (bit 7) first */

// whether to raise interrupt when data received (SPIF bit received)
#define SPI_NO_INTERRUPT 0
#define SPI_INTERRUPT 1

// slave or master with clock diviser
#define SPI_SLAVE 0xF0
#define SPI_MSTR_CLK4 0x00 /* chip clock/4 */
#define SPI_MSTR_CLK16 0x01 /* chip clock/16 */
#define SPI_MSTR_CLK64 0x02 /* chip clock/64 */
#define SPI_MSTR_CLK128 0x03 /* chip clock/128 */
#define SPI_MSTR_CLK2 0x04 /* chip clock/2 */
#define SPI_MSTR_CLK8 0x05 /* chip clock/8 */
#define SPI_MSTR_CLK32 0x06 /* chip clock/32 */



void SpiSetup(uint8_t mode, int dord, int interrupt, uint8_t clock) {
  SPSR = (((clock & 0x04) == 4) << SPI2X); // clock divisor SPI2X
  SPCR = ((interrupt ? 1 : 0)<<SPIE)       // interrupt enabled
    | (1<<SPE)                             // enable SPI
    | (dord<<DORD)                         // LSB or MSB
    | (1<<MSTR)                            // Master
    | (((mode & 0x02) == 2) << CPOL)       // clock timing mode CPOL
    | (((mode & 0x01)) << CPHA)            // clock timing mode CPHA
    | (((clock & 0x02) == 2) << SPR1)      // cpu clock divisor SPR1
    | ((clock & 0x01) << SPR0);            // cpu clock divisor SPR0
}


//
//
//
void SpiDisable(void) {
  SPCR=0;
}


//
//
//
void SpiEnable(uint8_t fast) {
  if (fast) {
    SpiSetup(SPI_MODE_0, SPI_MSB, SPI_NO_INTERRUPT, SPI_MSTR_CLK2);
  } else {
    SpiSetup(SPI_MODE_0, SPI_MSB, SPI_NO_INTERRUPT, SPI_MSTR_CLK128);
  }
}


//
//
//
uint8_t SpiSend(uint8_t data) {
  SPDR=data;
  while (!(SPSR & (1<<SPIF)));
  return SPDR;
}


