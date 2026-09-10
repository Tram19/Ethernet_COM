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

#ifndef __DRIVER_MAVLINK_INC_DRIVER_MAVLINK_SERIAL_COM_PORT_HPP__
# define __DRIVER_MAVLINK_INC_DRIVER_MAVLINK_SERIAL_COM_PORT_HPP__

/* Includes ------------------------------------------------------------------*/

# include "driver_mavlink_com_port.hpp"

# if (CONFIG_MAVLINK == 1)


/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

class Driver::Mavlink::SerialCOMPort : public COMPortBase {
public:

    /**
     * @brief Constructor for SerialCOMPort
     *
     * @param[in] _chan mavlink_channel_t, the channel number
     * @param[in] _uart HWv2::Peripheral::Uart&, the uart object
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
    SerialCOMPort(mavlink_channel_t _chan, HW::Peripheral::Uart &_uart)
        : COMPortBase(_chan),
          uart(_uart) {

        resourceInitialized = true;
    }

    ~SerialCOMPort() override { Close(); }

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

    // /**
    //  * @brief Set baudrate of the com port, only work for serial
    //  *
    //  * @param baudrate
    //  * @return true
    //  * @return false
    //  */
    // virtual bool SetBaudrate(HW::UartBaudRateEnum baudrate) { return false; }

private:

    /**
     * @brief Read data in serial port
     *
     */
    void ReadPort();

    HW::Peripheral::Uart &uart;

    bool resourceInitialized = false;
    bool isOpen              = false;
};

/* Exported functions --------------------------------------------------------*/

# endif /* (CONFIG_MAVLINK == 1) */

#endif /* __DRIVER_MAVLINK_INC_DRIVER_MAVLINK_SERIAL_COM_PORT_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
