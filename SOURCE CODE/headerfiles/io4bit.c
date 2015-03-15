#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "io.h"

#define SET_BIT(p,i) ((p) |= (1 << (i)))
#define CLR_BIT(p,i) ((p) &= ~(1 << (i)))
#define GET_BIT(p,i) ((p) & (1 << (i)))

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

unsigned char LCD_4bit = 0;

/*-------------------------------------------------------------------------*/
void LCD_WriteCommand_4(unsigned char);

void pulse(){
	PORTA = 0x00;
	delay_ms(1);
	PORTA = 0x02;
	delay_ms(1);
	PORTA = 0x00;
	delay_ms(1);
}

void pulse_write(){
	PORTA = 0x01;
	delay_ms(1);
	PORTA = 0x03;
	delay_ms(1);
	PORTA = 0x01;
	delay_ms(1);
}

void LCD_ClearScreen(void) {
	if(LCD_4bit){
		//LCD_WriteData4('c');
		//delay_ms(2000);
		CONTROL_BUS = 0x01;
		DATA_BUS = 0x00;
		pulse();
		DATA_BUS = 0x01;
		pulse();
		delay_ms(2);
	}
	else
	LCD_WriteCommand(0x01);
}

void LCD_init(void) {

	//wait for 100 ms.
	delay_ms(100);
	//0x38 for 8bit, 0x28 for 4bit
	LCD_WriteCommand(0x38);
	LCD_WriteCommand(0x06);
	LCD_WriteCommand(0x0f);
	LCD_WriteCommand(0x01);
	delay_ms(10);
}

void LCD_init4(void){
	//wait for 100 ms.
	delay_ms(500);
	//0x38 for 8bit, 0x28 for 4bit
	
	DATA_BUS = 0x03;
	pulse();
	delay_ms(5);
	
	DATA_BUS = 0x03;
	pulse();
	delay_ms(5);
	
	DATA_BUS = 0x03;
	pulse();
	delay_ms(1);
	
	//set to 4bit mode
	DATA_BUS = 0x02;
	pulse();
	
	LCD_WriteCommand_4(0x28);
	delay_ms(5);
	
	LCD_WriteCommand_4(0x06);
	//delay_ms(5);
	
	LCD_WriteCommand_4(0x04);
	pulse();
	
	LCD_WriteCommand_4(0x01);
	pulse();
	
	delay_ms(10);
	LCD_4bit = 1;
}

void LCD_WriteCommand_4 (unsigned char Command) {
	DATA_BUS = Command;
	pulse();
	DATA_BUS = Command >> 4;
	pulse();
	delay_ms(2);
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
	if(LCD_4bit){
		DATA_BUS = Data >> 4;
		pulse_write();
	}
}

void LCD_WriteData4(unsigned char Data) {
	SET_BIT(CONTROL_BUS,RS);
	DATA_BUS = Data >> 4;
	pulse_write();
	DATA_BUS = Data;
	pulse_write();
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

void LCD_DisplayString4( unsigned char column, const unsigned char* string) {
	LCD_ClearScreen();
	unsigned char c = column;
	LCD_Cursor4(column);
	while(*string) {
		
		LCD_WriteData4(*string++);
		//LCD_Cursor4(c++);
	}
}

void LCD_Cursor4(unsigned char column) {
	if ( column < 17 ) { // 16x1 LCD: column < 9
		// 16x2 LCD: column < 17
		DATA_BUS = (0x80 + column - 1) >> 4;
		pulse_write();
		DATA_BUS = 0x80 + column - 1;
		pulse_write();
		
	} 
	else {
		//LCD_WriteCommand_4(0xB8 + column - 9);	// 16x1 LCD: column - 1
		// 16x2 LCD: column - 9
		DATA_BUS = (0xB8 + column - 9) >> 4;
		pulse_write();
		DATA_BUS = 0xB8 + column - 9;
		pulse_write();
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
