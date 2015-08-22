/*
 * atmega8_led.c
 *
 * Created: 6/11/2012 1:17:53 AM
 *  Author: HWlab-SUN
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
int main(void)
{
	DDRD = 0xFF;
    while(1)
    {
		PORTD = 0xFF;
		_delay_ms(2000);
		PORTD = 0xAA;
		_delay_ms(2000);
		PORTD = 0x55;
		_delay_ms(2000);
		PORTD = 0x00;
		_delay_ms(2000);
    }
}