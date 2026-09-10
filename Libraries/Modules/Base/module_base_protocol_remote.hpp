 
/** 
  ******************************************************************************
  * @file    module_base_protocol_remote.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    May 17, 2023
  * @brief   This file contains all the functions prototypes for the module_base_protocol_remote.cpp 
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

#ifndef __MODULES_BASE_MODULE_BASE_PROTOCOL_REMOTE_HPP__
#define __MODULES_BASE_MODULE_BASE_PROTOCOL_REMOTE_HPP__

/* Includes ------------------------------------------------------------------*/

#include "module_base_common.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/// @brief dinh nghia class cho cac remote
class Module::Base::ProtocolRemote : public Module::Base::Common {

public:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________

    ProtocolRemoteModeEnum              mode;               /// kieu hoat dong cua remote

    /** Readonly */
    ///
    bool                                isFailsafe;
    bool                                isFramelost;

    /// Data
    uint16_t*                           txChannels;
    uint16_t                            txChannelCount;
    uint16_t                            txChannelValueMax;
    uint16_t                            txChannelValueMid;
    uint16_t                            txChannelValueMin;
    uint16_t                            txChannelValueRange;

    uint16_t*                           rxChannels;
    uint16_t                            rxChannelCount;
    uint16_t                            rxChannelValueMax;
    uint16_t                            rxChannelValueMid;
    uint16_t                            rxChannelValueMin;
    uint16_t                            rxChannelValueRange;

    /// OPERATOR _______________________________________________________________

    /** @brief operator constructor */
    ProtocolRemote()
    {
        mode                        = PROTOCOL_REMOTE_MODE_NONE;
        isFailsafe                  = false;
        isFramelost                 = false;

        txChannelCount              = 0;
        txChannels                  = NULL;
        txChannelValueMax           = 0;
        txChannelValueMid           = 0;
        txChannelValueMin           = 0;
        txChannelValueRange         = 0;

        rxChannelCount              = 0;
        rxChannels                  = NULL;
        rxChannelValueMax           = 0;
        rxChannelValueMid           = 0;
        rxChannelValueMin           = 0;
        rxChannelValueRange         = 0;

        rxStateCommon               = PROTOCOL_REMOTE_STATE_IDLE;
        txStateCommon               = PROTOCOL_REMOTE_STATE_IDLE;

        funcWriteCallback           = NULL;
        funcWriteCallbackArg        = NULL;

        funcErrorCallback           = NULL;
        funcErrorCallbackArg        = NULL;

        funcDataReadyCallback       = NULL;
        funcDataReadyCallbackArg    = NULL;
    }

    /** @breif operator copy constructor */
    ProtocolRemote(const ProtocolRemote &c)
    {
        mode                        = c.mode;
        isFailsafe                  = c.isFailsafe;
        isFramelost                 = c.isFramelost;

        txChannelCount              = c.txChannelCount;
        txChannels                  = c.txChannels;
        txChannelValueMax           = c.txChannelValueMax;
        txChannelValueMid           = c.txChannelValueMid;
        txChannelValueMin           = c.txChannelValueMin;
        txChannelValueRange         = c.txChannelValueRange;

        rxChannelCount              = c.rxChannelCount;
        rxChannels                  = c.rxChannels;
        rxChannelValueMax           = c.rxChannelValueMax;
        rxChannelValueMid           = c.rxChannelValueMid;
        rxChannelValueMin           = c.rxChannelValueMin;
        rxChannelValueRange         = c.rxChannelValueRange;

        rxStateCommon               = c.rxStateCommon;
        txStateCommon               = c.txStateCommon;

        funcWriteCallback           = c.funcWriteCallback;
        funcWriteCallbackArg        = c.funcWriteCallbackArg;

        funcErrorCallback           = c.funcErrorCallback;
        funcErrorCallbackArg        = c.funcErrorCallbackArg;

        funcDataReadyCallback       = c.funcDataReadyCallback;
        funcDataReadyCallbackArg    = c.funcDataReadyCallbackArg;
    }

    /** @brief operation delete */
    virtual ~ProtocolRemote()
    {
        mode                        = PROTOCOL_REMOTE_MODE_NONE;
        isFailsafe                  = false;
        isFramelost                 = false;

        txChannelCount              = 0;
        txChannels                  = NULL;
        txChannelValueMax           = 0;
        txChannelValueMid           = 0;
        txChannelValueMin           = 0;
        txChannelValueRange         = 0;

        rxChannelCount              = 0;
        rxChannels                  = NULL;
        rxChannelValueMax           = 0;
        rxChannelValueMid           = 0;
        rxChannelValueMin           = 0;
        rxChannelValueRange         = 0;

        rxStateCommon               = PROTOCOL_REMOTE_STATE_IDLE;
        txStateCommon               = PROTOCOL_REMOTE_STATE_IDLE;

        funcWriteCallback           = NULL;
        funcWriteCallbackArg        = NULL;

        funcErrorCallback           = NULL;
        funcErrorCallbackArg        = NULL;

        funcDataReadyCallback       = NULL;
        funcDataReadyCallbackArg    = NULL;
    }

    /** @brief operator assignment */
    ProtocolRemote &operator = (const ProtocolRemote &c);

    /** @brief Operation add function write callback
     *  @param[in] func function address
     *  @return class remote
     * */
    virtual ProtocolRemote &operator += (FuncProtocolRemoteWriteCallbackType func){
        funcWriteCallback = func;

        if(funcErrorCallbackArg == 0)
        {
            funcWriteCallbackArg = (ProtocolRemoteWriteType*)g_calloc(sizeof(ProtocolRemoteWriteType));
        }

        funcWriteCallbackArg->sender = (uint32_t)sender;

        return *this;
    }

    /** @brief Operation add function error callback
     *  @param[in] func function address
     *  @return class remote
     * */
    virtual ProtocolRemote &operator += (FuncProtocolRemoteErrorCallbackType func){
        funcErrorCallback = func;

        if(funcErrorCallbackArg == 0)
        {
            funcErrorCallbackArg = (ProtocolRemoteErrorType*)g_calloc(sizeof(ProtocolRemoteErrorType));
        }

        funcErrorCallbackArg->sender = (uint32_t)sender;

        return *this;
    }

    /** @brief operatio add function data ready callback
     *  @param[in] func function address
     *  @return class remote
     * */
    virtual ProtocolRemote &operator += (FuncProtocolRemoteDataReadyCallbackType func){
        funcDataReadyCallback = func;

        if(funcDataReadyCallbackArg == 0)
        {
            funcDataReadyCallbackArg = (ProtocolRemoteDataReadyType*)g_calloc(sizeof(ProtocolRemoteDataReadyType));
        }

        funcDataReadyCallbackArg->sender = (uint32_t)sender;

        return *this;
    }

    /// FUNCTION _______________________________________________________________

    /** @brief ham doc trang thai cua protocol remote sbus
     *  @param none
     *  @return trang thai cua qua trinh remote
     * */
    virtual ProtocolRemoteStateEnum GetRxState(void){return rxStateCommon;}
    virtual ProtocolRemoteStateEnum GetTxState(void){return txStateCommon;}

    /** @brief ham truyen gia tri doc duoc vao protocol remote sbus
     *  @param[in] c ky tu doc duoc
     *  @return trang thai qua trinh doc
     * */
    virtual StatusEnum ReceivedByte(uint8_t c){return ERROR_NOT_EXIST;};



protected:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________

    ProtocolRemoteStateEnum                 rxStateCommon;
    ProtocolRemoteStateEnum                 txStateCommon;

    FuncProtocolRemoteWriteCallbackType     funcWriteCallback;
    ProtocolRemoteWriteType*                funcWriteCallbackArg;

    FuncProtocolRemoteErrorCallbackType     funcErrorCallback;
    ProtocolRemoteErrorType*                funcErrorCallbackArg;

    FuncProtocolRemoteDataReadyCallbackType funcDataReadyCallback;
    ProtocolRemoteDataReadyType*            funcDataReadyCallbackArg;


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


private:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


};

/* Exported functions --------------------------------------------------------*/

#endif /* __MODULES_BASE_MODULE_BASE_PROTOCOL_REMOTE_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
