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
#include "rotary.h"
#include "pff.h"
#include "buttons.h"
#include "selectFile.h"




//
//
//
static void scan_files_header(char* path, uint16_t startNo) {
  LcdClear();
  LcdXY(0,0);
  LcdCharacter('[');
  if (strlen(path)) {
    LcdString(path);
  } else {
    LcdCharacter('/');
  }
  LcdCharacter(']');
}


//
//
//
static uint16_t scan_files(char* path, uint16_t startNo) {
  FRESULT res;
  FILINFO fno;
  DIR dir;
  int i;
  int line;
  uint8_t headerprinted;
  uint16_t holdStartNo;

  headerprinted=0;
  holdStartNo=startNo;
  line=1;

  res=pf_opendir(&dir, path);
  if (res!=FR_OK) return -1;
  for (;;) {
    res=pf_readdir(&dir, &fno);
    if (res!=FR_OK || fno.fname[0]==0) break;
    // Ignore hidden or system files
    if (fno.fattrib&(AM_HID|AM_SYS)) continue;
    if (startNo>0) {
      startNo--;
    } else {
      if (!headerprinted) {
        headerprinted=1;
        scan_files_header(path, holdStartNo);
        if (path[0] && holdStartNo==0) {
          LcdXY(8,line++);
          LcdString("..");
        }
      }
      LcdXY(8,line++);
      if (!(fno.fattrib&AM_DIR)) {
        LcdString(fno.fname);
      } else {
        LcdCharacter('[');
        LcdString(fno.fname);
        LcdCharacter(']');
      }
    }
    if (line==8) break;
  }
  return line;
}


//
//
//
static void get_filename(char* path, uint16_t startNo, char *filename) {
  FRESULT res;
  FILINFO fno;
  DIR dir;

  if (path[0]) {
    if (startNo==0) {
      strcpy(filename,"..");
      return;
    }
    startNo--;
  }

  filename[0]=0;
  res=pf_opendir(&dir, path);
  if (res!=FR_OK) return;
  for (;;) {
    res=pf_readdir(&dir, &fno);
    if (res!=FR_OK || fno.fname[0]==0) break;
    // Ignore hidden or system files
    if (fno.fattrib&(AM_HID|AM_SYS)) continue;
    if (startNo>0) {
      startNo--;
    } else {
      strcpy(filename,fno.fname);
      return;
    }
  }
}




//
//
//
void SelectFile(char *selectedFile) {
  uint8_t i;
  int16_t lastRotaryValue;
  int16_t myRotary;
  int16_t scrollValue;
  char filename[16];
  char path[16];

  selectedFile[0]=0;
  path[0]=0;
  scrollValue=0;
  rotaryValue=0;
  lastRotaryValue=-1;
  scan_files(path,0);
  for (;;) {
    myRotary=rotaryValue;
    // files start at 0, negative values is not allowed
    if (myRotary<0) {
      rotaryValue=0;
      myRotary=0;
    }

    if (BUTTONS&(BUTTON_ROTARY|BUTTON_OK)) {
      delay(25);
      while (BUTTONS&(BUTTON_ROTARY|BUTTON_OK)) delay(25);      
      get_filename(path, myRotary, filename);
      if (strcmp(filename,"..")==0) {
        path[0]=0;
      } else {
        if (!path[0]) { 
          strcpy(path,filename);
        } else {
          strcpy(selectedFile,path);
          strcat(selectedFile,"/");
          strcat(selectedFile,filename);
          return;
        }
      }
      scrollValue=0;
      rotaryValue=0;
      myRotary=0;
      lastRotaryValue=-1;
      scan_files(path,0);
      delay(25);
    }

    // If same value as before we don't need to update screen
    if (myRotary==lastRotaryValue) continue;

    lastRotaryValue=myRotary;
    // Clear the cursor column
    for (i=1; i<8; i++) {
      LcdXY(0,i);
      LcdCharacter(' ');
    }

    // If we're still inside the window just move the cursor,
    // when going outside update the scroll position
    i=myRotary-scrollValue;
    if (i<0) {
      // Reached top of screen
      scrollValue=myRotary;
      scan_files(path,scrollValue);
    } else if ((i>=0) && (i<7)) {
      // Still inside the window
    } else if (i>=7) {
      // Reached to bottom of screen
      scrollValue=myRotary-6;
      scan_files(path,scrollValue);
    } 
    LcdXY(0,1+myRotary-scrollValue);
    LcdCharacter(127);
  }
}
