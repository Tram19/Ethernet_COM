/**
  ******************************************************************************
  * @file    hardware_u5_peripheral_it.c
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    June 4, 2026
  * @brief   Interrupt helper definitions for STM32U5 series.
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2026 Gremsy. All rights reserved.
  *
  ******************************************************************************
*/

#include "main.h"
#include "Hardware/Stm32U5/Inc/hardware_u5_peripheral_it.h"

#if defined STM32U5 || defined STM32H5

g_hardware_u5_peripheral_exti_callback_t    __gp_hardware_exti_callback[16] = {0};
void*                                       __gp_hardware_exti_callback_arg[16] = {0};

void g_hardware_exti_set_func_callback(uint8_t pin_num, g_hardware_u5_peripheral_exti_callback_t func, void* arg)
{
    if(pin_num < 16)
    {
        __gp_hardware_exti_callback[pin_num]        = func;
        __gp_hardware_exti_callback_arg[pin_num]    = arg;
    }
}

#define DEFINE_EXTI_IRQHandler(id) \
void EXTI##id##_IRQHandler(void) \
{ \
    __IO uint16_t PIN = GPIO_PIN_##id; \
    if(__gp_hardware_exti_callback[id] != 0 && \
       __gp_hardware_exti_callback_arg[id] != 0) \
    { \
        PIN = __gp_hardware_exti_callback[id](__gp_hardware_exti_callback_arg[id], PIN); \
    } \
    HAL_GPIO_EXTI_IRQHandler(PIN); \
}

DEFINE_EXTI_IRQHandler(0)
DEFINE_EXTI_IRQHandler(1)
DEFINE_EXTI_IRQHandler(2)
DEFINE_EXTI_IRQHandler(3)
DEFINE_EXTI_IRQHandler(4)
DEFINE_EXTI_IRQHandler(5)
DEFINE_EXTI_IRQHandler(6)
DEFINE_EXTI_IRQHandler(7)
DEFINE_EXTI_IRQHandler(8)
DEFINE_EXTI_IRQHandler(9)
DEFINE_EXTI_IRQHandler(10)
DEFINE_EXTI_IRQHandler(11)
DEFINE_EXTI_IRQHandler(12)
DEFINE_EXTI_IRQHandler(13)
DEFINE_EXTI_IRQHandler(14)
DEFINE_EXTI_IRQHandler(15)

#endif /* STM32U5 */
