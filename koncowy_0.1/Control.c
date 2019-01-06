#include "Control.h"



void control_init_default(control_prams* c)
{
  c->sensitivity=0.05;
  c->one_step=10;
}
int check_axis_1(fotoresistors* f,control_prams* c)
{
  float sum1;
  float sum2;
  sum1=(f->Vr4+f->Vr3)/2.0;
  sum2=(f->Vr1+f->Vr2)/2.0;
  if((sum1-sum2)>c->sensitivity)
    return 1;
  else  if((sum2-sum1)>c->sensitivity)
    return -1;
  else
    return 0;
}

int check_axis_2(fotoresistors* f,control_prams* c)
{
  float sum1;
  float sum2;
  sum1=(f->Vr2+f->Vr3)/2.0;
  sum2=(f->Vr1+f->Vr4)/2.0;
  if((sum1-sum2)>c->sensitivity)
    return 1;
  else  if((sum2-sum1)>c->sensitivity)
    return -1;
  else
    return 0;
}
