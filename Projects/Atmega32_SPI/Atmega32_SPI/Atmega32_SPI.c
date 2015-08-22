/*
************************** SPI in polling method
 * Atmega32_SPI.c
 *
 * Created: 5/4/2012 4:00:34 AM
 *  Author: BB
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define SS 4
#define MOSI 5
#define MISO 6
#define SCK 7

void SPI_master_init()
{
	DDRB = (1 << MOSI)|(0 << MISO)|(1 << SCK)|(1 << SS);
	SPCR = (1 << SPE)|(1<<MSTR)|(1<<SPR0); // Set as master and clock fosc/16 & polling
}
int main(void)
{
	DDRA = 0xFF;
	char val = 0;
	SPI_master_init();
    while(1)
    {
        SPDR = val;
		while(!(SPSR &(1<<SPIF)));
		PORTA = SPDR;
		val++;
		_delay_ms(400);
    }
	return 0;
}