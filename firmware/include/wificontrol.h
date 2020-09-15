/**
 * @file wificontrol.h
 * @author Philipp Köhler
 * @brief Provides an interface to control the wifi module
 * @version 0.1
 * @date 2019-08-17 - created
 * @date 2020-08-04 - last modified
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef WIFICONTROL_H
#define WIFICONTROL_H

#include <Arduino.h>
#include <WiFi.h>

class WiFiControl
{
public:
    static WiFiControl * init(String hostname, 
                              String ssid,
                              String password,
                              bool enableAP,
                              String apssid,
                              String appass,
                              wifi_mode_t wifi_mode);
    void start();
    void connectToAP(String ssid, String password);
    void createAP(String ssid, String password);

    IPAddress getIP();
    IPAddress getSub();
    
private:
    WiFiControl(String hostname, 
                String ssid, 
                String password,
                bool enableAP,
                String apssid,
                String appass, 
                wifi_mode_t wifi_mode);

    static constexpr uint32_t TASK_STACKSIZE = 2048;
    static constexpr UBaseType_t TASK_PRIORITY = 4;
    static TaskHandle_t taskHandle;

    static String hostname;
    static String wifi_ssid;
    static String password;
    static String apssid;
    static String appass;
    static bool enableAP;

    static wifi_mode_t wifi_mode;

    static IPAddress ip_local;
    static IPAddress sub_local;
};

#endif //WIFICONTROL_H