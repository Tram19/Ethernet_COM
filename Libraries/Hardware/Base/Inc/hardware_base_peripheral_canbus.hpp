 
/** 
  ******************************************************************************
  * @file    hardware_base_peripheral_canbus.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Apr 25, 2023
  * @brief   This file contains all the functions prototypes for the hardware_base_peripheral_canbus.cpp 
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

#ifndef __HARDWARE_BASE_INC_HARDWARE_BASE_PERIPHERAL_CANBUS_HPP__
#define __HARDWARE_BASE_INC_HARDWARE_BASE_PERIPHERAL_CANBUS_HPP__

/* Includes ------------------------------------------------------------------*/
#include "hardware_base_common.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/// @brief class canbus dung chung
class HW::Base::Canbus : public HW::Base::Common {

public:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________

    CanbusIdEnum            id;
    uint16_t                stdId1;
    uint16_t                stdId2;
    uint16_t                stdId3;
    uint16_t                stdId4;
    //CanbusIdFilterEnum      idFilter;
    PriorityEnum            priority;

    /** readonly */
    PinEnum                 txPin;
    PinEnum                 rxPin;

    /// OPERATOR _______________________________________________________________
    /** @brief operator constructor */
    Canbus()
    {
        funcErrorCallback       = NULL;
        ptrFuncErrorCallbackArg = NULL;

        funcPacketDataReadyCallback = NULL;
        ptrFuncPacketDataReadyCallback = NULL;

        funcTxCpltCallback = NULL;
        ptrFuncTxCpltCallbackArg = NULL;

        id = CAN_NONE;
        stdId1 = 0;
        stdId2 = 0;
        stdId3 = 0;
        stdId4 = 0;

        priority = PRIORITY_NONE;
        txPin = PIN_NONE;
        rxPin = PIN_NONE;
    }

    /** @breif operator copy constructor */
    Canbus(const Canbus &c)
    {
        funcErrorCallback               = c.funcErrorCallback;
        ptrFuncErrorCallbackArg         = c.ptrFuncErrorCallbackArg;

        funcPacketDataReadyCallback     = c.funcPacketDataReadyCallback;
        ptrFuncPacketDataReadyCallback  = c.ptrFuncPacketDataReadyCallback;

        funcTxCpltCallback              = c.funcTxCpltCallback;
        ptrFuncTxCpltCallbackArg        = c.ptrFuncTxCpltCallbackArg;

        id          = c.id;
        stdId1      = c.stdId1;
        stdId2      = c.stdId2;
        stdId3      = c.stdId3;
        stdId4      = c.stdId4;

        priority    = c.priority;
        txPin       = c.txPin;
        rxPin       = c.rxPin;
    }

    /** @brief operation delete */
    virtual ~Canbus()
    {
        funcErrorCallback       = NULL;
        ptrFuncErrorCallbackArg = NULL;

        funcPacketDataReadyCallback = NULL;
        ptrFuncPacketDataReadyCallback = NULL;

        funcTxCpltCallback = NULL;
        ptrFuncTxCpltCallbackArg = NULL;

        id = CAN_NONE;
        stdId1 = 0;
        stdId2 = 0;
        stdId3 = 0;
        stdId4 = 0;

        priority = PRIORITY_NONE;
        txPin = PIN_NONE;
        rxPin = PIN_NONE;
    }

    /** @brief operator assignment */
    Canbus &operator = (const Canbus &c);

    /** @brief ham cai dat tra ve ham error */
    virtual Canbus &operator +=(const FuncPeripheralCanbusErrorCallbackType func){
        funcErrorCallback = func;

        if(ptrFuncErrorCallbackArg == 0)
        {
            ptrFuncErrorCallbackArg = (CanbusErrorType*)g_calloc(sizeof(CanbusErrorType));
        }

        ptrFuncErrorCallbackArg->sender = (uint32_t)sender;

        return *this;
    }

    /** @brief ham cai dat tra ve ham nhan packet */
    virtual Canbus &operator +=(const FuncPeripheralCanbusPacketDataReadyCallbackType func){
        funcPacketDataReadyCallback = func;

        if(ptrFuncPacketDataReadyCallback == 0)
        {
            ptrFuncPacketDataReadyCallback = (CanbusPacketType*)g_calloc(sizeof(CanbusPacketType));
        }

        ptrFuncPacketDataReadyCallback->sender = (uint32_t)sender;

        return *this;
    }

    /** @brief ham cai dat tra ve khi truyen het du lieu */
    virtual Canbus &operator +=(const FuncPeripheralCanbusTxCpltCallbackType func){
        funcTxCpltCallback = func;

        if(ptrFuncTxCpltCallbackArg == 0)
        {
            ptrFuncTxCpltCallbackArg = (CanbusTxCpltType*)g_calloc(sizeof(CanbusTxCpltType));
        }

        ptrFuncTxCpltCallbackArg->sender = (uint32_t)sender;

        return *this;
    }

    /// FUNCTION _______________________________________________________________

    /** @brief ham doc pin cua tx theo kieu PinEnum
     *  @param none
     *  @retur pin theo kieu PinEnum
     */
    virtual PinEnum GetTxPin(void){return txPin;}

    /** @brief ham doc pin cua rx theo kieu PinEnum
     *  @param none
     *  @retur pin theo kieu PinEnum
     */
    virtual PinEnum GetRxPin(void){return rxPin;}

    /** @brief ham write trong canbus
     *  @param[in] msgId id cua msg truyen
     *  @param[in] buff dia chi cua mang can truyen
     *  @param[in] buffsize do lon cua mang can truyen
     *  @return trang thai trong qua trinh truyen
     * */
    virtual StatusEnum Write(uint32_t msgId, uint8_t* buff, uint8_t buffsize){return ERROR_NOT_EXIST;}

protected:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________

    FuncPeripheralCanbusErrorCallbackType           funcErrorCallback;
    CanbusErrorType*                                ptrFuncErrorCallbackArg;

    FuncPeripheralCanbusPacketDataReadyCallbackType funcPacketDataReadyCallback;
    CanbusPacketType*                               ptrFuncPacketDataReadyCallback;

    FuncPeripheralCanbusTxCpltCallbackType          funcTxCpltCallback;
    CanbusTxCpltType*                               ptrFuncTxCpltCallbackArg;

    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


private:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


};

/* Exported functions --------------------------------------------------------*/

#endif /* __HARDWARE_BASE_INC_HARDWARE_BASE_PERIPHERAL_CANBUS_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
