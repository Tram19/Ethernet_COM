
/**
 ******************************************************************************
 * @file    hardware_include.hpp
 * @author  Gremsy Team
 * @version V2.0.0
 * @date    Apr 11, 2023
 * @brief   This file contains all the functions prototypes for the hardware_include.cpp
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

#ifndef __HARDWARE_HARDWARE_INCLUDE_HPP__
#define __HARDWARE_HARDWARE_INCLUDE_HPP__

/* Includes ------------------------------------------------------------------*/

#include "RTOS/rtos.hpp"
#include <concepts>
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

///
namespace HW
{
    /// TYPE -------------------------------------------------------------------
    /// COMMANDLINE ____________________________________________________________
    /// @brief Ham tra ve khong doi so
    typedef struct
    {
        uint32_t    sender;
    }
    CommandLineNoReturnType;
    typedef StatusEnum (*FuncCommandLineNoReturnCallbackType)(CommandLineNoReturnType* handle);

    /// @brief Ham tra ve co doi so dang int
    typedef struct
    {
        uint32_t    sender;
        int         number;
    }
    CommandLineReturnNumberType;
    typedef StatusEnum (*FuncCommandLineReturnNumberCallbackType)(CommandLineReturnNumberType* handle);

    /// @brief Ham tra ve co doi so dang chuoi
    typedef struct
    {
        uint32_t    sender;
        int         len;
    }
    CommandLineReturnStringType;
    typedef StatusEnum (*FuncCommandLineReturnStringCallbackType)(CommandLineReturnStringType* handle, const char* str);

    /// UART ___________________________________________________________________
    /** @brief dinh nghia ham tra ve khi peripheral loi */
    typedef struct
    {
        uint32_t    sender;
        uint32_t    code;
    }
    UartErrorType;
    typedef StatusEnum (*FuncPeriperalUartErrorCallbackType)(UartErrorType* handle);

    /** @brief dinh nghia ham tra ve khi nhan duoc data dang array tu ngoai vi */
    typedef struct
    {
        uint32_t    sender;
        uint32_t    id;
        int         dataSize;
    }
    UartPacketType;
    typedef StatusEnum (*FuncPeriperalUartPacketDataReadyCallbackType)(UartPacketType* handle, uint8_t* data);

    /** @brief dinh nghia ham tra ve khi nhan duoc data theo dang byte tu ngoai vi*/
    typedef struct
    {
        uint32_t    sender;
        uint32_t    id;
        uint8_t     c;
    }
    UartByteType;
    typedef StatusEnum (*FuncPeripheralUartByteDataReadyCallbackType)(UartByteType* handle);

    //
    using ReceivedPacketCallback = void(const uint8_t * data, uint16_t size);

    using LambdaReceivedPacketCallback = stdext::inplace_function<ReceivedPacketCallback>;

    /// PWMIN __________________________________________________________________
    /** @brief dinh nghia ham tra ve khi nhan pwm input ready
     * */
    typedef struct
    {
        uint32_t sender;

        uint32_t timeFreq;
        uint32_t timeDuty;
    }
    PwmInDataType;
    typedef StatusEnum (*FuncPeripheralPwmInDataReadyCallbackType)(PwmInDataType* handle);

    /// SPI ____________________________________________________________________
    /** @brief dinh nghia ham tra ve khi peripheral loi */
    typedef struct
    {
        uint32_t    sender;
        uint32_t    code;
    }
    SpiErrorType;
    typedef StatusEnum (*FuncPeriperalSpiErrorCallbackType)(SpiErrorType* handle);

    /** @brief dinh nghia ham truyen cho tung goi cua spi
     * */
    typedef struct
    {
        uint32_t        sender;
        uint32_t        time;       /// (Us) thoi gian giua cac lan truyen
    }
    SpiWriteType;
    typedef StatusEnum (*FuncPeripheralSpiWriteCallbackType)(SpiWriteType* handle);

    typedef struct
    {
        Type::String                        name;
        uint32_t                            func;
        SpiWriteType                        funcArg;
        uint32_t                            __timePrivate;
    }
    SpiElementWriteType;

    /// EXTI ___________________________________________________________________
    typedef struct
    {
        uint32_t    sender;
        bool        level;
        uint32_t    timeDuty;
        uint32_t    timeFrequency;
    }
    ExtiDataType;
    typedef StatusEnum (*FuncPeripheralExtiDataReadyCallbackType)(ExtiDataType* handle);

    /// CANBUS _________________________________________________________________
    /** @brief dinh nghia ham tra ve khi peripheral loi */
    typedef struct
    {
        uint32_t   sender;
        uint32_t    code;
    }
    CanbusErrorType;
    typedef StatusEnum (*FuncPeripheralCanbusErrorCallbackType)(CanbusErrorType* handle);

    /** @brief dinh nghia ham tra ve khi nhan duoc data dang array tu ngoai vi */
    typedef struct
    {
        uint32_t    sender;
        uint8_t     fifoNum;
        uint32_t    msgId;
        int         dataSize;
    }
    CanbusPacketType;
    typedef StatusEnum (*FuncPeripheralCanbusPacketDataReadyCallbackType)(CanbusPacketType* handle, uint8_t* data);

    /** @brief dinh nghia ham truyen thanh cong khi nhan du lieu */
    typedef struct
    {
        uint32_t    sender;
        uint8_t     mailboxNum;
        uint32_t    msgId;
    }
    CanbusTxCpltType;
    typedef StatusEnum (*FuncPeripheralCanbusTxCpltCallbackType)(CanbusTxCpltType* handle);

    /// BASE -------------------------------------------------------------------
    namespace Base
    {
        class Common;
        class Log;
        class CommandLine;

        class Adc;
        class Exti;
        class Output;
        class Input;
        class Time;
        class Uart;
        class PwmOut;
        class PwmIn;
        class Spi;
        class Canbus;
        class Flash;
        class I2c;
        class IO;
    }

    /// DEBUG ------------------------------------------------------------------
    namespace Debug
    {
        /// TYPEDEF


        ///
        class Log;
        class CommandLine;
    }

    /// PERIPHERAL -------------------------------------------------------------
    namespace Peripheral
    {
        /// TYPEDEF

        /// CLASS
        class Adc;
        class Exti;
        class Output;
        class Input;
        class Time;
        class Uart;
        class PwmOut;
        class PwmIn;
        class Spi;
        class Canbus;
        class Flash;
        class I2c;


//        template <IOTypeEnum S>
//        class IO;
    }

    enum UartBaudRateEnum : uint32_t
    {
        UART_BAUD_RATE_9600   = 9600,
        UART_BAUD_RATE_19200  = 19200,
        UART_BAUD_RATE_38400  = 38400,
        UART_BAUD_RATE_57600  = 57600,
        UART_BAUD_RATE_115200 = 115200,
        UART_BAUD_RATE_230400 = 230400,
        UART_BAUD_RATE_460800 = 460800,
        UART_BAUD_RATE_921600 = 921600,
    };

}

/* Exported functions --------------------------------------------------------*/

#endif /* __HARDWARE_HARDWARE_INCLUDE_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
