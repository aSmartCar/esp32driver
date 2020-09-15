/**
 * @file main.cpp
 * @author Philipp Köhler
 * @version 0.1
 * @date 2019-08-17 - created
 * @date 2020-08-04 - last modified
 * @copyright Copyright (c) 2020
 * 
 */
#include "main.h"

config_s config;
SensorControl *sensors = SensorControl::init(&config);
Control *control = Control::init(&config, sensors);
ConfigManager *configManager = ConfigManager::init(&config, control, "/config.json");
ConfigServer webconfig(configManager, sensors);

/**
 * @brief Setup all services
 * 
 */
void setup()
{
  Serial.begin(115200);

  if (!SPIFFS.begin())
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  configManager->loadConfig();
  configManager->startServices();
  webconfig.init();
  sensors->setNewDataFunktion(control->onNewDataFromSensors);
  sensors->start();
  Serial.println("Setup done");
}

/**
 * @brief Main loop
 * 
 */
void loop()
{
  control->run();
}
