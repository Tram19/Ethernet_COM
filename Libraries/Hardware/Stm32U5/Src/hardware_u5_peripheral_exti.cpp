/**
  ******************************************************************************
  * @file    hardware_u5_peripheral_exti.cpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    June 4, 2026
  * @brief   EXTI Peripheral Driver for STM32U5 series.
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2026 Gremsy. All rights reserved.
  *
  ******************************************************************************
*/

#include "Hardware/Stm32U5/Inc/hardware_u5_peripheral_exti.hpp"
#include "Hardware/Stm32U5/Inc/hardware_u5_peripheral_time.hpp"
#include "Hardware/Stm32U5/Inc/hardware_u5_peripheral_it.h"

#ifdef STM32U5

static uint16_t hardware_u5_peripheral_exti_callback(void* arg, uint16_t pin)
{
    if(arg)
    {
        HW::Peripheral::Exti* exti = (HW::Peripheral::Exti*)arg;
        exti->__ITProcess(pin);
    }
    return pin;
}

static void hardware_u5_peripheral_exti_task(void* arg)
{
    HW::Peripheral::Exti* exti = (HW::Peripheral::Exti*)arg;
    exti->__TaskProcess();
}

void HW::Peripheral::Exti::__ITProcess(uint16_t GPIO_Pin)
{
    if((GPIO_Pin == pinHal) && (isOpen == true))
    {
        if(HAL_GPIO_ReadPin(portHal, pinHal) == GPIO_PIN_SET)
        {
            isLevel = true;
        }
        else
        {
            isLevel = false;
        }
        task.Set(0x01);
    }
}

void HW::Peripheral::Exti::__TaskProcess(void)
{
    while(1)
    {
        uint32_t ret = task.Wait(0x01, 1000);
        if(ret == 0x01)
        {
            if(isLevel)
            {
                if(mode == EXTI_MODE_RISING)
                {
                    HW::Peripheral::Time* ptimeFeq = (HW::Peripheral::Time*)timeGetFrequency;
                    timeFrequency = (*ptimeFeq)>>TIME_LOOP;
                }
                else
                {
                    HW::Peripheral::Time* ptimeDuty = (HW::Peripheral::Time*)timeGetDuty;
                    *ptimeDuty = 0;
                }
            }
            else
            {
                HW::Peripheral::Time* ptimeFeq = (HW::Peripheral::Time*)timeGetFrequency;
                timeFrequency = (*ptimeFeq)>>TIME_LOOP;

                if(mode == EXTI_MODE_BOTH)
                {
                    HW::Peripheral::Time* ptimeDuty = (HW::Peripheral::Time*)timeGetDuty;
                    timeDuty = (*ptimeDuty)>>TIME_GET;
                }
            }

            if(funcDataReadyCallback)
            {
                ptrFuncDataReadyCallbackArg->level          = isLevel;
                ptrFuncDataReadyCallbackArg->timeFrequency  = timeFrequency;
                ptrFuncDataReadyCallbackArg->timeDuty       = timeDuty;
                ptrFuncDataReadyCallbackArg->sender         = (uint32_t)sender;
                funcDataReadyCallback(ptrFuncDataReadyCallbackArg);
            }
        }
    }
}

StatusEnum HW::Peripheral::Exti::Open(void)
{
    StatusEnum ret = OK;
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    uint8_t pinReal;
    uint8_t priorityHal = 15;

    if(!IS_PIN(pin))
    {
        return ERROR_NOT_EXIST;
    }

    if(isOpen == true)
    {
        return ERROR_EXIST;
    }

    if(priority == PRIORITY_NONE)
    {
        priority = PRIORITY_LOW;
    }

    if(size == 0)
    {
        size = 512;
    }

    if(priority >= 40)          priorityHal = 7;
    else if(priority >= 32)     priorityHal = 9;
    else if(priority >= 24)     priorityHal = 11;
    else if(priority >= 16)     priorityHal = 13;
    else if(priority >= 8)      priorityHal = 15;

    pinReal    = pin - 1;
    pinHal     = HARDWARE_GPIO_PIN[GPIO_PIN(pinReal)];
    portHal    = (GPIO_TypeDef*)HARDWARE_GPIO_PORT[GPIO_PORT(pinReal)];
    irqnHal    = HARDWARE_PERIPHERAL_EXTI_IRQn[GPIO_PIN(pinReal)];

    GPIO_InitStruct.Pin     = pinHal;
    GPIO_InitStruct.Mode    = HARDWARE_PERIPHERAL_EXTI_MODE[mode];
    if(pull == PIN_NO_PULL)
    {
        GPIO_InitStruct.Pull = GPIO_NOPULL;
    }
    else
    {
        if(mode == EXTI_MODE_RISING)
        {
            GPIO_InitStruct.Pull = GPIO_PULLDOWN;
        }
        else
        {
            GPIO_InitStruct.Pull = GPIO_PULLUP;
        }
    }
    GPIO_InitStruct.Speed   = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(portHal, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(irqnHal, priorityHal, 0);
    HAL_NVIC_EnableIRQ(irqnHal);

    task.name       = "Exti_";
    task.name      += HARDWARE_PIN_NAME[pin];
    task.option     = FLAGS_WAIT_ANY;
    task.priority   = priority;
    task.sender     = this;
    task.size       = size;
    task.timeSleep  = 1000;
    task           += hardware_u5_peripheral_exti_task;
    task.Open();

    if(timeGetFrequency == 0)
    {
        timeGetFrequency   = g_calloc(sizeof(HW::Peripheral::Time));
        HW::Peripheral::Time* ptimeFeq = (HW::Peripheral::Time*)timeGetFrequency;
        *ptimeFeq = TIME_US;
    }

    if(timeGetDuty == 0)
    {
        timeGetDuty        = g_calloc(sizeof(HW::Peripheral::Time));
        HW::Peripheral::Time* ptimeDuty = (HW::Peripheral::Time*)timeGetDuty;
        *ptimeDuty = TIME_US;
    }

    g_hardware_exti_set_func_callback(GPIO_PIN(pinReal), hardware_u5_peripheral_exti_callback, (void*)this);
    isOpen = true;

    return ret;
}

StatusEnum HW::Peripheral::Exti::Close(void)
{
    uint8_t pinReal;

    if(!IS_PIN(pin))
    {
        return ERROR_NOT_EXIST;
    }

    if(isOpen == false)
    {
        return ERROR_EXIST;
    }

    pinReal    = pin - 1;
    pinHal     = HARDWARE_GPIO_PIN[GPIO_PIN(pinReal)];
    portHal    = (GPIO_TypeDef*)HARDWARE_GPIO_PORT[GPIO_PORT(pinReal)];
    irqnHal    = HARDWARE_PERIPHERAL_EXTI_IRQn[GPIO_PIN(pinReal)];

    HAL_NVIC_DisableIRQ(irqnHal);
    HAL_GPIO_DeInit(portHal, pinHal);

    task.Close();

    if(timeGetFrequency != nullptr)
    {
        g_free(timeGetFrequency);
        timeGetFrequency = nullptr;
    }

    if(timeGetDuty != nullptr)
    {
        g_free(timeGetDuty);
        timeGetDuty = nullptr;
    }

    if(ptrFuncDataReadyCallbackArg != nullptr)
    {
        g_free(ptrFuncDataReadyCallbackArg);
        ptrFuncDataReadyCallbackArg = nullptr;
    }

    g_hardware_exti_set_func_callback(GPIO_PIN(pinReal), nullptr, nullptr);
    isOpen = false;

    return OK;
}

#endif /** STM32U5  */
