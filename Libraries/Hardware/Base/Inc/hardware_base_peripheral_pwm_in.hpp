 
/** 
  ******************************************************************************
  * @file    hardware_base_peripheral_pwm_in.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Apr 20, 2023
  * @brief   This file contains all the functions prototypes for the hardware_base_peripheral_pwm_in.cpp 
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

#ifndef __HARDWARE_BASE_INC_HARDWARE_BASE_PERIPHERAL_PWM_IN_HPP__
#define __HARDWARE_BASE_INC_HARDWARE_BASE_PERIPHERAL_PWM_IN_HPP__

/* Includes ------------------------------------------------------------------*/

#include "hardware_base_common.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/// @brief class su dung chun cho cac cai dat pwm input
class HW::Base::PwmIn : public HW::Base::Common {

public:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________

    TimeIdEnum  id;
    uint32_t    frequencyReadMin;


    /// OPERATOR _______________________________________________________________

    /** @brief operator constructor */
    PwmIn()
    {
        id                          = TIME_NONE;
        frequencyReadMin            = 0;
        frequencyCapture            = 0;
        dutyCount                   = 0;
        freqCount                   = 0;

        funcDataReadyCallback       = 0;
        ptrFuncDataReadyCallbackArg = 0;
    }

    /** @breif operator copy constructor */
    PwmIn(const PwmIn &c)
    {
        id                          = c.id;
        frequencyReadMin            = c.frequencyReadMin;
        frequencyCapture            = c.frequencyCapture;
        dutyCount                   = c.dutyCount;
        freqCount                   = c.freqCount;

        funcDataReadyCallback       = c.funcDataReadyCallback;
        ptrFuncDataReadyCallbackArg = c.ptrFuncDataReadyCallbackArg;
    }

    /** @brief operation delete */
    virtual ~PwmIn()
    {
        id                          = TIME_NONE;
        frequencyReadMin            = 0;
        frequencyCapture            = 0;
        dutyCount                   = 0;
        freqCount                   = 0;

        funcDataReadyCallback       = 0;
        ptrFuncDataReadyCallbackArg = 0;
    }

    /** @brief operator assignment */
    PwmIn &operator = (const PwmIn &c);

    /** @brief operation add data ready callback
     * */
    PwmIn &operator += (FuncPeripheralPwmInDataReadyCallbackType func){
        funcDataReadyCallback           = func;

        if(ptrFuncDataReadyCallbackArg == 0)
        {
            ptrFuncDataReadyCallbackArg = (PwmInDataType*)g_calloc(sizeof(PwmInDataType));
        }

        ptrFuncDataReadyCallbackArg->sender = (uint32_t)sender;

        return *this;
    }

    /// FUNCTION _______________________________________________________________

    /** @brief ham doc tan so doc cua time
     *  @param none
     *  @return tan so doc cua pwmin
     * */
    virtual uint32_t GetFreqRead(void){return 0;}

    /** @brief ham doc gia tri tan so
     *  @param none
     *  @return tan so do duoc
     * */
    virtual float GetFreq(void){return 0;}

    /** @brief ham doc duty percent
     *  @param none
     *  @return do rong xung theo phan tram
     * */
    virtual float GetDutyPercent(void){return 0;}

    /// PRIVATE ________________________________________________________________

    /** @brief function process interrupt of PWM input
     *  @param[in] freqVal value of frequency (tick)
     *  @param[in] dutyVal value of duty (tick)
     *  @return none
     * */
    virtual void __ITProcess(void* htim){}


protected:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________
    uint32_t            frequencyCapture;
    uint32_t            dutyCount;
    uint32_t            freqCount;

    FuncPeripheralPwmInDataReadyCallbackType    funcDataReadyCallback;
    PwmInDataType*                              ptrFuncDataReadyCallbackArg;


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


private:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


};

/* Exported functions --------------------------------------------------------*/

#endif /* __HARDWARE_BASE_INC_HARDWARE_BASE_PERIPHERAL_PWM_IN_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
