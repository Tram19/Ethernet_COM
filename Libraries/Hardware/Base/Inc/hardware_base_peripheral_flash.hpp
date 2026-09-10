 
/** 
  ******************************************************************************
  * @file    hardware_base_peripheral_flash.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Apr 28, 2023
  * @brief   This file contains all the functions prototypes for the hardware_base_peripheral_flash.cpp 
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

#ifndef __HARDWARE_BASE_INC_HARDWARE_BASE_PERIPHERAL_FLASH_HPP__
#define __HARDWARE_BASE_INC_HARDWARE_BASE_PERIPHERAL_FLASH_HPP__

/* Includes ------------------------------------------------------------------*/
#include "hardware_base_common.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/// @brief
class HW::Base::Flash : public HW::Base::Common {

public:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________
    SectorIdEnum id;

    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________

    /** @brief ham xoa flash
     *  @param none
     *  @return trang thai qua trinh
     * */
    virtual StatusEnum Earse(void){return ERROR_NOT_EXIST;}

    /** @brief ham get size
     *  @param none
     *  @return size cua flash
     * */
    virtual uint32_t GetSize(){return 0;}

    /** @brief ham ghi vao flash
     *  @param[in] add dia chi ghi vao
     *  @param[in] buff dia chi mang can ghi vao
     *  @param[in] len do dai mang
     *  @return trang thai qua trinh ghi
     * */
    virtual StatusEnum Write(uint32_t add, uint8_t* buff, int len){return ERROR_NOT_EXIST;}
    virtual StatusEnum Write(uint32_t add, const char* str){return ERROR_NOT_EXIST;}
    virtual StatusEnum Write(uint32_t add, uint8_t data){return ERROR_NOT_EXIST;}
    virtual StatusEnum Write(uint32_t add, uint16_t data){return ERROR_NOT_EXIST;}
    virtual StatusEnum Write(uint32_t add, uint32_t data){return ERROR_NOT_EXIST;}
    virtual StatusEnum Write(uint32_t add, uint64_t data){return ERROR_NOT_EXIST;}

    /** @brief ham doc flash
     *  @param[in] add dia chi doc
     *  @param[in] buff dia chi mang doc / @param[in] pdata dia chi bien doc
     *  @param[in[ buff dia chi mang doc
     *  @return trang thai qua trinh doc
     * */
    virtual StatusEnum Read(uint32_t add, uint8_t* buff, int len){return ERROR_NOT_EXIST;}
    virtual StatusEnum Read(uint32_t add, uint8_t* pdata){return ERROR_NOT_EXIST;}
    virtual StatusEnum Read(uint32_t add, uint16_t* pdata){return ERROR_NOT_EXIST;}
    virtual StatusEnum Read(uint32_t add, uint32_t* pdata){return ERROR_NOT_EXIST;}
    virtual StatusEnum Read(uint32_t add, uint64_t* pdata){return ERROR_NOT_EXIST;}

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

#endif /* __HARDWARE_BASE_INC_HARDWARE_BASE_PERIPHERAL_FLASH_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
