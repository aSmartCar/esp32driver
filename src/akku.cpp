/**
 * @file akku.cpp
 * @author Philipp Köhler
 * @version 0.1
 * @date 2019-08-17 - created
 * @date 2020-08-04 - last modified
 * @copyright Copyright (c) 2020
 * 
 */
#include "akku.h"

/**
 * @brief Construct a new Akku:: Akku object
 * 
 * @param gpio_akku 
 */
Akku::Akku(gpio_num_t gpio_akku, uint32_t voltage)
{
    this->gpio_akku = gpio_akku;
    this->voltage = voltage;

    pinMode(gpio_akku, INPUT);
}

/**
 * @brief Get the current voltage value
 * 
 * @return float 
 */
float Akku::getAkkuValue()
{
    analogReadResolution(12);
    double value = analogRead(this->gpio_akku);
    double tmp_voltage = this->voltage;
    tmp_voltage /= 4095;
    value *= tmp_voltage;
    return value;
}