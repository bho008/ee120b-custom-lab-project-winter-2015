
// Permission to copy is granted provided that this header remains intact. 
// This software is provided with no warranties.

////////////////////////////////////////////////////////////////////////////////

#ifndef USART_H
#define USART_H

// USART Setup Values
#define F_CPU 20000000UL // Assume uC operates at 8MHz
//#define BAUD_RATE 38400
//#define BAUD_PRESCALE (((F_CPU / (BAUD_RATE * 16UL))) - 1)

//#define FOSC 8000000
#define FOSC 8000000UL // Clock Speed
#define BAUD 38400
//#define MYUBRR FOSC/16/BAUD-1
#define MYUBRR (((FOSC / (BAUD * 16UL))) - 1)

void USART_Init( )
{
	int ubrr = MYUBRR;
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (0 << UMSEL00) | (0<<USBS0)|(1 << UCSZ00) | (1 << UCSZ01);//(3<<UCSZ00);
	//UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
	}

unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) )
	;
	/* Get and return received data from buffer */
	return UDR0;
}

void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

void USART_Flush()
{
	static unsigned char dummy;
	while ( UCSR0A & (1 << RXC0) ) { dummy = UDR0; }
}

unsigned char USART_HasReceived()
{
	return (UCSR0A & (1 << RXC0));
}

unsigned char USART_HasTransmitted()
{
	return (UCSR0A & (1 << TXC0));
}
////////////////////////////////////////////////////////////////////////////////
//Functionality - Initializes TX and RX on PORT D
//Parameter: None
//Returns: None
void initUSART()
{
	/*
	// Turn on the reception circuitry
	// Use 8-bit character sizes - URSEL bit set to select the UCRSC register
	// Turn on receiver and transmitter
	UCSRB |= (1 << RXEN)  | (1 << TXEN);
	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
	// Load lower 8-bits of the baud rate value into the low byte of the UBRR register
	UBRRL = BAUD_PRESCALE;
	// Load upper 8-bits of the baud rate value into the high byte of the UBRR register
	UBRRH = (BAUD_PRESCALE >> 8);
	*/
}


//void USART_Transmit( unsigned char data )
//{
	///* Wait for empty transmit buffer */
	//while ( !( UCSRnA & (1<<UDREn)) )
	//;
	///* Put data into buffer, sends the data */
	//UDRn = data;
//}

/*
////////////////////////////////////////////////////////////////////////////////
//Functionality - checks if USART is ready to send
//Parameter: None
//Returns: 1 if true else 0
unsigned char USART_IsSendReady()
{
	return (UCSRA & (1 << UDRE));
}
////////////////////////////////////////////////////////////////////////////////
//Functionality - checks if USART has recieved data
//Parameter: None
//Returns: 1 if true else 0
unsigned char USART_HasTransmitted()
{
	return (UCSRA & (1 << TXC));
}
////////////////////////////////////////////////////////////////////////////////
// **** WARNING: THIS FUNCTION BLOCKS MULTI-TASKING; USE WITH CAUTION!!! ****
//Functionality - checks if USART has recieved data
//Parameter: None
//Returns: 1 if true else 0
unsigned char USART_HasReceived()
{
	return (UCSRA & (1 << RXC));
}
////////////////////////////////////////////////////////////////////////////////
//Functionality - Flushes the data register
//Parameter: None
//Returns: None
void USART_Flush()
{
	static unsigned char dummy;
	while ( UCSRA & (1 << RXC) ) { dummy = UDR; }
}
////////////////////////////////////////////////////////////////////////////////
// **** WARNING: THIS FUNCTION BLOCKS MULTI-TASKING; USE WITH CAUTION!!! ****
//Functionality - Sends an 8-bit char value
//Parameter: Takes a single unsigned char value
//Returns: None
void USART_Send(unsigned char sendMe)
{
	while( !(UCSRA & (1 << UDRE)) );
	UDR = sendMe;
}
////////////////////////////////////////////////////////////////////////////////
// **** WARNING: THIS FUNCTION BLOCKS MULTI-TASKING; USE WITH CAUTION!!! ****
//Functionality - receives an 8-bit char value
//Parameter: None
//Returns: Unsigned char data from the receive buffer
unsigned char USART_Receive()
{
	while ( !(UCSRA & (1 << RXC)) ); // Wait for data to be received
	return UDR; // Get and return received data from buffer
}
*/
#endif //USART_H