/**
 * @file akku.h
 * @author Philipp Köhler
 * @brief Provides an interface to the voltage measurement
 * @version 0.1
 * @date 2019-08-17 - created
 * @date 2020-08-04 - last modified
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef AKKU_H
#define AKKU_H

#include <Arduino.h>

class Akku 
{
public:
    Akku(gpio_num_t gpio_akku, uint32_t voltage);
    float getAkkuValue();
private:
    gpio_num_t gpio_akku;
    uint32_t voltage;
};

#endif