// //
// // BEEPoo - Standalone AY-3-8912 player with ATMEGA32U4 with Arduino bootloader
// //
// // Copyright (c) 2014 SmallRoomLabs / Mats Engstrom - mats.engstrom@gmail.com
// // 
// // Hardware and Firmware released under the MIT License (MIT) than can be
// // found in full in the file named LICENCE in the git repository.
// //
// // https://github.com/SmallRoomLabs/BEEPoo
// //

// #include "Arduino.h"
// #include <stdint.h>

// #include "spi.h"
// #include "sd.h"

// #define CS_DISABLE  	PORTB |= B00000001
// #define CS_ENABLE		PORTB &= B11111110

// static uint32_t sd_sector;
// static uint16_t sd_pos;


// //
// //
// //
// uint8_t SdCommand(uint8_t cmd, uint32_t arg, uint8_t crc, uint8_t read) {
// 	uint8_t i;
// 	uint8_t buffer[32];
// 	uint8_t ret=0xFF;

// 	CS_ENABLE;
// 	SpiSend(cmd);
// 	SpiSend(arg>>24);
// 	SpiSend(arg>>16);
// 	SpiSend(arg>>8);
// 	SpiSend(arg);
// 	SpiSend(crc);

// 	for (i=0; i<read; i++) {
// 		buffer[i]=SpiSend(0xFF);
// 	}

// 	CS_DISABLE;		

// 	for (i=0; i<read; i++) {
// 		if (buffer[i]!=0xFF) ret=buffer[i];
// 	}

// 	return ret;
// }


// //
// //
// //
// char SdInit(void) {
// 	char i;

// 	SpiEnable(0);	// Slow speed

// 	// ]r:10
// 	CS_DISABLE;
// 	for (i=0; i<10; i++) { // idle for 1 bytes / 80 clocks
// 		SpiSend(0xFF);
// 	}

// 	// [0x40 0x00 0x00 0x00 0x00 0x95 r:8] until we get "1"
// 	for (i=0; i<10 && SdCommand(0x40, 0x00000000, 0x95, 8) != 1; i++) {
// 		delay(100);
// 	}
// 	if (i==10) { // card did not respond to initialization
// 		SpiDisable();
// 		return -1; 
// 	}

// 	// CMD1 until card comes out of idle, but maximum of 10 times
// 	for(i=0; i<10 && SdCommand(0x41, 0x00000000, 0xFF, 8) != 0; i++) {
// 		delay(100);
// 	}
// 	if (i==10) {	// card did not come out of idle
// 		SpiDisable();
// 		return -2; 
// 	}

// 	// SET_BLOCKLEN to 512
// 	SdCommand(0x50, 0x00000200, 0xFF, 8);
// 	sd_sector=0;
// 	sd_pos= 0;

// 	SpiDisable();
// 	return 0;
// }


// //
// // TODO: This function will not exit gracefully if SD card does not do what it should
// //
// void SdRead(uint32_t sector, uint16_t offset, uint8_t *buffer, uint16_t len) {
//     uint16_t i;
    
// 	SpiEnable(0);	// Slow speed
//     CS_ENABLE;
//     SpiSend(0x51);
//     SpiSend(sector>>15); // sector*512 >> 24
//     SpiSend(sector>>7);  // sector*512 >> 16
//     SpiSend(sector<<1);  // sector*512 >> 8
//     SpiSend(0);          // sector*512
//     SpiSend(0xFF);
    
//     for(i=0; i<10 && SpiSend(0xFF) != 0x00; i++) {} // wait for 0
//     for(i=0; i<10 && SpiSend(0xFF) != 0xFE; i++) {} // wait for data start
    
// 	// "skip" bytes
//     for(i=0; i<offset; i++) {
//         SpiSend(0xFF);
//     }
        
// 	// read len bytes
//     for(i=0; i<len; i++) {
//         buffer[i]=SpiSend(0xFF);
//     }
        
//  	// "skip" again
//     for(i+=offset; i<512; i++) {
//         SpiSend(0xFF);
//     }
        
//     // skip checksum
//     SpiSend(0xFF);
//     SpiSend(0xFF);    

//     CS_DISABLE;
//     SpiDisable();
// }


// static uint8_t buf[512];


// //
// //
// //
// char SdReadBuf(uint8_t bytes) {
//     SdRead(sd_sector, sd_pos, buf, bytes);
//     sd_pos+=(uint16_t)bytes;
    
//     if(sd_pos==512) {
//         sd_pos=0;
//         sd_sector++;
//     }
    
// 	return bytes;
// }
