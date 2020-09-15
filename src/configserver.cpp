/**
 * @file configserver.cpp
 * @author Philipp Köhler
 * @version 0.1
 * @date 2019-08-17 - created
 * @date 2020-08-03 - last modified
 * 
 * @copyright Copyright (c) 2020
 */
#include "configserver.h"

ConfigManager *ConfigServer::configManager;
SensorControl *ConfigServer::sensorControl;

/**
 * @brief Construct a new Config Server:: Config Server object
 * 
 * @param configManager 
 * @param port 
 */
ConfigServer::ConfigServer(ConfigManager *configManager, SensorControl *sensorControl, int port)
{
    this->server = new AsyncWebServer(port);
    this->configManager = configManager;
    this->sensorControl = sensorControl;
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "*");
}

/**
 * @brief Destroy the Config Server:: Config Server object
 * 
 */
ConfigServer::~ConfigServer()
{
    server->reset();
}

/**
 * @brief Initialize the webserver an routes
 * 
 */
void ConfigServer::init()
{
    serverRoutes();
    this->server->begin();
}

/**
 * @brief WiFi encryption mode to string
 * 
 * @param wifi_auth_mode 
 * @return String 
 */
String ConfigServer::encryptionToString(wifi_auth_mode_t wifi_auth_mode)
{
    switch (wifi_auth_mode)
    {
    case WIFI_AUTH_OPEN:
    {
        return "OPEN";
    }
    case WIFI_AUTH_WEP:
    {
        return "WEP";
    }
    case WIFI_AUTH_WPA_PSK:
    {
        return "WPA PSK";
    }
    case WIFI_AUTH_WPA_WPA2_PSK:
    {
        return "WPA WPA2 PSK";
    }
    case WIFI_AUTH_WPA2_PSK:
    {
        return "WPA2 PSK";
    }
    case WIFI_AUTH_WPA2_ENTERPRISE:
    {
        return "WPA2 ENTERPRISE";
    }
    default:
    {
        return "Unknown";
    }
    }
}

/**
 * @brief Defines the server routes
 * 
 */
void ConfigServer::serverRoutes()
{
    server->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/index.html.gz", "text/html", false);
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    });

    server->on("/css/chunk-vendors.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/css/chunk-vendors.css.gz", "text/css", false);
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    });

    server->on("/css/app.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/css/app.css", "text/css");
    });

    server->on("/js/chunk-vendors.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/js/chunk-vendors.js.gz", "text/javascript", false);
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    });

    server->on("/js/app.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/js/app.js.gz", "text/javascript", false);
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    });

    server->on("/img/logo.png", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/img/logo.png", "text/html");
    });

    server->on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse(SPIFFS, "/favicon.ico.gz", "text/html", false);
        response->addHeader("Content-Encoding", "gzip");
        request->send(response);
    });

    server->on(
        "/api/wifi", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        
        request->send(200,"application/json","{code:200}");
        
        ConfigServer::configManager->parseJSONObjectToFile((char *)data, "wifi"); });

    server->on(
        "/api/udp", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
            request->send(200, "application/json", "{code:200}");
            
            ConfigServer::configManager->parseJSONObjectToFile((char *)data, "udp"); });

    server->on(
        "/api/interfaces", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
            request->send(200, "application/json", "{code:200}");
            
            ConfigServer::configManager->parseJSONObjectToFile((char *)data, "interfaces"); });

    server->on(
        "/api/sensors", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
            request->send(200, "application/json", "{code:200}");
           
            ConfigServer::configManager->parseJSONObjectToFile((char *)data, "sensors"); });

    server->on(
        "/api/actuators", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        request->send(200,"application/json","{code:200}");
        
        ConfigServer::configManager->parseJSONObjectToFile((char *)data, "actuators"); });
    
    server->on(
        "/api/remote", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        request->send(200,"application/json","{code:200}");
        
        ConfigServer::configManager->parseJSONObjectToFile((char *)data, "remote"); });

    server->on(
        "/api/config", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
            Serial.printf("len: %d\nData: %s\n", len, (char *)data);
            ConfigServer::configManager->parseJSONObjectToFile((char *)data); 
            request->send(200,"application/json","{code:200}"); });

    server->on("/api/reset", HTTP_POST, [](AsyncWebServerRequest *request) {
        request->send(200, "application/json", "{code:200}");
        ESP.restart();
    });

    server->on("/api/stats", HTTP_GET, [](AsyncWebServerRequest *request) {
        DynamicJsonDocument jsonConfigDoc(1024);

        jsonConfigDoc["akku0"] = ConfigServer::sensorControl->sensorValues->akku0;
    
        jsonConfigDoc["akku1"] = ConfigServer::sensorControl->sensorValues->akku1;

        jsonConfigDoc["us0"] = ConfigServer::sensorControl->sensorValues->us0;

        jsonConfigDoc["us1"] = ConfigServer::sensorControl->sensorValues->us1;

        jsonConfigDoc["sharp0"] = ConfigServer::sensorControl->sensorValues->sharp0;

        jsonConfigDoc["sharp1"] = ConfigServer::sensorControl->sensorValues->sharp1;

        jsonConfigDoc["sharp2"] = ConfigServer::sensorControl->sensorValues->sharp2;

        jsonConfigDoc["sharp3"] = ConfigServer::sensorControl->sensorValues->sharp3;

        String buffer = jsonConfigDoc.as<String>();

        request->send(200, "application/json", buffer);
    });

    server->on("/api/interfaces", HTTP_GET, [](AsyncWebServerRequest *request) {
        StaticJsonDocument<200> filter;
        filter["interfaces"] = true;
        DynamicJsonDocument jsonConfigDoc(ESP.getMaxAllocHeap());
        ConfigServer::configManager->getConfigJsonFromFile(&jsonConfigDoc, &filter);
        jsonConfigDoc.shrinkToFit();

        request->send(200, "application/json", jsonConfigDoc["interfaces"]);
    });

    server->on("/api/wifi/conf", HTTP_GET, [](AsyncWebServerRequest *request) {
        StaticJsonDocument<200> filter;
        filter["wifi"] = true;
        DynamicJsonDocument jsonConfigDoc(ESP.getMaxAllocHeap());
        ConfigServer::configManager->getConfigJsonFromFile(&jsonConfigDoc, &filter);
        jsonConfigDoc.shrinkToFit();

        request->send(200, "application/json", jsonConfigDoc["wifi"]);
    });

    server->on("/api/wifi", HTTP_GET, [](AsyncWebServerRequest *request) {
        int count = WiFi.scanNetworks();
        String buf;
        DynamicJsonDocument jsonDoc(ESP.getMaxAllocHeap());
        JsonArray array = jsonDoc.to<JsonArray>();

        if (count > 10)
            count = 10;

        for (size_t i = 0; i < count; i++)
        {
            JsonObject jsonObj = array.createNestedObject();
            jsonObj["SSID"] = WiFi.SSID(i);
            jsonObj["RSSI"] = WiFi.RSSI(i);
            jsonObj["Encryption"] = encryptionToString(WiFi.encryptionType(i));
        }
        jsonDoc.shrinkToFit();
        serializeJson(jsonDoc, buf);
        request->send(200, "application/json", buf);
    });

    server->on("/api/udp", HTTP_GET, [](AsyncWebServerRequest *request) {
        StaticJsonDocument<200> filter;
        filter["udp"] = true;
        DynamicJsonDocument jsonConfigDoc(ESP.getMaxAllocHeap());
        ConfigServer::configManager->getConfigJsonFromFile(&jsonConfigDoc, &filter);
        jsonConfigDoc.shrinkToFit();

        request->send(200, "application/json", jsonConfigDoc["udp"]);
    });

    server->on("/api/sensors", HTTP_GET, [](AsyncWebServerRequest *request) {
        StaticJsonDocument<200> filter;
        filter["sensors"] = true;
        DynamicJsonDocument jsonConfigDoc(ESP.getMaxAllocHeap());
        ConfigServer::configManager->getConfigJsonFromFile(&jsonConfigDoc, &filter);
        jsonConfigDoc.shrinkToFit();
        request->send(200, "application/json", jsonConfigDoc["sensors"]);
    });

    server->on("/api/actuators", HTTP_GET, [](AsyncWebServerRequest *request) {
        StaticJsonDocument<200> filter;
        filter["actuators"] = true;
        DynamicJsonDocument jsonConfigDoc(ESP.getMaxAllocHeap());
        ConfigServer::configManager->getConfigJsonFromFile(&jsonConfigDoc, &filter);
        jsonConfigDoc.shrinkToFit();

        request->send(200, "application/json", jsonConfigDoc["actuators"]);
    });

    server->on("/api/remote", HTTP_GET, [](AsyncWebServerRequest *request) {
        StaticJsonDocument<200> filter;
        filter["remote"] = true;
        DynamicJsonDocument jsonConfigDoc(ESP.getMaxAllocHeap());
        ConfigServer::configManager->getConfigJsonFromFile(&jsonConfigDoc, &filter);
        jsonConfigDoc.shrinkToFit();

        request->send(200, "application/json", jsonConfigDoc["remote"]);
    });

    server->on("/api/config", HTTP_GET, [](AsyncWebServerRequest *request) {
        DynamicJsonDocument jsonConfigDoc(ESP.getMaxAllocHeap());
        ConfigServer::configManager->getConfigJsonFromFile(&jsonConfigDoc);
        jsonConfigDoc.shrinkToFit();

        request->send(200, "application/json", jsonConfigDoc.as<String>());
    });

    server->onNotFound([](AsyncWebServerRequest *request) {
        if (request->method() == HTTP_OPTIONS)
        {
            request->send(200);
        }
        else
        {
            request->send(404);
        }
    });
}
