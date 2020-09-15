/**
 * @file sensorcontrol.h
 * @author Philipp Köhler
 * @brief Provides a task for all sensors to read the current values
 * @version 0.1
 * @date 2019-08-17 - created
 * @date 2020-08-04 - last modified
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef SENSORCONTROL_H
#define SENSORCONTROL_H

#include <FreeRTOS.h>
#include "config.h"

class SensorControl
{
public:
    typedef struct
    {
        float akku0 = 0;
        float akku1 = 0;
        float us0 = 0;
        float us1 = 0;
        float sharp0 = 0;
        float sharp1 = 0;
        float sharp2 = 0;
        float sharp3 = 0;
    } sensors_s;

    static sensors_s *sensorValues;

    static SensorControl *init(config_s *config);
    void start();
    void setNewDataFunktion(std::function<void()> onNewDataFunction);

private:
    static constexpr uint32_t TASK_STACKSIZE = 2048;
    static constexpr UBaseType_t TASK_PRIORITY = 4; //< max 25
    TaskHandle_t taskHandle;

    static config_s *config;

    SensorControl(config_s *config);
    void run();
    static void taskfunc(void *instancePtr);
    std::function<void()> onNewDataFunction;
};

#endif