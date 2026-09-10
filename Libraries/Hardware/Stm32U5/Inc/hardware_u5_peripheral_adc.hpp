/** 
  ******************************************************************************
  * @file    hardware_u5_peripheral_adc.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Nov 25, 2024
  * @brief   This file contains all the functions prototypes for the hardware_u5_peripheral_adc.cpp 
  *          firmware library.
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2024 Gremsy. All rights reserved.
  *
  ******************************************************************************
*/

#ifndef __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_ADC_HPP__
#define __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_ADC_HPP__

/* Includes ------------------------------------------------------------------*/
#include "hardware_u5_peripheral_config.hpp"

#ifdef STM32U5
/* Exported class ------------------------------------------------------------*/

class HW::Peripheral::Adc : public HW::Base::Adc {

public:
#ifdef HAL_ADC_MODULE_ENABLED
    AdcChannelEnum  channel;
    ADC_HandleTypeDef*   __handle;
#endif

    StatusEnum Open(void);
    StatusEnum Close(void);
    uint32_t Get(void);

private:
#ifdef HAL_ADC_MODULE_ENABLED
    AdcConfigType*          paramConfig;
    AdcChannelConfigType*   paramChannelConfig;
    ADC_ChannelConfTypeDef  sConfig;
#endif
};

#endif /** STM32U5  */
#endif /* __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_ADC_HPP__ */
