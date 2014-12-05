#ifndef BUFFER_H
#define BUFFER_H

#define TX_BUFFER_SIZE 20
#define RX_BUFFER_SIZE 20

extern volatile unsigned char tx_buffer[TX_BUFFER_SIZE];
extern volatile unsigned char tx_buffer_count;
extern volatile unsigned char tx_buffer_in;
extern volatile unsigned char tx_buffer_out;

extern volatile unsigned char rx_buffer[RX_BUFFER_SIZE];
extern volatile unsigned char rx_buffer_count;
extern volatile unsigned char rx_buffer_in;
extern volatile unsigned char rx_buffer_out;


void put_tx_buffer(unsigned char data);
unsigned char get_tx_buffer(void);
unsigned char tx_buffer_empty(void);
unsigned char tx_buffer_full(void);
void put_rx_buffer(unsigned char data);
unsigned char get_rx_buffer(void);
unsigned char rx_buffer_empty(void);
unsigned char rx_buffer_full(void);

#endif
