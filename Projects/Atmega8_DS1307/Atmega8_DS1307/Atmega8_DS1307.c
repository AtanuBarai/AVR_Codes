/*
 *  Atmega8_DS1307.c
 *
 *  Created: 5/25/2012 6:57:49 PM
 *  Author: Atanu
 *  LCD Library: Peter Fleury <pfleury@gmx.ch>  http://jump.to/fleury
 */ 

#define F_CPU 16000000UL
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
	TWBR = 0x47; //Set SCL frequency ti 50KHz for XTAL = 8MHz
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

void RTC_init()
{
	i2c_init();
	i2c_start();
	i2c_write(0b11010000); /* Address 1101000(R/W bit) 0 for write */
	if ((TWSR & 0xF8) != 0x18) /*Check TWSR for successful SLA+W status*/
		error("Error while SLA+W write");
	i2c_write(0x07); /* Set register pointer to address 0x07 (control register) */
	if ((TWSR & 0xF8) != 0x28) /*Check TWSR for successful Data/Register Address transmission status*/
		error("Error while Data write");
	i2c_write(0x00);
	if ((TWSR & 0xF8) != 0x28) /*Check TWSR for successful Data transmission status*/
		error("Error while Data write");
	i2c_stop();
}

void RTC_SetTimeDate(uint8_t s, uint8_t m, uint8_t h, uint8_t day, uint8_t date,  uint8_t month, uint8_t year)
{
	i2c_start();
	i2c_write(0b11010000); /* Address 1101000(R/W bit) 0 for write */
	if ((TWSR & 0xF8) != 0x18)
		error("Error while SLA+W write");
	i2c_write(0x00); /* Set register pointer to address 0x00 */
	if ((TWSR & 0xF8) != 0x28)
		error("Error while Data write");
	i2c_write(s);
	if ((TWSR & 0xF8) != 0x28)
		error("Error while Data write");
	i2c_write(m);
	if ((TWSR & 0xF8) != 0x28)
		error("Error while Data write");
	i2c_write(h);
	if ((TWSR & 0xF8) != 0x28)
		error("Error while Data write");
	i2c_write(day);
	if ((TWSR & 0xF8) != 0x28)
		error("Error while Data write");
	i2c_write(date);
	if ((TWSR & 0xF8) != 0x28)
		error("Error while Data write");
	i2c_write(month);
	if ((TWSR & 0xF8) != 0x28)
		error("Error while Data write");
	i2c_write(year);
	if ((TWSR & 0xF8) != 0x28)
		error("Error while Data write");
	i2c_stop();
}

void RTC_GetTimeDate(uint8_t *s, uint8_t *m, uint8_t *h, uint8_t *day, uint8_t *date,  uint8_t *month, uint8_t *year)
{
	i2c_start();
	i2c_write(0b11010000); /* Address 1101000(R/W bit) 0 for write */
	if ((TWSR & 0xF8) != 0x18)
		error("Error while SLA+W write");
	i2c_write(0x00); /* Set register pointer to address 0x00 */
	if ((TWSR & 0xF8) != 0x28)
		error("Error while Address write");
	i2c_start(); // Repeated Start
	i2c_write(0b11010001);
	if ((TWSR & 0xF8) != 0x40)
		error("Error in SLA+R write");
	*s=i2c_read(1);
	if ((TWSR & 0xF8) != 0x50)
		error("Error in Data recieve");
	*m=i2c_read(1);
	if ((TWSR & 0xF8) != 0x50)
		error("Error in Data recieve");
	*h=i2c_read(1);
	if ((TWSR & 0xF8) != 0x50)
		error("Error in Data recieve");
	*day=i2c_read(1);
	if ((TWSR & 0xF8) != 0x50)
		error("Error in Data recieve");
	*date=i2c_read(1);
	if ((TWSR & 0xF8) != 0x50)
		error("Error in Data recieve");
	*month=i2c_read(1);
	if ((TWSR & 0xF8) != 0x50)
		error("Error in Data recieve");
	*year=i2c_read(0);
	if ((TWSR & 0xF8) != 0x58)
		error("Error in Data recieve");
	i2c_stop();
}

void bcd2ascii(uint8_t bcd)
{
	lcd_putc('0' + (bcd>>4));
	lcd_putc('0' + (bcd & 0x0F));	
}

int main(void)
{
	DDRB = 0xFF;
	uint8_t s, s_changed, m, h, day, date, month, year;
	lcd_init(LCD_DISP_ON_CURSOR_BLINK);
	RTC_init();
	RTC_SetTimeDate(0x30, 0x45, 0x19, 0x01, 0x26, 0x05, 0x12);
    while(1)
    {
		s_changed = s;
        RTC_GetTimeDate(&s, &m, &h, &day, &date, &month, &year);
		if (s_changed != s)
		{
			lcd_clrscr();
			lcd_gotoxy(4,0);
			bcd2ascii(h);
			lcd_putc(':');
			bcd2ascii(m);
			lcd_putc(':');
			bcd2ascii(s);
			lcd_gotoxy(4,1);
			bcd2ascii(date);
			lcd_putc(':');
			bcd2ascii(month);
			lcd_putc(':');
			bcd2ascii(year);
			//PORTB = ('0' + (s & 0x0F));
		}
    }
}