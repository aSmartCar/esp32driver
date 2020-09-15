/**
 * @file configmanager.cpp
 * @author Philipp Köhler
 * @version 0.1
 * @date 2019-08-17 - created
 * @date 2020-08-04 - last modified
 * @copyright Copyright (c) 2020
 * 
 */
#include "configmanager.h"

/**
 * @brief Creates and returns a Config Manager:: Config Manager object
 * 
 * @param config 
 * @param control 
 * @param path 
 * @return ConfigManager*  
 */
ConfigManager *ConfigManager::init(config_s *config, Control *control, String path)
{
    static ConfigManager self(config, control, path);
    Serial.printf("self path %s", self.stdPath.c_str());
    return &self;
}

/**
 * @brief Loads the config file and creats the needed objects
 * 
 * @param path 
 */
void ConfigManager::loadConfig()
{
    DynamicJsonDocument configJsonDoc(ESP.getMaxAllocHeap());
    getConfigJsonFromFile(&configJsonDoc);
    configJsonDoc.shrinkToFit();

    String hostname = configJsonDoc["wifi"]["hostname"];
    String SSID = configJsonDoc["wifi"]["SSID"];
    String password = configJsonDoc["wifi"]["password"];
    String APSSID = configJsonDoc["wifi"]["APSSID"];
    String APPass = configJsonDoc["wifi"]["APPass"];
    bool enableAP = configJsonDoc["wifi"]["enableAP"];
    wifi_mode_t mode;

    if (enableAP)
    {
        mode = WIFI_AP;
    }
    else
    {
        mode = WIFI_STA;
    }

    ConfigManager::config->wifi = WiFiControl::init(hostname, SSID, password, enableAP, APSSID, APPass, mode);

    uint32_t remotePort = configJsonDoc["udp"]["remotePort"];
    uint32_t localPort = configJsonDoc["udp"]["localPort"];
    bool broadcast = configJsonDoc["udp"]["broadcast"];
    String remoteIPString = configJsonDoc["udp"]["remoteIP"];
    IPAddress remoteIP;

    if (remoteIP.fromString(remoteIPString))
        ConfigManager::config->udp = UDPServer::init(localPort, broadcast, remotePort, remoteIP);
    else
        ConfigManager::config->udp = UDPServer::init(localPort, broadcast, remotePort);


    control->setMinPulse((float) configJsonDoc["remote"]["minPulse"]);
    control->setMaxPulse((float) configJsonDoc["remote"]["maxPulse"]);
    control->setVelocityDivider((float) configJsonDoc["remote"]["velocityDivider"]);
    control->setSteeringTrim((float) configJsonDoc["remote"]["steeringTrim"]);
    control->setHysteresis((float) configJsonDoc["remote"]["hysteresis"]);

    bool us0 = configJsonDoc["sensors"]["us0"]["enabled"];
    bool us1 = configJsonDoc["sensors"]["us1"]["enabled"];
    bool sharp0 = configJsonDoc["sensors"]["sharp0"]["enabled"];
    bool sharp1 = configJsonDoc["sensors"]["sharp1"]["enabled"];
    bool sharp2 = configJsonDoc["sensors"]["sharp2"]["enabled"];
    bool sharp3 = configJsonDoc["sensors"]["sharp3"]["enabled"];
    bool akku0 = configJsonDoc["sensors"]["akku0"]["enabled"];
    bool akku1 = configJsonDoc["sensors"]["akku1"]["enabled"];
    uint8_t akku0Voltage = configJsonDoc["sensors"]["akku0"]["voltage"];
    uint8_t akku1Voltage = configJsonDoc["sensors"]["akku1"]["voltage"];

    if (us0 == true) {
        ConfigManager::config->us0 = new Ultrasonic(GPIO_NUM_2, GPIO_NUM_13);
        control->sensorClearance->us0Clearance = (float) configJsonDoc["sensors"]["us0"]["clearance"];
    }
    if (us1 == true){
        ConfigManager::config->us1 = new Ultrasonic(GPIO_NUM_2, GPIO_NUM_14);
        control->sensorClearance->us1Clearance = (float) configJsonDoc["sensors"]["us1"]["clearance"];
    }
    if (sharp0 == true){
        ConfigManager::config->sharp0 = new Sharp(GPIO_NUM_33);
        control->sensorClearance->sharp0Clearance = (float) configJsonDoc["sensors"]["sharp0"]["clearance"];
    }
    if (sharp1 == true){
        ConfigManager::config->sharp1 = new Sharp(GPIO_NUM_32);
        control->sensorClearance->sharp1Clearance = (float) configJsonDoc["sensors"]["sharp1"]["clearance"];
    }
    if (sharp2 == true){
        ConfigManager::config->sharp2 = new Sharp(GPIO_NUM_34);
        control->sensorClearance->sharp2Clearance = (float) configJsonDoc["sensors"]["sharp2"]["clearance"];
    }
    if (sharp3 == true){
        ConfigManager::config->sharp3 = new Sharp(GPIO_NUM_35);
        control->sensorClearance->sharp3Clearance = (float) configJsonDoc["sensors"]["sharp3"]["clearance"];
    }
    if (akku0 == true)
        ConfigManager::config->akku0 = new Akku(GPIO_NUM_36, akku0Voltage);
    if (akku1 == true)
        ConfigManager::config->akku1 = new Akku(GPIO_NUM_39, akku1Voltage);

    Serial.println("Sensors initialized");

    bool motorA = configJsonDoc["actuators"]["motorA"]["enabled"];
    bool motorB = configJsonDoc["actuators"]["motorB"]["enabled"];

    if (motorA || motorB)
    {
        if (motorA == true)
        {
            uint32_t frequenz = configJsonDoc["actuators"]["motorA"]["frequenz"];
            static Motor staticMotorA(GPIO_NUM_18, GPIO_NUM_21, GPIO_NUM_19, GPIO_NUM_0, MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM0A, frequenz);
            ConfigManager::config->motorA = &staticMotorA;
        }

        if (motorB == true)
        {
            uint32_t frequenz = configJsonDoc["actuators"]["motorB"]["frequenz"];
            static Motor staticMotorB(GPIO_NUM_25, GPIO_NUM_22, GPIO_NUM_23, GPIO_NUM_0, MCPWM_UNIT_0, MCPWM_TIMER_1, MCPWM1A, frequenz);
            ConfigManager::config->motorB = &staticMotorB;
        }
    }
    else
    {
        bool servo0 = configJsonDoc["actuators"]["servo0"]["enabled"];
        bool servo1 = configJsonDoc["actuators"]["servo1"]["enabled"];
        bool servo2 = configJsonDoc["actuators"]["servo2"]["enabled"];
        bool servo3 = configJsonDoc["actuators"]["servo3"]["enabled"];

        if (servo0 == true)
        {
            uint32_t frequenz = configJsonDoc["actuators"]["servo0"]["frequenz"];
            uint32_t startDuty = configJsonDoc["actuators"]["servo0"]["startDuty"];
            static Servo staticServo0(GPIO_NUM_19, startDuty, frequenz, LEDC_TIMER_0, LEDC_TIMER_15_BIT, LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
            ConfigManager::config->servo0 = &staticServo0;
        }

        if (servo1 == true)
        {
            uint32_t frequenz = configJsonDoc["actuators"]["servo1"]["frequenz"];
            uint32_t startDuty = configJsonDoc["actuators"]["servo1"]["startDuty"];
            static Servo staticServo1(GPIO_NUM_21, startDuty, frequenz, LEDC_TIMER_0, LEDC_TIMER_15_BIT, LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1);
            ConfigManager::config->servo1 = &staticServo1;
        }

        if (servo2 == true)
        {
            uint32_t frequenz = configJsonDoc["actuators"]["servo2"]["frequenz"];
            uint32_t startDuty = configJsonDoc["actuators"]["servo2"]["startDuty"];
            static Servo staticServo2(GPIO_NUM_22, startDuty, frequenz, LEDC_TIMER_1, LEDC_TIMER_15_BIT, LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);
            ConfigManager::config->servo2 = &staticServo2;
        }

        if (servo3 == true)
        {
            uint32_t frequenz = configJsonDoc["actuators"]["servo3"]["frequenz"];
            uint32_t startDuty = configJsonDoc["actuators"]["servo3"]["startDuty"];
            static Servo staticServo3(GPIO_NUM_21, startDuty, frequenz, LEDC_TIMER_1, LEDC_TIMER_15_BIT, LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_1);
            ConfigManager::config->servo3 = &staticServo3;
        }
    }

    Serial.println("Actuators initialized");


    bool ws2812 = configJsonDoc["interfaces"]["ws2812"]["enabled"];
    bool i2c = configJsonDoc["interfaces"]["i2c"]["enabled"];
    bool can = configJsonDoc["interfaces"]["can"]["enabled"];
    bool uart = configJsonDoc["interfaces"]["uart"]["enabled"];

    if (ws2812 == true)
    {
        uint32_t count = configJsonDoc["interfaces"]["ws2812"]["count"];

        CRGB leds[count];
        static LED led(leds, count, GPIO_NUM_12);

        led.setBrightnes(125);
        uint8_t r, g, b;
        for (int i = 0; i < count; i++)
        {
            r = configJsonDoc["interfaces"]["ws2812"]["colors"][i]["r"];
            g = configJsonDoc["interfaces"]["ws2812"]["colors"][i]["g"];
            b = configJsonDoc["interfaces"]["ws2812"]["colors"][i]["b"];
            led.setColor(i, r, g, b);
        }
        led.show();
        ConfigManager::config->led = &led;
    }

    if (i2c == true)
    {
        uint32_t clock = configJsonDoc["interfaces"]["i2c"]["clock"];
        Wire.begin(GPIO_NUM_4, GPIO_NUM_5, clock);
    }

    if (can == true)
    {
        Serial.println("CAN interface not yet implemented.");
    }

    if (uart == true)
    {
        Serial2.begin(115200, SERIAL_8N1, GPIO_NUM_16, GPIO_NUM_17);
        Serial2.println("UART successfull implemented");
    }

    Serial.println("Interfaces initialized");

    Serial.println("Loading configfile done");
}

/**
 * @brief Start all needed services
 * 
 */
void ConfigManager::startServices()
{
    ConfigManager::config->wifi->start();
    ConfigManager::config->udp->start(ConfigManager::config->wifi->getIP(),
                                      ConfigManager::config->wifi->getSub());
}

/**
 * @brief Parse a json object string into the config file
 * 
 * @param buffer 
 * @param jsonObject 
 */
void ConfigManager::parseJSONObjectToFile(char * buffer, String jsonObject)
{
    DynamicJsonDocument configJsonDoc(ESP.getMaxAllocHeap());
    DeserializationError error = deserializeJson(configJsonDoc, buffer);
    configJsonDoc.shrinkToFit();
    

    if (error)
        Serial.printf("Could not deserialize json: %s", error.c_str());
    else
    {        
        DynamicJsonDocument jsonConfigDoc(ESP.getMaxAllocHeap());
        getConfigJsonFromFile(&jsonConfigDoc);


        if (jsonObject != "")
        {
            if ( jsonConfigDoc.containsKey(jsonObject))
                jsonConfigDoc[jsonObject] = configJsonDoc;
            // Serial.printf("configJsonDoc: %s\njsonConfigDoc %s \njsonObject: %s\ndyn: %s\n", configJsonDoc.as<String>().c_str(), jsonConfigDoc.as<String>().c_str(), jsonObject.c_str(), dyn.as<String>().c_str());
        }
        else
            jsonConfigDoc = configJsonDoc;

        jsonConfigDoc.shrinkToFit();

        writeConfigJsonToFile(&jsonConfigDoc);
    }
}

/**
 * @brief Read the config file and returns a json document
 * 
 * @param jsonDoc 
 * @param path 
 */
void ConfigManager::getConfigJsonFromFile(DynamicJsonDocument * jsonDoc)
{
    FileHandler handler(this->stdPath);

    String buffer;
    handler.readFile(buffer);

    DeserializationError error = deserializeJson(*jsonDoc, buffer);

    if (error)
        Serial.println("Deserialize Error");
}

/**
 * @brief Read the config file and only returns the filterd objects
 * 
 * @param jsonDoc 
 * @param filter 
 * @param path 
 */
void ConfigManager::getConfigJsonFromFile(DynamicJsonDocument * jsonDoc,StaticJsonDocument<200> *filter)
{
    FileHandler handler(this->stdPath);

    String buffer;
    handler.readFile(buffer);

    DeserializationError error = deserializeJson(*jsonDoc, buffer, DeserializationOption::Filter(*filter));

    if (error)
        Serial.println("Deserialize Error");
}

/**
 * @brief Write a json document into the config file
 * 
 * @param jsonDoc 
 * @param path 
 */
void ConfigManager::writeConfigJsonToFile(DynamicJsonDocument *jsonDoc)
{
    FileHandler handler(this->stdPath);

    String buffer;
    serializeJson(*jsonDoc, buffer);

    handler.writeFile(buffer);
}

/**
 * @brief Returns a pointer to the config struct
 * 
 * @return config_s* 
 */
config_s * ConfigManager::getConfig()
{
    return ConfigManager::config;
}

