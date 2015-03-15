#ifndef __Enable_ADC_
#define __Enable_ADC_

#include <avr/io.h>
uint16_t adc_value;        //Variable used to store the value read from the ADC converter

void initADC(){
	
	ADCSRA |= ((1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));
	ADMUX |= (1<<REFS0) | (1 << MUX2);
	ADMUX &= ~(1<<REFS1);                
	ADCSRB &= ~((1<<ADTS2)|(1<<ADTS1)|(1<<ADTS0));    //ADC in free-running mode
	ADCSRA |= (1<<ADATE)|(1<<ADEN)|(1<<ADSC);           
}


#endif