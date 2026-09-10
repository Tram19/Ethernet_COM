 
/** 
  ******************************************************************************
  * @file    hardware_base_peripheral_time.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Apr 13, 2023
  * @brief   This file contains all the functions prototypes for the hardware_base_peripheral_time.cpp 
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

#ifndef __HARDWARE_BASE_INC_HARDWARE_BASE_PERIPHERAL_TIME_HPP__
#define __HARDWARE_BASE_INC_HARDWARE_BASE_PERIPHERAL_TIME_HPP__

/* Includes ------------------------------------------------------------------*/

#include "hardware_base_common.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/// @brief dinh nghia su dung chun cho time trong cac platform
class HW::Base::Time/// : public HW::Base::Common
{

public:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________
    I2cIdEnum   id;
    uint32_t    deviceAddress;

    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________

    /** @brief write an amount of data to specific mem address
     *  @param[in] memAddress internal memory address
     *  @param[in] data pointer to the data buffer
     *  return status
     * */
    StatusEnum SendMem(uint32_t memAddress, uint8_t data);


    /** @brief send data in master mode
     *  @param[in] buff pointer to the data buffer
     *  @param[in] len amount of data to be sent
     *  @return status
     * */
    StatusEnum Write(uint8_t *buff, int len);


protected:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


private:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


};

/* Exported functions --------------------------------------------------------*/

#endif /* __HARDWARE_BASE_INC_HARDWARE_BASE_PERIPHERAL_TIME_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
