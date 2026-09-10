 
/** 
  ******************************************************************************
  * @file    module_base_encoder_pwm.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    May 15, 2023
  * @brief   This file contains all the functions prototypes for the module_base_encoder_pwm.cpp 
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

#ifndef __MODULES_BASE_MODULE_BASE_ENCODER_PWM_HPP__
#define __MODULES_BASE_MODULE_BASE_ENCODER_PWM_HPP__

/* Includes ------------------------------------------------------------------*/

#include "module_base_common.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/// @brief class su dung chun cho cac pwm su dung pwm
class Module::Base::EncoderPwm : public Module::Base::Common {

public:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________
    TimeIdEnum              timeId;

    int32_t                 countMax;
    RotationDirectionEnum   direction;
    PriorityEnum            priority;

    /// OPERATOR _______________________________________________________________

    /** @brief operator constructor */
    EncoderPwm()
    {
        timeId      = TIME_NONE;
        countMax    = 0;
        count       = 0;
        direction   = NORMAL;
        priority    = PRIORITY_NONE;

        funcDataReadyCallback       = NULL;
        funcDataReadyCallbackArg    = NULL;
        funcLostconnectCallback     = NULL;
        funcLostconnectCallbackArg  = NULL;
        funcReconnectCallback       = NULL;
        funcReconnectCallbackArg    = NULL;

        stateCommon                 = ENCODER_PWM_STATE_IDLE;
        pwmin.id                    = TIME_NONE;
        task.priority               = PRIORITY_NONE;
    }

    /** @breif operator copy constructor */
    EncoderPwm(const EncoderPwm &c)
    {
        timeId      = c.timeId;
        countMax    = c.countMax;
        count       = c.count;
        direction   = c.direction;
        priority    = c.priority;

        funcDataReadyCallback       = c.funcDataReadyCallback;
        funcDataReadyCallbackArg    = c.funcDataReadyCallbackArg;
        funcLostconnectCallback     = c.funcLostconnectCallback;
        funcLostconnectCallbackArg  = c.funcLostconnectCallbackArg;
        funcReconnectCallback       = c.funcReconnectCallback;
        funcReconnectCallbackArg    = c.funcReconnectCallbackArg;

        stateCommon                 = c.stateCommon;
        pwmin.id                    = c.pwmin.id;
        task.priority               = c.task.priority;
    }

    /** @brief operation delete */
    virtual ~EncoderPwm()
    {
        timeId      = TIME_NONE;
        countMax    = 0;
        count       = 0;
        direction   = NORMAL;
        priority    = PRIORITY_NONE;

        funcDataReadyCallback       = NULL;
        funcDataReadyCallbackArg    = NULL;
        funcLostconnectCallback     = NULL;
        funcLostconnectCallbackArg  = NULL;
        funcReconnectCallback       = NULL;
        funcReconnectCallbackArg    = NULL;

        stateCommon                 = ENCODER_PWM_STATE_IDLE;
        pwmin.id                    = TIME_NONE;
        task.priority               = PRIORITY_NONE;
    }

    /** @brief operator assignment */
    EncoderPwm &operator = (const EncoderPwm &c);

    /** @brief operator add function data ready callback */
    virtual EncoderPwm &operator += (const FuncEncoderPwmDataReadyCallbackType func){
        funcDataReadyCallback = func;

        if(funcDataReadyCallbackArg == 0)
        {
            funcDataReadyCallbackArg = (EncoderPwmDataReadyType*)g_calloc(sizeof(EncoderPwmDataReadyType));
        }

        funcDataReadyCallbackArg->sender = (uint32_t)sender;

        return *this;
    }

    /** @brief operator add function reconnect callback */
    virtual EncoderPwm &operator += (const FuncEncoderPwmReconnectCallbackType func){
        funcReconnectCallback = func;

        if(funcReconnectCallbackArg == 0)
        {
            funcReconnectCallbackArg = (EncoderPwmReconnectType*)g_calloc(sizeof(EncoderPwmReconnectType));
        }

        funcReconnectCallbackArg->sender = (uint32_t)sender;

        return *this;
    }

    /** @brief operator add function lostconnect callback */
    virtual EncoderPwm &operator += (const FuncEncoderPwmLostconnectCallbackType func){
        funcLostconnectCallback = func;

        if(funcLostconnectCallbackArg == 0)
        {
            funcLostconnectCallbackArg = (EncoderPwmLostconnectType*)g_calloc(sizeof(EncoderPwmLostconnectType));
        }

        funcLostconnectCallbackArg->sender = (uint32_t)sender;

        return *this;
    }

    /// FUNCTION _______________________________________________________________

    /** @brief ham doc trang thai hoat dong
     *  @param none
     *  @return trang thai hoat dong
     * */
    virtual EncoderPwmStateEnum GetState(void){return stateCommon;}

    /** @brief ham doc gia tri so dem cua encoder
     *  @param none
     *  @return so dem cua encoder
     * */
    virtual int32_t GetCount(void){return count;}


    /// PRIVATE ________________________________________________________________
    /** @brief ham xu ly task chay an
     *  @param none
     *  @return none
     * */
    virtual void __TaskProcess(void){};

    /** @brief ham xu ly ngat cua pwmin
     *  @param[in] freqval gia tri cua tan so theo tick cua time
     *  @param[in] dutyval gia tri cua do rong xung theo tick cua time
     *  @return trang thai qua trinh xu ly ngat
     * */
    virtual StatusEnum __PwmInDataReadyCallback(int32_t freqval, int32_t dutyval){return ERROR_NOT_EXIST;};

protected:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________

    /** */
    FuncEncoderPwmDataReadyCallbackType     funcDataReadyCallback;
    EncoderPwmDataReadyType*                funcDataReadyCallbackArg;

    FuncEncoderPwmReconnectCallbackType     funcReconnectCallback;
    EncoderPwmReconnectType*                funcReconnectCallbackArg;

    FuncEncoderPwmLostconnectCallbackType   funcLostconnectCallback;
    EncoderPwmLostconnectType*              funcLostconnectCallbackArg;

    /**
     * */
    EncoderPwmStateEnum                     stateCommon;

    /** */
    HW::Peripheral::PwmIn                   pwmin;

    /** */
    RTOS::TaskEvent                         task;

    /** @brief so dem cua encoder */
    int32_t                                 count;

    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


private:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


};

/* Exported functions --------------------------------------------------------*/
//
#endif /* __MODULES_BASE_MODULE_BASE_ENCODER_PWM_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
