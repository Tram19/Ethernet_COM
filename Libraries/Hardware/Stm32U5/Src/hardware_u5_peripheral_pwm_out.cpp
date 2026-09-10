/**
  ******************************************************************************
  * @file    hardware_u5_peripheral_pwm_out.cpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    June 4, 2026
  * @brief   PWM Output Driver for STM32U5 series.
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2026 Gremsy. All rights reserved.
  *
  ******************************************************************************
*/

#include "Hardware/Stm32U5/Inc/hardware_u5_peripheral_pwm_out.hpp"

#ifdef STM32U5

void HW::Peripheral::PwmOut::SetPrescaler(uint16_t num_prescaler)
{
#ifdef HAL_TIM_MODULE_ENABLED
    if(num_prescaler != prescaler)
    {
        if(isCh1 != 0xFF || isCh2 != 0xFF || isCh3 != 0xFF || isCh4 != 0xFF)
        {
            prescaler = num_prescaler;
            *regPre = prescaler;

            frequency = (SystemCoreClock / timeConfig->systemClockPrescaler) / ((handle->Init.Prescaler + 1) * (handle->Init.Period + 1));
        }
    }
#endif
}

void HW::Peripheral::PwmOut::Set1(int duty)
{
#ifdef HAL_TIM_MODULE_ENABLED
    if(isCh1 != 0xFF)
    {
        if((uint32_t)duty > handle->Init.Period)
        {
            duty = handle->Init.Period;
        }

        *regCh1 = duty;
        ch1Precent = (float)duty / (float)handle->Init.Period;
    }
#endif
}

void HW::Peripheral::PwmOut::Set2(int duty)
{
#ifdef HAL_TIM_MODULE_ENABLED
    if(isCh2 != 0xFF)
    {
        if((uint32_t)duty > handle->Init.Period)
        {
            duty = handle->Init.Period;
        }

        *regCh2 = duty;
        ch2Precent = (float)duty / (float)handle->Init.Period;
    }
#endif
}

void HW::Peripheral::PwmOut::Set3(int duty)
{
#ifdef HAL_TIM_MODULE_ENABLED
    if(isCh3 != 0xFF)
    {
        if((uint32_t)duty > handle->Init.Period)
        {
            duty = handle->Init.Period;
        }

        *regCh3 = duty;
        ch3Precent = (float)duty / (float)handle->Init.Period;
    }
#endif
}

void HW::Peripheral::PwmOut::Set4(int duty)
{
#ifdef HAL_TIM_MODULE_ENABLED
    if(isCh4 != 0xFF)
    {
        if((uint32_t)duty > handle->Init.Period)
        {
            duty = handle->Init.Period;
        }

        *regCh4 = duty;
        ch4Precent = (float)duty / (float)handle->Init.Period;
    }
#endif
}

void HW::Peripheral::PwmOut::Set1(float percent)
{
#ifdef HAL_TIM_MODULE_ENABLED
    if(isCh1 != 0xFF)
    {
        uint32_t duty = handle->Init.Period * percent;

        if(duty > handle->Init.Period)
        {
            duty = handle->Init.Period;
        }

        *regCh1 = duty;
        ch1Precent = percent;
    }
#endif
}

void HW::Peripheral::PwmOut::Set2(float percent)
{
#ifdef HAL_TIM_MODULE_ENABLED
    if(isCh2 != 0xFF)
    {
        uint32_t duty = handle->Init.Period * percent;

        if(duty > handle->Init.Period)
        {
            duty = handle->Init.Period;
        }

        *regCh2 = duty;
        ch2Precent = percent;
    }
#endif
}

void HW::Peripheral::PwmOut::Set3(float percent)
{
#ifdef HAL_TIM_MODULE_ENABLED
    if(isCh3 != 0xFF)
    {
        uint32_t duty = handle->Init.Period * percent;

        if(duty > handle->Init.Period)
        {
            duty = handle->Init.Period;
        }

        *regCh3 = duty;
        ch3Precent = percent;
    }
#endif
}

void HW::Peripheral::PwmOut::Set4(float percent)
{
#ifdef HAL_TIM_MODULE_ENABLED
    if(isCh4 != 0xFF)
    {
        uint32_t duty = handle->Init.Period * percent;

        if(duty > handle->Init.Period)
        {
            duty = handle->Init.Period;
        }

        *regCh4 = duty;
        ch4Precent = percent;
    }
#endif
}

StatusEnum HW::Peripheral::PwmOut::Open(void)
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

    timeConfig = &HARDWARE_PERIPHERAL_TIME[id];
    handle  = timeConfig->handle;
    isCh1   = 0xFF;
    isCh2   = 0xFF;
    isCh3   = 0xFF;
    isCh4   = 0xFF;

    if(frequency != 0)
    {
        uint32_t period  = 0;

        HAL_TIM_Base_DeInit(handle);

        handle->Init.Prescaler = prescaler;
        period = ((SystemCoreClock / timeConfig->systemClockPrescaler) / (handle->Init.Prescaler + 1)) / frequency - 1;

        if(period > 0xFFFF)
        {
            return ERROR_HAL;
        }

        handle->Init.Period = period;

        HAL_TIM_Base_Init(handle);
    }

    if(timeConfig->ch1Port)
    {
        ret = (StatusEnum)HAL_TIM_PWM_Start(handle, TIM_CHANNEL_1);
        if(ret != OK)
        {
            return ret;
        }
        else
        {
            isCh1   = TIM_CHANNEL_1;
            regCh1  = &handle->Instance->CCR1;
        }
    }

    if(timeConfig->ch2Port)
    {
        ret = (StatusEnum)HAL_TIM_PWM_Start(handle, TIM_CHANNEL_2);
        if(ret != OK)
        {
            return ret;
        }
        else
        {
            if(isCh1 == 0xFF)
            {
                isCh1  = TIM_CHANNEL_2;
                regCh1 = &handle->Instance->CCR2;
            }
            else
            {
                isCh2  = TIM_CHANNEL_2;
                regCh2 = &handle->Instance->CCR2;
            }
        }
    }

    if(timeConfig->ch3Port)
    {
        ret = (StatusEnum)HAL_TIM_PWM_Start(handle, TIM_CHANNEL_3);
        if(ret != OK)
        {
            return ret;
        }
        else
        {
            if(isCh1 == 0xFF)
            {
                isCh1   = TIM_CHANNEL_3;
                regCh1  = &handle->Instance->CCR3;
            }
            else if(isCh2 == 0xFF)
            {
                isCh2   = TIM_CHANNEL_3;
                regCh2  = &handle->Instance->CCR3;
            }
            else
            {
                isCh3   = TIM_CHANNEL_3;
                regCh3  = &handle->Instance->CCR3;
            }
        }
    }

    if(timeConfig->ch4Port)
    {
        ret = (StatusEnum)HAL_TIM_PWM_Start(handle, TIM_CHANNEL_4);
        if(ret != OK)
        {
            return ret;
        }
        else
        {
            if(isCh1 == 0xFF)
            {
                isCh1   = TIM_CHANNEL_4;
                regCh1  = &handle->Instance->CCR4;
            }
            else if(isCh2 == 0xFF)
            {
                isCh2   = TIM_CHANNEL_4;
                regCh2  = &handle->Instance->CCR4;
            }
            else if(isCh3 == 0xFF)
            {
                isCh3   = TIM_CHANNEL_4;
                regCh3  = &handle->Instance->CCR4;
            }
            else
            {
                isCh4   = TIM_CHANNEL_4;
                regCh4  = &handle->Instance->CCR4;
            }
        }
    }

    if(isCh1 == 0xFF && isCh2 == 0xFF && isCh3 == 0xFF && isCh4 == 0xFF)
    {
        return ERROR_NOT_EXIST;
    }

    regPre = &handle->Instance->PSC;
    frequency = (SystemCoreClock / timeConfig->systemClockPrescaler) / ((handle->Init.Prescaler + 1) * (handle->Init.Period + 1));
    dutyMax = handle->Init.Period;
    isOpen = true;

    return ret;
#endif

    return ERROR_NOT_EXIST;
}

StatusEnum HW::Peripheral::PwmOut::Close(void)
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
    handle  = timeConfig->handle;

    if(isCh1 != 0xFF)
    {
        ret = (StatusEnum)HAL_TIM_PWM_Stop(handle, isCh1);
        if(ret != OK)
        {
            return ret;
        }
        isCh1 = 0xFF;
    }

    if(isCh2 != 0xFF)
    {
        ret = (StatusEnum)HAL_TIM_PWM_Stop(handle, isCh2);
        if(ret != OK)
        {
            return ret;
        }
        isCh2 = 0xFF;
    }

    if(isCh3 != 0xFF)
    {
        ret = (StatusEnum)HAL_TIM_PWM_Stop(handle, isCh3);
        if(ret != OK)
        {
            return ret;
        }
        isCh3 = 0xFF;
    }

    if(isCh4 != 0xFF)
    {
        ret = (StatusEnum)HAL_TIM_PWM_Stop(handle, isCh4);
        if(ret != OK)
        {
            return ret;
        }
        isCh4 = 0xFF;
    }

    isOpen = false;
    return ret;
#endif

    return ERROR_NOT_EXIST;
}

#endif /** STM32U5 */
