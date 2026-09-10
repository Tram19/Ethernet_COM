/**
 ******************************************************************************
 * @file    driver_mavlink_serial_com_port.cpp
 * @author  Gremsy Team
 * @version V2.0.0
 * @date    Nov 22, 2024
 * @brief
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
/* Includes ------------------------------------------------------------------*/

#include "Driver/Mavlink/Inc/driver_mavlink_serial_com_port.hpp"

#if (CONFIG_MAVLINK == 1)

/* Private typedef -----------------------------------------------------------*/

using namespace Driver::Mavlink;

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
 * @brief Open the COM port
 *
 * @return StatusEnum
 */
StatusEnum SerialCOMPort::Open() {
    if (isOpen) {
        LOGE << ("Trying to open an already open COM port");
        return ERROR_HAL;
    }

    if (!resourceInitialized) {
        LOGE << ("Trying to open an uninitialized COM port");
        return ERROR_HAL;
    }

    if (COMPortBase::Open() != OK) {
        return ERROR_HAL;
    }


    uart += [this](const uint8_t* data, uint16_t size) -> void {
        if (size > 0) {
            this->ReadDataCallback(std::span<const uint8_t>(data, size));
        }
    };

    uart.Open();

    isOpen = true;
    return OK;
}

/**
 * @brief Close the COM port
 *
 * @return StatusEnum
 */
StatusEnum SerialCOMPort::Close() {
    if (!isOpen) {
        LOGE << ("Trying to close an already closed COM port");
        return ERROR_HAL;
    }

    uart.Close();

    COMPortBase::Close();

    isOpen = false;
    return OK;
}

/**
 * @brief Send raw buffer data to low level port without packing msg
 *
 * @param buff
 * @return true
 * @return false
 */
bool SerialCOMPort::SinkIt(std::span<const uint8_t> buff) { return (uart.Write(buff) == OK); }

///**
// * @brief Read data in serial port
// *
// */
//void SerialCOMPort::ReadPort() {
//    size_t readLen = uart.Read({rxBuffer.data(), rxBuffer.capacity()});
//
//    if (readLen > 0) {
//        ReadDataCallback({rxBuffer.data(), readLen});
//    }
//}


#endif /* (CONFIG_MAVLINK == 1) */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
