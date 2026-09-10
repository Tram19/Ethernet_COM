 
/** 
  ******************************************************************************
  * @file    hardware_base_peripheral_uart.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Apr 14, 2023
  * @brief   This file contains all the functions prototypes for the hardware_base_peripheral_uart.cpp 
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

#ifndef __HARDWARE_BASE_INC_HARDWARE_BASE_PERIPHERAL_UART_HPP__
#define __HARDWARE_BASE_INC_HARDWARE_BASE_PERIPHERAL_UART_HPP__

/* Includes ------------------------------------------------------------------*/

#include "hardware_base_common.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/// @brief class uart su dung chung cho uart
class HW::Base::Uart : public HW::Base::Common {

public:

    /// TYPEDEF ________________________________________________________________
    Uart(UartIdEnum _id) : id(_id) , baudrate(baudrate), readTime(1), readBufferSize(1024){};
    /// PARRAM _________________________________________________________________
    UartIdEnum          id;         /// ma dinh danh cho tung uart
    PriorityEnum        priority;   /// do uu tien

    uint32_t            baudrate;   /// toc do truyen cua uart
    StopbitEnum         stopbits;
    ParityEnum          parity;
    WordlengthEnum      wordlength;

    TTLLevelInvertEnum  rxLevelInvert;  /// tinh nang chi ho tro mot so dong chip
    TTLLevelInvertEnum  txLevelInvert;  /// tinh nang chi ho tro mot so dong chip

    int                 readTime;       /// thoi gian doc
    int                 readBufferSize;/// do lon cua mang doc
    int                 readTaskSize{4096}; /// stack size cua task doc

    /** @readonly */
    PinEnum             txPin;
    PinEnum             rxPin;

    osMessageQueueId_t txQueue;
    osThreadId_t       txTaskHandle;

    /// OPERATOR _______________________________________________________________

    /** @brief operator constructor */
    Uart()
    {
        id              = UART_NONE;
        priority        = PRIORITY_NONE;
        baudrate        = 115200;
        stopbits        = STOPBITS_1;
        parity          = PARITY_NONE;
        wordlength      = WORDLENGTH_8B;
        rxLevelInvert   = TTL_LEVEL_INV_DEFAULT;
        txLevelInvert   = TTL_LEVEL_INV_DEFAULT;

        readTime        = 100;
        readBufferSize  = 100;

        txPin           = PIN_NONE;
        rxPin           = PIN_NONE;

        funcErrorCallback                   = NULL;
        ptrFuncErrorCallbackArg             = NULL;

        funcByteDataReadyCallback           = NULL;
        ptrFuncByteDataReadyCallbackArg     = NULL;

        funcPacketDataReadyCallback         = NULL;
        ptrFuncPacketDataReadyCallbackArg   = NULL;
    }

    /** @breif operator copy constructor */
    Uart(const Uart &c)
    {
        id              = c.id;
        priority        = c.priority;
        baudrate        = c.baudrate;
        stopbits        = c.stopbits;
        parity          = c.parity;
        wordlength      = c.wordlength;
        rxLevelInvert   = c.rxLevelInvert;
        txLevelInvert   = c.txLevelInvert;

        readTime        = c.readTime;
        readBufferSize  = c.readBufferSize;

        txPin           = c.txPin;
        rxPin           = c.rxPin;

        funcErrorCallback                   = c.funcErrorCallback;
        ptrFuncErrorCallbackArg             = c.ptrFuncErrorCallbackArg;

        funcByteDataReadyCallback           = c.funcByteDataReadyCallback;
        ptrFuncByteDataReadyCallbackArg     = c.ptrFuncByteDataReadyCallbackArg;

        funcPacketDataReadyCallback         = c.funcPacketDataReadyCallback;
        ptrFuncPacketDataReadyCallbackArg   = c.ptrFuncPacketDataReadyCallbackArg;
    }

    /** @brief operation delete */
    virtual ~Uart()
    {
        id              = UART_NONE;
        priority        = PRIORITY_NONE;
        baudrate        = 115200;
        stopbits        = STOPBITS_1;
        parity          = PARITY_NONE;
        wordlength      = WORDLENGTH_8B;
        rxLevelInvert   = TTL_LEVEL_INV_DEFAULT;
        txLevelInvert   = TTL_LEVEL_INV_DEFAULT;

        readTime        = 100;
        readBufferSize  = 100;

        txPin           = PIN_NONE;
        rxPin           = PIN_NONE;

        funcErrorCallback                   = NULL;
        ptrFuncErrorCallbackArg             = NULL;

        funcByteDataReadyCallback           = NULL;
        ptrFuncByteDataReadyCallbackArg     = NULL;

        funcPacketDataReadyCallback         = NULL;
        ptrFuncPacketDataReadyCallbackArg   = NULL;
    }

    /** @brief operator assignment */
    Uart &operator = (const Uart &c);

    /** @brief ham add function error
     *  @param[in] func dia chi func error callback
     *  @return dia chi uart
     * */
    virtual Uart &operator +=(const FuncPeriperalUartErrorCallbackType func){
        funcErrorCallback = func;

        if(ptrFuncErrorCallbackArg == 0)
        {
            ptrFuncErrorCallbackArg = (UartErrorType*)g_calloc(sizeof(UartErrorType));
        }

        ptrFuncErrorCallbackArg->sender = (uint32_t)sender;

        return *this;
    }

    /** @brief ham add function paket data ready
     *  @param[in] func dia chi func paket data ready
     *  @return dia chi uart
     * */
    virtual Uart &operator +=(const FuncPeriperalUartPacketDataReadyCallbackType func){
        funcPacketDataReadyCallback = func;

        if(ptrFuncPacketDataReadyCallbackArg == 0)
        {
            ptrFuncPacketDataReadyCallbackArg = (UartPacketType*)g_calloc(sizeof(UartPacketType));
        }

        ptrFuncPacketDataReadyCallbackArg->sender = (uint32_t)sender;

        return *this;
    }

    virtual Uart &operator +=(LambdaReceivedPacketCallback && func){
        lambdaPacketReadyCallback = std::move(func);
        return *this;
    }
    /** @brief ham add function byte data ready
     *  @param[in] func dia chi func byte data ready
     *  @return dia chi uart
     * */
    virtual Uart &operator +=(const FuncPeripheralUartByteDataReadyCallbackType func){
        funcByteDataReadyCallback = func;

        if(ptrFuncByteDataReadyCallbackArg == 0)
        {
            ptrFuncByteDataReadyCallbackArg = (UartByteType*)g_calloc(sizeof(UartByteType));
        }

        ptrFuncByteDataReadyCallbackArg->sender = (uint32_t)sender;

        return *this;
    }

    /// FUNCTION _______________________________________________________________

    /** @brief ham truyen gia tri
     *  @param[in] str chuoi can truyen
     *  @return trang thai qua trinh truyen
     * */
    virtual StatusEnum Write(Type::String str){return ERROR_HAL;}

    /** @brief ham truyen gia tri
     *  @param[in] buf dia chi mang truyen
     *  @param[in] len do dang mang truyen
     *  @return trang thai qua trinh truyen
     * */
    virtual StatusEnum Write(uint8_t* buff, int len){return ERROR_HAL;}

    /** @breif ham get pin tx
     * */
    virtual PinEnum GetTxPin(void){
        return txPin;
    }

    /** @breif ham get pin rx
     * */
    virtual PinEnum GetRxPin(void){
        return rxPin;
    }



protected:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________

    FuncPeriperalUartErrorCallbackType              funcErrorCallback;
    UartErrorType*                                  ptrFuncErrorCallbackArg;

    FuncPeriperalUartPacketDataReadyCallbackType    funcPacketDataReadyCallback;
    UartPacketType*                                 ptrFuncPacketDataReadyCallbackArg;

    FuncPeripheralUartByteDataReadyCallbackType     funcByteDataReadyCallback;
    UartByteType*                                   ptrFuncByteDataReadyCallbackArg;

    LambdaReceivedPacketCallback                lambdaPacketReadyCallback;

    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


private:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


};

/* Exported functions --------------------------------------------------------*/

#endif /* __HARDWARE_BASE_INC_HARDWARE_BASE_PERIPHERAL_UART_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
