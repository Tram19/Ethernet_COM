 
/** 
  ******************************************************************************
  * @file    rtos_base_common.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Apr 12, 2023
  * @brief   This file contains all the functions prototypes for the rtos_base_common.cpp 
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

#ifndef __RTOS_BASE_INC_RTOS_BASE_COMMON_HPP__
#define __RTOS_BASE_INC_RTOS_BASE_COMMON_HPP__

/* Includes ------------------------------------------------------------------*/

#include "RTOS/rtos_include.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/// @brief class su dung chung cho cac class trong RTOS
class RTOS::Base::Common {

public:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________


    /// OPERATOR _______________________________________________________________
    /** @brief operator constructor */
    Common()
    {
        isOpen = false;
    }

    /** @breif operator copy constructor */
    Common(const Common &c)
    {
        isOpen = c.isOpen;
    }

    /** @brief operation delete */
    virtual ~Common()
    {
        isOpen = false;
    }

    /** @brief operator assignment */
    Common &operator = (const Common &c);

    /// FUNCTION _______________________________________________________________

    /** @brief ham open hardware
     *  @param none
     *  @return trang thai qua trinh open
     * */
    virtual StatusEnum Open(void){return ERROR_HAL;}

    /** @brief ham close hardware
     *  @param none
     *  @return trang thai qua trinh close
     * */
    virtual StatusEnum Close(void){return ERROR_HAL;}

    /** @brief ham doc gia tri co mo api de cho hoat dong khong
     *  @param none
     *  @return true rtos duoc mo
     *          false rtos khong duoc mo
     * */
    virtual bool IsOpen(void){return isOpen;}

protected:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________

    bool isOpen{false};            /// bien quan hien thi da mo hay chua

    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


private:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


};

/* Exported functions --------------------------------------------------------*/

#endif /* __RTOS_BASE_INC_RTOS_BASE_COMMON_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
