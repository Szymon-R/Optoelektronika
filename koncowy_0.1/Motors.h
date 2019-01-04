#include <Servo.h>
#include "Arduino.h"

typedef struct motor
{
  uint16_t current_position;
  uint16_t setpoint_position;
  uint8_t pin;
  uint16_t min_pulse;
  uint16_t max_pulse;
  uint8_t min_step;
  uint8_t medium_step;
  uint8_t max_step;
  uint8_t offset;
  uint16_t wait;
  Servo myservo;
}motor;

void motors_init_default(motor* m1, motor* m2);
void motor_move(motor* m);
void motor_get_position(motor* m);
void motor_homing(motor* m);
void motor_enable(motor* m);
void motor_disable(motor* m);
void motor_homing(motor* m);
