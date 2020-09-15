/**
 * @file sharp.h
 * @author Philipp Köhler
 * @brief Provides an interface to get the current distance values
 * @version 0.1
 * @date 2019-08-17 - created
 * @date 2020-08-04 - last modified
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef SHARP_H
#define SHARP_H
#include <Arduino.h>

class Sharp{
public:
    Sharp(gpio_num_t gpio_input);
    float getSharpValue();

private:
    gpio_num_t gpio_input;
};
#endif