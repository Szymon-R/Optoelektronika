#include <SoftwareSerial.h>

#include "Motors.h"



void motors_init_default(motor* m1, motor* m2)
{
  m1->pin = 9;
  m2->pin = 10;
 // DDRD|=(1<<m1->pin);
//  DDRD|=(1<<m2->pin);
//  PORTD|=(1<<m1->pin);
//  PORTD|=(1<<m2->pin);
  m1->min_step=1;
  m1->medium_step=20;
  m1->max_step=50;
  m1->offset=100;
  m1->min_pulse=600;
  m1->max_pulse=2600;
  m1->wait=10;

}

void motor_get_position(motor* m)
{
  m->current_position=m->myservo.read();
}

void motor_move(motor* m)
{
  uint16_t temp;
  bool dir;

  //zabezpieczenia
  if(!m->myservo.attached())
    motor_enable(m);
  if(m->setpoint_position>m->max_pulse)
    m->setpoint_position=m->max_pulse;
  if(m->setpoint_position<m->min_pulse)
    m->setpoint_position=m->min_pulse;
      
  //wyznaczanie kierunku obrotow
  if(m->setpoint_position>m->current_position)
    dir=1;
  else if(m->setpoint_position<m->current_position)
    dir=0;
  else 
    return;

    //obroty w prawo
  if(dir==1)
  {
    int diff;
    while(1)
    {
      diff=m->setpoint_position-m->current_position;
      if(diff<0)
        break;
      if(diff>=500)
        m->myservo.write(m->current_position+=m->max_step);
      else if(diff<500&&diff>=100)
        m->myservo.write(m->current_position+=m->medium_step);
      else if(diff<=100)
        m->myservo.write(m->current_position+=m->min_step);
      else
        break;
      delay(m->wait);
    }
  }
  
   //obroty w lewo
  else if (dir==0)
  {
    int diff;
    while(1)
    {
      diff=m->setpoint_position-m->current_position;
      if(diff>=0)
        break;
      if(diff<=(-500))
        m->myservo.writeMicroseconds(m->current_position-=m->max_step);
      else if(diff>(-500)&&diff<=(-100))
        m->myservo.writeMicroseconds(m->current_position-=m->medium_step);
      else if(diff>(-100))
        m->myservo.writeMicroseconds(m->current_position-=m->min_step);
      else
        break;
      delay(m->wait);
    }
  }
  motor_disable(m);
  return;
}
void motor_enable(motor* m)
{
  m->myservo.attach(m->pin,m->min_pulse,m->max_pulse);
}
void motor_disable(motor* m)
{
  m->myservo.detach();
}

void motor_homing(motor* m)
{
  if(!m->myservo.attached())
  motor_enable(m);
  m->setpoint_position=m->max_pulse;
  motor_move(m);
  motor_disable(m);
}
