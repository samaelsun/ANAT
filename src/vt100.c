#include "main.h"
#include "vt100.h" 

void (*vt100_putchar_int)(unsigned char);

void vt100_assign_put_char_function(void* function_address)
{
vt100_putchar_int = function_address;
}

//***********************************************************

void vt100_putchar(unsigned char data)
    {
    vt100_putchar_int(data);
    }

//***********************************************************

void vt100_puts(const char * s)           			// write a string of chars
{
while(*s)                                       // while not reach NULL char
  {
  vt100_putchar_int(*s++);
  }
}

//***********************************************************
void vt100_backspace(unsigned char count)			// Clear some character to the left
{
if(count)
	{
	while(count)
		{
		vt100_putchar_int(27);
		vt100_puts("[1D ");
		vt100_putchar_int(27);
		vt100_puts("[1D");
		count--;
		}
	}
}

//***********************************************************
void vt100_clrscr(void)                       // clear terminal window
{
vt100_putchar_int(27);
vt100_puts("[2J");
vt100_putchar_int(27);
vt100_puts("[f");
}

//***********************************************************
void vt100_putint(unsigned int data,unsigned char digit)
{
unsigned char caract[6];
unsigned char compteur = 0; 

while(data)
	{
	caract[compteur++] = (unsigned long)(data % 10); 
	data /= 10;
	}
while(compteur < digit)
	{
	caract[compteur] = 0;
	compteur++;
  	}
while(digit>0)
	{
	digit--;
	vt100_putchar_int(caract[digit]+0x30);
	}
}

//***********************************************************
void vt100_gotoxy(unsigned char xpos, unsigned char ypos)
{
vt100_putchar_int(27);
vt100_puts("[");
if(ypos < 10 )
        {
        vt100_putchar_int(ypos+0x30);
        }
else
        {
        vt100_putchar_int((vt100_math_divide_10(ypos))+ 0x30);
        vt100_putchar_int((vt100_math_modulo_10(ypos))+ 0x30);
        }
vt100_putchar_int(';');
if(xpos < 10 )
        {
        vt100_putchar_int(xpos+0x30);
        }
else
        {
        vt100_putchar_int((vt100_math_divide_10(xpos))+ 0x30);
        vt100_putchar_int((vt100_math_modulo_10(xpos))+ 0x30);
        }
vt100_putchar_int('H');
}

void vt100_puthex(unsigned char data)
{
unsigned char carac_send;

carac_send = (data & 0x0f);
if((carac_send) <= 9)
    carac_send += 0x30;
else
    carac_send += 55;  // -10 + 'A'

data >>= 4;

if(data <= 9)
    vt100_putchar_int(data + 0x30);
else
    vt100_putchar_int(data + 55);     // -10 + 'A'

vt100_putchar_int(carac_send);
}

//***********************************************************

unsigned char vt100_math_modulo_10(unsigned char input)
	{
	return(input % 10);
	}

unsigned char vt100_math_divide_10(unsigned char input)
	{
	return(input / 10);
	}

void vt100_left(unsigned char number)
	{
	vt100_putchar_int(27);
	vt100_putchar_int('[');
	vt100_putint(number,2);
	vt100_putchar_int('D');
	}

void vt100_frame(unsigned char left_x, unsigned char right_x, unsigned char top_y, unsigned char bot_y)
    {
    unsigned char i;

    // Draw the top horizontal line
    vt100_gotoxy(left_x, top_y);
    for (i = left_x; i < right_x; i++)
	{
	vt100_putchar_int(205);
	}

    // Draw the bot horizontal line
    vt100_gotoxy(left_x, bot_y);
    for (i = left_x; i < right_x; i++)
	{
	vt100_putchar_int(205);
	}

    // Draw the two side line
    for (i = top_y; i < bot_y; i++)
	{
	vt100_gotoxy(left_x, i);
	vt100_putchar_int(186);
	vt100_gotoxy(right_x, i);
	vt100_putchar_int(186);
	}

    //draw the four corner
    vt100_gotoxy(left_x, top_y);
    vt100_putchar_int(201);
    vt100_gotoxy(right_x, top_y);
    vt100_putchar_int(187);
    vt100_gotoxy(left_x, bot_y);
    vt100_putchar_int(200);
    vt100_gotoxy(right_x, bot_y);
    vt100_putchar_int(188);
    }

void vt100_frame_title(unsigned char left_x, unsigned char right_x, unsigned char top_y, unsigned char bot_y,unsigned char title_length)
    {
    unsigned char i;

    // Draw the top horizontal line
    vt100_gotoxy(left_x, top_y);
    for (i = left_x; i < right_x; i++)
	{
	vt100_putchar_int(205);
	}

    // Draw the bot horizontal line
    vt100_gotoxy(left_x, bot_y);
    for (i = left_x; i < right_x; i++)
	{
	vt100_putchar_int(205);
	}

    // Draw the title horizontal line
    vt100_gotoxy(left_x, top_y+title_length+1);
    for (i = left_x; i < right_x; i++)
	{
	vt100_putchar_int(196);
	}

    // Draw the two side line
    for (i = top_y; i < bot_y; i++)
	{
	vt100_gotoxy(left_x, i);
	vt100_putchar_int(186);
	vt100_gotoxy(right_x, i);
	vt100_putchar_int(186);
	}

    //draw the four corner
    vt100_gotoxy(left_x, top_y);
    vt100_putchar_int(201);
    vt100_gotoxy(right_x, top_y);
    vt100_putchar_int(187);
    vt100_gotoxy(left_x, bot_y);
    vt100_putchar_int(200);
    vt100_gotoxy(right_x, bot_y);
    vt100_putchar_int(188);
    
    // Draw the title_line connection
    vt100_gotoxy(left_x, top_y+title_length+1);
    vt100_putchar_int(199);
    vt100_gotoxy(right_x, top_y+title_length+1);
    vt100_putchar_int(182);
    
    
    
    
    }


/*
unsigned int rs232_get_int(unsigned char num_digit)
	{
	unsigned int value = 0;  
  unsigned char key = 0;
	unsigned char key_count = 0;

	while(key != 27)
		{
		key = rs232_getch();    
		if(key == 13)
			{
			return(value);
			}
		else if((key == 8) && (key_count>0))					// if key is backspace and number to erase
			{
			rs232_backspace(1);
			key_count--;
			value /= 10;
			}
		else if(key_count >= num_digit)
			{
			//discard this key because user is too moron !!!
			}
		else if((key >= '0') && (key<='9'))
			{
			rs232_putchar(key);
			key_count++;
			key-=0x30;
			value *= 10;
			value += key;
			}
		}
	return(0);
	}        
*/
