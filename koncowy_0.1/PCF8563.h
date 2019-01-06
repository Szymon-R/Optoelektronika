#include "Arduino.h"
#define SLA_W 0xA2
#define SLA_R 0xA3

typedef struct registers
{
  uint8_t control1;
  uint8_t control2;
  uint8_t seconds;
  uint8_t minutes;
  uint8_t hours;
  uint8_t days;
  uint8_t weekdays;
  uint8_t months;
  uint8_t years;
  uint8_t minute_alarm;
  uint8_t hour_alarm;
  uint8_t day_alarm;
  uint8_t weekday_alarm;
  uint8_t CLKOUT_control;
  uint8_t timer_control;
  uint8_t timer;
}registers;


typedef struct date
{
  uint8_t seconds;
  uint8_t minutes;
  uint8_t hours;
  uint8_t days;
  uint8_t weekdays;
  uint8_t months;
  uint8_t years;
}date;

uint8_t to_BCD(uint8_t number);
uint8_t from_BCD(uint8_t number);
void read_date(date* d,registers* r);
void set_date(date* d,registers* r);
void PCF8563_init_default(registers* r);
void print_date(date* d);
void alarm_enable(registers* r);
void alarm_disable(registers* r);
void alarm_set_absolute(date* d,registers* r);
void alarm_set_incremental(date* d,registers* r);
void alarm_reset(registers* r);//ta funkcja jest w zasadzie niepotrzebna
void alarm_accept(registers* r);
bool check_alarm(registers* r);
void reset_all(registers* r);
void print_date(date* d);
