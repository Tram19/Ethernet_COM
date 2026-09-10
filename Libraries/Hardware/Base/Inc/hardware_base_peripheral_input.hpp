 
/** 
  ******************************************************************************
  * @file    hardware_base_peripheral_input.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Apr 20, 2023
  * @brief   This file contains all the functions prototypes for the hardware_base_peripheral_input.cpp 
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

#ifndef __HARDWARE_BASE_INC_HARDWARE_BASE_PERIPHERAL_INPUT_HPP__
#define __HARDWARE_BASE_INC_HARDWARE_BASE_PERIPHERAL_INPUT_HPP__

/* Includes ------------------------------------------------------------------*/

#include "hardware_base_common.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ----------    --------------------------------------------------*/

/// @brief class dung cho input
class HW::Base::Input : public HW::Base::Common {

public:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________

    PinEnum     pin;
    bool        levelActive;


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________

    /** @brief function get status of PIN
     *  @param none
     *  @return PIN status
     * */
    virtual bool Get(void){return false;}

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

#endif /* __HARDWARE_BASE_INC_HARDWARE_BASE_PERIPHERAL_INPUT_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
