#ifndef _ADC_H_
#define _ADC_H_

void adc_init(void);
unsigned int adc_read(unsigned char channel);
void adc_start_conversion(unsigned char channel);
unsigned char adc_conversion_ready(unsigned char channel);

#endif //_ADC_H_
