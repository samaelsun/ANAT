#ifndef _VT100_H_
#define _VT100_H_

#include <avr/io.h>
#include <inttypes.h>

void vt100_assign_put_char_function(void* function_address);
void vt100_putchar(unsigned char data);
void vt100_puts(const char * s);           			// write a string of chars
void vt100_backspace(unsigned char count);			// Clear some character to the left
void vt100_clrscr(void);                       // clear terminal window
void vt100_putint(unsigned int data,unsigned char digit);
void vt100_gotoxy(unsigned char xpos, unsigned char ypos);
void vt100_puthex(unsigned char data);
unsigned char vt100_math_modulo_10(unsigned char input);
unsigned char vt100_math_divide_10(unsigned char input);
void vt100_left(unsigned char number);
unsigned int rs232_get_int(unsigned char num_digit);
void vt100_frame(unsigned char left_x,unsigned char right_x,unsigned char top_y,unsigned char bot_y);
void vt100_frame_title(unsigned char left_x, unsigned char right_x, unsigned char top_y, unsigned char bot_y,unsigned char title_length);
#endif //_VT100_H_
