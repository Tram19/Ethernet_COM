/**
 ******************************************************************************
 * @file    driver_mavlink_com_port.cpp
 * @author  Gremsy Team
 * @version V2.0.0
 * @date    Nov 19, 2024
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

#include "Driver/Mavlink/Inc/driver_mavlink_com_port.hpp"

#if (CONFIG_MAVLINK == 1)

# include "Driver/Mavlink/Inc/driver_mavlink_manager.hpp"

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
StatusEnum COMPortBase::Open() { return (Manager::GetInstance().AddCOMPort(*this, chan)); };

/**
 * @brief Close the COM port
 *
 * @return StatusEnum
 */
StatusEnum COMPortBase::Close() { return (Manager::GetInstance().RemoveCOMPort(chan)); }

/**
 * @brief Read data callback, must be called from the implementation of the port after read data from low level
 *
 * @param data
 */
void COMPortBase::ReadDataCallback(std::span<const uint8_t> data) {
    for (auto c : data) {
        if (mavlink_parse_char(chan, c, &rxMsg, &rxStatus)) {
            Manager::GetInstance().ParseMessageCallback(chan, rxMsg);
        }
    }
}

#endif /* CONFIG_MAVLINK */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
