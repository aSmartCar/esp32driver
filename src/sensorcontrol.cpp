/**
 * @file sensorcontrol.cpp
 * @author Philipp Köhler
 * @version 0.1
 * @date 2019-08-17 - created
 * @date 2020-08-04 - last modified
 * @copyright Copyright (c) 2020
 * 
 */
#include "sensorcontrol.h"

SensorControl::sensors_s *SensorControl::sensorValues;
config_s *SensorControl::config;

/**
 * @brief Create singelton object
 * 
 * @param config 
 * @return SensorControl* 
 */
SensorControl *SensorControl::init(config_s *config)
{
    static SensorControl self(config);
    return &self;
}

/**
 * @brief Construct a new Sensor Control:: Sensor Control object
 * 
 * @param config 
 */
SensorControl::SensorControl(config_s *config)
{
    this->config = config;
    SensorControl::sensorValues = new SensorControl::sensors_s();
}

/**
 * @brief Initialize the task for the sensorcontrol
 * 
 */
void SensorControl::start()
{
    BaseType_t success = xTaskCreate(this->taskfunc, "SensorControl", TASK_STACKSIZE,
                                     this, TASK_PRIORITY, &this->taskHandle);

    if (success != pdPASS)
    {
        ESP_ERROR_CHECK(ESP_FAIL);
    }
}

/**
 * @brief Taskfunction to run
 * 
 * @param instancePtr 
 */
void SensorControl::taskfunc(void *instancePtr)
{
    SensorControl *filterInstancePtr = static_cast<SensorControl *>(instancePtr);
    filterInstancePtr->run();
    vTaskDelete(filterInstancePtr->taskHandle);
}

/**
 * @brief Main function running inside the task
 * 
 */
void SensorControl::run()
{
    while (1)
    {
        if (SensorControl::config->akku0 != NULL)
        {
            SensorControl::sensorValues->akku0 = SensorControl::config->akku0->getAkkuValue();
        }

        if (SensorControl::config->akku1 != NULL)
        {
            SensorControl::sensorValues->akku1 = SensorControl::config->akku1->getAkkuValue();
        }

        if (SensorControl::config->us0 != NULL)
        {
            SensorControl::sensorValues->us0 = SensorControl::config->us0->getUltrasonicValue();
        }

        if (SensorControl::config->us1 != NULL)
        {
            SensorControl::sensorValues->us1 = SensorControl::config->us0->getUltrasonicValue();
        }

        if (SensorControl::config->sharp0 != NULL)
        {
           SensorControl::sensorValues->sharp0 = SensorControl::config->sharp0->getSharpValue();
        }

        if (SensorControl::config->sharp1 != NULL)
        {
           SensorControl::sensorValues->sharp1 = SensorControl::config->sharp1->getSharpValue();
        }

        if (SensorControl::config->sharp2 != NULL)
        {
            SensorControl::sensorValues->sharp2 = SensorControl::config->sharp2->getSharpValue();
        }

        if (SensorControl::config->sharp3 != NULL)
        {
            SensorControl::sensorValues->sharp3 = SensorControl::config->sharp3->getSharpValue();
        }
        onNewDataFunction();
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
}

/**
 * @brief Callback function
 * 
 * @param onNewDataFunction 
 */
void SensorControl::setNewDataFunktion(std::function<void()> onNewDataFunction)
{
    this->onNewDataFunction = onNewDataFunction;
}