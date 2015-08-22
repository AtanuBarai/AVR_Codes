/*
 * Atmega8_AT45DB011B.c
 *
 * Created: 6/3/2012 10:52:31 PM
 *  Author: HWlab-SUN
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "lcd.h"

#define SS 2
#define MOSI 3
#define MISO 4
#define SCK 5

void SPI_master_init()
{
	DDRB = (1 << MOSI)|(0 << MISO)|(1 << SCK)|(1 << SS);
	SPCR = (1 << SPE)|(1<<MSTR)|(1<<CPOL)|(1<<CPHA)|(1<<SPR0); // Set as master and clock fosc/16 & polling
}

uint8_t spi_TxRx(uint8_t val)
{
	SPDR = val;
	while(!(SPSR &(1<<SPIF)));
	return SPDR;
}

void main_page_read()
{
	PORTB &= !(1<<SS);
	spi_TxRx(0x52);
	PORTB |= (1<<SS);
}

void buffer_read()
{
	PORTB &= !(1<<SS);
	spi_TxRx(0x54);
	PORTB |= (1<<SS);
}

void memPage2buffer()
{
	PORTB &= !(1<<SS);
	spi_TxRx(0x53);
	PORTB |= (1<<SS);
}

void memPage2bufferComp()
{
	PORTB &= !(1<<SS);
	spi_TxRx(0x60);
	PORTB |= (1<<SS);
}


void buffer_write()
{
	PORTB &= !(1<<SS);
	spi_TxRx(0x84);
	PORTB |= (1<<SS);
}

void buffer2memPage_WBIE()
{
	PORTB &= !(1<<SS);
	spi_TxRx(0x83);
	PORTB |= (1<<SS);
}

void buffer2memPage_WOBIE()
{
	PORTB &= !(1<<SS);
	spi_TxRx(0x88);
	PORTB |= (1<<SS);
}

void page_erase()
{
	PORTB &= !(1<<SS);
	spi_TxRx(0x81);
	PORTB |= (1<<SS);
}

void block_erase()
{
	PORTB &= !(1<<SS);
	spi_TxRx(0x50);
	PORTB |= (1<<SS);
}

int main(void)
{
    while(1)
    {
        
    }
}