/**
  ******************************************************************************
  * @file    hardware_u5_peripheral_io.cpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    June 4, 2026
  * @brief   Flexible IO Peripheral Driver for STM32U5 series.
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2026 Gremsy. All rights reserved.
  *
  ******************************************************************************
*/

#include "Hardware/Stm32U5/Inc/hardware_u5_peripheral_io.hpp"
#include <algorithm>
#include <vector>

#if defined STM32U5 || defined STM32H5

static std::vector<std::pair<uint16_t, TrigFuncCallBack>> statePair;

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
    for (auto it = statePair.begin(); it != statePair.end(); ++it) {
        if(it->first == GPIO_Pin)
        {
            if(it->second)
            {
                it->second();
            }
        }
    }
}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
    for (auto it = statePair.begin(); it != statePair.end(); ++it) {
        if(it->first == GPIO_Pin)
        {
            if(it->second)
            {
                it->second();
            }
        }
    }
}

FlexibleIO::FlexibleIO(PinEnum _pin)
{
    this->pin = _pin;
}

StatusEnum FlexibleIO::ConfigAsInput(void)
{
    this->type = INPUT_PIN;
    return Open();
}

StatusEnum FlexibleIO::ConfigAsOutput(bool _levelActive, PinSpeedEnum _speed)
{
    this->type = OUTPUT_PIN;
    this->speed = _speed;
    this->levelActive = _levelActive;
    return Open();
}

StatusEnum FlexibleIO::ConfigAsInterrupt(IOEdgeITEnum _edge, TrigFuncCallBack &&_func)
{
    this->type = IT_PIN;
    this->edge = _edge;
    this->ITcallback = std::move(_func);
    return Open();
}

bool FlexibleIO::Get(void)
{
    if(isOpen)
    {
        return (HAL_GPIO_ReadPin(portHal, pinHal) == GPIO_PIN_SET);
    }
    return false;
}

StatusEnum FlexibleIO::On(void)
{
    if(isOpen && type == OUTPUT_PIN)
    {
        HAL_GPIO_WritePin(portHal, pinHal, (GPIO_PinState)levelActive);
        return OK;
    }
    return ERROR_NOT_EXIST;
}

StatusEnum FlexibleIO::Off(void)
{
    if(isOpen && type == OUTPUT_PIN)
    {
        HAL_GPIO_WritePin(portHal, pinHal, (GPIO_PinState)(levelActive ^ 1));
        return OK;
    }
    return ERROR_NOT_EXIST;
}

StatusEnum FlexibleIO::Close(void)
{
    if(!IS_PIN(pin))
    {
        return ERROR_NOT_EXIST;
    }

    if(isOpen == false)
    {
        return ERROR_EXIST;
    }

    if(type == IT_PIN)
    {
        auto it = std::find_if(statePair.begin(), statePair.end(),
            [this](const auto& pair) {
                return pair.first == this->pinHal;
            });

        if(it != statePair.end()) {
            statePair.erase(it);
        }
    }

    HAL_GPIO_DeInit(portHal, pinHal);
    isOpen = false;
    ITcallback = nullptr;

    return OK;
}

uint8_t FlexibleIO::GetIRQNumber(uint16_t pinMask)
{
    switch(pinMask) {
        case GPIO_PIN_0:  return EXTI0_IRQn;
        case GPIO_PIN_1:  return EXTI1_IRQn;
        case GPIO_PIN_2:  return EXTI2_IRQn;
        case GPIO_PIN_3:  return EXTI3_IRQn;
        case GPIO_PIN_4:  return EXTI4_IRQn;
        case GPIO_PIN_5:  return EXTI5_IRQn;
        case GPIO_PIN_6:  return EXTI6_IRQn;
        case GPIO_PIN_7:  return EXTI7_IRQn;
        case GPIO_PIN_8:  return EXTI8_IRQn;
        case GPIO_PIN_9:  return EXTI9_IRQn;
        case GPIO_PIN_10: return EXTI10_IRQn;
        case GPIO_PIN_11: return EXTI11_IRQn;
        case GPIO_PIN_12: return EXTI12_IRQn;
        case GPIO_PIN_13: return EXTI13_IRQn;
        case GPIO_PIN_14: return EXTI14_IRQn;
        case GPIO_PIN_15: return EXTI15_IRQn;
        default: return 0xFF;
    }
}

StatusEnum FlexibleIO::Open(void)
{
    switch (this->type) {
        case INPUT_PIN:
        {
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
            HAL_GPIO_DeInit(portHal, pinHal);

            gpioInitStruct.Mode   = GPIO_MODE_INPUT;
            gpioInitStruct.Pin    = pinHal;
            gpioInitStruct.Speed  = GPIO_SPEED_FREQ_LOW;
            HAL_GPIO_Init(portHal, &gpioInitStruct);

            isOpen = true;
            return OK;
        }
        break;

        case OUTPUT_PIN:
        {
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
            HAL_GPIO_DeInit(portHal, pinHal);

            gpioInitStruct.Mode   = GPIO_MODE_OUTPUT_PP;
            gpioInitStruct.Pin    = pinHal;
            gpioInitStruct.Speed  = speed;
            HAL_GPIO_Init(portHal, &gpioInitStruct);

            isOpen = true;
            Off();
            return OK;
        }
        break;

        case IT_PIN:
        {
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
            pinHal = HARDWARE_GPIO_PIN[GPIO_PIN(pinReal)];
            HAL_GPIO_DeInit(portHal, pinHal);

            gpioInitStruct.Mode = edge;
            gpioInitStruct.Pin  = pinHal;
            gpioInitStruct.Pull = GPIO_PULLUP;
            HAL_GPIO_Init(portHal, &gpioInitStruct);

            statePair.push_back(std::make_pair(pinHal, std::move(ITcallback)));

            uint8_t irqNumber = GetIRQNumber(pinHal);
            if(irqNumber != 0xFF) {
                if(NVIC_GetEnableIRQ((IRQn_Type)irqNumber) == 0)
                {
                    HAL_NVIC_SetPriority((IRQn_Type)irqNumber, 5, 0);
                    HAL_NVIC_EnableIRQ((IRQn_Type)irqNumber);
                }
            }
            isOpen = true;
            return OK;
        }
        break;

        default:
            return ERROR_NOT_EXIST;
    }
}

#endif /** STM32U5 */
