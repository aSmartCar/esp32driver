/**
 * @file control.h
 * @author Philipp Köhler
 * @brief Controlunit for all components
 * @version 0.1
 * @date 2019-08-17 - created
 * @date 2020-08-31 - last modified
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef CONTROL_H
#define CONTROL_H

#define ARDUINOJSON_USE_LONG_LONG 1

#include <ArduinoJson.h>
#include "config.h"
#include "motor.h"
#include "sensorcontrol.h"

class Control
{
public:
    typedef enum
    {
        STOP = 0,
        MANUAL = 1,
        AUTO = 2,
        MODE_MAX = 3
    } DRIVING_MODES_E;

    typedef struct 
    {
        float us0Clearance = 0;
        float us1Clearance = 0;
        float sharp0Clearance = 0;
        float sharp1Clearance = 0;
        float sharp2Clearance = 0;
        float sharp3Clearance = 0;
    } sensorClearance_s;
    
    static sensorClearance_s *sensorClearance;

    static Control *init(config_s *config, SensorControl *sensorControl);
    static void onNewDataFromSensors();
    void run();

    void setMaxPulse(float maxPulse);
    void setMinPulse(float minPulse);
    void setVelocityDivider(float divider);
    void setHysteresis(float hysteresis);
    void setSteeringTrim(float steeringTrim);

private:
    static SensorControl *sensorControl;
    static config_s *config;
    static float maxPulse;
    static float minPulse;
    static float udpThrottle;
    static float udpSteering;
    static float hysteresis;
    static float velocityDivider;
    static float steeringTrim;
    static uint32_t udpReceiveTime;
    static uint64_t measurement_t1;
    static uint64_t measurement_t2;
    static uint64_t measurement_t3;
    static uint64_t timestampRec;
    static bool newUDPData;
    static DRIVING_MODES_E mode;
    static DRIVING_MODES_E sensorMode;

    Control(config_s *config, SensorControl *sensorControl);

    void drivingControl();
    void sendValuesOverUDP();
    void receiveValuesOverUDP();
    String getModeString(Control::DRIVING_MODES_E mode);
    Control::DRIVING_MODES_E getModeFromString(String mode);
    static float min(float a, float b);
    float normalize(float input);
};

#endif //CONTROL_H