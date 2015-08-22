/*
 * Atmega8_Avi_gluco.c
 *
 * Created: 18-Mar-13 3:28:19 AM
 *  Author: Atanu
 */ 


#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#define  PORT_ON(port,pin) port |= (1<<pin)
#define  PORT_OFF(port,pin)  port &=  ~(1<<pin)

uint16_t ReadADC(uint8_t ch)
{
	ch=ch&0b00000111;
	   ADMUX|=ch;
	//Start Single conversion
	   ADCSRA|=(1<<ADSC);
	//Wait for conversion to complete
	while(!(ADCSRA & (1<<ADIF)));
	ADCSRA|=(1<<ADIF);
	return(ADC);
}
		
void calc()
{
	unsigned int red_val, green_val, blue_val;
	//calc red
	PORTB = 0x01;
	_delay_ms(5000);
	ADCSRA  |= (1<<ADSC);	// Start conversion
	while(!(ADCSRA & (1<<ADIF)));
	ADCSRA|=(1<<ADIF);
	// wait until conversion  completes; ADSC=0 means Complete
  	red_val = ADCW;	//Store ADC result
	
	//calc green
	PORTB = 0x02;
	_delay_ms(5000);
	ADCSRA  |= (1<<ADSC);	// Start conversion
	while(!(ADCSRA & (1<<ADIF)));
	ADCSRA|=(1<<ADIF);
	// wait until conversion  completes; ADSC=0 means Complete
  	green_val = ADCW;	//Store ADC result
	
	//calc blue
	PORTB = 0x04;
	_delay_ms(5000);
	ADCSRA  |= (1<<ADSC);	// Start conversion
	while(!(ADCSRA & (1<<ADIF)));
	ADCSRA|=(1<<ADIF);
	// wait until conversion  completes; ADSC=0 means Complete
  	blue_val = ADCW;	//Store ADC result
	
	
	PORTB = 0x00;
	if((red_val > blue_val) && (red_val > green_val))
	{
		PORTB = 0x08;
	}
	else if ((blue_val > red_val) && (blue_val > green_val))
	{
		PORTB = 0x10;
	}
	else if (green_val > red_val && green_val > blue_val)
	{
		PORTB = 0x20;
	}
	_delay_ms(5000);
}
int  main(void)
{
	unsigned  char color;	// Variable to hold ADC  result
	DDRB=0xFF;	// Set Port B.0, 1, 2 for LED output
	PORTB  = 0x00;	// Clear Portd pins
 	ADCSRA  = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1);
	// Set ADCSRA Register with division factor 64

	ADMUX=0x01;	//Binary equivalent of 0001
			
	while (1)	//Forever since it is in  single conversion mode
		{
  			calc();
		}			
		return 0;  
	}