/**
 * @file led.h
 * @author Philipp Köhler
 * @brief Provides an interface to control the WS2812 LEDs
 * @version 0.1
 * @date 2019-08-17 - created
 * @date 2020-08-04 - last modified
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef LED_H
#define LED_H

#include <Arduino.h>
#include <FastLED.h>

class LED
{
public:

    typedef struct
    {
        uint8_t red;
        uint8_t green;
        uint8_t blue;
        uint8_t brightnes;
    }LED_PROPERTIES_S;

    LED(CRGB * led, uint32_t count, gpio_num_t gpio);
    void setColor(uint32_t id, uint8_t r, uint8_t g, uint8_t b);
    void setColor(uint32_t id, CRGB::HTMLColorCode color);
    void setBrightnes(uint8_t bright);
    void getColor(uint32_t id, uint8_t &r, uint8_t &g, uint8_t &b);
    uint32_t getCount();
    void show();
private:
    CRGB * led;
    uint32_t count;
    const gpio_num_t gpio;
    std::vector<LED_PROPERTIES_S> properties;
};

#endif