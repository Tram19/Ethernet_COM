 
/** 
  ******************************************************************************
  * @file    hardware_base_common.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Apr 11, 2023
  * @brief   This file contains all the functions prototypes for the hardware_base_common.cpp 
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

#ifndef __HARDWARE_BASE_INC_HARDWARE_BASE_COMMON_HPP__
#define __HARDWARE_BASE_INC_HARDWARE_BASE_COMMON_HPP__

/* Includes ------------------------------------------------------------------*/

#include "Hardware/hardware_include.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

///
class HW::Base::Common
{
public:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________

    object sender;

    /// OPERATOR _______________________________________________________________

    /** @brief operator constructor */
    Common()
    {
        sender = NULL;
        isOpen = false;
    }

    /** @breif operator copy constructor */
    Common(const Common &c)
    {
        sender = c.sender;
        isOpen = c.isOpen;
    }

    /** @brief operation delete */
    virtual ~Common()
    {
        sender = NULL;
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

    /** @brief ham doc xem doi tuong da duoc cai dat hoan thanh chua
     *  @param none
     *  @return trang thai cai dat
     * */
    virtual bool IsOpen(void){return isOpen;}

protected:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________

    bool isOpen{false};

    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


private:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


};

/* Exported functions --------------------------------------------------------*/

#endif /* __HARDWARE_BASE_INC_HARDWARE_BASE_COMMON_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
