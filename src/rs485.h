#ifndef RS485_H
#define RS485_H

void rs485_init(uint32_t baud_rate);
void rs485_txen(unsigned char txen);
void rs485_putchar(unsigned char data);
unsigned char rs485_getchar(void);
unsigned char rs485_char_ready(void);

#endif
