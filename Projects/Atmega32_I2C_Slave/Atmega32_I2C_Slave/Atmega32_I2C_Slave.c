/*
 * Atmega32_I2C_Slave.c
 *
 * Created: 5/26/2012 4:32:42 PM
 *  Author: BB
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"

void i2c_init()
{
	TWSR = 0x00;
	TWBR = 0x47;
	TWCR = (1<<TWEN);
}

void i2c_start()
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while((TWCR & (1 << TWINT)) == 0);
}

void i2c_stop()
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}

void i2c_write(uint8_t data)
{
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while((TWCR & (1 << TWINT)) == 0);
}

uint8_t i2c_read(uint8_t multi)
{
	if (multi)
		TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	else
		TWCR = (1<<TWINT)|(1<<TWEN);
	while((TWCR & (1 << TWINT))==0);
	return TWDR;
}	

int main(void)
{
	uint8_t val = 0;
	lcd_init(LCD_DISP_ON_CURSOR_BLINK);
	lcd_home();
	lcd_puts("DS1307 Interfacing");
	i2c_init();
	i2c_start();
	i2c_write(0b01001000); // transmit sla+address
	i2c_write(0b11011001);
	val = i2c_read(0);
	i2c_stop();
    while(1)
    {
        
    }
}