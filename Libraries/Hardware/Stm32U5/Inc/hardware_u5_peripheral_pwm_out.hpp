/**
  ******************************************************************************
  * @file    hardware_u5_peripheral_pwm_out.hpp
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

#ifndef __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_PWM_OUT_HPP__
#define __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_PWM_OUT_HPP__

#include "hardware_u5_peripheral_config.hpp"

#ifdef STM32U5

class HW::Peripheral::PwmOut : public HW::Base::PwmOut {
public:
    StatusEnum Open(void);
    StatusEnum Close(void);

    void SetPrescaler(uint16_t num_prescaler);

    void Set1(int duty);
    void Set2(int duty);
    void Set3(int duty);
    void Set4(int duty);

    void Set1(float percent);
    void Set2(float percent);
    void Set3(float percent);
    void Set4(float percent);

    volatile void* GetRegisterChannel1(void) { return regCh1; }
    volatile void* GetRegisterChannel2(void) { return regCh2; }
    volatile void* GetRegisterChannel3(void) { return regCh3; }
    volatile void* GetRegisterChannel4(void) { return regCh4; }

private:
#ifdef HAL_TIM_MODULE_ENABLED
    TIM_HandleTypeDef* handle;
    const TimeConfigType* timeConfig;
#endif

    __IO uint32_t*  regCh1;
    __IO uint32_t*  regCh2;
    __IO uint32_t*  regCh3;
    __IO uint32_t*  regCh4;
    __IO uint32_t*  regPre;
};

#endif /* STM32U5 */
#endif /* __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_PWM_OUT_HPP__ */
