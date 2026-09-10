/** 
  ******************************************************************************
  * @file    hardware_u5_peripheral_canbus.cpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Nov 25, 2024
  * @brief   CAN (FDCAN) Driver implementation for STM32U5.
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2024 Gremsy. All rights reserved.
  *
  ******************************************************************************
*/ 

#include "Hardware/Stm32U5/Inc/hardware_u5_peripheral_canbus.hpp"

#ifdef STM32U5

#ifdef HAL_FDCAN_MODULE_ENABLED
static HW::Peripheral::Canbus* __gp_hardware_peripheral_can[CAN_COUNT];

static HW::Peripheral::Canbus* hardware_u5_peripheral_canbus_get_pinstance(FDCAN_HandleTypeDef *hfdcan)
{
    for(int i = 0; i < CAN_COUNT; i++)
    {
        if(__gp_hardware_peripheral_can[i] != nullptr)
        {
            if(__gp_hardware_peripheral_can[i]->__handle == hfdcan) return __gp_hardware_peripheral_can[i];
        }
    }
    return nullptr;
}

void HAL_FDCAN_ErrorCallback(FDCAN_HandleTypeDef *hfdcan)
{
    HW::Peripheral::Canbus* pcan = hardware_u5_peripheral_canbus_get_pinstance(hfdcan);
    if(pcan)
    {
        pcan->__ErrorProcess();
    }
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
    if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != 0)
    {
        HW::Peripheral::Canbus* pcan = hardware_u5_peripheral_canbus_get_pinstance(hfdcan);
        if(pcan)
        {
            pcan->__ITRxFIFO0Process();
        }
    }
}

void HAL_FDCAN_RxFifo1Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo1ITs)
{
    if((RxFifo1ITs & FDCAN_IT_RX_FIFO1_NEW_MESSAGE) != 0)
    {
        HW::Peripheral::Canbus* pcan = hardware_u5_peripheral_canbus_get_pinstance(hfdcan);
        if(pcan)
        {
            pcan->__ITRxFIFO1Process();
        }
    }
}

void HAL_FDCAN_TxFifoEmptyCallback(FDCAN_HandleTypeDef *hfdcan)
{
    HW::Peripheral::Canbus* pcan = hardware_u5_peripheral_canbus_get_pinstance(hfdcan);
    if(pcan)
    {
        pcan->__ITTxCpltProcess();
    }
}

void HW::Peripheral::Canbus::__ErrorProcess(void)
{
    if(funcErrorCallback)
    {
        ptrFuncErrorCallbackArg->code = __handle->ErrorCode;
        ptrFuncErrorCallbackArg->sender = sender;
        funcErrorCallback(ptrFuncErrorCallbackArg);
    }
}

void HW::Peripheral::Canbus::__ITRxFIFO0Process(void)
{
    if (HAL_FDCAN_GetRxMessage(__handle, FDCAN_RX_FIFO0, &rxMsg0, rxData0) == HAL_OK)
    {
        if(funcPacketDataReadyCallback)
        {
            ptrFuncPacketDataReadyCallback->fifoNum     = 0;
            ptrFuncPacketDataReadyCallback->msgId       = rxMsg0.Identifier;
            ptrFuncPacketDataReadyCallback->data        = rxData0;
            ptrFuncPacketDataReadyCallback->dataSize    = rxMsg0.DataLength >> 16;
            ptrFuncPacketDataReadyCallback->sender      = sender;
            funcPacketDataReadyCallback(ptrFuncPacketDataReadyCallback);
        }
    }
}

void HW::Peripheral::Canbus::__ITRxFIFO1Process(void)
{
    if (HAL_FDCAN_GetRxMessage(__handle, FDCAN_RX_FIFO1, &rxMsg1, rxData1) == HAL_OK)
    {
        if(funcPacketDataReadyCallback)
        {
            ptrFuncPacketDataReadyCallback->fifoNum     = 1;
            ptrFuncPacketDataReadyCallback->msgId       = rxMsg1.Identifier;
            ptrFuncPacketDataReadyCallback->data        = rxData1;
            ptrFuncPacketDataReadyCallback->dataSize    = rxMsg1.DataLength >> 16;
            ptrFuncPacketDataReadyCallback->sender      = sender;
            funcPacketDataReadyCallback(ptrFuncPacketDataReadyCallback);
        }
    }
}

void HW::Peripheral::Canbus::__ITTxCpltProcess(void)
{
    if(funcTxCpltCallback)
    {
        ptrFuncTxCpltCallbackArg->mailboxNum    = 0;
        ptrFuncTxCpltCallbackArg->msgId         = txMsg.Identifier;
        ptrFuncTxCpltCallbackArg->sender        = sender;
        funcTxCpltCallback(ptrFuncTxCpltCallbackArg);
    }
}

#endif

StatusEnum HW::Peripheral::Canbus::Write(uint32_t msgId, uint8_t* buff, uint8_t len)
{
#ifdef HAL_FDCAN_MODULE_ENABLED
    if(len <= 8 && len > 0)
    {
        txMsg.Identifier = msgId;
        txMsg.DataLength = (uint32_t)len << 16;
        if(HAL_FDCAN_AddMessageToTxFifoQ(__handle, &txMsg, buff) == HAL_OK)
        {
            return OK;
        }
        return ERROR_HAL;
    }
#endif
    return ERROR_NOT_EXIST;
}

StatusEnum HW::Peripheral::Canbus::Open(void)
{
#ifdef HAL_FDCAN_MODULE_ENABLED
    StatusEnum ret = OK;

    if(!IS_CANBUS_ID(id))
    {
        return ERROR_NOT_EXIST;
    }

    if(isOpen == true)
    {
        return ERROR_EXIST;
    }

    const CanbusConfigType* pcanConfig = &HARDWARE_CAN_CONFIG[id];
    __handle = pcanConfig->handle;

    HAL_FDCAN_DeInit(__handle);

    ret = (StatusEnum)HAL_FDCAN_Init(__handle);
    if(ret != OK)
    {
        return ret;
    }

    s0FilterConfig.IdType = FDCAN_STANDARD_ID;
    s0FilterConfig.FilterIndex = 0;
    s0FilterConfig.FilterType = FDCAN_FILTER_DUAL;
    s0FilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
    s0FilterConfig.FilterID1 = stdId1;
    s0FilterConfig.FilterID2 = stdId2;
    HAL_FDCAN_ConfigFilter(__handle, &s0FilterConfig);

    s1FilterConfig.IdType = FDCAN_STANDARD_ID;
    s1FilterConfig.FilterIndex = 1;
    s1FilterConfig.FilterType = FDCAN_FILTER_DUAL;
    s1FilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO1;
    s1FilterConfig.FilterID1 = stdId3;
    s1FilterConfig.FilterID2 = stdId4;
    HAL_FDCAN_ConfigFilter(__handle, &s1FilterConfig);

    txPin = g_hardware_convert_gpio_to_pin_enum(pcanConfig->txPort, pcanConfig->txPin);
    rxPin = g_hardware_convert_gpio_to_pin_enum(pcanConfig->rxPort, pcanConfig->rxPin);

    txMsg.IdType = FDCAN_STANDARD_ID;
    txMsg.TxFrameType = FDCAN_DATA_FRAME;
    txMsg.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    txMsg.BitRateSwitch = FDCAN_BRS_OFF;
    txMsg.FDFormat = FDCAN_CLASSIC_CAN;
    txMsg.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    txMsg.MessageMarker = 0;

    if(HAL_FDCAN_ActivateNotification(__handle, FDCAN_IT_RX_FIFO0_NEW_MESSAGE | FDCAN_IT_RX_FIFO1_NEW_MESSAGE, 0) != HAL_OK)
    {
        return ERROR_HAL;
    }

    if(HAL_FDCAN_Start(__handle) != HAL_OK)
    {
        return ERROR_HAL;
    }

    isOpen = true;
    __gp_hardware_peripheral_can[id] = this;

    return ret;
#endif
    return ERROR_NOT_EXIST;
}

StatusEnum HW::Peripheral::Canbus::Close(void)
{
#ifdef HAL_FDCAN_MODULE_ENABLED
    if(!IS_CANBUS_ID(id))
    {
        return ERROR_NOT_EXIST;
    }

    if(isOpen == false)
    {
        return ERROR_EXIST;
    }

    if(HAL_FDCAN_DeactivateNotification(__handle, FDCAN_IT_RX_FIFO0_NEW_MESSAGE | FDCAN_IT_RX_FIFO1_NEW_MESSAGE) != HAL_OK)
    {
        return ERROR_HAL;
    }

    if(HAL_FDCAN_Stop(__handle) != HAL_OK)
    {
        return ERROR_HAL;
    }

    if(HAL_FDCAN_DeInit(__handle) != HAL_OK)
    {
        return ERROR_HAL;
    }

    __gp_hardware_peripheral_can[id] = nullptr;
    isOpen = false;

    return OK;
#endif
    return ERROR_NOT_EXIST;
}

#endif

