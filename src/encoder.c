#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
//#include <avr/signal.h>

#include "main.h"
#include "encoder.h"

signed long encoder_angle = 0;

void encoder_init(void)
	{
	MCUCSR 	|= (0<<ISC2); 		// Select falling edge to trig the interrupt
	GICR 	|= (1<<INT2);		//Enable external interrupt
	}

signed long get_encoder(void)
	{
	signed long latch;	
	cli();
	latch = encoder_angle;
	sei();

	return latch;
	}

void reset_encoder(void)
	{
	cli();
	encoder_angle = 0;
	sei();
	}		

ISR (INT2_vect)
{
//NOTE Global Interrupt bit is cleared here.  For nested interrupt, you must set it here
//by this instruction sei()		//Set Global interrupt

if(PINB & 0x01)		//Check quadrature signal to determine direction PD1
	encoder_angle++;
else
	encoder_angle--;
}
