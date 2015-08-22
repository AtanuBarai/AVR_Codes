/*
 * Atmega32.c
 *
 * Created: 24/04/2012 12:42:03
 *  Author: Kallol
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>

int main(void)
{
	DDRB |= 0x82;
	DDRC = 0x00;
	DDRD = 0xFF;
	
	TCNT0 = -160;
	TCCR0 = 0x01;
	TCNT1H = (-640)>>8;
	TCNT1L = -640;
	TCCR1A = 0x00;
	TCCR1B = 0x01;
	TIMSK = (1<<TOIE0)|(1<<TOIE1);
	sei();
    while(1)
    {
        PORTD = PINC;
    }
}

ISR (TIMER0_OVF_vect)
{
	TCNT0 = -160;
	PORTB ^= 1<<PINB0;
}

ISR (TIMER1_OVF_vect)
{
	TCNT1H = (-640)>>8;
	TCNT1L = -640;
	PORTB ^= 1<<PINB7;
}