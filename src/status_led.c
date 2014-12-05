#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/signal.h> 

#include "main.h"
#include "status_led.h"

volatile unsigned char status_led_timer_100ms;

void status_led_init(void)
{
DDRD |= (1<<PD2);
}

void process_status_led(void)
{
if(!status_led_timer_100ms)
	{
	PORTD ^= (1<<PD2);
	status_led_timer_100ms = 4;
	}
}

void process_status_led_timer_100ms(void)
{
if(status_led_timer_100ms)
	status_led_timer_100ms--;
}

