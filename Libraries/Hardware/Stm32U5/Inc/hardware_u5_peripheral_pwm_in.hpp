/**
  ******************************************************************************
  * @file    hardware_u5_peripheral_pwm_in.hpp
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

#ifndef __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_PWM_IN_HPP__
#define __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_PWM_IN_HPP__

#include "hardware_u5_peripheral_config.hpp"

#if defined STM32U5 || defined STM32H5

class HW::Peripheral::PwmIn : public HW::Base::PwmIn {
public:
#ifdef HAL_TIM_MODULE_ENABLED
    TIM_HandleTypeDef* __handle;
#endif

    StatusEnum Open(void);
    StatusEnum Close(void);

    uint32_t GetFreqRead(void);
    float GetFreq(void);
    float GetDutyPercent(void);

    void __ITProcess(void* htim);
};

#endif /* STM32U5 */
#endif /* __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_PWM_IN_HPP__ */
