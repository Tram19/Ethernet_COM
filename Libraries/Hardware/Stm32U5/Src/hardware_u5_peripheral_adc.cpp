/** 
  ******************************************************************************
  * @file    hardware_u5_peripheral_adc.cpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Nov 25, 2024
  * @brief   ADC Peripheral Driver for STM32U5 series.
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2024 Gremsy. All rights reserved.
  *
  ******************************************************************************
*/ 

#include "Hardware/Stm32U5/Inc/hardware_u5_peripheral_adc.hpp"

#ifdef STM32U5

uint32_t HW::Peripheral::Adc::Get(void)
{
#ifdef HAL_ADC_MODULE_ENABLED
    if(isOpen)
    {
        uint32_t primask = __get_PRIMASK();
        __disable_irq();

        if (HAL_ADC_ConfigChannel(__handle, &sConfig) != HAL_OK)
        {
            __set_PRIMASK(primask);
            return 0;
        }

        HAL_ADC_Start(__handle);
        HAL_ADC_PollForConversion(__handle, 100);
        value = HAL_ADC_GetValue(__handle);
        HAL_ADC_Stop(__handle);

        __set_PRIMASK(primask);
        return value;
    }
#endif
    return 0;
}

StatusEnum HW::Peripheral::Adc::Open(void)
{
#ifdef HAL_ADC_MODULE_ENABLED
    StatusEnum ret = OK;

    if((IS_ANALOG_ID(id) == false) || (IS_ANALOG_CHANNEL(channel) == false))
    {
        return ERROR_NOT_EXIST;
    }

    if(isOpen == true)
    {
        return ERROR_EXIST;
    }

    paramConfig         = (AdcConfigType*)&HARDWARE_PERIPHERAL_ADC[id];
    paramChannelConfig  = (AdcChannelConfigType*)&HARDWARE_PERIPERAL_ADC_CHANNEL[channel];
    __handle            = paramConfig->handle;

    sConfig.Channel         = paramChannelConfig->halChannel;

    if(__handle->Instance == ADC4)
    {
        __handle->Init.ScanConvMode         = ADC4_SCAN_DISABLE;
        __handle->Init.ContinuousConvMode   = DISABLE;
        __handle->Init.NbrOfConversion      = 1;
        __handle->Init.DMAContinuousRequests = DISABLE;
        __handle->Init.EOCSelection         = ADC_EOC_SINGLE_CONV;
        HAL_ADC_Init(__handle);
        HAL_ADCEx_Calibration_Start(__handle, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);

        sConfig.Rank            = ADC4_REGULAR_RANK_1;
        sConfig.SamplingTime    = ADC4_SAMPLINGTIME_COMMON_1;
    }
    else
    {
        __handle->Init.ScanConvMode         = ADC_SCAN_DISABLE;
        __handle->Init.ContinuousConvMode   = DISABLE;
        __handle->Init.NbrOfConversion      = 1;
        __handle->Init.DMAContinuousRequests = DISABLE;
        __handle->Init.EOCSelection         = ADC_EOC_SINGLE_CONV;
        HAL_ADC_Init(__handle);
        HAL_ADCEx_Calibration_Start(__handle, ADC_CALIB_OFFSET_LINEARITY, ADC_SINGLE_ENDED);

        sConfig.Rank            = ADC_REGULAR_RANK_1;
        sConfig.SamplingTime    = ADC_SAMPLETIME_12CYCLES;
        sConfig.SingleDiff      = ADC_SINGLE_ENDED;
        sConfig.OffsetNumber    = ADC_OFFSET_NONE;
        sConfig.Offset          = 0;
    }

    isOpen = true;
    return ret;
#endif
    return ERROR_NOT_EXIST;
}

StatusEnum HW::Peripheral::Adc::Close(void)
{
#ifdef HAL_ADC_MODULE_ENABLED
    if((IS_ANALOG_ID(id) == false) || (IS_ANALOG_CHANNEL(channel) == false))
    {
        return ERROR_NOT_EXIST;
    }

    if(isOpen == false)
    {
        return ERROR_EXIST;
    }

    isOpen = false;
    return OK;
#endif
    return ERROR_NOT_EXIST;
}

#endif /** STM32U5 */
