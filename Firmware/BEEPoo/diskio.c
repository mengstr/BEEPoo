/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for Petit FatFs (C)ChaN, 2014      */
/*-----------------------------------------------------------------------*/
#include <Arduino.h>

#include "diskio.h"
#include "ay38912.h"


// Definitions for MMC/SDC command
#define CMD0	(0x40+0)	/* GO_IDLE_STATE */
#define CMD1	(0x40+1)	/* SEND_OP_COND (MMC) */
#define	ACMD41	(0xC0+41)	/* SEND_OP_COND (SDC) */
#define CMD8	(0x40+8)	/* SEND_IF_COND */
#define CMD16	(0x40+16)	/* SET_BLOCKLEN */
#define CMD17	(0x40+17)	/* READ_SINGLE_BLOCK */
#define CMD24	(0x40+24)	/* WRITE_BLOCK */
#define CMD55	(0x40+55)	/* APP_CMD */
#define CMD58	(0x40+58)	/* READ_OCR */


/* Card type flags (CardType) */
#define CT_MMC				0x01	/* MMC ver 3 */
#define CT_SD1				0x02	/* SD ver 1 */
#define CT_SD2				0x04	/* SD ver 2 */
#define CT_BLOCK			0x08	/* Block addressing */


static BYTE CardType;

#define DESELECT()  	PORTB |= B00000001
#define SELECT()		PORTB &= B11111110



/*-----------------------------------------------------------------------*/
/* Send a command packet to MMC                                          */
/*-----------------------------------------------------------------------*/

static BYTE send_cmd (
	BYTE cmd,		/* 1st byte (Start + Index) */
	DWORD arg		/* Argument (32 bits) */
)
{
	BYTE n, res;

	if (cmd & 0x80) {	/* ACMD<n> is the command sequense of CMD55-CMD<n> */
		cmd &= 0x7F;
		res = send_cmd(CMD55, 0);
		if (res > 1) return res;
	}

	SELECT();
	for (n=0; n<100; n++) {
		res=SpiSend(0xFF);
		if (res==0xFF) break;
	}

	/* Send a command packet */
	SpiSend(cmd);						/* Start + Command index */
	SpiSend((BYTE)(arg >> 24));			/* Argument[31..24] */
	SpiSend((BYTE)(arg >> 16));			/* Argument[23..16] */
	SpiSend((BYTE)(arg >> 8));			/* Argument[15..8] */
	SpiSend((BYTE)arg);					/* Argument[7..0] */
	n = 0x01;							/* Dummy CRC + Stop */
	if (cmd == CMD0) n = 0x95;			/* Valid CRC for CMD0(0) */
	if (cmd == CMD8) n = 0x87;			/* Valid CRC for CMD8(0x1AA) */
	SpiSend(n);

	/* Receive a command response */
	for (n=0; n<100; n++) {
		res=SpiSend(0xFF);
		if (res<128) break;
	}

//	DESELECT();
	return res;			/* Return with the response value */
}




/*-----------------------------------------------------------------------*/
/* Initialize Disk Drive                                                 */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(void) {
	BYTE n, cmd, ty, ocr[4];
	UINT tmr;

	AyPort(0);
#if _USE_WRITE
	if (CardType && SELECTING) disk_writep(0, 0);	/* Finalize write process if it is in progress */
#endif
	SpiEnable(0);	// Slow speed
	DESELECT();
	for (n = 10; n; n--) SpiSend(0xFF);	/* 80 dummy clocks with CS=H */

	ty = 0;
	if (send_cmd(CMD0, 0) == 1) {			/* GO_IDLE_STATE */
		if (send_cmd(CMD8, 0x1AA) == 1) {	/* SDv2 */
			AyPortBit1(0);
			// Get trailing return value of R7 resp
			for (n=0; n<4; n++) ocr[n]=SpiSend(0xFF);		
			// Check if the card can work at vdd range of 2.7-3.6V
			if (ocr[2] == 0x01 && ocr[3] == 0xAA) {
				AyPortBit1(1);
				for (tmr = 10000; tmr && send_cmd(ACMD41, 1UL << 30); tmr--) delay(1);	/* Wait for leaving idle state (ACMD41 with HCS bit) */
				if (tmr && send_cmd(CMD58, 0) == 0) {		/* Check CCS bit in the OCR */
					for (n = 0; n < 4; n++) ocr[n] = SpiSend(0xFF);
					ty = (ocr[0] & 0x40) ? CT_SD2 | CT_BLOCK : CT_SD2;	/* SDv2 (HC or SC) */
				}
			} else {
				AyPortBit1(2);
			}
		} else {							/* SDv1 or MMCv3 */
			AyPortBit1(1);
			if (send_cmd(ACMD41, 0) <= 1) 	{
				ty = CT_SD1; cmd = ACMD41;	/* SDv1 */
			} else {
				ty = CT_MMC; cmd = CMD1;	/* MMCv3 */
			}
			for (tmr = 10000; tmr && send_cmd(cmd, 0); tmr--) delay(1);	/* Wait for leaving idle state */
			if (!tmr || send_cmd(CMD16, 512) != 0) {			/* Set R/W block length to 512 */
				ty = 0;
			}
		}
	}
	CardType = ty;
	DESELECT();
	SpiSend(0xFF);
	SpiDisable();

	return ty ? 0 : STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Partial Sector                                                   */
/*-----------------------------------------------------------------------*/

DRESULT disk_readp (
	BYTE* buff,		/* Pointer to the destination object */
	DWORD sector,	/* Sector number (LBA) */
	UINT offset,	/* Offset in the sector */
	UINT count		/* Byte count (bit15:destination) */
)
{
	DRESULT res;
	BYTE rc;
	UINT bc;


	if (!(CardType & CT_BLOCK)) sector *= 512;	/* Convert to byte address if needed */

	SpiEnable(1);	// High speed
	res = RES_ERROR;
	if (send_cmd(CMD17, sector) == 0) {	/* READ_SINGLE_BLOCK */

		bc = 40000;	/* Time counter */
		do {				/* Wait for data packet */
			rc = SpiSend(0xFF);
		} while (rc == 0xFF && --bc);

		if (rc == 0xFE) {	/* A data packet arrived */

			bc = 512 + 2 - offset - count;	/* Number of trailing bytes to skip */

			/* Skip leading bytes */
			while (offset--) SpiSend(0xFF);

			/* Receive a part of the sector */
			do {
				*buff++ = SpiSend(0xFF);
			} while (--count);

			/* Skip trailing bytes and CRC */
			do SpiSend(0xFF); while (--bc);

			res = RES_OK;
		}
	}

	DESELECT();
	SpiSend(0xFF);
	SpiDisable();

	return res;
}



/*-----------------------------------------------------------------------*/
/* Write Partial Sector                                                  */
/*-----------------------------------------------------------------------*/
DRESULT disk_writep (
	const BYTE* buff,		/* Pointer to the data to be written, NULL:Initiate/Finalize write operation */
	DWORD sc		/* Sector number (LBA) or Number of bytes to send */
)
{
	DRESULT res;


	if (!buff) {
		if (sc) {

			// Initiate write process

		} else {

			// Finalize write process

		}
	} else {

		// Send data to the disk

	}

	return res;
}
















