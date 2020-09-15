/**
 * @file config.h
 * @author Philipp Köhler
 * @brief Provides a struct with pointers to all global used objects
 * @version 0.1
 * @date 2019-08-17 - created
 * @date 2020-08-04 - last modified
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef CONFIG_H
#define CONFIG_H

#include <FastLED.h>

#include "sharp.h"
#include "servo.h"
#include "motor.h"
#include "akku.h"
#include "ultrasonic.h"
#include "wificontrol.h"
#include "udpserver.h"
#include "led.h"

typedef struct 
{
    //Actuators
    Motor *motorA;
    Motor *motorB;
    Servo *servo0;
    Servo *servo1;
    Servo *servo2;
    Servo *servo3;
    LED * led;

    //Sensors
    Ultrasonic *us0;
    Ultrasonic *us1;
    Sharp *sharp0;
    Sharp *sharp1;
    Sharp *sharp2;
    Sharp *sharp3;
    Akku *akku0;
    Akku *akku1;

    //Interfaces
    WiFiControl *wifi;
    UDPServer *udp;

}config_s;

#endif //CONFIG_H