#include "Arduino.h"
#include "ADC.h"

typedef struct control_prams
{
  float sensitivity;
  float one_step;
}control_prams;

void control_init_default(control_prams* c);
int check_axis_1(fotoresistors* f,control_prams* c);
int check_axis_2(fotoresistors* f,control_prams* c);
void correct_position(fotoresistors* f,control_prams* c);
