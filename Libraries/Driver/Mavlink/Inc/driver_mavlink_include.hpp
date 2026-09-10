/**
 ******************************************************************************
 * @file    driver_mavlink_include.hpp
 * @author  Gremsy Team
 * @version V2.0.0
 * @date    Nov 16, 2024
 * @brief   This file contains all the functions prototypes for the driver_mavlink_include.cpp
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

#ifndef __DRIVER_MAVLINK_INC_DRIVER_MAVLINK_INCLUDE_HPP__
# define __DRIVER_MAVLINK_INC_DRIVER_MAVLINK_INCLUDE_HPP__

/* Includes ------------------------------------------------------------------*/

# include "main.h"

# if (CONFIG_MAVLINK == 1)

//#  include "common.hpp"
//#  include "hardware_v2.hpp"
//#  include "rtos_v2.hpp"
#include "gremsy.hpp"
#include "gremsy_include.hpp"
#include "Hardware/hardware.hpp"
#include "Hardware/hardware_include.hpp"

#ifndef SG14_INPLACE_FUNCTION_THROW
#  define SG14_INPLACE_FUNCTION_THROW(x) ((void)x)
#endif

#  include "inplace_function.h"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

// Include mavlink header lastly
#  include "ThirdParty/MavlinkV2/all/mavlink.h"

/* Exported class ------------------------------------------------------------*/

namespace Driver {
    namespace Mavlink {

        enum ComponentIndexEnum : uint8_t {
            MAV_COMP_0,
            MAV_COMP_1,
			MAV_COMP_2,
			MAV_COMP_3,
            MAV_COMP_COUNT,
            MAV_COMP_ALL,
        };

        class Component;
        class COMPortBase;
        class Manager;

        class SerialCOMPort;
        class UdpCOMPort;
        class UsbCOMPort;

        constexpr uint8_t NUM_COM_PORTS = 5;
    }  // namespace Mavlink
}  // namespace Driver

/* Exported functions --------------------------------------------------------*/

extern "C" {

void _mavlink_send_uart(mavlink_channel_t chan, const char *buf, uint16_t len);
void _mavlink_resend_uart(mavlink_channel_t chan, const mavlink_message_t *msg);

} /* extern "C" */

# endif /* CONFIG_MAVLINK */

#endif /* __DRIVER_MAVLINK_INC_DRIVER_MAVLINK_INCLUDE_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
