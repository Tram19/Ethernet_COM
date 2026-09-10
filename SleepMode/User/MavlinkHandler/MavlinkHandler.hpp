/**
  ******************************************************************************
  * @file    mavlink_com.hpp
  * @author  Gremsy Team
  * @version V1.0.0
  * @date    2024
  * @brief   MAVLink COM Manager - handles heartbeat TX, sleep command RX,
  *          and GPIO PC13 switching to power off SOM.
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2024 Gremsy. All rights reserved.
  ******************************************************************************
*/

#ifndef __MAVLINK_COM_HPP__
#define __MAVLINK_COM_HPP__

#include "main.h"
#include "SleepMode_Com/SleepMode_Com.hpp"
#include "gremsy.hpp"
#include "gremsy_include.hpp"
#include "Driver/Mavlink/Inc/driver_mavlink.hpp"
#include "Driver/Mavlink/Inc/driver_mavlink_serial_com_port.hpp"
#include "StorageParam/StorageParam.hpp"
#include "RTOS/FreeRTOS/Inc/free_rtos_mutex.hpp"
#include <span>
using namespace Driver::Mavlink;




/* Exported macro ------------------------------------------------------------*/
#define SOM_SLEEP_CMD_FLAG      0x01
#define SOM_HB_FLAG             0x02
#define SOM_ON_CMD_FLAG			0x04
#define SOM_SWITCH_GPIO_PORT    GPIOC
#define SOM_SWITCH_GPIO_PIN     GPIO_PIN_13

/* Exported class ------------------------------------------------------------*/

/// @brief MAVLink COM - Gửi heartbeat, nhận lệnh sleep từ FC, điều khiển GPIO tắt SOM
class MavlinkHandler : public Driver::Mavlink::Component
{
public:

    //Component id______________________________________________________________
        explicit MavlinkHandler(SleepMode_Com& sleep_mode)
                : Component(_own_comp_id, msgCallbacks, periodicSendMsgs),
                    _sleep_mode(sleep_mode) {}
    MavlinkHandler(MavlinkHandler const&)            = delete;
    MavlinkHandler& operator=(MavlinkHandler const&) = delete;
    /// TYPEDEF ________________________________________________________________

 
    /// PARAM __________________________________________________________________
    static constexpr uint8_t                         _own_system_id{1};
    static constexpr uint8_t                         _own_comp_id{MAV_COMP_ID_USER12};
    mavlink_command_long_t          sleep_cmd{0};
    mavlink_heartbeat_t             hb_received{0};

    static constexpr std::array<const char*, 9> param_list = {"DRONE_IP", "DEVICE_IP", "OWN_IP", "DRONE_PORT", "DEVICE_PORT", "UDP_MODE_DR", "UDP_MODE_DV", "TEST_WWDG", "RESET_UART"};



    /// FUNCTION _______________________________________________________________
    /**
     * @brief Task process.
     */
   // static MavlinkHandler& GetInstance() { return instance; }

    /**
     * @brief Open the bootloader.
     *
     * @return StatusEnum
     */
    StatusEnum Open();

    /**
     * @brief Close the bootloader.
     *
     * @return StatusEnum
     */
    StatusEnum Close();

    
    StatusEnum  _mavlinkTaskFunc();

    void Reset() override {};
    
    void _send_component_information_basic(mavlink_channel_t chan);
    // static void _sleep_mode_ack_callback(
    //     void* context,
    //     SleepMode_Com::PowerAction action,
    //     SleepMode_Com::AckResult result);    

    static bool _sleep_mode_ack_callback(
        void* context,
        SleepMode_Com::PowerAction action,
        SleepMode_Com::AckResult result);

    bool SendCommandAck(uint16_t command, uint8_t result, uint8_t target_sys, uint8_t target_comp,
                        mavlink_channel_t chan = MAVLINK_COMM_2);
    bool SendCommandAckAircraft(uint16_t command, uint8_t result, uint8_t target_sys, uint8_t target_comp);
    
//    void        set_own_id(uint8_t system_id, uint8_t comp_id) { _own_comp_id = comp_id;
//                                                                 _own_system_id=system_id;}



private:
    SleepMode_Com& _sleep_mode;

    struct CmdAckResponse {
        mavlink_system_t  target;          // System ID
        mavlink_channel_t target_channel;  // Channel ID

        uint16_t command;  // Response command
        uint8_t  result;   // Response result
    }cmdAckResponse;

    struct CmdExtResponse {
        mavlink_system_t  target;          // System ID
        mavlink_channel_t target_channel;  // Channel ID

        uint32_t result; 
    }cmdExtResponse;




    /// PARAM __________________________________________________________________
    HW::Peripheral::Time time;


    
    bool                isOpen{false};
    static constexpr uint8_t                   NUM_MSG_CALLBACKS      = 10;
    static constexpr uint8_t                   NUM_PERIODIC_SEND_MSGS = 10;
    etl::unordered_map<uint8_t, MsgCallback, NUM_MSG_CALLBACKS> msgCallbacks;
    etl::vector<PeriodicSendMsg, NUM_PERIODIC_SEND_MSGS>        periodicSendMsgs;

    /// FUNCTION _______________________________________________________________

    bool _transmitted_heartbeat_callback(mavlink_channel_t chan);
    bool _transmitted_system_status_callback(mavlink_channel_t chan);
    bool _send_command_ack();
    bool _send_param_value(const char* param_id, uint8_t ack);
    bool _send_param_ext_ack(const char* param_id,const char* param_value, uint8_t ack);
    void _received_heartbeat_callback(mavlink_channel_t chan, const mavlink_message_t& msg);
    void _received_cmd_long_callback(mavlink_channel_t chan, const mavlink_message_t& msg);
    void _received_param_ext_callback(mavlink_channel_t chan, const mavlink_message_t& msg);
    void _received_param_ext_request_read_callback(mavlink_channel_t chan, const mavlink_message_t& msg);
    void _received_param_ext_request_list_callback(mavlink_channel_t chan, const mavlink_message_t& msg);
    
 
    static constexpr uint32_t MAVLINKCOM_TASK_STACK_SIZE = 4096U;

    RTOS::TaskStatic<MAVLINKCOM_TASK_STACK_SIZE> _Mavlink_Main_Task{
        PRIORITY_NORMAL,
        [this]() { this->_mavlinkTaskFunc(); },
        "mavlink com task"
    };    
};

#endif /* __MAVLINK_COM_HPP__ */
