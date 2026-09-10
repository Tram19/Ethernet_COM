/**
  ******************************************************************************
  * @file    hardware_u5_peripheral_exti.hpp
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

#ifndef __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_EXTI_HPP__
#define __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_EXTI_HPP__

#include "hardware_u5_peripheral_config.hpp"

#ifdef STM32U5

class HW::Peripheral::Exti : public HW::Base::Exti {
public:
    StatusEnum Open(void);
    StatusEnum Close(void);

    void __ITProcess(uint16_t GPIO_Pin);
    void __TaskProcess(void);

protected:
    GPIO_TypeDef* portHal;
    uint16_t pinHal;
    IRQn_Type irqnHal;

    RTOS::TaskEvent task;

    void* timeGetFrequency;
    void* timeGetDuty;
};

#endif /* STM32U5 */
#endif /* __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_EXTI_HPP__ */
