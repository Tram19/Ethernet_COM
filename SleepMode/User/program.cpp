/** 
  ******************************************************************************
  * @file    program.cpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Apr 11, 2023
  * @brief   User coding
  *
  ******************************************************************************
  * @copyright
  * COPYRIGHT NOTICE: (c) 2023 Gremsy.
  * All rights reserved.
  *
  * The information contained herein is confidential
  * property of Company. The use, copying, transfer or 
  * disclosure of such information is prohibited except
  * by express written agreement with Company.
  *
  ******************************************************************************
*/ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "wwdg.h"
#include "program.hpp"
#include "gremsy.hpp"
#include "gremsy_include.hpp"
#include "SleepMode_Com/SleepMode_Com.hpp"
#include "StorageParam/StorageParam.hpp"
#include "ComPortManager/ComPortManager.hpp"
#include "MavlinkHandler/MavlinkHandler.hpp"

/* Private variables ---------------------------------------------------------*/
HW::Peripheral::Flash _flash;
SleepMode_Com         sleepmode_com;

MavlinkHandler        mavlink_handler(sleepmode_com);

enum class AppState : uint8_t {
    APPLY_CONFIG,
    OPEN_COM,
    OPEN_SLEEPMODE_MANAGER,
    OPEN_MAVLINK,
    OPEN_GLINK,
    READY,
    ERROR,
};
static AppState _app_state{AppState::APPLY_CONFIG};

void setup(void) {

}

/** @brief ham loop chay lien tuc
 * */
void loop(void) {
    switch (_app_state) {
        case AppState::APPLY_CONFIG: {
            StorageManager::GetInstance().Init(_flash);
            ComPortManager::GetInstance().ApplyNetworkConfig(StorageNetworkParam::GetInstance().GetConfig());
            _app_state = AppState::OPEN_COM;
            break;
        }
        case AppState::OPEN_COM: {
            if (ComPortManager::GetInstance().Open() != OK) {
                LOGE << "COM port manager Open Fail" << LEND;
                _app_state = AppState::ERROR;
                }
            _app_state = AppState::OPEN_SLEEPMODE_MANAGER;
            break;
        }
        case AppState::OPEN_SLEEPMODE_MANAGER: {
            if (sleepmode_com.Open() != OK) {
                LOGE << "Sleep Mode Com Open Fail" << LEND;
                _app_state = AppState::ERROR;
                }
            _app_state = AppState::OPEN_MAVLINK;
            
            break;
        }
        case AppState::OPEN_MAVLINK: {
            if (mavlink_handler.Open() != OK) {
                LOGE << "Mavlink Handler Open Fail" << LEND;
                _app_state = AppState::ERROR;
                }
            _app_state = AppState::READY;
            
            break;
        }
        // case AppState::OPEN_GLINK: {
        //     glink_handler.AttachImu(&ComPortManager::GetInstance().Imu());
        //     if (glink_handler.Open() != OK) {
        //         _app_state = AppState::ERROR;
        //     }
        //     glink_handler.InitPps(&sleepmode_com.GetIOExpander(), 6,
        //                           g_hardware_convert_gpio_to_pin_enum(GPIOB, GPIO_PIN_8));
        
        //     // Connect aircraft ACK callback from GLink to MavlinkHandler
        //     glink_handler.SetAircraftCommandAckCallback(
        //         [](void* context, uint16_t command, uint8_t result, uint8_t target_sys, uint8_t target_comp) {
        //             auto* mav = static_cast<MavlinkHandler*>(context);
        //             // Translate GLink result enum to MAVLink MAV_RESULT enum!
        //             // In GLink: IN_PROGRESS = 4, FAILED = 2, DENIED = 1, ACCEPTED = 0
        //             // In MAVLink: IN_PROGRESS = 5, FAILED = 4, DENIED = 2, ACCEPTED = 0
        //             uint8_t mav_result = MAV_RESULT_FAILED;
        //             switch (result) {
        //                 case GLINK_CMD_RESULT_ACCEPTED:
        //                     mav_result = MAV_RESULT_ACCEPTED;    // 0
        //                     break;
        //                 case GLINK_CMD_RESULT_IN_PROGRESS:
        //                     mav_result = MAV_RESULT_IN_PROGRESS; // 5 (CRITICAL FIX: was 4=FAILED in MAVLink!)
        //                     break;
        //                 case GLINK_CMD_RESULT_DENIED:
        //                     mav_result = MAV_RESULT_DENIED;      // 2
        //                     break;
        //                 case GLINK_CMD_RESULT_UNSUPPORTED:
        //                     mav_result = MAV_RESULT_UNSUPPORTED; // 3
        //                     break;
        //                 case GLINK_CMD_RESULT_FAILED:
        //                 default:
        //                     mav_result = MAV_RESULT_FAILED;      // 4
        //                     break;
        //             }
        //             mav->SendCommandAckAircraft(command, mav_result, target_sys, target_comp);
        //         },
        //         &mavlink_handler
        //     );
            
        //     _app_state = AppState::READY;
            
        //     break;
        // }
        case AppState::READY: {
            // All components are open and ready
            break;
        }
        case AppState::ERROR: {
            // Handle error state
            LOGE<<"Application error"<<LEND;
            break;
        }
    }
    g_os_delay(100);
}

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
