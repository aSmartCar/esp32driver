/**
 * @file ultrasonic.h
 * @author Philipp Köhler
 * @brief Provides an interface to control the ultrasonic modules
 * @version 0.1
 * @date 2019-08-17 - created
 * @date 2020-08-04 - last modified
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef ULTRASONIC_H
#define ULTRASONIC_H
#include <Arduino.h>

class Ultrasonic
{
public:
    Ultrasonic(gpio_num_t gpio_echo, gpio_num_t gpio_trigger);
    float getUltrasonicValue();

private:
    gpio_num_t gpio_echo;
    gpio_num_t gpio_trigger;
};

#endif