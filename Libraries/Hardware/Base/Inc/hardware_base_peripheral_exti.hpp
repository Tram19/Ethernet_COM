 
/** 
  ******************************************************************************
  * @file    hardware_base_peripheral_exti.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Apr 24, 2023
  * @brief   This file contains all the functions prototypes for the hardware_base_peripheral_exti.cpp 
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

#ifndef __HARDWARE_BASE_INC_HARDWARE_BASE_PERIPHERAL_EXTI_HPP__
#define __HARDWARE_BASE_INC_HARDWARE_BASE_PERIPHERAL_EXTI_HPP__

/* Includes ------------------------------------------------------------------*/
#include "hardware_base_common.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/// @brief Class Exti su dung chung
class HW::Base::Exti : public HW::Base::Common {

public:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________
    PinEnum         pin;
    ExtiModeEnum    mode;
    PinPupdEnum     pull;
    PriorityEnum    priority;
    uint32_t        size;

    /** @readonly */
    bool            isLevel;
    uint32_t        timeFrequency;
    uint32_t        timeDuty;


    /// OPERATOR _______________________________________________________________

    /** @brief operator constructor */
    Exti()
    {
        pin                             = PIN_NONE;
        mode                            = EXTI_MODE_DEFAULT;
        pull                            = PIN_NO_PULL;
        priority                        = PRIORITY_NONE;
        size                            = 0;
        isLevel                         = false;
        timeFrequency                   = 0;
        timeDuty                        = 0;
        funcDataReadyCallback           = NULL;
        ptrFuncDataReadyCallbackArg     = NULL;
    }

    /** @breif operator copy constructor */
    Exti(const Exti &c)
    {
        pin                             = c.pin;
        mode                            = c.mode;
        pull                            = c.pull;
        priority                        = c.priority;
        size                            = c.size;
        isLevel                         = c.isLevel;
        timeFrequency                   = c.timeFrequency;
        timeDuty                        = c.timeDuty;
        funcDataReadyCallback           = c.funcDataReadyCallback;
        ptrFuncDataReadyCallbackArg     = c.ptrFuncDataReadyCallbackArg;
    }

    /** @brief operation delete */
    virtual ~Exti()
    {
        pin                             = PIN_NONE;
        mode                            = EXTI_MODE_DEFAULT;
        pull                            = PIN_NO_PULL;
        priority                        = PRIORITY_NONE;
        size                            = 0;
        isLevel                         = false;
        timeFrequency                   = 0;
        timeDuty                        = 0;
        funcDataReadyCallback           = NULL;
        ptrFuncDataReadyCallbackArg     = NULL;
    }

    /** @brief operator assignment */
    Exti &operator = (const Exti &c);

    /** @brief operation add function interrupt callback */
    virtual Exti &operator += (FuncPeripheralExtiDataReadyCallbackType func){
        funcDataReadyCallback = func;

        if(ptrFuncDataReadyCallbackArg == 0)
        {
            ptrFuncDataReadyCallbackArg = (ExtiDataType*)g_calloc(sizeof(ExtiDataType));
        }

        ptrFuncDataReadyCallbackArg->sender = (uint32_t)sender;

        return *this;
    }

    /// FUNCTION _______________________________________________________________


protected:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________

    FuncPeripheralExtiDataReadyCallbackType     funcDataReadyCallback;
    ExtiDataType*                               ptrFuncDataReadyCallbackArg;

    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


private:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


};

/* Exported functions --------------------------------------------------------*/


#endif /* __HARDWARE_BASE_INC_HARDWARE_BASE_PERIPHERAL_EXTI_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
