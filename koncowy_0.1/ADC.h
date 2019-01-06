#ifndef ADC_H
#define ADC_H

#include "Arduino.h"

typedef struct fotoresistors
{
  float Vr1;
  float Vr2;
  float Vr3;
  float Vr4;
  uint8_t state;
  
}fotoresistors;

typedef struct ADC_params
{
  uint16_t resolution;
  uint8_t reference_voltage;
  
}ADC_params;

void ADC_Init(fotoresistors* f, ADC_params* params);
void read_resistors(fotoresistors* f, ADC_params* params);
void correct_fotoresistors(fotoresistors* f, ADC_params* params);

#endif /*ADC_C*/
