#include <avr/io.h>
#include <avr/interrupt.h>

#include "main.h"
#include "timer.h"
#include "status_led.h"
#include "boucle_courant.h"
#include "protocol.h"

unsigned char timer_10ms;
unsigned char timer_100ms;
unsigned char timer_s;

void timebase_init(void)
{
TCCR0 = 0x03; 			// Prescalede 64, timer activated in normal mode
TIMSK |= (1<<TOIE0); 	// Active l'interrupt overflow du timer 0
}

ISR(TIMER0_OVF_vect)
{
// 1.024ms periodic call

main_timer_1ms = 1;

protocol_timer_ms();


if(timer_10ms)
	{
	timer_10ms--;
	}
else
	{
	timer_10ms = 9;
	// 10ms Periodic call

	
	if(timer_100ms)
		{
		timer_100ms--;
		}
	else
		{
		timer_100ms = 9;
		// 100ms Periodic call
		
		process_status_led_timer_100ms();
	
		if(timer_s)
			{
			timer_s--;
			}
		else
			{
			timer_s = 9;
			// sec Periodic call
			
		
			}
		}
	}
}
