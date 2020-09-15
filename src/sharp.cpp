/**
 * @file sharp.cpp
 * @author Philipp Köhler
 * @version 0.1
 * @date 2019-08-17 - created
 * @date 2020-08-04 - last modified
 * @copyright Copyright (c) 2020
 * 
 */
#include "sharp.h"

/**
 * @brief Construct a new Sharp:: Sharp object
 * 
 * @param gpio_input 
 */
Sharp::Sharp(gpio_num_t gpio_input)
{
    this->gpio_input = gpio_input;

    pinMode(gpio_input, INPUT);
}

/**
 * @brief Get the current sensor value
 * 
 * @return float 
 */
float Sharp::getSharpValue()
{
    analogReadResolution(12);
    return analogRead(this->gpio_input);
}