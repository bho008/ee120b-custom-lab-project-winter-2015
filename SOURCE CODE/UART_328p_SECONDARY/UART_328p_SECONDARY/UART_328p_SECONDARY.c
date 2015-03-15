/*
 * UART_328p_SECONDARY.c
 *
 * Created: 3/5/2015 2:59:53 AM
 *  Author: Brian
 */ 


#include <avr/io.h>
#include <avr/delay.h>
#include "usart_328p_SECONDARY.h"
//#include "usart_ATmega32.h"

int main(void)
{
	//DDRD = (1 << 2) | (1 << 3); PORTD = ~(1<<2) | (1 << 3);
	DDRC = 0x00; PORTC = 0xff;
	DDRB = 0xff; PORTB = 0xff;
	DDRD = 0x00; PORTD = 0xff;
	USART_Init();
	//initUSART();
	
	uint8_t button = 0;
	uint8_t button1 = 0;
	uint8_t button2 = 0;
	uint8_t button3 = 0;
	uint8_t button4 = 0;
	uint8_t button5 = 0;
	uint8_t button6 = 0;
	uint8_t temp = 0;
	
	//unsigned char button2 = 0x00;
	unsigned char upButton2 = 0x00;
	unsigned char downButton2 = 0x00;
	unsigned char leftButton2 = 0x00;
	unsigned char rightButton2 = 0x00;
	unsigned char aux_a_2 = 0x00; //pc5
	unsigned char aux_b_2 = 0x00; //pc4
	unsigned char aux_start_2 = 0x00; //pd7
	while(1)
	{
		button = ~PINC & 0xff;
		aux_start_2 = ~PIND & 0x80;
		PORTB = button;
		button1 = button & 0x01;
		button2 = button & 0x02;
		button3 = button & 0x04;
		button4 = button & 0x08;
		button5 = button & 0x10;
		button6 = button & 0x20;
		if(button5){
			USART_Transmit(6);
		}
		else if(button1){
			USART_Transmit(3);
		}
		else if(button2){
			USART_Transmit(2);
		}
		else if(button3){
			button3 = 3;
			USART_Transmit(4);
		}
		else if(button4){
			button4 = 4;
			USART_Transmit(1);
		}
		else if(button6){
			//button4 = 4;
			USART_Transmit(5);
		}
		else if(aux_start_2){
			USART_Transmit(7);
		}
		
		if(USART_HasTransmitted())
		USART_Flush();
		temp++;
		if(temp >= 9){
			temp = 0;
		}
		_delay_ms(10);
	}
}