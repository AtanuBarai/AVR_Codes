/*#########################################################

Demo program for testing the IR Remote control libraries.
-----------------------------------------------------------

Copyright Avinash Gupta 2008 avinash@eXtremeElectronics.co.in

Web: www.eXtremeElectronics.co.in

Please give credit or reference to www.eXtremeElectronics.co.in
if you use this in your projects and find it useful.


##########################################################*/
#include <avr/io.h>

#include "remote.h"
//#include "lcd.h"


// UBBR 51 for FOSC 8.0 MHz and BAUD 9600
#define UBBR 51


void USART_Init( unsigned int ubrr)
{
	/* Set baud rate */
	UBRRH = (unsigned char)(ubrr>>8);
	UBRRL = (unsigned char)ubrr;
	//For Transmitter enable
	UCSRB = (1<<TXEN);
	/* Set frame format: 8data, 2stop bit */
	UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);
}
void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)) );
	/* Put data into buffer, sends the data */
	UDR = data;
}




int main()
{
	uint8_t cmd=0;


	RemoteInit();
	USART_Init(UBBR);

	while(1)
	{
		cmd=GetRemoteCmd(1);
		USART_Transmit(cmd);	
		cmd = 0;
	}
	return 0;
}
