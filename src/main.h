#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <stdio.h>
#include <math.h>

#include "buffer.h"
#include "rs485.h"
#include "timer.h"
#include "status_led.h"
#include "pwm.h"
#include "encoder.h"
#include "adc.h"
#include "vt100.h"
#include "pid.h"
#include "protocol.h"
#include "boucle_courant.h"

#ifndef MAIN_H

#define MAIN_H

extern unsigned char main_timer_1ms;

#define LSTA_DDR 	DDRB
#define LSTA_PORT 	PORTB
#define LSTA_PIN	(1<<PB1)
#define LSTA_PORTIN PINB
#define LSTA_READ	(PINB & LSTA_PIN)

#define LSTA_INIT	LSTA_DDR &= ~LSTA_PIN; LSTA_PORT |= LSTA_PIN

#define RSTA_DDR 	DDRB
#define RSTA_PORT 	PORTB
#define RSTA_PIN	(1<<PB3)
#define RSTA_PORTIN PINB
#define RSTA_READ	(PINB & RSTA_PIN)

#define RSTA_INIT	RSTA_DDR &= ~RSTA_PIN; RSTA_PORT |= RSTA_PIN


#define ENABLE_OUT_DDR 		DDRA
#define ENABLE_OUT_PORT 	PORTA
#define ENABLE_OUT_PIN		(1<<PA6)

#define ENABLE_IN_DDR 		DDRA
#define ENABLE_IN_PORT 		PORTA
#define ENABLE_IN_PIN		(1<<PA2)
#define ENABLE_IN_READ		(PINA & ENABLE_IN_PIN)

#define ENABLE_IN_INIT		ENABLE_IN_DDR &= ~ENABLE_IN_PIN; ENABLE_IN_PORT |= ENABLE_IN_PIN

#define ENABLE_OUT_INIT		ENABLE_OUT_DDR |= ENABLE_OUT_PIN

#define ENABLE_OUT_SET		ENABLE_OUT_PORT |= ENABLE_OUT_PIN
#define ENABLE_OUT_CLEAR	ENABLE_OUT_PORT &= (~ENABLE_OUT_PIN)

#define SPARE_OUT_DDR 		DDRA
#define SPARE_OUT_PORT 		PORTA
#define SPARE_OUT_PIN		(1<<PA7)

#define SPARE_OUT_INIT		SPARE_OUT_DDR |= SPARE_OUT_PIN

#define SPARE_OUT_SET		SPARE_OUT_PORT |= SPARE_OUT_PIN
#define SPARE_OUT_CLEAR		SPARE_OUT_PORT &= (~SPARE_OUT_PIN)


//Condition de fin de l'allocation de l'adressage dynamique
#define PROT_GOTO_RUN_MODE 99

#endif

