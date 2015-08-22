/*
 * atmega8_led.c
 *
 * Created: 6/11/2012 1:17:53 AM
 *  Author: HWlab-SUN
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#define ubrr 51 // for baud rate 9600 and F_CPU 8MHz ubrr 51



void USART_Initialize()
{
	/* Set baud rate */
	UBRRH = (unsigned char)(ubrr>>8);
	UBRRL = (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSRB = (1<<RXEN)|(1<<TXEN);
	/* Set frame format: 8data, 1stop bit */
	UCSRC = (1<<URSEL)|(3<<UCSZ0);
}

void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)) )
	;
	/* Put data into buffer, sends the data */
	UDR = data;
}

unsigned char USART_Receive( void )
{
	/* Wait for data to be received */
	while ( !(UCSRA & (1<<RXC)) );
	/* Get and return received data from buffer */
	return UDR;
}

int main(void)
{
	unsigned char ch;
	unsigned int i;
	DDRB = 0xFF;
	USART_Initialize();
    while(1)
    {
		ch = USART_Receive();
		USART_Transmit(ch);
		if (ch == 'f')
		{
			PORTB = 0x0A;
		}
		else if (ch == 'b')
		{
			PORTB = 0x05;
		}
		else if (ch == 'l')
		{
			PORTB = 0x08;
			for (i=0; i<1;i++)
			{
				_delay_ms(1000);
			}
			PORTB = 0x0A;
			
		}
		else if (ch == 'r')
		{
			PORTB = 0x02;
			for (i=0; i<1;i++)
			{
				_delay_ms(1000);
			}
			PORTB = 0x0A;
		}
		else if (ch == 's')
		{
			PORTB = 0x00;
		}
    }
}