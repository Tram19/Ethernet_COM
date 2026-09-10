 
/** 
  ******************************************************************************
  * @file    hardware_base_peripheral_adc.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    May 4, 2023
  * @brief   This file contains all the functions prototypes for the hardware_base_peripheral_adc.cpp 
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

#ifndef __HARDWARE_BASE_INC_HARDWARE_BASE_PERIPHERAL_ADC_HPP__
#define __HARDWARE_BASE_INC_HARDWARE_BASE_PERIPHERAL_ADC_HPP__

/* Includes ------------------------------------------------------------------*/
#include "hardware_base_common.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/// @brief class adc su dung chung
class HW::Base::Adc : public HW::Base::Common {

public:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________
    AnalogIdEnum    id;

    /** read only */
    uint32_t        value;

    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________

    /** @brief ham doc gia tri tu ADC
     *  @param none
     *  @return gia tri adc
     * */
    virtual uint32_t Get(void){return 0;}


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

#endif /* __HARDWARE_BASE_INC_HARDWARE_BASE_PERIPHERAL_ADC_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
