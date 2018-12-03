#include <avr/sleep.h>
#include "ADC.h"

void ADC_Init(fotoresistors* f, ADC_params* params)
{
  ADCSRA = (1<<ADEN) |(1<<ADPS0) |(1<<ADPS1)  |(1<<ADPS2)|(1<<ADIE);  
  ADMUX  =    (0<<REFS1) | (0<<REFS0)   |(0<<MUX2) | (0<<MUX0);
   f->state=0;
   params->resolution=1024;
   params->reference_voltage=5;
   params->reference_voltage=5;
   params->resolution=1024;
   DDRC=0x00;
   PINC=0x01;

}
void read_resistors(fotoresistors* f, ADC_params* params)
{
  set_sleep_mode(SLEEP_MODE_ADC);    //Set sleep mode
  ADMUX  &=  0xF0;
  ADMUX |=  (0<<MUX3) | (0<<MUX2)| (0<<MUX1)| (0<<MUX0);
  f->state++;
  sleep_cpu  ();
  delay(2);//jedna konwersja trwa 200us
  
  ADMUX  &=  0xF0;
  ADMUX|=  (0<<MUX3) | (0<<MUX2)| (0<<MUX1)| (1<<MUX0);
  f->state++;
  sleep_cpu  ();
  delay(2);//jedna konwersja trwa 200us
  
  ADMUX  &=  0xF0;
  ADMUX|=  (0<<MUX3) | (0<<MUX2)| (1<<MUX1)| (0<<MUX0);
  f->state++;
  sleep_cpu  ();
  delay(2);//jedna konwersja trwa 200us
  
  ADMUX  &=  0xF0;
  ADMUX|=  (0<<MUX3) | (0<<MUX2)| (1<<MUX1)| (1<<MUX0);
  f->state++;
  sleep_cpu();
  delay(2);//jedna konwersja trwa 200us
}
