 
/** 
  ******************************************************************************
  * @file    hardware_base_peripheral_output.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Apr 20, 2023
  * @brief   This file contains all the functions prototypes for the hardware_base_peripheral_output.cpp 
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

#ifndef __HARDWARE_BASE_INC_HARDWARE_BASE_PERIPHERAL_OUTPUT_HPP__
#define __HARDWARE_BASE_INC_HARDWARE_BASE_PERIPHERAL_OUTPUT_HPP__

/* Includes ------------------------------------------------------------------*/

#include "hardware_base_common.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/// @brief class su dung chung cho output
class HW::Base::Output : public HW::Base::Common {

public:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________

    PinEnum     pin;
    bool        levelActive;

    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________

    /** @brief ham on led
     *  @param none
     *  @return none
     * */
    virtual StatusEnum On(void){return ERROR_HAL;}

    /** @brief ham off led
     *  @param none
     *  @return none
     * */
    virtual StatusEnum Off(void){return ERROR_HAL;}

    /** @brief ham toggle trang thai led
     *  @param none
     *  @return trang thai qua trinh toggle
     * */
    virtual StatusEnum Toggle(void){return ERROR_HAL;}

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

#endif /* __HARDWARE_BASE_INC_HARDWARE_BASE_PERIPHERAL_OUTPUT_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
