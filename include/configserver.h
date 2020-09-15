/**
 * @file configserver.h
 * @author Philipp Köhler
 * @brief Provides the routes for the webserver
 * @version 0.1
 * @date 2019-08-17 - created
 * @date 2020-08-04 - last modified
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef CONFIGSERVER_H
#define CONFIGSERVER_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <esp_log.h>
#include <SPIFFS.h>

#include "wificontrol.h"
#include "ultrasonic.h"
#include "sharp.h"
#include "akku.h"
#include "motor.h"
#include "servo.h"
#include "configmanager.h"
#include "sensorcontrol.h"


class ConfigServer
{
public:

    ConfigServer(ConfigManager *configManager, SensorControl *sensorControl, int port = 80);
    ~ConfigServer();
    
    void init();
    
    
private:
    AsyncWebServer *server;
    static ConfigManager *configManager;
    static SensorControl *sensorControl;

    static String encryptionToString(wifi_auth_mode_t wifi_auth_mode);
    void serverRoutes();
    
};

#endif //CONFIGSERVER_H