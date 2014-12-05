#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
//#include <avr/signal.h>

#include "main.h"
#include "buffer.h"

volatile unsigned char tx_buffer[TX_BUFFER_SIZE];
volatile unsigned char tx_buffer_count = 0;
volatile unsigned char tx_buffer_in = 0;
volatile unsigned char tx_buffer_out = 0;

volatile unsigned char rx_buffer[RX_BUFFER_SIZE];
volatile unsigned char rx_buffer_count = 0;
volatile unsigned char rx_buffer_in = 0;
volatile unsigned char rx_buffer_out = 0;


void put_tx_buffer(unsigned char data)
{
while(tx_buffer_count == TX_BUFFER_SIZE);

tx_buffer[tx_buffer_in] = data;
tx_buffer_in ++;
tx_buffer_count++;

if(tx_buffer_in == TX_BUFFER_SIZE)
	{
	tx_buffer_in = 0;
	}
}

unsigned char get_tx_buffer(void)
{
unsigned char data_out = 0;

if(tx_buffer_count != 0)
	{
	data_out = tx_buffer[tx_buffer_out];
	tx_buffer_out++;
	tx_buffer_count--;
	
	if(tx_buffer_out == TX_BUFFER_SIZE)
		{
		tx_buffer_out = 0;
		}
	}
return data_out;
}

unsigned char tx_buffer_empty(void)
{
if(tx_buffer_count == 0)
	return 1;
return 0;
}

unsigned char tx_buffer_full(void)
{
if(tx_buffer_count == TX_BUFFER_SIZE)
	return 1;
return 0;
}

void put_rx_buffer(unsigned char data)
{
if(rx_buffer_count != RX_BUFFER_SIZE)
	{
	rx_buffer[rx_buffer_in] = data;
	rx_buffer_in ++;
	rx_buffer_count++;
	
	if(rx_buffer_in == RX_BUFFER_SIZE)
		{
		rx_buffer_in = 0;
		}
	}
}

unsigned char get_rx_buffer(void)
{
unsigned char data_out;

if(rx_buffer_count != 0)
	{
	cli();
	
	data_out = rx_buffer[rx_buffer_out];
	rx_buffer_out++;
	rx_buffer_count--;
	
	if(rx_buffer_out == RX_BUFFER_SIZE)
		{
		rx_buffer_out = 0;
		}
	
	sei();
	}
return data_out;
}

unsigned char rx_buffer_empty(void)
{
if(rx_buffer_count == 0)
	return 1;
return 0;
}

unsigned char rx_buffer_full(void)
{
if(rx_buffer_count == RX_BUFFER_SIZE)
	return 1;
return 0;
}


