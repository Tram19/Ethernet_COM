/**
 ******************************************************************************
 * @file    driver_mavlink_com_port.hpp
 * @author  Gremsy Team
 * @version V2.0.0
 * @date    Nov 18, 2024
 * @brief   This file contains all the functions prototypes for the driver_mavlink_com_port.cpp
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

#ifndef __DRIVER_MAVLINK_INC_DRIVER_MAVLINK_COM_PORT_HPP__
# define __DRIVER_MAVLINK_INC_DRIVER_MAVLINK_COM_PORT_HPP__

/* Includes ------------------------------------------------------------------*/

# include "driver_mavlink_include.hpp"
#include "Driver/ConnectionBase.hpp"
# if (CONFIG_MAVLINK == 1)

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

class Driver::Mavlink::COMPortBase : public Connection {
public:


protected:

    COMPortBase(mavlink_channel_t _chan) : chan(_chan) {}

    virtual ~COMPortBase() { Close(); }

    /**
     * @brief Open the COM port
     *
     * @return StatusEnum
     */
    virtual StatusEnum Open();

    /**
     * @brief Close the COM port
     *
     * @return StatusEnum
     */
    virtual StatusEnum Close();

    /**
     * @brief Read data callback, must be called from the implementation of the port after read data from low level
     *
     * @param data
     */
    void ReadDataCallback(std::span<const uint8_t> data) override;

private:

    mavlink_channel_t chan;
    mavlink_message_t rxMsg;
    mavlink_status_t  rxStatus;
};

/* Exported functions --------------------------------------------------------*/

# endif /* CONFIG_MAVLINK */

#endif /* __DRIVER_MAVLINK_INC_DRIVER_MAVLINK_COM_PORT_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
