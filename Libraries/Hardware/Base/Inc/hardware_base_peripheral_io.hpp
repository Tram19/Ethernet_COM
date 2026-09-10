 
/** 
  ******************************************************************************
  * @file    hardware_base_peripheral_io.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Apr 3, 2025
  * @brief   This file contains all the functions prototypes for the hardware_base_peripheral_io.cpp 
  *          firmware library.
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2018 Gremsy. All rights reserved.
  *
  * The information contained herein is confidential
  * property of Company. The use, copying, transfer or 
  * disclosure of such information is prohibited except
  * by express written agreement with Company.
  *
  ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __HARDWARE_BASE_INC_HARDWARE_BASE_PERIPHERAL_IO_HPP__
#define __HARDWARE_BASE_INC_HARDWARE_BASE_PERIPHERAL_IO_HPP__

/* Includes ------------------------------------------------------------------*/
#include "hardware_base_common.hpp"
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
/// @brief
class  HW::Base::IO : public HW::Base::Common {

public:
    PinEnum     pin = {PIN_NONE};
    virtual StatusEnum On(void) {return ERROR_HAL;};
    virtual StatusEnum Off(void) {return ERROR_HAL;};
    virtual StatusEnum Toggle(void) {return ERROR_HAL;};
    virtual bool Get(void) {return false;};
protected:
    /// PARRAM _________________________________________________________________
    IOTypeEnum          type	        = {NONE_PIN};
    GPIO_InitTypeDef    gpioInitStruct	= {0};
    GPIO_TypeDef*       portHal	        = {0};
    uint16_t            pinHal          = {0};
};
#endif /* __HARDWARE_BASE_INC_HARDWARE_BASE_PERIPHERAL_IO_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
