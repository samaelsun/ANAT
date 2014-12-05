#include <avr/io.h>

#include "main.h"
#include "pwm.h"

void pwm_init(void)
{
DDRD |= ( (1<<PD7) | (1<<PD4) | (1<<PD6));

//Set timer 1 mode  
TCCR1A = (1<<COM1B1) | (0<<WGM10) | (1<<WGM11);

//Start timer by selecting clock source
TCCR1B|= ( 1<<CS10 ) | (1<<WGM12);

TCNT1 = 0x00;		//Set counter1 to known state 
}

void pwm_set(signed int data)
{
unsigned int data_parsed;
if(data == 0)
	{
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	PORTD |= (1<<PD6);;
	data_parsed = 0;
	}
else if(data > 0)
	{
	PORTD &= (~(1<<PD6));	
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	data_parsed = (unsigned int)(data);
	PORTD |= (1<<PD7);
	}
else
	{
	PORTD &= (~(1<<PD6));
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	data_parsed = (unsigned int)(-data);
	PORTD &= (~(1<<PD7));
	}

if(data_parsed >= 511)
	{
	data_parsed = 511;
	}

OCR1B = data_parsed;
}

