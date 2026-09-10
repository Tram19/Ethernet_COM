/** 
  ******************************************************************************
  * @file    hardware_u5_peripheral_input.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Nov 25, 2024
  * @brief   This file contains all the functions prototypes for the hardware_u5_peripheral_input.cpp 
  *          firmware library.
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2024 Gremsy. All rights reserved.
  *
  ******************************************************************************
*/

#ifndef __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_INPUT_HPP__
#define __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_INPUT_HPP__

/* Includes ------------------------------------------------------------------*/
#include "hardware_u5_peripheral_config.hpp"

#if defined STM32U5 || defined STM32H5
/* Exported class ------------------------------------------------------------*/

class HW::Peripheral::Input : public HW::Base::Input {

public:
    PinPupdEnum pull;

    GPIO_TypeDef*       portHal;
    uint16_t            pinHal;

    StatusEnum Open(void);
    StatusEnum Close(void);
    bool Get(void);

private:
    GPIO_InitTypeDef    gpioInputStruct;
};

#endif /** STM32U5  */
#endif /* __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_INPUT_HPP__ */
