/**
 * @file servo.h
 * @author Philipp Köhler
 * @brief Provides an interface to the servo motors 
 * @version 0.1
 * @date 2019-08-17 - created
 * @date 2020-08-04 - last modified
 * @copyright Copyright (c) 2020
 */
#ifndef SERVO_H
#define SERVO_H

#include <Arduino.h>
#include <driver/ledc.h>

class Servo
{
public:
    Servo(gpio_num_t gpio_num,
         uint32_t startDuty,
         uint32_t frequency,
         ledc_timer_t ledc_timer,
         ledc_timer_bit_t ledc_timer_bit,
         ledc_mode_t ledc_mode,
         ledc_channel_t ledc_channel);

    void setMaxPulseWidth(uint32_t maxPulseWidth);
    void setMinPulseWidth(uint32_t maxPulseWidth);
    void setServoDuty(uint32_t duty);
    
    uint32_t getMaxPulseWidth();
    uint32_t getMinPulseWidth();

private:
    const ledc_timer_t ledc_timer;
    const ledc_timer_bit_t ledc_timer_bit;
    const ledc_mode_t ledc_mode;
    const ledc_channel_t ledc_channel;

    gpio_num_t gpio_num;
    uint32_t frequency;
};

#endif //SERVO_H