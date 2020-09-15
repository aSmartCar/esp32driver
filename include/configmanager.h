/**
 * @file configmanager.h
 * @author Philipp Köhler
 * @brief Initialize global used objects and load/save configuration settings
 * @version 0.1
 * @date 2019-08-17 - created
 * @date 2020-08-04 - last modified
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H


#include <ArduinoJson.h>
#include <FastLED.h>
#include <FreeRTOS.h>
#include <Wire.h>

#include "control.h"
#include "config.h"
#include "filehandler.h"
#include "sharp.h"
#include "servo.h"
#include "motor.h"
#include "akku.h"
#include "ultrasonic.h"
#include "wificontrol.h"
#include "udpserver.h"


class ConfigManager
{
public:
    static ConfigManager *init(config_s *config, Control *control, String path);
    void loadConfig();
    void writeConfig();
    void startServices();
    void getConfigJsonFromFile(DynamicJsonDocument * jsonDoc);
    void getConfigJsonFromFile(DynamicJsonDocument * jsonDoc, StaticJsonDocument<200> *filter);
    void writeConfigJsonToFile(DynamicJsonDocument *jsonDoc);
    void parseJSONObjectToFile(char * buffer, String jsonObject = "");
    config_s * getConfig();

private:
    config_s *config;
    Control *control;
    String stdPath;

    ConfigManager(config_s * config, Control * control, String path): config(config), control(control), stdPath(path){;}; 
};

#endif //CONFIGMANAGER_H