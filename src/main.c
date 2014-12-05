#include "main.h"

unsigned char main_timer_1ms;

//#define DEBUG_TERMINAL
#define NORMAL_MODE
//#define SCOPE_MODE


int main(void)
    {
#ifdef NORMAL_MODE
    unsigned char id_state = 0;
//    unsigned char commande_test = 0;
#endif

#ifdef SCOPE_MODE
    float time = 0;
    float sin_val;
#endif

#ifdef DEBUG_TERMINAL
    unsigned int rx_char;
    signed int pwm_value = 0;
#endif

    SFIOR &= ~(1 << PUD); // enable all the pull-up

    DDRA &= (~(1 << PA4));

    LSTA_INIT;
    RSTA_INIT;

    ENABLE_OUT_INIT;
    ENABLE_OUT_CLEAR;

    ENABLE_IN_INIT;

    SPARE_OUT_INIT;

    rs485_init(115200);
    timebase_init();
    status_led_init();
    pwm_init();
    encoder_init();
    adc_init();
    boucle_courant_init();
    protocol_init();

    // Activer le printf
    fdevopen((void *) rs485_putchar, 0);
    vt100_assign_put_char_function(rs485_putchar);

    sei();

    while (1)
	{
	process_status_led();

	if (main_timer_1ms)
	    {
	    main_timer_1ms = 0;
	    adc_start_conversion(0);
	    }
	if (adc_conversion_ready(0))
	    {
	    //current_read = (float)((517 - ((signed int)adc_read(0))) * 0.00468750); // Lecture de l'ADC ajusté pour avoir des mA
	    current_read = (float) adc_read(0);
	    current_read = (-0.0000143934 * current_read * current_read * current_read)
		    + (0.022306707 * current_read * current_read) - (12.5172850725 * current_read)
		    + 2513.5726145723;
	    current_read /= 1000.0; // conversion en amp

#ifdef NORMAL_MODE
	    process_boucle_courant();
#endif

#ifdef SCOPE_MODE
	    time+= 0.001;
	    sin_val = sin(4.0 * 2.0 * 3.1416 * time); // 1Hz sinus
	    current_ref = sin_val / 2.0; // 500mA Amplitude

	    process_boucle_courant();

	    printf("%6.3f,%5.3f,%5.3f\r\n",time,current_ref,current_read);

#endif
	    }

#ifdef NORMAL_MODE
	 if (id_state == 1)
	 {
	 process_protocol_tx();
	 process_protocol_rx();
	 }
	 else
	 {
	 id_state = process_protocol_alloc_id();
	 }
#endif

#ifdef DEBUG_TERMINAL

	if (rs485_char_ready())
	    {
	    rx_char = rs485_getchar();

	    switch (rx_char)
		{
		case ' ':
		    {
		    vt100_clrscr();

		    vt100_gotoxy(1, 1);
		    vt100_puts("Menu de debug pour controlleur ANAT V7.1");

		    vt100_frame_title(1, 35, 3, 8, 1);
		    vt100_gotoxy(2, 4);
		    vt100_puts("Signaux de sortie");
		    vt100_gotoxy(2, 6);
		    vt100_puts("(q/a)Enable Out   :");
		    vt100_gotoxy(2, 7);
		    vt100_puts("(w/s)Spare Out    :");

		    vt100_frame_title(1, 35, 9, 22, 1);
		    vt100_gotoxy(2, 10);
		    vt100_puts("(i)Signaux d'entree");
		    vt100_gotoxy(2, 12);
		    vt100_puts("36V (ADC)         :");
		    vt100_gotoxy(2, 13);
		    vt100_puts("Current (ADC)     :");
		    vt100_gotoxy(2, 14);
		    vt100_puts("Current (ADC)     :");
		    vt100_gotoxy(2, 15);
		    vt100_puts("Encoder value     :");
		    vt100_gotoxy(2, 16);
		    vt100_puts("RSTA              :");
		    vt100_gotoxy(2, 17);
		    vt100_puts("LSTA              :");
		    vt100_gotoxy(2, 18);
		    vt100_puts("ENABLE_IN         :");
		    vt100_gotoxy(2, 19);
		    vt100_puts("RESET             :");
		    vt100_gotoxy(2, 20);
		    vt100_puts("STOP              :");
		    vt100_gotoxy(2, 21);
		    vt100_puts("SPARE_IN          :");

		    vt100_frame_title(37, 70, 3, 8, 1);
		    vt100_gotoxy(38, 4);
		    vt100_puts("PWM Moteur");
		    vt100_gotoxy(38, 6);
		    vt100_puts("(e/d) PWM         :");
		    vt100_gotoxy(38, 7);
		    vt100_puts("(r) Reset Encoder");

		    }
		break;

		case 'e':
		    {

		    pwm_value += 1;
		    pwm_set(pwm_value);

		    vt100_gotoxy(58, 6);
		    printf("%d   ", pwm_value);
		    }
		break;
		case 'd':
		    {
		    pwm_value -= 1;
		    pwm_set(pwm_value);

		    vt100_gotoxy(58, 6);
		    printf("%d   ", pwm_value);
		    }
		break;

		case 'r':
		    {
		    reset_encoder();
		    }
		break;
		case 'i':
		    {
		    vt100_gotoxy(22, 12);
		    printf("%d", adc_read(1));
		    vt100_gotoxy(22, 13);
		    printf("%d(adc)", adc_read(0));
		    vt100_gotoxy(22, 14);
		    printf("%5.2f(amp)", (double)current_read);
		    vt100_gotoxy(22, 15);
		    printf("%li", get_encoder());
		    vt100_gotoxy(22, 16);
		    printf("%d", (PINB & (1<<PB1))!=0);
		    vt100_gotoxy(22, 17);
		    printf("%d", (PINB & (1<<PB3))!=0);
		    vt100_gotoxy(22, 18);
		    printf("%d", (PINA & (1<<PA2))!=0);
		    vt100_gotoxy(22, 19);
		    printf("%d", (PINA & (1<<PA3))!=0);
		    vt100_gotoxy(22, 20);
		    printf("%d", (PINA & (1<<PA4))!=0);
		    vt100_gotoxy(22, 21);
		    printf("%d", (PINA & (1<<PA5))!=0);

		    }
		break;

		case 'q':
		    {
		    ENABLE_OUT_SET;
		    vt100_gotoxy(22, 6);
		    vt100_putchar('1');
		    }
		break;
		case 'a':
		    {
		    ENABLE_OUT_CLEAR;
		    vt100_gotoxy(22, 6);
		    vt100_putchar('0');
		    }
		break;
		case 'w':
		    {
		    SPARE_OUT_SET;
		    vt100_gotoxy(22, 7);
		    vt100_putchar('1');
		    }
		break;
		case 's':
		    {
		    SPARE_OUT_CLEAR;
		    vt100_gotoxy(22, 7);
		    vt100_putchar('0');
		    }
		break;
		}
	    }
#endif		
	}
    return 0;
    }
