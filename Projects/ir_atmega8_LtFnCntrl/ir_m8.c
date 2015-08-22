#include <avr/io.h>

#include "remote.h"



int main()
{
	uint8_t cmd=0;
	DDRC = (1<<DDC5)|(1<<DDC4)|(1<<DDC3)|(1<<DDC2)|(1<<DDC0);
	PORTC = (0<<PC5)|(0<<PC4)|(0<<PC3)|(0<<PC2)|(0<<PC0);
	RemoteInit();
	while(1)
	{
		cmd=GetRemoteCmd(1);
		if(cmd == 156) //For Light on off
		{
			PORTC = PORTC ^ 0x01; //Toggole
		}
		else if(cmd == 217) //For Fan off
		{
			PORTC &=0b11000011;
		}
		else if(cmd == 146) //For Fan speed 1
		{	
			PORTC |= 0b00000100;
			PORTC &= 0b11000111;
		}
		else if(cmd == 147) //For Fan Speed Up 2
		{
			PORTC |= 0b00001000;
			PORTC &= 0b11001011;
		}
		else if(cmd == 204) //For Fan Speed Up 3
		{
			PORTC |= 0b00010000;
			PORTC &= 0b11010011;
		}
		else if(cmd == 142) //For Fan Speed Up 4
		{
			PORTC |= 0b00100000;
			PORTC &= 0b11100011;
		}
	}
	return 0;
}
