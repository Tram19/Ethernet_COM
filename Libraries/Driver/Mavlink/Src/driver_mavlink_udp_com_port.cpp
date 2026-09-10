/**
 ******************************************************************************
 * @file    driver_mavlink_udp_com_port.cpp
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
#include <gremsy_include.hpp>

#ifdef GREMSY_MODULE_ETHERNET

/* Includes ------------------------------------------------------------------*/

#include "Driver/Mavlink/Inc/driver_mavlink_udp_com_port.hpp"

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

StatusEnum UdpCOMPort::Open() {
    if (isOpen) {
        LOGE << ("Trying to open an already open COM port")<<LEND;
        return ERROR_HAL;
    }

    if (!resourceInitialized) {
        LOGE << ("Trying to open an uninitialized COM port")<<LEND;
        return ERROR_HAL;
    }

    if (COMPortBase::Open() != OK) {
    	LOGE << ("Trying to open Com port base fail")<<LEND;
        return ERROR_HAL;
    }

    // PRIMARY: Signal task từ interrupt (< 1µs)
    if (!eth_.RegisterUdpReceiver(cfg_.destPort, this)){
    	LOGE << "Register UDP Receiver Fail"<<LEND;
    }

    isOpen = true;
    return OK;
}

/**
 * @brief Close the COM port
 *
 * @return StatusEnum
 */
StatusEnum UdpCOMPort::Close() {
    if (!isOpen) {
        LOGE << ("Trying to close an already closed COM port");
        return ERROR_HAL;
    }
    eth_.UnregisterUdpReceiver(cfg_.destPort);

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
bool UdpCOMPort::SinkIt(std::span<const uint8_t> buff) 
{
    return (eth_.SendUdp(cfg_.destIp, cfg_.destPort, cfg_.udpmode, buff.data(), buff.size()) == OK);
}

void UdpCOMPort::OnUdpReceived(const uint8_t* data, uint16_t len) 
{
    if (!isOpen) return;
    ReadDataCallback(std::span<const uint8_t>(data, len));
}


#endif /* (CONFIG_MAVLINK == 1) */

#endif // GREMSY_MODULE_ETHERNET

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
