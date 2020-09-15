/**
 * @file motor.cpp
 * @author Philipp Köhler
 * @version 0.1
 * @date 2019-08-17 - created
 * @date 2020-08-04 - last modified
 * @copyright Copyright (c) 2020
 * 
 */
#include "motor.h"

/**
 * @brief Construct a new Motor:: Motor object
 * 
 * @param gpio_pwm 
 * @param gpio_input_1 
 * @param gpio_input_2 
 * @param gpio_stby 
 * @param mcpwm_unit 
 * @param mcpwm_timer 
 * @param mcpwm_io_signals 
 * @param frequency 
 */
Motor::Motor(gpio_num_t gpio_pwm, 
             gpio_num_t gpio_input_1, 
             gpio_num_t gpio_input_2, 
             gpio_num_t gpio_stby, 
             mcpwm_unit_t mcpwm_unit,
             mcpwm_timer_t mcpwm_timer, 
             mcpwm_io_signals_t mcpwm_io_signals, 
             int frequency)
{
    this->gpio_pwm = gpio_pwm;
    this->gpio_input_1 = gpio_input_1;
    this->gpio_input_2 = gpio_input_2;
    this->gpio_stby = gpio_stby;
    this->mcpwm_unit = mcpwm_unit;
    this->mcpwm_timer = mcpwm_timer;
    this->mcpwm_io_signals = mcpwm_io_signals;
    this->frequency = frequency;
    this->firstTime = false;

    pinMode(gpio_pwm, OUTPUT);
    pinMode(this->gpio_input_1, OUTPUT);
    pinMode(this->gpio_input_2, OUTPUT);
    pinMode(this->gpio_stby, OUTPUT);

    mcpwm_gpio_init(mcpwm_unit, mcpwm_io_signals, gpio_pwm);

    mcpwm_config_t pwm_config;
    pwm_config.frequency = frequency; //frequency,
    pwm_config.cmpr_a = 0; //duty cycle of PWMxA = 0
    pwm_config.cmpr_b = 0; //duty cycle of PWMxb = 0
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;

    mcpwm_init(mcpwm_unit, mcpwm_timer, &pwm_config);
}

/**
 * @brief Set the current acceleration
 * 
 * @param direction 
 * @param dutyCycle 
 */
void Motor::setAcceleration(Motor::DIRECTION_E direction, float dutyCycle)
{
    // Serial.printf("Direction: %d \t Old Dir: %d \n", direction, this->direction);

    if (this->direction != direction || firstTime == false)
    {
        this->direction = direction;
        
        if (direction == FORWARD)
        {
            digitalWrite(this->gpio_input_1, 1);
            digitalWrite(this->gpio_input_2, 0);
            digitalWrite(this->gpio_stby, 1);
        }
        else if (direction == BACKWARD)
        {
            digitalWrite(this->gpio_input_1, 0);
            digitalWrite(this->gpio_input_2, 1);
            digitalWrite(this->gpio_stby, 1);
        }
        firstTime = true;
    }    

    mcpwm_set_signal_low(mcpwm_unit, mcpwm_timer, MCPWM_OPR_A);
    mcpwm_set_duty(mcpwm_unit, mcpwm_timer, MCPWM_OPR_A, dutyCycle);
    mcpwm_set_duty_type(mcpwm_unit, mcpwm_timer, MCPWM_OPR_A, MCPWM_DUTY_MODE_0);
}

/**
 * @brief Set the gpio port 
 * 
 * @param gpio_input_1 
 */
void Motor::setGPIO_Input_1(gpio_num_t gpio_input_1)
{
    this->gpio_input_1 = gpio_input_1;
}

/**
 * @brief Set the gpio port 
 * 
 * @param gpio_input_2 
 */
void Motor::setGPIO_Input_2(gpio_num_t gpio_input_2)
{
    this->gpio_input_2 = gpio_input_2;
}

/**
 * @brief Set the standby gpio port 
 * 
 * @param mode 
 */
void Motor::setSTBY(uint8_t mode)
{
    digitalWrite(this->gpio_stby, mode);
}

/**
 * @brief Set the pwm gpio port
 * 
 * @param gpio_pwm 
 */
void Motor::setGPIO_PWM(gpio_num_t gpio_pwm)
{
    this->gpio_pwm = gpio_pwm;
}

/**
 * @brief Set the motor direction
 * 
 * @param direction 
 */
void Motor::setDirection(Motor::DIRECTION_E direction)
{
    this->direction = direction;
}

/**
 * @brief Set the pwm frequency
 * 
 * @param frequency 
 */
void Motor::setFrequency(uint32_t frequency)
{
    this->frequency = frequency;
}

/**
 * @brief Set the mcpwm unit of the esp32
 * 
 * @param mcpwm_unit 
 */
void Motor::setMCPWM_Unit(mcpwm_unit_t mcpwm_unit)
{
    this->mcpwm_unit = mcpwm_unit;
}

/**
 * @brief Set the mcpwm timer of the esp32
 * 
 * @param mcpwm_timer 
 */
void Motor::setMCPWM_Timer(mcpwm_timer_t mcpwm_timer)
{
    this->mcpwm_timer = mcpwm_timer;
}

/**
 * @brief Set the mcpwm signals
 * 
 * @param mcpwm_io_signals 
 */
void Motor::setMCPWM_IO_Signals(mcpwm_io_signals_t mcpwm_io_signals)
{
    this->mcpwm_io_signals = mcpwm_io_signals;
}

/**
 * @brief Get the gpio pwm port
 * 
 * @return gpio_num_t 
 */
gpio_num_t Motor::getGPIO_PWM(){
    return this->gpio_pwm;
}

/**
 * @brief Get the motor direction
 * 
 * @return Motor::DIRECTION_E 
 */
Motor::DIRECTION_E Motor::getDirection()
{
    return this->direction;
}

/**
 * @brief Get the current pwm frequency
 * 
 * @return uint32_t 
 */
uint32_t Motor::getFrequency()
{
    return this->frequency;
}

/**
 * @brief Get the mcpwm unit
 * 
 * @return mcpwm_unit_t 
 */
mcpwm_unit_t Motor::getMCPWM_Unit()
{
    return this->mcpwm_unit;
}

/**
 * @brief Get the mcpwm timer
 * 
 * @return mcpwm_timer_t 
 */
mcpwm_timer_t Motor::getMCPWM_Timer()
{
    return this->mcpwm_timer;
}

/**
 * @brief Get the mcpwm io signals
 * 
 * @return mcpwm_io_signals_t 
 */
mcpwm_io_signals_t Motor::getMCPWM_IO_Signals()
{
    return this->mcpwm_io_signals = mcpwm_io_signals;
}

/**
 * @brief Get the gpio input port
 * 
 * @return gpio_num_t 
 */
gpio_num_t Motor::getGPIO_Input_1()
{
    return this->gpio_input_1;
}

/**
 * @brief Get the gpio input port
 * 
 * @return gpio_num_t 
 */
gpio_num_t Motor::getGPIO_Input_2()
{
    return this->gpio_input_2;
}

/**
 * @brief Get the gpio standby port
 * 
 * @return gpio_num_t 
 */
gpio_num_t Motor::getGPIO_STBY()
{
    return this->gpio_stby;
}
