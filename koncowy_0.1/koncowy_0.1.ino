
#include "PCF8563.c"
#include "TWI.c"
#include "PowerDown.c"
#include "Motors.h"
#include "Control.c"
#include "ADC.c"

fotoresistors f;
ADC_params params;
registers r;
date d1,d2;
motor m1,m2;
control_prams c;
void setup() {
  
  ADC_Init(&f,&params);
  TWIInit();
  PCF8563_init_default(&r);
  control_init_default(&c);
  reset_all(&r);
  init_power_down();
  motors_init_default(&m1,&m2);
  d2.seconds=50;
  d2.minutes=1;
  d2.hours=1;
  d2.days=1;
  set_date(&d2,&r);
 
  //d1.seconds=15;

 // d1.hours=14;
 // d1.days=5;
  delay(10);
     d1.minutes=1;
  alarm_set_incremental(&d1,&r);
  delay(10);
  alarm_enable(&r);
  delay(10);
  Serial.begin(9600);
  sleep_enable();
  sei(); 
 // motor_homing(&m1);
  m1.current_position=m1.max_pulse;
}
void loop() {
  
//mozna dodac minimalny poziom, przy ktorym nastapi detekcja
  go_to_sleep();
  delay(100);
  alarm_accept(&r);
  delay(100);
  read_resistors(&f,&params);
  correct_fotoresistors(&f,&params);
  print_fotoresistors(&f);
  correct_position(&f,&c,&m1,&m2);
  delay(10);
  d1.minutes=10;
  alarm_set_incremental(&d1,&r);



  /*
  read_resistors(&f,&params);
  correct_fotoresistors(&f,&params);
  print_fotoresistors(&f);
  Serial.println(check_axis_1(&f,&c));
  delay(100);*/
}


ISR(ADC_vect)
{
  uint16_t result;
  result=ADCL;
  result|=(((uint16_t)ADCH)<<8);
  float temp;
  temp=(float)result*(float)params.reference_voltage/(float)params.resolution;
  switch (f.state)
  {
    case 1:
    f.Vr4=temp;
    break;
    
    case 2:
    f.Vr3=temp;
    break;
    
    case 3:
    f.Vr2=temp;
    break;
    
    case 4:
    f.Vr1=temp;
    f.state=0;
    break;
  
  }
  //Serial.println("Tutaj2");
}

ISR(INT0_vect)
{
    
} 

void print_date(date* d)
{
  
  Serial.print(d->hours);
  Serial.print(":");
  Serial.print(d->minutes);
    Serial.print(":");
  Serial.print(d->seconds);
  Serial.print("   ");
  Serial.print(d->days);
    Serial.print("/");
  Serial.print(d->months);
  Serial.print("/20");
  Serial.print(d->years);
  Serial.print("\n");
}

void print_fotoresistors(fotoresistors* f)
{
  
    Serial.print(f->Vr1);
    Serial.print("  ");
    Serial.print(f->Vr2);
    Serial.print("  ");
    Serial.print(f->Vr3);
    Serial.print("  ");
    Serial.print(f->Vr4);
    Serial.print("\n");
}

void read_all(registers* r)
{
  uint8_t temp;
  for(int i=0; i<16;++i)
  {
    Serial.print("0x");
    Serial.print(i,HEX);
    Serial.print(" ");
    Serial.print(((read_register(i))));
    Serial.print("\n");
  }
  Serial.print("\n");
}
void print_servo_info(motor* m1,motor* m2)
{
  Serial.print("Servo1: ");
  Serial.print(m1->current_position);
  Serial.print(" Servo2: ");
  Serial.print(m2->current_position);
  Serial.print("\n");
}

int correct_position(fotoresistors* f,control_prams* c,motor* m1, motor* m2)
{

  while(check_axis_2(f,c)!=0)
  {
    if(check_axis_2(f,c)==-1)
    {
      if(m1->current_position>=m1->max_pulse)
      {
        Serial.println("max1");
        break;
      }
      else
      {
        m1->setpoint_position=(m1->current_position+c->one_step);
        motor_move(m1);
      }
    }
    else if(check_axis_2(f,c)==1)
    {
      if(m1->current_position<=m1->min_pulse)
      {
        Serial.println("min1");
        break;
      }
      else
      {
        m1->setpoint_position=(m1->current_position-c->one_step);
        motor_move(m1);
      }
    }
    read_resistors(f,&params);
    correct_fotoresistors(f,&params);
    print_fotoresistors(f); 
  }
}
