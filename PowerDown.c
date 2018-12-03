#include <avr/sleep.h>
#define interrupt_pin PD2

void init_power_down()
{
 // DDRD|= 1<<interrupt_pin;    // Set PD2 as input (Using for interupt INT0)
  PORTD|= 1<<interrupt_pin;   // Enable PD2 pull-up resistor
  
  EIMSK |= 1<<INT0;         // Enable INT0
  EICRA |= 1<<ISC01 ;  // Trigger INT0 on falling
}

void go_to_sleep()
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);    //Set sleep mode
  sleep_cpu( );
}
