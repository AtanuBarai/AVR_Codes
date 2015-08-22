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
#include "lcd.h"
#define  PORT_ON(port,pin) port |= (1<<pin)
#define  PORT_OFF(port,pin)  port &=  ~(1<<pin)
	
		
int  main(void)
	{
		unsigned  int adc_value;	// Variable to hold ADC  result
		DDRB=0xF7;	// Set Port B.0 for LED output
		PORTB  = 0x00;	// Clear Portd pins
		lcd_init(LCD_DISP_ON_CURSOR_BLINK);
 		ADCSRA  = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1);
		// Set ADCSRA Register with division factor 64

		ADMUX=0x05;	//Binary equivalent of 0101
			
		while (1)	//Forever since it is in  single conversion mode
		{
			while(PINB & 0x08);
			lcd_clrscr();
			lcd_puts("Checking..");
			lcd_gotoxy(0,1);
			lcd_puts("Please Wait");
			_delay_ms(6000);
  			ADCSRA  |= (1<<ADSC);	// Start conversion
			while (ADCSRA &  (1<<ADSC));
			// wait until conversion  completes; ADSC=0 means Complete

  			adc_value = ADCW;	//Store ADC result
			  
			  if (adc_value < 2)
			  {
				  lcd_clrscr();
				  lcd_puts("65mg/dL");
			  }
			  else if (adc_value >= 2 && adc_value < 4)
			  {
				  lcd_clrscr();
				  lcd_puts("67mg/dL");
			  }
			  else if (adc_value >= 4 && adc_value < 8)
			  {
				  lcd_clrscr();
				  lcd_puts("69mg/dL");
			  }
			  else if (adc_value >= 8 && adc_value < 16)
			  {
				  lcd_clrscr();
				  lcd_puts("72mg/dL");
			  }
			  else if (adc_value >= 16 && adc_value < 32)
			  {
				  lcd_clrscr();
				  lcd_puts("78mg/dL");
			  }
			  else if (adc_value >= 32 && adc_value < 48)
			  {
				  lcd_clrscr();
				  lcd_puts("82mg/dL");
			  }
			  else if (adc_value >= 48 && adc_value < 64)
			  {
				  lcd_clrscr();
				  lcd_puts("87mg/dL");
			  }
			  
			  else if (adc_value >= 64 && adc_value < 90)
			  {
				  lcd_clrscr();
				  lcd_puts("93mg/dL");
			  }
			  
			  else if (adc_value >= 90 && adc_value < 128)
			  {
				  lcd_clrscr();
				  lcd_puts("99mg/dL");
			  }
			  else if (adc_value >= 128 && adc_value < 150)
			  {
				  lcd_clrscr();
				  lcd_puts("107mg/dL");
			  }
			  
			  else if (adc_value >= 150 && adc_value < 180)
			  {
				  lcd_clrscr();
				  lcd_puts("118mg/dL");
			  }
			  
			  else if (adc_value >= 180 && adc_value < 210)
			  {
				  lcd_clrscr();
				  lcd_puts("135mg/dL");
			  }
			  
			  else if (adc_value >= 210 && adc_value < 240)
			  {
				  lcd_clrscr();
				  lcd_puts("163mg/dL");
			  }
			  else if (adc_value >= 240 && adc_value < 280)
			  {
				  lcd_clrscr();
				  lcd_puts("182mg/dL");
			  }
			  else if (adc_value >= 280 && adc_value < 310)
			  {
				  lcd_clrscr();
				  lcd_puts("207mg/dL");
			  }
				else if (adc_value >= 310 && adc_value < 350)
  				{
					  lcd_clrscr();
					  lcd_puts("238mg/dL");
  				}
				else if (adc_value >= 350 && adc_value < 400)
  				{
					  lcd_clrscr();
					  lcd_puts("280mg/dL");
  				}
				
  			else
  			{
				  lcd_clrscr();
				  lcd_puts("Out of range");
  			}
			return 0;
		}			  
	}