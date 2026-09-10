/**
  ******************************************************************************
  * @file    hardware_u5_peripheral_it.h
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    June 4, 2026
  * @brief   Interrupt helper declarations for STM32U5 series.
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2026 Gremsy. All rights reserved.
  *
  ******************************************************************************
*/

#ifndef __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_IT_H__
#define __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_IT_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>

typedef uint16_t (*g_hardware_u5_peripheral_exti_callback_t)(void* arg, uint16_t pin);
void g_hardware_exti_set_func_callback(uint8_t pin_num, g_hardware_u5_peripheral_exti_callback_t func, void* arg);

#ifdef __cplusplus
}
#endif

#endif /* __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_IT_H__ */
