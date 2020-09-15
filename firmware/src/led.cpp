/**
 * @file led.cpp
 * @author Philipp Köhler
 * @version 0.1
 * @date 2019-08-17
 * @date 2020-08-04 - last modified
 * @copyright Copyright (c) 2020
 * 
 */
#include "led.h"

/**
 * @brief Construct a new LED::LED object
 * 
 * @param led 
 * @param count 
 * @param gpio 
 */
LED::LED(CRGB *led, uint32_t count, const gpio_num_t gpio) : gpio(gpio)
{
    this->led = led;
    this->count = count;
    FastLED.addLeds<NEOPIXEL, GPIO_NUM_12>(led, count);

    LED::LED_PROPERTIES_S props;
    for(int i = 0; i < count; i++)
        this->properties.push_back(props);
}

/**
 * @brief Define color for the selected led
 * 
 * @param id 
 * @param r 
 * @param g 
 * @param b 
 */
void LED::setColor(uint32_t id, uint8_t r, uint8_t g, uint8_t b)
{
    this->properties[id].red = r;
    this->properties[id].green = g;
    this->properties[id].blue = b;
    this->led[id].red = r;
    this->led[id].green = g;
    this->led[id].blue = b;
}

/**
 * @brief Update the leds for the new settings
 * 
 */
void LED::show()
{
    FastLED.show();
}

/**
 * @brief Set led brightnes
 * 
 * @param bright 
 */
void LED::setBrightnes(uint8_t bright)
{

    for (int i = 0; i < this->count; i++)
    {
        this->properties[i].brightnes = bright;
        this->setColor(i, this->properties[i].red, this->properties[i].green, this->properties[i].blue);
    }
    FastLED.setBrightness(bright);
    FastLED.show();
}