/** 
  ******************************************************************************
  * @file    hardware_u5_peripheral_input.cpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Nov 25, 2024
  * @brief   GPIO Input Driver for STM32U5 series.
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2024 Gremsy. All rights reserved.
  *
  ******************************************************************************
*/ 

#include "Hardware/Stm32U5/Inc/hardware_u5_peripheral_input.hpp"

#ifdef STM32U5

bool HW::Peripheral::Input::Get(void)
{
    if(isOpen)
    {
        if(levelActive == false)
        {
            if(HAL_GPIO_ReadPin(portHal, pinHal) == GPIO_PIN_RESET)
            {
                return true;
            }
        }
        else
        {
            if(HAL_GPIO_ReadPin(portHal, pinHal) == GPIO_PIN_SET)
            {
                return true;
            }
        }
        return false;
    }
    return false;
}

StatusEnum HW::Peripheral::Input::Open(void)
{
    StatusEnum ret = OK;
    uint8_t pinReal;

    if(!IS_PIN(pin))
    {
        return ERROR_NOT_EXIST;
    }

    if(isOpen == true)
    {
        return ERROR_EXIST;
    }

    pinReal = pin - 1;
    portHal = (GPIO_TypeDef*)HARDWARE_GPIO_PORT[GPIO_PORT(pinReal)];
    pinHal  = HARDWARE_GPIO_PIN[GPIO_PIN(pinReal)];

    gpioInputStruct.Mode   = GPIO_MODE_INPUT;
    gpioInputStruct.Pin    = pinHal;
    gpioInputStruct.Speed  = GPIO_SPEED_FREQ_LOW;

    if(pull == PIN_PULL)
    {
        if(levelActive)
        {
            gpioInputStruct.Pull   = GPIO_PULLDOWN;
        }
        else
        {
            gpioInputStruct.Pull   = GPIO_PULLUP;
        }
    }
    else
    {
        gpioInputStruct.Pull   = GPIO_NOPULL;
    }

    HAL_GPIO_Init(portHal, &gpioInputStruct);
    isOpen = true;

    return ret;
}

StatusEnum HW::Peripheral::Input::Close(void)
{
    if(!IS_PIN(pin))
    {
        return ERROR_NOT_EXIST;
    }

    if(isOpen == false)
    {
        return ERROR_EXIST;
    }

    HAL_GPIO_DeInit(portHal, pinHal);
    isOpen = false;

    return OK;
}

#endif /** STM32U5 */
