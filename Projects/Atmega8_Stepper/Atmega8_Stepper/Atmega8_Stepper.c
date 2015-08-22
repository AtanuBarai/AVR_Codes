/*
 * Atmega8_Stepper.c
 *
 * Created: 7/5/2012 4:00:34 PM
 *  Author: BB
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRC = 0xFF;
    while(1)
    {
		/*
		// Wave Driving
        PORTC = 0b00000001;
		_delay_ms(10);
		PORTC = 0b00000010;
		_delay_ms(10);
		PORTC = 0b00000100;
		_delay_ms(10);
		PORTC = 0b00001000;
		_delay_ms(10);
		*/
		
		//Full Driving
		PORTC = 0b00000011;
		_delay_ms(10);
		PORTC = 0b00000110;
		_delay_ms(10);
		PORTC = 0b00001100;
		_delay_ms(10);
		PORTC = 0b00001001;
		_delay_ms(10);
		
		/*
		//Half Driving
		PORTC = 0b00000001;
		_delay_ms(10);
		PORTC = 0b00000011;
		_delay_ms(10);
		PORTC = 0b00000010;
		_delay_ms(10);
		PORTC = 0b00000110;
		_delay_ms(10);
		PORTC = 0b00000100;
		_delay_ms(10);
		PORTC = 0b00001100;
		_delay_ms(10);
		PORTC = 0b00001000;
		_delay_ms(10);
		PORTC = 0b00001001;
		_delay_ms(10);
		*/
    }
}