/**
 * @file wificontrol.cpp
 * @author Philipp Köhler
 * @version 0.1
 * @date 2019-08-17 - created
 * @date 2020-08-04 - last modified
 * @copyright Copyright (c) 2020
 * 
 */
#include "wificontrol.h"

TaskHandle_t WiFiControl::taskHandle;
String WiFiControl::hostname;
String WiFiControl::wifi_ssid;
String WiFiControl::password;
String WiFiControl::apssid;
String WiFiControl::appass;
wifi_mode_t WiFiControl::wifi_mode;
IPAddress WiFiControl::ip_local;
IPAddress WiFiControl::sub_local;
bool WiFiControl::enableAP;

/**
 * @brief Construct a new Wi Fi Control:: Wi Fi Control object
 * 
 * @param hostname 
 * @param ssid 
 * @param password 
 * @param enableAP 
 * @param apssid 
 * @param appass 
 * @param wifi_mode 
 */
WiFiControl::WiFiControl(String hostname, 
                         String ssid, 
                         String password,
                         bool enableAP,
                         String apssid,
                         String appass,
                         wifi_mode_t wifi_mode)
{
    WiFiControl::hostname = hostname;
    WiFiControl::wifi_ssid = ssid;
    WiFiControl::password = password;
    WiFiControl::wifi_mode = wifi_mode;
    WiFiControl::apssid = apssid;
    WiFiControl::appass = appass;
    WiFiControl::enableAP = enableAP;
    
    WiFi.mode(wifi_mode);
    WiFi.setHostname(hostname.c_str());

    if (wifi_mode != WIFI_AP && wifi_mode != WIFI_AP_STA && wifi_mode != WIFI_STA)
    {
        Serial.println("WiFi Mode not supported");
    }
}

/**
 * @brief Initialize the Wi Fi Control:: Object as singelton
 * 
 * @param hostname 
 * @param ssid 
 * @param password 
 * @param enableAP 
 * @param apssid 
 * @param appass 
 * @param wifi_mode 
 * @return WiFiControl* 
 */
WiFiControl * WiFiControl::init(String hostname, 
                                String ssid,
                                String password,
                                bool enableAP,
                                String apssid,
                                String appass,
                                wifi_mode_t wifi_mode)
{
    static WiFiControl self(hostname, ssid, password, enableAP,apssid, appass, wifi_mode);
    return &self;
}

/**
 * @brief Start the WiFi interface
 * 
 */
void WiFiControl::start()
{
    if(this->enableAP)
        createAP(WiFiControl::apssid, WiFiControl::appass);
    else
        connectToAP(WiFiControl::wifi_ssid, WiFiControl::password);    
}

/**
 * @brief Connect to an AccessPoint
 * 
 * @param ssid 
 * @param password 
 */
void WiFiControl::connectToAP(String ssid, String password)
{
    Serial.print("Connect to Network: ");
    Serial.println(wifi_ssid);

    WiFi.disconnect();
    WiFi.begin(ssid.c_str(), password.length() > 0 ? password.c_str() : NULL);

    int count = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        count++;

        if ((count / 2) == 15)
        {
            Serial.print("Could not connect. Timeout");
            break;
        }
    }
    Serial.println("");

    if (WiFi.status() == WL_CONNECTED)
    {
        WiFiControl::ip_local = WiFi.localIP();
        WiFiControl::sub_local = WiFi.subnetMask();
        Serial.println("Connected");
        
        Serial.print("WiFi SSID: ");
        Serial.println(this->wifi_ssid);
        Serial.print("IP: ");
        Serial.println(WiFi.localIP() ? WiFi.localIP(): WiFi.softAPIP());
        Serial.print("SUB: ");
        Serial.println(WiFi.subnetMask());
        Serial.print("Mac: ");
        Serial.println(WiFi.macAddress());
        Serial.print("WiFi Rssi: ");
        Serial.println(WiFi.RSSI());
    }
    else
    {
        WiFi.mode(WIFI_AP);
        createAP(WiFiControl::apssid, WiFiControl::appass);
    }
}

/**
 * @brief Create an own AccessPoint
 * 
 * @param ssid 
 * @param password 
 */
void WiFiControl::createAP(String ssid, String password)
{
    Serial.println("Create AP");
    if (WiFi.softAP(ssid.c_str(), (password.length() > 0) ? password.c_str() : NULL))
    {
        Serial.println("WiFi AP created");
        WiFiControl::ip_local = WiFi.softAPIP();
        WiFiControl::sub_local = IPAddress(255,255,255,0);
        
        Serial.print("WiFi SSID: ");
        Serial.println(ssid);
        Serial.print("IP: ");
        Serial.println(WiFi.localIP() ? WiFi.localIP(): WiFi.softAPIP());
        Serial.print("SUB: ");
        Serial.println(WiFiControl::sub_local);
        Serial.print("Mac: ");
        Serial.println(WiFi.macAddress());
        Serial.print("WiFi Rssi: ");
        Serial.println(WiFi.RSSI());
    }
    else
    {
        Serial.println("Error could not create WiFi AP");
    }
}

/**
 * @brief Get the current ip address
 * 
 * @return IPAddress 
 */
IPAddress WiFiControl::getIP()
{
    return WiFiControl::ip_local;
}

/**
 * @brief Get the current subnetmask
 * 
 * @return IPAddress 
 */
IPAddress WiFiControl::getSub()
{
    return WiFiControl::sub_local;
}