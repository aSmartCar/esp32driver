/**
 * @file control.cpp
 * @author Philipp Köhler
 * @version 0.1
 * @date 2019-08-17 - created
 * @date 2020-08-04 - last modified
 * @copyright Copyright (c) 2020
 * 
 */
#include "control.h"

SensorControl *Control::sensorControl;
config_s *Control::config;
Control::sensorClearance_s *Control::sensorClearance;
float Control::udpThrottle;
float Control::udpSteering;
float Control::hysteresis;
float Control::maxPulse;
float Control::minPulse;
float Control::velocityDivider;
float Control::steeringTrim = 0;
uint32_t Control::udpReceiveTime;
uint64_t Control::measurement_t1 = 0;
uint64_t Control::measurement_t2 = 0;
uint64_t Control::measurement_t3 = 0;
uint64_t Control::timestampRec = 0;
bool Control::newUDPData;
Control::DRIVING_MODES_E Control::mode;
Control::DRIVING_MODES_E Control::sensorMode;

/**
 * @brief Construct a new Control:: Control object
 * 
 * @param config 
 * @param sensorControl 
 */
Control::Control(config_s *config, SensorControl *sensorControl)
{
    Control::config = config;
    Control::sensorControl = sensorControl;
    Control::sensorClearance = new Control::sensorClearance_s();
}

/**
 * @brief Creates an singelton pattern
 * 
 * @param config 
 * @param sensorControl 
 * @return Control* 
 */
Control *Control::init(config_s *config, SensorControl *sensorControl)
{
    static Control self(config, sensorControl);
    return &self;
}

/**
 * @brief Main function for the complete control
 */
void Control::run()
{
    this->receiveValuesOverUDP();
    this->drivingControl();

    if ((xTaskGetTickCount() - udpReceiveTime) > ((500 + portTICK_PERIOD_MS / 2) / portTICK_PERIOD_MS))
    {
        Control::mode = STOP;
    }

    if (Control::newUDPData)
    {
        this->sendValuesOverUDP();
    }
}

/**
 * @brief Returns the min value of two float values
 * 
 * @param a 
 * @param b 
 * @return float 
 */
float Control::min(float a, float b)
{
    return (a < b) ?  a :  b;
}

float Control::normalize(float input)
{
    return 2 * (input - Control::minPulse) / (Control::maxPulse - Control::minPulse) - 1;
}

/**
 * @brief Sets the motor speed for diferent steering and throttle inputs
 * 
 */
void Control::drivingControl()
{
   
    float hysteresis = normalize(Control::hysteresis);
    float throttle = normalize(Control::udpThrottle);
    float steering = normalize(Control::udpSteering);
    float courseCorrection = normalize(Control::steeringTrim);
    
    
    if(throttle < 0 && throttle <= hysteresis)
        throttle += hysteresis;
    else if (throttle > 0 && throttle >= hysteresis)
        throttle -= hysteresis;
    if (steering < 0 && steering <= hysteresis)
        steering += hysteresis;
    else if ( steering > 0 && steering >= hysteresis)
        steering -= hysteresis;

    steering /= Control::velocityDivider;
    throttle /= Control::velocityDivider;

    float dutyRight = 0;
    float dutyLeft = 0;

    if (Control::mode != DRIVING_MODES_E::STOP && Control::sensorMode != DRIVING_MODES_E::STOP)
    {
        dutyLeft = throttle * (1 + Control::min(steering, 0.0));
        dutyRight = throttle * (1 + Control::min(-steering, 0.0));
    }

    if (courseCorrection > 0)
        dutyLeft = dutyLeft * (1 - courseCorrection);
    else if (courseCorrection < 0)
        dutyRight = dutyRight * (1 + courseCorrection);

    Motor::DIRECTION_E directionLeft = dutyLeft >= 0 ? Motor::FORWARD : Motor::BACKWARD;
    Motor::DIRECTION_E directionRight = dutyRight >= 0 ? Motor::FORWARD : Motor::BACKWARD;

    if (Control::config->motorA != NULL && Control::config->motorB != NULL)
    {
        Control::config->motorA->setAcceleration(directionLeft, fabs(dutyLeft) * 100);
        Control::config->motorB->setAcceleration(directionRight, fabs(dutyRight) * 100);
    }
    Serial.printf("Hysteresis: %f\tthrottle:%f\tsteering:%f\tcorrection:%f\tdutyLeft:%f\tdutyRight:%f\tdirectionLeft:%d\tdirectionRight:%d\tmode:%d\n", hysteresis, throttle, steering, courseCorrection, dutyLeft, dutyRight, directionLeft, directionRight, Control::mode);
}

/**
 * @brief Send the json objects via udp to communicate with the smartphone
 * 
 */
void Control::sendValuesOverUDP()
{
    String buffer;
    DynamicJsonDocument jsonDoc(ESP.getMaxAllocHeap());

    jsonDoc["ts"] = millis();
    if (timestampRec != 0)
        jsonDoc["ts_rec"] = timestampRec;
    jsonDoc["mode"] = getModeString(Control::mode);

    if (Control::Control::sensorControl->sensorValues->akku0 != 0)
        jsonDoc["akku0"] = Control::Control::sensorControl->sensorValues->akku0;
    if (Control::Control::sensorControl->sensorValues->akku1 != 0)
        jsonDoc["akku1"] = Control::Control::sensorControl->sensorValues->akku1;

    if (measurement_t1 != 0)
    {
        measurement_t3 = micros();
        jsonDoc["t1"] = measurement_t1;
        jsonDoc["t2"] = measurement_t2 * 1000;
        jsonDoc["t3"] = measurement_t3 * 1000;
    }
    jsonDoc.shrinkToFit();

    serializeJson(jsonDoc, buffer);

    Control::config->udp->sendUDP(buffer);
    Control::newUDPData = false;
}

/**
 * @brief Receive values over udp 
 * 
 */
void Control::receiveValuesOverUDP()
{
    String buffer;
    Control::config->udp->receiveUDP(&buffer);
    if (buffer != "")
    {
        DynamicJsonDocument jsonDoc(ESP.getMaxAllocHeap());
        DeserializationError derror = deserializeJson(jsonDoc, buffer);
        jsonDoc.shrinkToFit();

        if (derror)
        {
            Serial.print("DeserializationError: ");
            Serial.println(derror.c_str());
        }

        Control::udpThrottle = 0;
        Control::udpSteering = 0;
        Control::timestampRec = 0;

        if (jsonDoc.containsKey("ts_rec"))
            Control::timestampRec = jsonDoc["ts_rec"];
        if (jsonDoc.containsKey("throttle"))
            Control::udpThrottle = jsonDoc["throttle"];
        if (jsonDoc.containsKey("steering"))
            Control::udpSteering = jsonDoc["steering"];
        if (jsonDoc.containsKey("mode"))
            Control::mode = getModeFromString(jsonDoc["mode"]);
        if (jsonDoc.containsKey("t1"))
            measurement_t1 = jsonDoc["t1"];

        newUDPData = true;
        udpReceiveTime = xTaskGetTickCount();
    }
}

/**
 * @brief Callbackfunction for new sensor values
 * 
 */
void Control::onNewDataFromSensors()
{
    if (Control::sensorControl->sensorValues->us0 < Control::sensorClearance->us0Clearance && Control::sensorControl->sensorValues->us0 != 0.00)
    {
        Control::sensorMode = STOP;
    }
    else
        Control::sensorMode = Control::mode;

    if (Control::sensorControl->sensorValues->us1 < Control::sensorClearance->us1Clearance && Control::sensorControl->sensorValues->us1 != 0.00)
    {
        Control::sensorMode = STOP;
    }
    else
        Control::sensorMode = Control::mode;

    if (Control::sensorControl->sensorValues->sharp0 > Control::sensorClearance->sharp0Clearance && Control::sensorControl->sensorValues->sharp0 != 0.00)
    {
        Control::sensorMode = STOP;
    }
    else
        Control::sensorMode = Control::mode;

    if (Control::sensorControl->sensorValues->sharp1 > Control::sensorClearance->sharp1Clearance && Control::sensorControl->sensorValues->sharp1 != 0.00)
    {
        Control::sensorMode = STOP;
    }
    else
        Control::sensorMode = Control::mode;

    if (Control::sensorControl->sensorValues->sharp2 > Control::sensorClearance->sharp2Clearance && Control::sensorControl->sensorValues->sharp2 != 0.00)
    {
        Control::sensorMode = STOP;
    }
    else
        Control::sensorMode = Control::mode;

    if (Control::sensorControl->sensorValues->sharp3 > Control::sensorClearance->sharp3Clearance && Control::sensorControl->sensorValues->sharp3 != 0.00)
    {
        Control::sensorMode = STOP;
    }
    else
        Control::sensorMode = Control::mode;
}

// Setter
/**
 * @brief Set the maximum value from the controller
 * 
 * @param maxPulse 
 */
void Control::setMaxPulse(float maxPulse)
{
    Control::maxPulse = maxPulse;
}

/**
 * @brief Set the minimum value from the controller
 * 
 * @param minPulse 
 */
void Control::setMinPulse(float minPulse)
{
    Control::minPulse = minPulse;
}

/**
 * @brief Set the hysteresis for the mid value of the controller
 * 
 * @param hysteresis 
 */
void Control::setHysteresis(float hysteresis)
{
    Control::hysteresis = hysteresis;
}

/**
 * @brief Set the velocity divider
 * 
 * @param divider 
 */
void Control::setVelocityDivider(float divider)
{
    Control::velocityDivider = divider;
}

void Control::setSteeringTrim(float steeringTrim)
{
    Control::steeringTrim = steeringTrim;
}

/**
 * @brief Returns the driving mode as string
 * 
 * @param mode 
 * @return String 
 */
String Control::getModeString(Control::DRIVING_MODES_E mode)
{

    if (mode == Control::MANUAL)
        return "manual";
    else if (mode == Control::AUTO)
        return "auto";
    else
        return "stop";
}

/**
 * @brief Convert string mode into enum driving mode
 * 
 * @param mode 
 * @return Control::DRIVING_MODES_E 
 */
Control::DRIVING_MODES_E Control::getModeFromString(String mode)
{
    if (mode == "manual")
        return Control::MANUAL;
    else if (mode == "auto")
        return Control::AUTO;
    else
        return Control::STOP;
}