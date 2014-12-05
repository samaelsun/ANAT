#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
//#include <avr/signal.h>

#include "main.h"
#include "buffer.h"
#include "rs485.h"




void rs485_init(uint32_t baud_rate)
{
    /*
    unsigned char dummy;
    unsigned int temp;

        DDRD |= (1<<PD1);

    	temp = (F_CPU / (16 * baud_rate)) - 1; // 9600 Bd

        UBRRH = (unsigned char)(temp >> 8);
        UBRRL = (unsigned char)temp;
    	UCSRA = 0x00;
        UCSRB = (1<<RXEN) | (1<<RXCIE) | (1<<TXEN) | (1<<UDRIE);
        UCSRC = (1<<UCSZ1) | (1<<UCSZ0) | (1<<URSEL);

    	dummy = UDR;
*/


    unsigned char dummy;

UCSRA = ( (1<<U2X) );
UCSRB = ( (1<<TXCIE) | (1<<UDRIE) | (1<<RXCIE) | (1<<RXEN) );
UCSRC = ( (1<<UCSZ1) | (1<<UCSZ0) | (1<<UCSRC) );


UBRRH = 0;
//UBRRL = 51; // Set 38400 bauds
UBRRL = 16; // Set 115200 bauds

DDRC  |= ( (1<<PC6) | (1<<PC7) );

dummy = UDR; // Read the data register to flush any crap in it !

rs485_txen(0);
}


void rs485_txen(unsigned char txen)
{
unsigned char dummy;

if(txen)
	{
	UCSRB &=  (~(1<<RXCIE) );	// desactive l'interrupt RX
	
	PORTC |= (1<<PC6); 			// !RXEN
	PORTC |= (1<<PC7); 			// TXEN
	}
else
	{
	PORTC &= (~(1<<PC7)); 		// TXEN
	PORTC &= (~(1<<PC6));		// !RXEN

	dummy = UDR;	
	UCSRB |=  (1<<RXCIE);		// active l'interrupt RX
	}
}

void rs485_putchar(unsigned char data)
{
cli();	//Clear GLOBAL Interrupt

if(tx_buffer_full())
	{
	sei();
	while(tx_buffer_full());
	cli();
	}
	
if( (!(UCSRB & (1<<TXEN)))) // si le tx n'est pas actif
	{
	rs485_txen(1);			// Active la pin de txen
	UDR = data;				// charge le uart avec une donne
	UCSRB |= (1<<TXEN);		// Active le TX
	}
else if((!(UCSRB & (1<<UDRIE))) ) // si l'interrupt n'est pas active
	{
	rs485_txen(1);			// Active la pin de txen
	UDR = data;				// charge le uart avec une donne
	}
else
	{
	put_tx_buffer(data);
	}
UCSRB |= (1<<UDRIE);	// Active l<interrupt du TX

sei();
}

// Send a new data in the uart if any to send
ISR(USART_UDRE_vect)
{
if(tx_buffer_empty())
	{
	UCSRB &= (~(1<<UDRIE)); 	//disable the interrupt
	}
else
	{
	UDR = get_tx_buffer();
	}
}

// Clear the tx_enable on the buffer if nothing else to send
ISR(USART_TXC_vect)
{
rs485_txen(0);				// Clear the txen pin
}


unsigned char rs485_getchar(void)
{
unsigned char data;

while(rx_buffer_empty());

cli();
data = get_rx_buffer();
sei();

return (data);
}

unsigned char rs485_char_ready(void)
{
if(rx_buffer_empty())
	{
	return 0;
	}
return 1;
}
// Put the received char in the buffer
ISR (USART_RXC_vect)
{
unsigned char data_read;

data_read = UDR;

if(!rx_buffer_full())
	{	
	put_rx_buffer(data_read);	
	}
else
	{
	// On perd des donnees !!!
	}
}

