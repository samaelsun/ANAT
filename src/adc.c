#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/signal.h> 

volatile unsigned char adc_channel = 0;
volatile unsigned int adc_result[2];
unsigned char adc_result_done[2];

void adc_init(void)
{
	ADMUX =( 1<<REFS0 | 0<<REFS1 );	// Ref is VCC

	ADCSRA= (1<<ADEN | 0<<ADATE) | (7<<ADPS0) | (1<<ADIE) | (0<<ADSC);
	
	PORTA &= (~( (1<<PA0) | (1<<PA1) )); // disable the pullup ADC pin

}

void adc_start_conversion(unsigned char channel)
    {

    	channel &= 0x1F;
    	adc_channel = channel;
    	
    	
    	ADMUX |= channel;
    	ADMUX &= (0xE0 | channel);
    	adc_result_done[channel] = 0;
    	ADCSRA|= 1<<ADSC; //AD Start Conversion    	
    }
 unsigned char adc_conversion_ready(unsigned char channel)
    {
    if(adc_result_done[channel])
	{
	adc_result_done[channel] = 0;
	return 1;
	}
    return 0;
    }

unsigned int adc_read(unsigned char channel)
	{
	unsigned int latch;

	cli();
	latch = adc_result[channel];
	sei();

	return latch;
	}

ISR(ADC_vect)
	{
	unsigned int temp;

	adc_result[adc_channel] = ADCL;
	temp = (ADCH & 0x03);
	temp <<= 8;
	adc_result[adc_channel] += temp;
    	adc_result_done[adc_channel] = 1;
	}

