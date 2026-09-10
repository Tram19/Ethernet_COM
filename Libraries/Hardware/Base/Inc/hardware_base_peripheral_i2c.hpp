 
/** 
  ******************************************************************************
  * @file    hardware_base_peripheral_i2c.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    May 4, 2023
  * @brief   This file contains all the functions prototypes for the hardware_base_peripheral_i2c.cpp 
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

#ifndef __HARDWARE_BASE_INC_HARDWARE_BASE_PERIPHERAL_I2C_HPP__
#define __HARDWARE_BASE_INC_HARDWARE_BASE_PERIPHERAL_I2C_HPP__

/* Includes ------------------------------------------------------------------*/

#include "hardware_base_common.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/// @brief class su dung chung cho i2c
class HW::Base::I2c : public HW::Base::Common {

public:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________

     I2cIdEnum  id;
     uint32_t   deviceAddress;

    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________

     /** @brief Send an amount of data to specific mem address
      *  @param[in] memAddress internal memory address
      *  @param[in] data gia tri muon ghi
      *  return status
      * */
     virtual StatusEnum SendMem(uint16_t memAddress, uint8_t data){return ERROR_NOT_EXIST;}

     /** @brief send data in master mode
      *  @param[in] buff pointer to the data buffer
      *  @param[in] len amount of data to be sent
      *  @return status
      * */
     virtual StatusEnum Write(uint8_t *buff, int len){return ERROR_NOT_EXIST;}

     /** @brief Read one byte from a slave register */
     virtual StatusEnum GetMem(uint16_t memAddress, uint8_t* pdata){return ERROR_NOT_EXIST;}


     /** @brief Master Rx — read buffer from slave */
     virtual StatusEnum Read(uint8_t* buff, int len){return ERROR_NOT_EXIST;}

     virtual StatusEnum Reset(){return ERROR_NOT_EXIST;}
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

#endif /* __HARDWARE_BASE_INC_HARDWARE_BASE_PERIPHERAL_I2C_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
