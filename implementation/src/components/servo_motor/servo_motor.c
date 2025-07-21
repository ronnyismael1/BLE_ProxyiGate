#include "servo_motor.h"
#include "driver/ledc.h"

#define SPEED_MODE      LEDC_LOW_SPEED_MODE
#define TIMER_NUM       LEDC_TIMER_0
#define DUTY_RESOLUTION LEDC_TIMER_13_BIT
#define FREQUENCY       50
#define CLK_CONFIG      LEDC_AUTO_CLK
#define CHANNEL         LEDC_CHANNEL_0
#define INTR_TYPE       LEDC_INTR_DISABLE

#define SERVO_MIN_PULSEWIDTH 1120  // us
#define SERVO_MAX_PULSEWIDTH 1920  // us
#define SERVO_MAX_DEGREE     90

void setup_pwm(uint8_t SERVO_PIN)
{
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = SPEED_MODE,
        .timer_num        = TIMER_NUM,
        .duty_resolution  = DUTY_RESOLUTION,
        .freq_hz          = FREQUENCY,
        .clk_cfg          = CLK_CONFIG
    };
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
        .speed_mode     = SPEED_MODE,
        .channel        = CHANNEL,
        .timer_sel      = TIMER_NUM,
        .intr_type      = INTR_TYPE,
        .gpio_num       = SERVO_PIN,
        .duty           = 0,
        .hpoint         = 0
    };
    ledc_channel_config(&ledc_channel);
}

void set_servo_angle(int angle)
{
    int pulsewidth = SERVO_MIN_PULSEWIDTH +
                     ((SERVO_MAX_PULSEWIDTH - SERVO_MIN_PULSEWIDTH) * angle) /
                     SERVO_MAX_DEGREE;
    int duty = (pulsewidth * 8192) / 20000;  // for 13-bit resolution
    ledc_set_duty(SPEED_MODE, CHANNEL, duty);
    ledc_update_duty(SPEED_MODE, CHANNEL);
}

