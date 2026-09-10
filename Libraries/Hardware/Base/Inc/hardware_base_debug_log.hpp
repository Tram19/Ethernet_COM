 
/** 
  ******************************************************************************
  * @file    hardware_base_debug_log.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Apr 11, 2023
  * @brief   This file contains all the functions prototypes for the hardware_base_debug_log.cpp 
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

#ifndef __HARDWARE_BASE_INC_HARDWARE_BASE_DEBUG_LOG_HPP__
#define __HARDWARE_BASE_INC_HARDWARE_BASE_DEBUG_LOG_HPP__

/* Includes ------------------------------------------------------------------*/

#include "hardware_base_common.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

///
class HW::Base::Log : HW::Base::Common
{
public:

    /// TYPEDEF


    /// PARAM


    /// OPERATOR

    /** @brief operator truyen chuoi */
    virtual Log &operator << (const char* str){return *this;}

    /** @brief operator truyen so int */
    virtual Log &operator << (int num){return *this;}

    /** @brief operator truyen so float */
    virtual Log &operator << (float num){return *this;}


    /** @brief operation delete */
    virtual ~Log(){}

    /// FUNCTION

    /** @brief ham truyen gia tri trong mang
     *  @param[in] buff     dia chi mang truyen
     *  @param[in] size     do lon mang truyen
     *  @return trang thai qua trinh truyen
     * */
    virtual StatusEnum Write(uint8_t* buff, int size){return ERROR_HAL;}

    /** @brief ham truyen gia tri trong mang
     *  @param[in] str    dia chi chuoi can truyen
     *  @return trang thai qua trinh truyen
     * */
    virtual StatusEnum Write(const char* str){return ERROR_HAL;}

    /** @brief ham truyen thong tin cua debug
     *  @param none
     *  @return none
     * */
    virtual void Info(void){}


protected:

    /// TYPEDEF


    /// PARAM

    /** @brief bien su dung de chon truyen dma hay khong */
    bool isTransmitBuffer;

    /// OPERATOR


    /// FUNCTION


};

/* Exported functions --------------------------------------------------------*/

#endif /* __HARDWARE_BASE_INC_HARDWARE_BASE_DEBUG_LOG_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
