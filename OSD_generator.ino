/*
 * NM1227A OSD IC control scetch
 * Thanks to oppeola for helping with
 * data format and to wuffe for helping with code
 */

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "Arduino.h"

/*Some library stuff:
 * void clear();
 * void home();
 * void noDisplay();
 * void display();
 * void setCursor();
 * void position();
 */

uint8_t _ldi_pin;
uint8_t _data_pins[7];
int c = 0;
int BLK =0b000000;
void setup() {
  osd_init(13,2,3,4,5,6,7,8);


  osd_write(127,127); //"reset lines"
  osd_write(62,0b111110); //this was in original controller data, some kind of init?
  osd_clear();
 
   for (int i = 0; i < 48; i++) { //prints char map
        osd_write(i,c);
      c++;
    }

//prints hello world on last row
  osd_write(48,7);
  osd_write(49,4);
  osd_write(50,11);
  osd_write(51,11);
  osd_write(52,17);
  osd_write(53,15);
  osd_write(54,25);
  osd_write(55,17);
  osd_write(56,20);
  osd_write(57,11);
  osd_write(58,3);
  osd_write(59,15);
  
  osd_write(60,20); //horizonal display position 0-63
  osd_write(61,20); //vertical display position  0-63
  
  osd_write(64,0b000001); //char blanking row
  osd_write(65,0b000001); //char blanking col
  osd_write(66,0); //char blanking col cont.
  bitSet(BLK,5);
  osd_write(62,BLK);  //bit 0-1 size, bit 4 blk, bit 5 blkb
 // osd_write(63,0); //DO0-DO3 outputs

  }

void loop() {

}

void osd_init(uint8_t ldi, uint8_t da0, uint8_t da1, uint8_t da2, uint8_t da3, uint8_t da4, uint8_t da5, uint8_t da6){
  _ldi_pin = ldi;
  _data_pins[0] = da0;
  _data_pins[1] = da1;
  _data_pins[2] = da2;
  _data_pins[3] = da3;
  _data_pins[4] = da4;
  _data_pins[5] = da5;
  _data_pins[6] = da6;
}

void osd_write(uint8_t address, uint8_t data) { //write address,data to output pins,
  delayMicroseconds(1); 
  digitalWrite(_ldi_pin, LOW);
  
  for (int i = 0; i < 7; i++) {
    pinMode(_data_pins[i], OUTPUT);
    digitalWrite(_data_pins[i], (address >> i) & 0x01);  
    }
     
  delayMicroseconds(7); 
  digitalWrite(_ldi_pin, HIGH);
  delayMicroseconds(13); 

  for (int i = 0; i < 7; i++) {
    pinMode(_data_pins[i], OUTPUT);
    digitalWrite(_data_pins[i], (data >> i) & 0x01);  
    }
 
  delayMicroseconds(7); 
  digitalWrite(_ldi_pin, LOW);
  delayMicroseconds(220); 
}

void osd_clear(){
  for (int i = 0; i < 59; i++){
    osd_write(i,15);
  }
}


int char_size(int sz){ //Size 0-3, 2H, 4H, 6H, 8H
 osd_write(62,sz); //not a good way, does reset BLK&BLKB to 0!
}


int row(int ptr){return ptr/12;}
int col(int ptr){return ptr % 12;}  
int ptr(int row, int col){return col + row * 12;}

char remap(const char c) {
    if (c >= 48 && c < 58) {
    return c - 48 + 32;
  } else if (c >= 65 && c < 78) {
    return c - 65;
  } else if (c >= 78 && c < 91) {
    return c - 78 + 16;
  } else if (c >= 97 && c < 110) {
    return c - 97;
  } else if (c >= 110 && c < 123) {
    return c - 110 + 16;
  } else if (c == 32) {
    return 15;
  } else {
    return 29;
  }
}


