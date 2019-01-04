#include "PCF8563.h"
#include "TWI.h"
void read_date(date* d,registers* r)
{
  TWIStart();
  TWIWrite(SLA_W);
  TWIWrite(r->seconds);
  TWIStart();
  TWIWrite(SLA_R);
  d->seconds=from_BCD(TWIReadACK()& B01111111);
  d->minutes=from_BCD(TWIReadACK()& B01111111);
  d->hours=from_BCD(TWIReadACK()& B00111111);
  d->days=from_BCD(TWIReadACK()& B00111111);
  d->weekdays=from_BCD(TWIReadACK()& B00000111);
  d->months=from_BCD(TWIReadACK()& B00011111);
  d->years=from_BCD(TWIReadNACK());
  TWIStop();

}

void set_date(date*d, registers* r)
{
  TWIStart();
  TWIWrite(SLA_W);
  TWIWrite((r->seconds));
  TWIWrite(to_BCD(d->seconds));
  TWIWrite(to_BCD(d->minutes));
  TWIWrite(to_BCD(d->hours));
  TWIWrite(to_BCD(d->days));
  TWIWrite(to_BCD(d->weekdays));
  TWIWrite(to_BCD(d->months));
  TWIWrite(to_BCD(d->years));
  TWIStop();
}

uint8_t write_register(uint8_t destination, uint8_t data)
{
  TWIStart();
  TWIWrite(SLA_W);
  TWIWrite(destination);
  TWIWrite(data);
  TWIStop();
}

uint8_t read_register(uint8_t destination)
{
  uint8_t temp;
  TWIStart();  
  if ((TWSR & 0xF8) !=0x08)
    ERROR(1);
  TWIWrite(SLA_W);
    if ((TWSR & 0xF8) !=0x18)
    ERROR(2);
  TWIWrite(destination);
      if ((TWSR & 0xF8) !=0x28)
    ERROR(3);
  TWIStart();
        if ((TWSR & 0xF8) !=0x10)
    ERROR(4);
  TWIWrite(SLA_R);
        if ((TWSR & 0xF8) !=0x40)
       ERROR(5);
  temp=TWIReadNACK();
          if ((TWSR & 0xF8) !=0x58)
       ERROR(5);
  TWIStop();
  return temp;
//  return from_BCD(temp);
}

void PCF8563_init_default(registers* r)
{
  r->control1=0x00;
  r->control2=0x01;
  r->seconds=0x02;
  r->minutes=0x03;
  r->hours=0x04;
  r->days=0x05;
  r->weekdays=0x06;
  r->months=0x07;
  r->years=0x08;
  r->minute_alarm=0x09;
  r->hour_alarm=0x0A;
  r->day_alarm=0x0B;
  r->weekday_alarm=0x0C;
  r->CLKOUT_control=0x0E;
  r->timer=0x0F;
}
uint8_t to_BCD(uint8_t number)
{
  uint8_t temp=(number%10);
  number/=10;
  temp+=number<<4;
  return temp;
}

uint8_t from_BCD(uint8_t number)
{
  uint8_t temp=number&0x0F;
  temp+=(number>>4)*10;
  return temp;
}

void alarm_enable(registers* r)
{
  uint8_t temp;
  temp=read_register(r->control2);
  temp|=(0b00000010);
  write_register(r->control2,temp);
}

void alarm_disable(registers* r)
{
  uint8_t temp;
  temp=read_register(r->control2);
  temp&=(~0b00000010);
  write_register(r->control2,temp);
}

void alarm_set_absolute(date* d,registers* r)
{
  if((d->minutes)!=0)
  {
    write_register(r->minute_alarm,to_BCD(d->minutes));
    d->minutes=0;
  }
  if((d->hours)!=0)
  {
    write_register(r->hour_alarm,to_BCD(d->hours));
    d->hours=0;
  }
  if((d->days)!=0)
  {
    write_register(r->day_alarm,to_BCD(d->days));
    d->days=0;
  }
  if((d->weekdays)!=0)
  {
    write_register(r->weekday_alarm,to_BCD(d->weekdays));
     d->weekdays=0;
  }
}

//miesiące są niedokończone
void alarm_set_incremental(date* d,registers* r)
{
  date d1;
  read_date(&d1,r);
  if(d->seconds!=0)
  {
    d->seconds+=d1.seconds; 
    d->minutes+=(d->seconds>60 ? 1 : 0);
    d->seconds%=60;
  }
  

  if(d->minutes!=0)
  {
    d->minutes+=d1.minutes; 
    d->hours+=(d->minutes>60 ? 1 : 0);
    d->minutes%=60;
  }
//  else
//    d->minutes=0x80;
  if(d->hours!=0)
  {
    d->hours+=d1.hours; 
    d->days+=(d->hours>24 ? 1 : 0);
    d->days%=24;
  }  
 //   else
 //   d->hours=0x80;
  alarm_set_absolute(d,r); 
}

void alarm_reset(registers* r)
{
   write_register(r->minute_alarm,0x80);
   write_register(r->hour_alarm,0x80);
   write_register(r->day_alarm,0x80);
   write_register(r->weekday_alarm,0x80);
}
void alarm_accept(registers* r)
{
  uint8_t temp;
  temp=read_register(r->control2);
  temp&=(~0b00001000);
  write_register(r->control2,temp);
}

bool check_alarm(registers* r)
{
  uint8_t temp=0;
  if(read_register(r->control2)&0b00001000)
    return 1;
   return 0;
}
void reset_all(registers* r)
{
  for(int i=0; i<9;++i)
  {
    write_register(i,0);
  }
  alarm_reset(r);
 write_register(0x0D,0x83);
 write_register(0x0E,0x03);
 write_register(0x0F,0x00);
}
