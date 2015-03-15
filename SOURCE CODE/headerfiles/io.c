#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "io.h"

#define SET_BIT(p,i) ((p) |= (1 << (i)))
#define CLR_BIT(p,i) ((p) &= ~(1 << (i)))
#define GET_BIT(p,i) ((p) & (1 << (i)))

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80
// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00
// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00
// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00
// flags for function set
//#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00
#define LCD_Right 0
#define LCD_Left 1
//#define DELAYPERCHAR 320
#define DELAYPERCHAR 1
          
/*-------------------------------------------------------------------------*/
/*
#define DATA_BUS PORTC		// port connected to pins 7-14 of LCD display
#define CONTROL_BUS PORTD	// port connected to pins 4 and 6 of LCD disp.
#define RS 6			// pin number of uC connected to pin 4 of LCD disp.
#define E 7			// pin number of uC connected to pin 6 of LCD disp.
*/
#define DATA_BUS PORTD		// port connected to pins 7-14 of LCD display
#define CONTROL_BUS PORTA	// port connected to pins 4 and 6 of LCD disp.
#define RS 0			// pin number of uC connected to pin 4 of LCD disp.
#define E 1			// pin number of uC connected to pin 6 of LCD disp.
#define _enable_pin 1
#define _rs_pin 0
/*-------------------------------------------------------------------------*/

void LCD_ClearScreen(void) {
   LCD_WriteCommand(0x01);
}

void send(uint8_t value, uint8_t mode) {
	uint8_t en = _enable_pin;
	
	//digitalWrite(_rs_pin, mode);
	
	SET_BIT(CONTROL_BUS, RS);
	
	/*
	SET_BIT(DATA_BUS, 7)
	SET_BIT(DATA_BUS, 6)
	SET_BIT(DATA_BUS, 5)
	SET_BIT(DATA_BUS, 4)
	SET_BIT(CONTROL_BUS, en);
	CLR_BIT(CONTROL_BUS, en);
	SET_BIT(DATA_BUS, 3)
	SET_BIT(DATA_BUS, 2)
	SET_BIT(DATA_BUS, 1)
	SET_BIT(DATA_BUS, 0)
	*/
	SET_BIT(CONTROL_BUS, en);
	CLR_BIT(CONTROL_BUS, en);
	
	DATA_BUS = value;
	
	/*
	digitalWrite(_data_pins[0], value & 0x10);
	digitalWrite(_data_pins[1], value & 0x20);
	digitalWrite(_data_pins[2], value & 0x40);
	digitalWrite(_data_pins[3], value & 0x80);
	digitalWrite(en, HIGH); // enable pulse must be >450ns
	digitalWrite(en, LOW);
	digitalWrite(_data_pins[0], value & 0x01);
	digitalWrite(_data_pins[1], value & 0x02);
	digitalWrite(_data_pins[2], value & 0x04);
	digitalWrite(_data_pins[3], value & 0x08);
	digitalWrite(en, HIGH); // enable pulse must be >450ns
	digitalWrite(en, LOW);
	*/
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void createChar(uint8_t location, uint8_t charmap[]) {
	location &= 0x7; // we only have 8 locations 0-7
	uint8_t _chip; 
 {
		uint8_t chipSave = _chip;
		_chip = 0;
		LCD_WriteCommand(LCD_SETCGRAMADDR | (location << 3));
		for (uint8_t i=0; i<8; i++) {
			send(charmap[i],1);
		}
		_chip = 2;
		LCD_WriteCommand(LCD_SETCGRAMADDR | (location << 3));
		for (uint8_t i=0; i<8; i++) {
			send(charmap[i],1);
		}
		_chip = chipSave;
	}
}


void LCD_init(void) {

    //wait for 100 ms.
	delay_ms(100);
	LCD_WriteCommand(0x38);
	LCD_WriteCommand(0x06);
	LCD_WriteCommand(0x0f);
	LCD_WriteCommand(0x01);
	delay_ms(10);						 
}

void LCD_WriteCommand (unsigned char Command) {
   CLR_BIT(CONTROL_BUS,RS);
   DATA_BUS = Command;
   SET_BIT(CONTROL_BUS,E);
   asm("nop");
   CLR_BIT(CONTROL_BUS,E);
   delay_ms(2); // ClearScreen requires 1.52ms to execute
}

void LCD_WriteData(unsigned char Data) {
   SET_BIT(CONTROL_BUS,RS);
   DATA_BUS = Data;
   SET_BIT(CONTROL_BUS,E);
   asm("nop");
   CLR_BIT(CONTROL_BUS,E);
   delay_ms(1);
}

void LCD_DisplayString( unsigned char column, const unsigned char* string) {
   LCD_ClearScreen();
   unsigned char c = column;
   while(*string) {
      LCD_Cursor(c++);
      LCD_WriteData(*string++);
   }
}

void LCD_Cursor(unsigned char column) {
   if ( column < 17 ) { // 16x1 LCD: column < 9
						// 16x2 LCD: column < 17
      LCD_WriteCommand(0x80 + column - 1);
   } else {
      LCD_WriteCommand(0xB8 + column - 9);	// 16x1 LCD: column - 1
											// 16x2 LCD: column - 9
   }
}

void delay_ms(int miliSec) //for 8 Mhz crystal

{
    int i,j;
    for(i=0;i<miliSec;i++)
    for(j=0;j<775;j++)
  {
   asm("nop");
  }
}
