/**
 ******************************************************************************
 * @file    driver_mavlink_serial_com_port.hpp
 * @author  Gremsy Team
 * @version V2.0.0
 * @date    Nov 22, 2024
 * @brief   This file contains all the functions prototypes for the driver_mavlink_serial_com_port.cpp
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

#ifndef __DRIVER_MAVLINK_INC_DRIVER_MAVLINK_UDP_COM_PORT_HPP__
# define __DRIVER_MAVLINK_INC_DRIVER_MAVLINK_UDP_COM_PORT_HPP__

#include <gremsy_include.hpp>

#ifdef GREMSY_MODULE_ETHERNET

/* Includes ------------------------------------------------------------------*/

# include "driver_mavlink_com_port.hpp"
#include "Modules/Ethernet_COM/IUdpReceiver.hpp"
#include "Modules/Ethernet_COM/Ethernet_COM.hpp"
# if (CONFIG_MAVLINK == 1)


/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

class Driver::Mavlink::UdpCOMPort : public COMPortBase,  public IUdpReceiver   {
public:

    /**
     * @brief Constructor for UdpCOMPort
     *
     * @param[in] _chan mavlink_channel_t, the channel number
     * @param[in] _sock HWv2::Peripheral::Socket&, the socket object
     * @param[in] _bufferSize size_t, the size of rx buffer (default = 256)
     * @param[in] _priority PriorityEnum, the priority of the task (default = PRIORITY_HIGH)
     * @param[in] _rate Unit::Frequency::Hz, the rate of the task (default = 500_Hz)
     *
     * @details
     * This constructor creates a periodic dynamic task for reading the serial port and a buffer
     * with the given size. The task will be created with the given priority and rate.
     * If the task creation or buffer allocation fails, the task will be reset and the rxBuffer
     * will be invalidated.
     */

    struct Config {
        uint8_t  destIp[4] = {};    // fixed IP gửi đi
        uint16_t destPort = 14550; //SERVER mode: cfg_.destPort = local listening port của board, còn reply uses peerPort
                                   //CLIENT mode: cfg_.destPort = remote destination port thật sự, vì board gửi thẳng tới đó
        UdpMode  udpmode = UdpMode::SERVER;
    };


    UdpCOMPort(mavlink_channel_t chan,
               Ethernet_COM& eth)
        : COMPortBase(chan), eth_(eth)
    {
        resourceInitialized = true;
    }

    ~UdpCOMPort() override { Close(); }

    /**
     * @brief Open the COM port
     *
     * @return StatusEnum
     */
    StatusEnum Open() override;

    /**
     * @brief Close the COM port
     *
     * @return StatusEnum
     */
    StatusEnum Close() override;

    /**
     * @brief Send raw buffer data to low level port without packing msg
     *
     * @param buff
     * @return true
     * @return false
     */
    bool SinkIt(std::span<const uint8_t> buff) override;

    void OnUdpReceived(const uint8_t* data, uint16_t len) override;

    /**
     * @brief Set the destination IP address for the UDP communication
     * @param ip The destination IP address
     */
    void SetDestIp  (const uint8_t* ip)
    {
        memcpy(cfg_.destIp, ip, 4);
    }
    /**
     * @brief Set the listening port(server mode) or destination port(client mode) of sleepmode board for each UdpCOMPort instance.
     * @param port The listening port (server mode) or destination port (client mode) of the sleepmode board.
     * @note The drone or other device should send UDP packets to this port on the board's IP address.
     */
    void SetDestPort(uint16_t port)
    {
        if (isOpen) {
            eth_.UnregisterUdpReceiver(cfg_.destPort);
            cfg_.destPort = port;
            eth_.RegisterUdpReceiver(cfg_.destPort, this);
        }
        else {
            cfg_.destPort = port;
        }
    }
    /**
     * @brief Set the UDP mode for the communication
     * @param mode The UDP mode (SERVER or CLIENT)
     */
    void SetUdpMode(uint8_t mode) { cfg_.udpmode = (UdpMode)mode; }

private:

    Ethernet_COM& eth_;   // reference, không own
    Config                 cfg_;

    RTOS::TaskEvent task;
    uint32_t missedInterrupts = 0;
    bool resourceInitialized = false;
    bool isOpen              = false;
};

/* Exported functions --------------------------------------------------------*/

# endif /* (CONFIG_MAVLINK == 1) */

#endif // GREMSY_MODULE_ETHERNET
#endif /* __DRIVER_MAVLINK_INC_DRIVER_MAVLINK_UDP_COM_PORT_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
