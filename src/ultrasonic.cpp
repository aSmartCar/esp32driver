/**
 * @file ultrasonic.cpp
 * @author Philipp Köhler
 * @version 0.1
 * @date 2019-08-17 - created
 * @date 2020-08-04 - last modified
 * @copyright Copyright (c) 2020
 * 
 */
#include "ultrasonic.h"

/**
 * @brief Construct a new Ultrasonic:: Ultrasonic object
 * 
 * @param gpio_echo 
 * @param gpio_trigger 
 */
Ultrasonic::Ultrasonic(gpio_num_t gpio_echo, gpio_num_t gpio_trigger)
{
    this->gpio_echo = gpio_echo;
    this->gpio_trigger = gpio_trigger;

    pinMode(gpio_echo, INPUT_PULLUP);
    pinMode(gpio_trigger, OUTPUT);
}

/**
 * @brief Get current sensor values as distance in cm
 * 
 * @return float 
 */
float Ultrasonic::getUltrasonicValue()
{
    digitalWrite(this->gpio_trigger, LOW);
    delayMicroseconds(3);
    noInterrupts();
    digitalWrite(this->gpio_trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(this->gpio_trigger, LOW);
    float value = pulseIn(this->gpio_echo, LOW, 200000);
    interrupts();
    value /= 2;
    value /= 29.1;
    return value;
}