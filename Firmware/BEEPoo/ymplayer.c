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

//
//
// 00000000  59 4d 35 21 4c 65 4f 6e  41 72 44 21 00 00 36 9b  |YM5!LeOnArD!..6.|
// 00000010  00 00 00 01 00 00 00 1e  84 80 00 32 00 00 00 00  |...........2....|
// 00000020  00 00 4c 61 73 74 20 4e  69 6e 6a 61 00 4d 61 64  |..Last Ninja.Mad|
// 00000030  20 4d 61 78 00 47 65 6e  65 72 61 74 65 64 20 77  | Max.Generated w|
// 00000040  69 74 68 20 53 61 69 6e  54 00 a6 9e 96 96 9e a6  |ith SainT.......|
// 00000050  ae b6 ae a6 9e 96 96 9e  a6 ae b6 ae a6 9e 96 96  |................|
// 00000060  9e a6 ae b6 ae a6 9e 96  96 9e a6 ae b6 ae a6 9e  |................|
// 00000070  96 96 9e a6 ae b6 ae a6  9e 96 96 9e a6 ae b6 ae  |................|
//
//
//
// fileid 		59 4d 35 21 				YM5!
// check$		4c 65 4f 6e 41 72 44 21 	LeOnArD!
// frame# 		00 00 36 9b
// attributes 	00 00 00 01 
// drums# 		00 00 
// clock 		00 1e 84 80 
// framerate 	00 32 
// loopframe	00 00 00 00
// skip bytes 	00 00 
// song name 	4c 61 73 74 20 4e 69 6e 6a 61 00 
// author name 4d 61 64 20 4d 61 78 00 
// comment 	47 65 6e  65 72 61 74 65 64 20 77 69 74 68 20 53 61 69 6e 54 00 
// data 		a6 9e 96 96 9e a6 ae b6 ae a6 9e 96 96 9e a6 ae 
// 			b6 ae a6 9e 96 96 9e a6 ae b6 ae a6 9e 96 96 9e 
// 			a6 ae b6 ae a6 9e 96 96 9e a6 ae b6 ae a6 9e 96 
//
//
//
// 0 	LWORD		4 	File ID "YM6!"
// 4 	STRING[8]	8 	Check string "LeOnArD!"
// 12 	LWORD		4 	Nb of frame in the file
// 16	LWORD		4 	Song attributes
// 20 	WORD 		2 	Nb of digidrum samples in file (can be 0)
// 22 	LWORD		4 	YM master clock in Hz(2000000=ATARI-ST,  1773400=ZX-SPECTRUM)
// 26 	WORD		2 	Original player frame in Hz (traditionnaly 50)
// 28 	LWORD 		4 	Loop frame (traditionnaly 0 to loop at the beginning)
// 32 	WORD 		2 	Bytes of extra data to skip
//
// Then, for each digidrum: (nothing if no digidrum)
// 34 	LWORD 		4 	Sample size
// 38 	BYTES 		n 	Sample data (8 bits sample)
//
// Then some additionnal informations
// ? 	NT-String 	? 	Song name
// ?	NT-String 	?	Author name
// ?	NT-String	?	Song comment
// ?	BYTES 		?	YM register data bytes. (r0,r1,r2....,r15 for each frame). Order depend on the "interleaved" bit. It takes 16*nbFrame bytes.
//
// ?	LWORD		4 	End ID marker. Must be "End!"
