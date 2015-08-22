/*
************************** SPI in polling method
 * Atmega8_Gyro.c
 *
 * Created: 5/4/2012 4:00:34 AM
 *  Author: Atanu Barai
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

uint8_t spi_transfer(uint8_t val)
{
	SPDR = val;
	while(!(SPSR &(1<<SPIF)));
	return SPDR;
}

void enableADC()
{
	uint8_t data1, data2;
  
	PORTB &= !(1<<SS);
	spi_transfer(0b10010100);// ADCC --> ADC Conversion of Angular Rate (CHAN bit = 0)
	data1 = spi_transfer(0x00);
	data2 = spi_transfer(0x00);
	_delay_us(200);
	PORTB |= 1<<SS;
	
	
	if(data1 & 0b10000000) //If instruction not recieved
	{
		lcd_clrscr();
		lcd_puts("ADC not Enabled");
		_delay_ms(2000);
	//	PORTC = 0b00001000;
	//	PORTC = 0x0F;
	}		
	else
	{
		
		lcd_clrscr();
		lcd_puts("ADC Enabled");
		_delay_ms(2000);
	//	PORTC = 0b00100000;
	//	PORTC = 0xF0;
	}		
	_delay_ms(1000);
}

void sleepADC()
{
	uint8_t data1, data2;
  
	PORTB &= !(1<<SS);
	spi_transfer(0b10010000);// ADCC --> ADC Conversion of Angular Rate (CHAN bit = 0)
	data1 = spi_transfer(0x00);
	data2 = spi_transfer(0x00);
	_delay_us(250);
	PORTB |= 1<<SS;
	
	if(data1 & 0b10000000)
    {
		
		lcd_clrscr();
		lcd_puts("ADC not in Sleep");
		_delay_ms(2000);
	//	PORTC = 0b00001000;
	//	PORTC=0x0F;
    }
	else
	{
		lcd_clrscr();
		lcd_puts("ADC in Sleep");
		_delay_ms(2000);
		
	//	PORTC = 0b00100000;
	//	PORTC=0xF0;
	}		
	_delay_ms(1000);
}


unsigned int getAngularRate()
{
	uint8_t data1, data2;
	unsigned int result=0;
	PORTB &= !(1<<SS);
	spi_transfer(0b10010100);// ADCC --> ADC Conversion of Angular Rate (CHAN bit = 0)
	_delay_us(250);
	PORTB |= 1<<SS;
	
	
	PORTB &= !(1<<SS);
	spi_transfer(0b10000000);  // Send SPI ADCR Instruction
	data1 = spi_transfer(0x00);
	data2 = spi_transfer(0x00);
	_delay_us(250);
	PORTB |= 1<<SS;
	
	
	if(!(data1 & 0b10000000))
    {
        while(!(data1 & 0x20)); // Wait while EOC is 0
        result = ((data1 & 0x0F) << 7) + (data2>>1);
    }
	else
	{
		//PORTC = 0b00100000;
		lcd_clrscr();
		lcd_puts("Angular Rate not Calculated");
		_delay_ms(2000);
	}	
	return result;
}

unsigned int getTemperature()
{
	uint8_t data1, data2;
	unsigned int result=0;
	PORTB &= !(1<<SS);
	spi_transfer(0b10011100);// ADCC --> ADC Conversion of Temperature (CHAN bit = 1)
	_delay_us(250);
	PORTB |= 1<<SS;
	

	PORTB &= !(1<<SS);
	spi_transfer(0b10000000);  // Send SPI ADCR Instruction
	data1 = spi_transfer(0x00);
	data2 = spi_transfer(0x00);
	_delay_us(250);
	PORTB |= 1<<SS;
	
	
	if(!(data1 & 0b10000000))
    {
        while(!(data1 & 0x20)); // Wait while EOC is 0
        result = ((data1 & 0x0F) << 7) + (data2>>1);
    }
	else
	{
		//PORTC = 0b00100000;
		lcd_clrscr();
		lcd_puts("Temperature not Calculated");
		_delay_ms(2000);
	}	
	
	return result;
}

int toAngularRate(unsigned int adcValue)
{
	int conversion = (adcValue * 25/12)+400;
	signed int offset = 2500;
	return (conversion - offset)/6.67;
}

int toTemp(unsigned int adcValue)
{
	int conversion = (adcValue * 25/16)+300;
	signed int offset = 2500;
	return 25 + ((conversion - offset)/10);
}


int main(void)
{
	char buffer1[5], buffer2[5];
	SPI_master_init();
	DDRC = 0xFF;
	PORTC = 0xFF;
	_delay_ms(2000);
	PORTC = 0x00;
	_delay_ms(2000);
	lcd_init(LCD_DISP_ON_CURSOR_BLINK);
	lcd_puts("WELCOME");
	enableADC();    
    while(1)
    {
		lcd_puts("WELCOME");
		/*
		
		unsigned int rate = getAngularRate();
		unsigned int temp = getTemperature();
		if ((rate ==0) && (temp == 0))
		{
			PORTC = 0b01111111;
			_delay_ms(5000);
		}
		else
		{
			sprintf(buffer1, "%d", toAngularRate(rate));
			sprintf(buffer2, "%d", toTemp(temp));
			lcd_clrscr();
			lcd_puts("Angular Rotation ");
			lcd_puts(buffer1);
			lcd_gotoxy(0,1);
			lcd_puts("Temperature");
			lcd_puts(buffer2);
		}
		*/
		_delay_ms(2000);
    }
	return 0;
}