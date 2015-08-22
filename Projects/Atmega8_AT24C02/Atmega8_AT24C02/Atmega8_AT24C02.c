
/*
 * Atmega8_AT24C02.c
 *
 * Created: 5/27/2012 3:41:35 PM
 *  Author: HWlab-SUN
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "lcd.h"

void error(char *message)
{
	lcd_clrscr();
	lcd_puts(message);
	_delay_ms(500);
}

void i2c_init()
{
	TWSR = 0x00;
	TWBR = 0x0C; //Set SCL frequency ti 400KHz for XTAL = 8MHz  3
	TWCR = (1<<TWEN);
}

void i2c_start()
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while((TWCR & (1 << TWINT)) == 0);
	if (((TWSR & 0xF8) != 0x08) && ((TWSR & 0xF8) != 0x10))		/*Check TWSR for successful start or repeated start*/
		error("Error while start");
}

void i2c_stop()
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	_delay_ms(10);
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

void EEP_WriteByte(uint8_t address, uint8_t data)
{
	i2c_start();
	i2c_write(0b10100100); /* Address 10100100(R/W bit) 0 for write */
	if ((TWSR & 0xF8) != 0x18)
		error("Error while SLA+W write");
	i2c_write(address); /* Set register pointer to address 0x00 */
	if ((TWSR & 0xF8) != 0x28)
		error("Error while Data write");
	i2c_write(data);
	if ((TWSR & 0xF8) != 0x28)
		error("Error while Data write");
	i2c_stop();
}

void EEP_WritePage(uint8_t address, uint8_t *data)
{
	uint8_t i=0;
	i2c_start();
	i2c_write(0b10100100); /* Address 1101000(R/W bit) 0 for write */
	if ((TWSR & 0xF8) != 0x18)
		error("Error while SLA+W write");
	i2c_write(address & 0xF8); /* Set register pointer to address lower 3 bit are masked as they are auto incremented and rolled if reached page boundary*/
	if ((TWSR & 0xF8) != 0x28)
		error("Error while Data write");
	for (i=0; i<8; i++)
	{
		i2c_write(*data);
		if ((TWSR & 0xF8) != 0x28)
			error("Error while Data write");
		data++;
	}
	i2c_stop();
}

uint8_t EEP_ReadByte(uint8_t address)
{
    uint8_t data;
	i2c_start();
    i2c_write(0b10100100); /* Address 10100100(R/W bit) 0 for write */
	if ((TWSR & 0xF8) != 0x18)
		error("Error in SLA+W write");
	i2c_write(address); /* Set register pointer to address 0x00 */
	if ((TWSR & 0xF8) != 0x28)
		error("Error in Data write");
    //send repeated start
    i2c_start();
    //select devise and send read bit
    i2c_write(0b10100101); /* Address 10100100(R/W bit) 1 for read */
	if ((TWSR & 0xF8) != 0x40)
		error("Error in SLA+R write");
	data = i2c_read(0);
	if ((TWSR & 0xF8) != 0x58)
		error("Error in Data Read");
    i2c_stop();
	return data;
}

void EEP_ReadPage(uint8_t address, uint8_t *pageRead)
{
    uint8_t i;
	i2c_start();
    i2c_write(0b10100100); /* Address 10100100(R/W bit) 0 for write */
	if ((TWSR & 0xF8) != 0x18)
		error("Error in SLA+W write");
	i2c_write(address); /* Set register pointer to address 0x00 */
	if ((TWSR & 0xF8) != 0x28)
		error("Error in Data write");
    //send repeated start
    i2c_start();
    //select devise and send read bit
    i2c_write(0b10100101); /* Address 10100100(R/W bit) 1 for read */
	if ((TWSR & 0xF8) != 0x40)
		error("Error in SLA+R write");
	
	for (i=1; i<=7; i++)
	{
		*pageRead = i2c_read(1);
		if ((TWSR & 0xF8) != 0x50)
			error("Error while Data write");
		pageRead++;
	}
	
	*pageRead = i2c_read(0);
	if ((TWSR & 0xF8) != 0x58)
		error("Error in Data Read");
    i2c_stop();
}

int main(void)
{
	uint8_t pageWrite[]={1,2,3,4,5,6,7,8}, pageRead[8]={0}, dataByte, address;
	char buffer[3]={0};
	DDRB = 0xFF;
	lcd_init(LCD_DISP_ON_CURSOR_BLINK);
	i2c_init();
	address = 0x04;
	EEP_WriteByte(address, 46);
	address = 0xF0;
	EEP_WritePage(address, pageWrite);
	address = 0x04;
	dataByte = EEP_ReadByte(address);
	sprintf(buffer,"Address: %d",address);
	lcd_clrscr();
	lcd_puts(buffer);
	sprintf(buffer,"Data: %d",dataByte);
	lcd_gotoxy(0,1);
	lcd_puts(buffer);
	_delay_ms(1000);
	address = 0xF0;
	EEP_ReadPage(address, pageRead);
	lcd_clrscr();
	lcd_home();
	sprintf(buffer,"%d %d %d %d %d %d %d %d", pageRead[0],pageRead[1],pageRead[2],pageRead[3],pageRead[4],pageRead[5],pageRead[6],pageRead[7]);
	lcd_puts(buffer);
	lcd_gotoxy(0,1);
	sprintf(buffer,"Address St:%d",address);
	lcd_puts(buffer);
	
    while(1)
    {
		
    }
}