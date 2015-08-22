#include <avr/io.h>
#include <util/delay.h>

#define FREQ 16000000 //Crystal frequency
#define duty_cycle 50 // duty cycle require
#define prescaler 8


void change_pwm(unsigned int cnl_A, unsigned int cnl_B)
{
	if(cnl_A <= 100 && cnl_B <= 100)
	OCR1A = (int)ICR1*(cnl_A/100); // Set duty cycle
	OCR1B = (int)ICR1*(cnl_B/100); // Set duty cycle
}

int main(void)
{
	
	// Initialize 16 bit timer(timer1)
	// Phase Correct + non inverting + 8 prescaler
	ICR1 = 50000; // To set pwm_frequency ----------- Here 10 Hz
	TCCR1A = (1<<WGM11)|(1<<COM1A1)|(1<<COM1B1);
	TCCR1B = (1<<WGM13)|(1<<CS11);
	DDRD|=(1<<PD4);
	DDRD|=(1<<PD5);
	OCR1A = ICR1/2; // Set duty cycle
	OCR1B = ICR1/2; // Set duty cycle
	//-------------------------------------------------------------
	
	DDRB = 0x0F;
	DDRA = 0xFF;
	PORTA = 0x00;
	PORTB = 0x0A;
	while (1)
	{
		
	}
}