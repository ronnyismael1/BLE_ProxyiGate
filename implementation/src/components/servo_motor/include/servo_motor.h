#ifndef _SERVO_MOTOR_H
#define _SERVO_MOTOR_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void setup_pwm(uint8_t servo_pin);
void set_servo_angle(int angle);

#ifdef __cplusplus
}
#endif

#endif /* _SERVO_MOTOR_H */
