/**
 * @file motor.h
 * @author Philipp Köhler
 * @brief Provides an interface to the dc motors
 * @version 0.1
 * @date 2019-08-17 - created
 * @date 2020-08-04 - last modified
 * @copyright Copyright (c) 2020
 * 
 */
#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include <driver/mcpwm.h>

class Motor
{
public:
    typedef enum
    {
        FORWARD,
        BACKWARD
    } DIRECTION_E;

    Motor(gpio_num_t gpio_pwm,
          gpio_num_t gpio_input_1,
          gpio_num_t gpio_input_2,
          gpio_num_t gpio_stby,
          mcpwm_unit_t mcpwm_unit = MCPWM_UNIT_0,
          mcpwm_timer_t mcpwm_timer = MCPWM_TIMER_0,
          mcpwm_io_signals_t mcpwm_io_signals = MCPWM0A,
          int frequency = 2000);

    void setGPIO_Input_1(gpio_num_t gpio_input_1);
    void setGPIO_Input_2(gpio_num_t gpio_input_2);
    void setSTBY(uint8_t mode);
    void setGPIO_PWM(gpio_num_t gpio_num);
    void setFrequency(uint32_t frequency);
    void setDirection(DIRECTION_E direction);
    void setMCPWM_Unit(mcpwm_unit_t mcpwm_unit);
    void setMCPWM_Timer(mcpwm_timer_t mcpwm_timer);
    void setMCPWM_IO_Signals(mcpwm_io_signals_t mcpwm_io_signals);

    gpio_num_t getGPIO_PWM();
    gpio_num_t getGPIO_Input_1();
    gpio_num_t getGPIO_Input_2();
    gpio_num_t getGPIO_STBY();
    uint32_t getFrequency();
    DIRECTION_E getDirection();
    mcpwm_unit_t getMCPWM_Unit();
    mcpwm_timer_t getMCPWM_Timer();
    mcpwm_io_signals_t getMCPWM_IO_Signals();

    void setAcceleration(DIRECTION_E direction, float dutyCycle);

private:
    gpio_num_t gpio_input_1;
    gpio_num_t gpio_input_2;
    gpio_num_t gpio_stby;
    gpio_num_t gpio_pwm;
    uint32_t frequency;
    DIRECTION_E direction;
    mcpwm_unit_t mcpwm_unit;
    mcpwm_timer_t mcpwm_timer;
    mcpwm_io_signals_t mcpwm_io_signals;
    bool firstTime;
};

#endif