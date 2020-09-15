/**
 * @file servo.cpp
 * @author Philipp Köhler
 * @version 0.1
 * @date 2019-08-17
 * @date 2020-08-04 - last modified
 * @copyright Copyright (c) 2020
 * 
 */
#include "servo.h"

/**
 * @brief Construct a new Servo:: Servo object
 * 
 * @param gpio_num 
 * @param startDuty 
 * @param frequency 
 * @param ledc_timer 
 * @param ledc_timer_bit 
 * @param ledc_mode 
 * @param ledc_channel 
 */
Servo::Servo(gpio_num_t gpio_num,
             uint32_t startDuty,
             uint32_t frequency,
             ledc_timer_t ledc_timer,
             ledc_timer_bit_t ledc_timer_bit,
             ledc_mode_t ledc_mode,
             ledc_channel_t ledc_channel)
             :ledc_timer(ledc_timer),
             ledc_timer_bit(ledc_timer_bit),
             ledc_mode(ledc_mode),
             ledc_channel(ledc_channel)
{
    this->gpio_num = gpio_num;
    this->frequency = frequency;

    ledc_timer_config_t timer_conf;
	timer_conf.bit_num    = this->ledc_timer_bit;
	timer_conf.freq_hz    = this->frequency;
	timer_conf.speed_mode = this->ledc_mode;
	timer_conf.timer_num  = this->ledc_timer;
	ledc_timer_config(&timer_conf);

    ledc_channel_config_t ledc_conf;
	ledc_conf.channel    = this->ledc_channel;
	ledc_conf.duty       = startDuty;
	ledc_conf.gpio_num   = gpio_num;
	ledc_conf.intr_type  = LEDC_INTR_DISABLE;
	ledc_conf.speed_mode = this->ledc_mode;
	ledc_conf.timer_sel  = this->ledc_timer;
	ledc_channel_config(&ledc_conf);
}

/**
 * @brief Set the duty cycle value
 * 
 * @param duty 
 */
void Servo::setServoDuty(uint32_t duty)
{
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, duty);
	ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
}

