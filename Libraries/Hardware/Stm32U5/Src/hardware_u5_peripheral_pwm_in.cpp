/**
  ******************************************************************************
  * @file    hardware_u5_peripheral_pwm_in.cpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    June 4, 2026
  * @brief   PWM Input Driver for STM32U5 series.
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2026 Gremsy. All rights reserved.
  *
  ******************************************************************************
*/

#include "Hardware/Stm32U5/Inc/hardware_u5_peripheral_pwm_in.hpp"

#ifdef STM32U5

#ifdef HAL_TIM_MODULE_ENABLED

#if !defined(USE_HAL_TIM_REGISTER_CALLBACKS) || (USE_HAL_TIM_REGISTER_CALLBACKS != 1U)
#error "hardware_u5_peripheral_pwm_in requires USE_HAL_TIM_REGISTER_CALLBACKS = 1U in stm32u5xx_hal_conf.h"
#endif

static HW::Peripheral::PwmIn* __gp_hardware_peripheral_pwmin[TIME_COUNT];

static HW::Peripheral::PwmIn* hardware_u5_peripheral_pwmin_get_pinstance(struct __TIM_HandleTypeDef *htim)
{
    for(int i = 0; i < TIME_COUNT; i++)
    {
        if(__gp_hardware_peripheral_pwmin[i] != 0)
        {
            if(__gp_hardware_peripheral_pwmin[i]->__handle == (TIM_HandleTypeDef*)htim){
                return __gp_hardware_peripheral_pwmin[i];
            }
        }
    }
    return 0;
}

static void hardware_u5_peripheral_pwmin_ic_capture_callback(struct __TIM_HandleTypeDef *htim)
{
    HW::Peripheral::PwmIn* ppwmin = hardware_u5_peripheral_pwmin_get_pinstance(htim);
    if(ppwmin)
    {
        ppwmin->__ITProcess(htim);
    }
}

void HW::Peripheral::PwmIn::__ITProcess(void *arg)
{
    TIM_HandleTypeDef* htim = (TIM_HandleTypeDef*)arg;
    freqCount  = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
    dutyCount  = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);

    if(funcDataReadyCallback)
    {
        ptrFuncDataReadyCallbackArg->sender   = (uint32_t)sender;
        ptrFuncDataReadyCallbackArg->timeDuty = dutyCount;
        ptrFuncDataReadyCallbackArg->timeFreq = freqCount;
        funcDataReadyCallback(ptrFuncDataReadyCallbackArg);
    }
}

#endif

uint32_t HW::Peripheral::PwmIn::GetFreqRead(void)
{
    if(isOpen)
    {
        return frequencyCapture;
    }
    return 0;
}

float HW::Peripheral::PwmIn::GetFreq(void)
{
    if(isOpen && freqCount != 0)
    {
        return ((float)frequencyCapture / (float)freqCount);
    }
    return 0;
}

float HW::Peripheral::PwmIn::GetDutyPercent(void)
{
    if(isOpen && freqCount != 0 && dutyCount != 0)
    {
        return ((float)dutyCount / (float)freqCount);
    }
    return 0;
}

StatusEnum HW::Peripheral::PwmIn::Open(void)
{
#ifdef HAL_TIM_MODULE_ENABLED
    StatusEnum ret = OK;

    if(!IS_TIME_ID(id))
    {
        return ERROR_NOT_EXIST;
    }

    if(isOpen == true)
    {
        return ERROR_EXIST;
    }

    const TimeConfigType* timeConfig = &HARDWARE_PERIPHERAL_TIME[id];

    if(timeConfig->ch1Port)
    {
        __handle  = timeConfig->handle;

        HAL_TIM_Base_DeInit(__handle);

        __handle->Init.Prescaler = ((SystemCoreClock / timeConfig->systemClockPrescaler) / 65536) / frequencyReadMin;

        ret = (StatusEnum)HAL_TIM_Base_Init(__handle);
        if(ret != OK)
        {
            return ret;
        }

        if(HAL_TIM_IC_Start_IT(__handle, TIM_CHANNEL_1) != HAL_OK)
        {
            return ERROR_HAL;
        }

        if(HAL_TIM_IC_Start(__handle, TIM_CHANNEL_2) != HAL_OK)
        {
            return ERROR_HAL;
        }

        frequencyCapture = (SystemCoreClock / timeConfig->systemClockPrescaler) / (__handle->Init.Prescaler + 1);

        __handle->IC_CaptureCallback = hardware_u5_peripheral_pwmin_ic_capture_callback;

        __gp_hardware_peripheral_pwmin[id]  = this;
        isOpen                              = true;
    }
    else
    {
        ret = ERROR_HAL;
    }

    return ret;
#endif

    return ERROR_NOT_EXIST;
}

StatusEnum HW::Peripheral::PwmIn::Close(void)
{
#ifdef HAL_TIM_MODULE_ENABLED
    StatusEnum ret = OK;

    if(!IS_TIME_ID(id))
    {
        return ERROR_NOT_EXIST;
    }

    if(isOpen == false)
    {
        return ERROR_EXIST;
    }

    const TimeConfigType* timeConfig = &HARDWARE_PERIPHERAL_TIME[id];

    if(timeConfig->ch1Port)
    {
        __handle  = timeConfig->handle;

        ret = (StatusEnum)HAL_TIM_Base_DeInit(__handle);

        HAL_TIM_IC_Stop_IT(__handle, TIM_CHANNEL_1);
        HAL_TIM_IC_Stop(__handle, TIM_CHANNEL_2);

        __gp_hardware_peripheral_pwmin[id] = nullptr;

        if(ptrFuncDataReadyCallbackArg != nullptr)
        {
            g_free(ptrFuncDataReadyCallbackArg);
            ptrFuncDataReadyCallbackArg = nullptr;
        }

        isOpen = false;
        return ret;
    }
    else
    {
        ret = ERROR_HAL;
    }

    isOpen = false;
    return ret;
#endif

    return ERROR_NOT_EXIST;
}

#endif /** STM32U5 */
