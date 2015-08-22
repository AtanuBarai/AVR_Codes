/*
 * HeartRateMonitor_for_Imtiaz.c
 *
 * Created: 5/16/2012 12:20:13 AM
 *  Author: HWlab-SUN
 */ 
#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "lcd.h"
char rate=0;

ISR(TIMER1_OVF_vect)
{
	//TCNT1H = (-29296)>>8;
	//TCNT1L = (-29296);
	lcd_clrscr();
	lcd_home();
	lcd_puts("Heart Rate : ");
	char buffer[3];
	sprintf(buffer,"%d", rate*2);
	lcd_puts(buffer);
}

int main(void)
{
	lcd_init(LCD_DISP_ON_CURSOR_BLINK);
	lcd_home();
	lcd_puts("Measuring Heart Rate");
	DDRB = 0xF0;
	//TCNT1H = (-29296)>>8;
	//TCNT1L = (-29296);
	TCNT1 = (-29296);
	TCCR1A = 0x00;
	TCCR1B = 0b00000101; //clk/1024 prescalar
	TIMSK = (1<<TOIE1);
	sei();
    while(1)
	{
		if(PINB & 0x01)
		{
			while(PINB & 0x01);
			rate++;
		}
		PORTB ^= 0xF0;
		_delay_ms(500);.
	}
}