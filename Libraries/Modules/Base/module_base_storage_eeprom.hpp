 
/** 
  ******************************************************************************
  * @file    module_base_storage_eeprom.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    May 5, 2023
  * @brief   This file contains all the functions prototypes for the module_base_storage_eeprom.cpp 
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

#ifndef __MODULES_BASE_MODULE_BASE_STORAGE_EEPROM_HPP__
#define __MODULES_BASE_MODULE_BASE_STORAGE_EEPROM_HPP__

/* Includes ------------------------------------------------------------------*/

#include "module_base_common.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/// @brief class dung chung cho cac luu chu lien quan den eeprom
class Module::Base::StorageEeprom : public Module::Base::Common {

public:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________

    uint32_t    paramCount;
    uint16_t*   paramKey;

    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________

    /** @brief ham doc gia tri tu dia chi
     *  @param[in] add dia chi can doc
     *  @param[in] pdata dia chi luu gia tri da doc
     *  @return trang thai qua trinh doc
     * */
    virtual StatusEnum Read(uint16_t add, uint16_t* pdata){return ERROR_NOT_EXIST;}

    /** @brief ham ghi gia tri vao ee
     *  @param[in] add dia chi can ghi
     *  @param[in] data gia tri can ghi
     *  @return trang thai qua trinh ghi
     * */
    virtual StatusEnum Write(uint16_t add, uint16_t data){return ERROR_NOT_EXIST;}

    /** @brief ham ghi dua ve gia tri mac dinh
     *  @param none
     *  @return trang thai qua trinh ghi
     * */
    virtual StatusEnum Format(void){return ERROR_NOT_EXIST;}

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

#endif /* __MODULES_BASE_MODULE_BASE_STORAGE_EEPROM_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
