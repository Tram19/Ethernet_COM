 
/** 
  ******************************************************************************
  * @file    hardware_u5_debug_log.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Nov 25, 2024
  * @brief   This file contains all the functions prototypes for the hardware_u5_debug_log.cpp 
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

#ifndef __HARDWARE_STM32U5_INC_HARDWARE_U5_DEBUG_LOG_HPP__
#define __HARDWARE_STM32U5_INC_HARDWARE_U5_DEBUG_LOG_HPP__

/* Includes ------------------------------------------------------------------*/
#include <Hardware/Stm32U5/Inc/hardware_u5_peripheral_config.hpp>
#if defined STM32U5 || defined STM32H5
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

///
class HW::Debug::Log : HW::Base::Log {
public:

    ///TYPEDEF


    /// PARAM


    /// OPERATOR
    /** @brief operator truyen chuoi */
    Log &operator << (const char* str){
        Write(str);
        return *this;
    }

    /** @brief operator truyen kiey string */
    Log &operator << (Type::String str){
        Write((uint8_t*)str.array, str.length);
        return *this;
    }

    /** @brief operator truyen so uint32_t */
    Log &operator << (uint32_t num){
        int len = sprintf(buffNumTemp, "%d", (int)num);
        buffNumTemp[len] = 0;
        Write((const char*)buffNumTemp);
        return *this;
    }

    /** @brief operator truyen so int32_t */
    Log &operator << (int32_t num){
        int len = sprintf(buffNumTemp, "%d", (int)num);
        buffNumTemp[len] = 0;
        Write((const char*)buffNumTemp);
        return *this;
    }

    /** @brief operator truyen so int */
    Log &operator << (int num){
        int len = sprintf(buffNumTemp, "%d",num);
        buffNumTemp[len] = 0;
        Write((const char*)buffNumTemp);
        return *this;
    }

    /** @brief operator truyen so float */
    Log &operator << (float num){
        int len = sprintf(buffNumTemp, "%d.%d", (int)num, abs((int)(num*100.0f) - (int)num*100));
        buffNumTemp[len] = 0;
        Write((const char*)buffNumTemp);
        return *this;
    }

    /// FUNCTION

    /** @brief ham cai dat ban dau cho log
     *  @param none
     *  @return none
     * */
    static StatusEnum Init(void);

    /** @brief ham set truyen bang dma
     *  @param none
     *  @return none
     * */
    void SetTransmitBuffer();

    /** @brief ham truyen gia tri trong mang
     *  @param[in] buff     dia chi mang truyen
     *  @param[in] size     do lon mang truyen
     *  @return trang thai qua trinh truyen
     * */
    StatusEnum Write(uint8_t* buff, int size);

    /** @brief ham truyen gia tri trong mang
     *  @param[in] str    dia chi chuoi can truyen
     *  @return trang thai qua trinh truyen
     * */
    StatusEnum Write(const char* str);

    /** @brief ham truyen thong tin cua debug
     *  @param none
     *  @return none
     * */
    void Info();

private:

    /// TYPEDEF


    /// PARAM
    char buffNumTemp[33];

    /// OPERATOR


    /// FUNCTION


};

/* Exported functions --------------------------------------------------------*/

#endif /** STM32U5 */
#endif /* __HARDWARE_STM32U5_INC_HARDWARE_U5_DEBUG_LOG_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
