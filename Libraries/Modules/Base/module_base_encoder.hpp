 
/** 
  ******************************************************************************
  * @file    module_base_encoder.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Sep 15, 2023
  * @brief   This file contains all the functions prototypes for the module_base_encoder.cpp 
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

#ifndef __MODULES_BASE_MODULE_BASE_ENCODER_HPP__
#define __MODULES_BASE_MODULE_BASE_ENCODER_HPP__

/* Includes ------------------------------------------------------------------*/

#include "module_base_common.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/// @brief class su dung chun cho cac encoder
class Module::Base::Encoder : public Module::Base::Common {

public:

    /// TYPEDEF ___________________________________________________________________
    RotationDirectionEnum   direction;
    uint32_t                countMax;

    /// OPERATOR _______________________________________________________________

    /** @brief operator constructor */
    Encoder()
    {
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

        stateCommon                 = ENCODER_STATE_IDLE;
    }

    /** @breif operator copy constructor */
    Encoder(const Encoder &c)
    {
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
    }

    /** @brief operation delete */
    virtual ~Encoder()
    {
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

        stateCommon                 = ENCODER_STATE_IDLE;
    }

    /** @brief operator assignment */
    Encoder &operator = (const Encoder &c);

    /** @brief operator add function encoder data ready callback
     * */
    virtual Encoder &operator += (const FuncEncoderDataReadyCallbackType func) {
        funcDataReadyCallback = func;

        if(funcDataReadyCallbackArg == 0)
        {
            funcDataReadyCallbackArg = (EncoderDataReadyType*)g_calloc(sizeof(EncoderDataReadyType));
        }

        funcDataReadyCallbackArg->sender = (uint32_t)sender;

        return *this;
    }

    /** @brief operator add function reconnect callback */
    virtual Encoder &operator += (const FuncEncoderReconnectCallbackType func){
        funcReconnectCallback = func;

        if(funcReconnectCallbackArg == 0)
        {
            funcReconnectCallbackArg = (EncoderReconnectType*)g_calloc(sizeof(EncoderReconnectType));
        }

        funcReconnectCallbackArg->sender = (uint32_t)sender;

        return *this;
    }

    /** @brief operator add function lostconnect callback */
    virtual Encoder &operator += (const FuncEncoderLostconnectCallbackType func){
        funcLostconnectCallback = func;

        if(funcLostconnectCallbackArg == 0)
        {
            funcLostconnectCallbackArg = (EncoderLostconnectType*)g_calloc(sizeof(EncoderLostconnectType));
        }

        funcLostconnectCallbackArg->sender = (uint32_t)sender;

        return *this;
    }

    /// FUNCTION ________________________________________________________________
    /** @brief ham doc trang thai hoat dong
     *  @param none
     *  @return trang thai hoat dong
     * */
    virtual EncoderStateEnum GetState(void){return stateCommon;}

    /** @brief ham doc gia tri so dem cua encoder
     *  @param none
     *  @return so dem cua encoder
     * */
    virtual int32_t GetCount(void){return count;}

    /// PRIVATE ________________________________________________________________

protected:

    /// TYPEDEF ____________________________________________________________________

    /// PARRAM __________________________________________________________________

    /** @brief so dem cua encoder */
    int32_t                                 count;
    EncoderStateEnum                        stateCommon;

    FuncEncoderDataReadyCallbackType        funcDataReadyCallback;
    EncoderDataReadyType*                   funcDataReadyCallbackArg;

    FuncEncoderReconnectCallbackType        funcReconnectCallback;
    EncoderReconnectType*                   funcReconnectCallbackArg;

    FuncEncoderLostconnectCallbackType      funcLostconnectCallback;
    EncoderLostconnectType*                 funcLostconnectCallbackArg;

    /// OPERATOR  __________________________________________________________________

    /// FUNCTION  __________________________________________________________________
    /** @brief ham delay su dung trong cai dat
     *  @param[in] time (ms) thoi gian
     *  @return none
     * */
    static void Delay(uint32_t time)
    {
        g_hardware_delay(time);
    }

    /** @brief ham convert 2byte fifo thanh int16
        @param arg dia chi bien dua vao
        @return none
    */
    virtual int16_t U8ToI16(uint8_t* addr)
    {
        return (int16_t)((int16_t)*addr << 8) | (int16_t)*(addr + 1);
    }

private:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________

    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________

};


/* Exported functions --------------------------------------------------------*/


#endif /* __MODULES_BASE_MODULE_BASE_ENCODER_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
