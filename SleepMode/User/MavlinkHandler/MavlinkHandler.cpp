/*
 * MavlinkHandler.cpp
 *
 *  Created on: May 13, 2026
 *      Author: USER
 */
#include "MavlinkHandler.hpp"
#include "SleepMode_Com/SleepMode_Com.hpp"
#include "ComPortManager/ComPortManager.hpp"
#include "Modules/Ethernet_COM/Ethernet_COM.hpp"
#include "StorageParam/StorageParam.hpp"
#include "gremsy.hpp"
#include "RTOS/rtos.hpp"
#include "RTOS/rtos_include.hpp"
#include "Driver/Mavlink/Inc/driver_mavlink.hpp"
#include "Driver/Mavlink/Inc/driver_mavlink_include.hpp"
#include "IOExpander/IOExpander.hpp"
#include "PowerManager/powermanager.hpp"
#include "wwdg.h"
#include "charconv"
/* Private define ------------------------------------------------------------*/
using namespace Driver::Mavlink;

Module::Protocol::Mavlink   _mavlink;






/* ---------------------------------------------------------------------------
 * Open / Close
 * ---------------------------------------------------------------------------*/
StatusEnum MavlinkHandler::Open()
{
    if (isOpen) {
        LOGE<<"Trying to open an already open mavlink task"<<LEND;
        return ERROR_EXIST;
    }
    HAL_WWDG_Refresh(&hwwdg);
     
    Driver::Mavlink::Manager::GetInstance().ConfigMaster(_own_system_id);

    //-- TX messages -----------------------------------------------------------------------------------------------
    AddPeriodicSendMsg(MAVLINK_MSG_ID_HEARTBEAT,
                   [this](mavlink_channel_t chan) -> bool { return _transmitted_heartbeat_callback(chan); }, 3000);

    // AddPeriodicSendMsg(MAVLINK_MSG_ID_SYS_STATUS,
    //                [this](mavlink_channel_t chan) -> bool { return _transmitted_system_status_callback(chan); }, 1000);    

    //-- RX messages -----------------------------------------------------------------------------------------------
    AddMsgCallback(MAVLINK_MSG_ID_HEARTBEAT,
                   [this](mavlink_channel_t chan, const mavlink_message_t& msg) { _received_heartbeat_callback(chan, msg); });

    AddMsgCallback(MAVLINK_MSG_ID_COMMAND_LONG,
                   [this](mavlink_channel_t chan, const mavlink_message_t& msg) { _received_cmd_long_callback(chan, msg); });

    AddMsgCallback(MAVLINK_MSG_ID_PARAM_EXT_SET,
                       [this](mavlink_channel_t chan, const mavlink_message_t& msg) { _received_param_ext_callback(chan, msg); });

    AddMsgCallback(MAVLINK_MSG_ID_PARAM_EXT_REQUEST_READ,
                       [this](mavlink_channel_t chan, const mavlink_message_t& msg) { _received_param_ext_request_read_callback(chan, msg); });

    AddMsgCallback(MAVLINK_MSG_ID_PARAM_EXT_REQUEST_LIST,
                       [this](mavlink_channel_t chan, const mavlink_message_t& msg) { _received_param_ext_request_list_callback(chan, msg); });

    Component::Open();

    // Main task
    if (_Mavlink_Main_Task.Open() != OK) {
        LOGE << "Main task Open FAIL" <<LEND;
        return ERROR_HAL;
    }
    LOGI<<"Mavlink main task Open"<<LEND;
    isOpen = true;
    time   = TIME_MS;

    return OK;
}

StatusEnum MavlinkHandler::Close()
{
    if (!isOpen) {
        LOGE<<"Trying to close an already closed mavlink task"<<LEND;
        return ERROR_NOT_EXIST;
    }
    if (_Mavlink_Main_Task.Close() != OK) {
        LOGE << "Main task Close FAIL" <<LEND;
        return ERROR_HAL;
    }
    
    isOpen = false;
    return OK;
}

bool MavlinkHandler::_sleep_mode_ack_callback(
    void* context,
    SleepMode_Com::PowerAction action,
    SleepMode_Com::AckResult result)
{
    auto* handler = static_cast<MavlinkHandler*>(context);
    if (handler == nullptr) return false;
    handler->cmdAckResponse.result = result == SleepMode_Com::AckResult::InProgress
        ? MAV_RESULT_IN_PROGRESS
        : (result == SleepMode_Com::AckResult::Accepted
            ? MAV_RESULT_ACCEPTED
            : MAV_RESULT_FAILED);
    return handler->_send_command_ack();
}


/// MODULE MAVLINK CALLBACK ____________________________________________________
/** @brief ham truyen gia tri duoc goi tu mav
 *  @param[in] handle dia chi bien tra ve tu mav
 *  @return trang thai qua
 * */
//StatusEnum mav_write_callback(Module::ProtocolMavlinkWriteType* handle)
//{
//    _uart.Write(handle->buff, handle->buffsize);
//
//    return OK;
//}

/** @brief ham truoc khi truyen heartbeat
 *  @return trang thai qua trinh
 * */
bool MavlinkHandler::_transmitted_heartbeat_callback(mavlink_channel_t chan)
{
    mavlink_message_t msg;

    mavlink_heartbeat_t heartbeat = {
           .type            = MAV_TYPE_ONBOARD_CONTROLLER,
           .autopilot       = MAV_AUTOPILOT_INVALID,
           .system_status   = MAV_STATE_ACTIVE,
           .mavlink_version = MAVLINK_VERSION,
       };
//    mavlink_heartbeat_t heartbeat = {
//           .type            = MAV_TYPE_GCS,
//           .autopilot       = MAV_AUTOPILOT_INVALID,
//           .system_status   = MAV_STATE_ACTIVE,
//           .mavlink_version = MAVLINK_VERSION,
//       };

       mavlink_msg_heartbeat_encode_chan(Manager::GetInstance().GetSysid(), _own_comp_id, chan, &msg, &heartbeat);
       return Manager::GetInstance().SendMessage(chan, msg);
}

bool MavlinkHandler::_transmitted_system_status_callback(mavlink_channel_t chan)
{
    mavlink_message_t msg;

    mavlink_sys_status_t sys_status = {
        .onboard_control_sensors_present = 0,
        .onboard_control_sensors_enabled = 0,
        .onboard_control_sensors_health  = 0,
        .load                           = 500, // Load in 1/10th percent
        .voltage_battery                = 12000, // Battery voltage in millivolts
        .current_battery                = -1, // Battery current in 10*milliamperes, -1: autopilot does not measure the current
        .battery_remaining              = -1, // Remaining battery energy: (0%: 0, 100%: 100), -1: autopilot does not estimate the remaining battery
    };

    mavlink_msg_sys_status_encode_chan(Manager::GetInstance().GetSysid(), _own_comp_id, chan, &msg, &sys_status);
    return Manager::GetInstance().SendMessage(chan, msg);
}
bool MavlinkHandler::SendCommandAck(uint16_t command, uint8_t result, uint8_t target_sys, uint8_t target_comp, mavlink_channel_t chan)
{
    mavlink_message_t message{};
    mavlink_command_ack_t command_ack{};
    command_ack.command          = command;
    command_ack.result           = result;
    command_ack.progress         = (result == MAV_RESULT_ACCEPTED) ? 100 : 0;
    command_ack.result_param2    = 0;
    command_ack.target_system    = target_sys;
    command_ack.target_component = target_comp;

    LOGI<<"Sending command ack: command="<<command
         <<", result="<<static_cast<int>(result)
         <<", target_sys="<<static_cast<int>(target_sys)
         <<", target_comp="<<static_cast<int>(target_comp)
         <<", channel="<<static_cast<int>(chan)<<LEND;
    mavlink_msg_command_ack_encode_chan(
        Manager::GetInstance().GetSysid(), _own_comp_id, chan, &message, &command_ack);
    return Manager::GetInstance().SendMessage(chan, message);
}

bool MavlinkHandler::SendCommandAckAircraft(uint16_t command, uint8_t result, uint8_t target_sys, uint8_t target_comp)
{
    // Gửi ACK tới máy bay qua cả UART (MAVLINK_COMM_0) và Ethernet (MAVLINK_COMM_2)
    bool ok_uart = SendCommandAck(command, result, target_sys, target_comp, MAVLINK_COMM_0);
    bool ok_eth  = SendCommandAck(command, result, target_sys, target_comp, MAVLINK_COMM_2);
    return ok_uart || ok_eth;
}

bool MavlinkHandler::_send_command_ack(){
    return SendCommandAck(cmdAckResponse.command, cmdAckResponse.result,
                          cmdAckResponse.target.sysid, cmdAckResponse.target.compid,
                          cmdAckResponse.target_channel);
}
bool MavlinkHandler::_send_param_ext_ack(const char* param_id,const char* param_value, uint8_t param_result) {
    mavlink_message_t message;
    mavlink_param_ext_ack_t ack = {
        .param_id = {0},
		.param_value = {0},
		.param_type = MAV_PARAM_EXT_TYPE_CUSTOM,
        .param_result = param_result,
    };
    strncpy((char*)ack.param_id, param_id, sizeof(ack.param_id) - 1);
    ack.param_id[sizeof(ack.param_id) - 1] = '\0';
    strncpy((char*)ack.param_value, param_value, sizeof(ack.param_value) - 1);
    ack.param_value[sizeof(ack.param_value) - 1] = '\0';
    mavlink_msg_param_ext_ack_encode_chan(Manager::GetInstance().GetSysid(), _own_comp_id, cmdExtResponse.target_channel, &message, &ack);

    return Manager::GetInstance().SendMessage(cmdExtResponse.target_channel, message);
}

bool MavlinkHandler::_send_param_value(const char* param_id, uint8_t param_result) {
	mavlink_message_t message;
    mavlink_param_ext_value_t param_value = {
        .param_id = {0},
        .param_value = {0},
        .param_type = MAV_PARAM_EXT_TYPE_CUSTOM,
    };
    char valueStr[128] = {};

    if (param_result != PARAM_ACK_ACCEPTED) {
        // Nếu có lỗi, gửi ACK với kết quả lỗi
        return _send_param_ext_ack(param_id,param_value.param_value, param_result);
    }

    if (!StorageNetworkParam::GetInstance().GetParamString(param_id, valueStr, sizeof(valueStr))) {
        return _send_param_ext_ack(param_id, "", PARAM_ACK_VALUE_UNSUPPORTED);
    }
    strncpy((char*)param_value.param_id, param_id, sizeof(param_value.param_id) - 1);
    strncpy((char*)param_value.param_value, valueStr, sizeof(param_value.param_value) - 1);
    // Không có giá trị thực nên để mặc định 0
    mavlink_msg_param_ext_value_encode_chan(Manager::GetInstance().GetSysid(), _own_comp_id, cmdExtResponse.target_channel, &message, &param_value);
    return Manager::GetInstance().SendMessage(cmdExtResponse.target_channel, message);
}

void MavlinkHandler::_send_component_information_basic(mavlink_channel_t chan) {
    mavlink_message_t message;
    mavlink_component_information_basic_t comp_info = {0}; 
    strncpy(comp_info.vendor_name, "Gremsy", sizeof(comp_info.vendor_name) - 1);
    comp_info.vendor_name[sizeof(comp_info.vendor_name) - 1] = '\0';
    strncpy(comp_info.model_name, "SleepMode", sizeof(comp_info.model_name) - 1);  
    comp_info.model_name[sizeof(comp_info.model_name) - 1] = '\0';
    snprintf(comp_info.software_version, 
             sizeof(comp_info.software_version), 
             "%d.%d.%d", 
             SW_MAJOR, SW_MINOR, SW_PATCH);
    mavlink_msg_component_information_basic_encode_chan(Manager::GetInstance().GetSysid(), _own_comp_id, chan, &message, &comp_info);
    Manager::GetInstance().SendMessage(chan, message);
}
/* ---------------------------------------------------------------------------
 * RX Handlers
 * ---------------------------------------------------------------------------*/

/** @brief ham nhan heatbeat
 *  @param[in] handle dia chi cua bien tra ve tu mavlink
 *  @return trang thai qua trinh
 * */
void MavlinkHandler::_received_heartbeat_callback(mavlink_channel_t chan, const mavlink_message_t& msg)
{

	__mavlink_heartbeat_t _heartbeat;
	mavlink_msg_heartbeat_decode(&msg, &_heartbeat);
    // IS SOM: // todo
	if(msg.compid >= MAV_COMP_ID_CAMERA && msg.compid <=MAV_COMP_ID_CAMERA6 ) {
        _sleep_mode.Heartbeat_Som();
	}
}

void MavlinkHandler::_received_cmd_long_callback(mavlink_channel_t chan, const mavlink_message_t& msg){
    mavlink_command_long_t command_long;

    //LOGI<<"Receive cmd long"<<LEND;
    mavlink_msg_command_long_decode(&msg, &command_long);

    if (command_long.target_system != Manager::GetInstance().GetSysid() || command_long.target_component != _own_comp_id) {
        return;
    }

    cmdAckResponse.target         = {msg.sysid, msg.compid};
    cmdAckResponse.target_channel = chan;
    cmdAckResponse.command        = command_long.command;


   switch (command_long.command) {
        case MAV_CMD_PREFLIGHT_REBOOT_SHUTDOWN: 
            if (command_long.param3 == 10) {
        	    LOGW<<"Received off command"<<LEND;
                cmdAckResponse.result = MAV_RESULT_IN_PROGRESS;
                _send_command_ack();
                // Power off system
                if (_sleep_mode.RequestPower(
                        SleepMode_Com::PowerAction::Sleep,
                        &_sleep_mode_ack_callback, this) != OK) {
                    cmdAckResponse.result = MAV_RESULT_DENIED;
                    _send_command_ack();
                }

            }
            else if (command_long.param3 == 4) {
        	    // Power on system
        	    LOGW<<"Received on command"<<LEND;
                cmdAckResponse.result = MAV_RESULT_IN_PROGRESS;
                _send_command_ack();
                if (_sleep_mode.RequestPower(
                        SleepMode_Com::PowerAction::Wake,
                        &_sleep_mode_ack_callback, this) != OK) {
                    cmdAckResponse.result = MAV_RESULT_DENIED;
                    _send_command_ack();
                }

            }
            else if (command_long.param3 == 1) {
        	    // Power on system
        	    LOGW<<"Received reboot command"<<LEND;
                cmdAckResponse.result = MAV_RESULT_IN_PROGRESS;
                if (_sleep_mode.ForceReboot() != OK) {
                    cmdAckResponse.result = MAV_RESULT_DENIED;
                }
                else cmdAckResponse.result = MAV_RESULT_ACCEPTED;
                _send_command_ack();
                

            }
        break;
        case MAV_CMD_REQUEST_MESSAGE:
            if (command_long.param1 == MAVLINK_MSG_ID_COMPONENT_INFORMATION_BASIC){
                cmdAckResponse.result = MAV_RESULT_ACCEPTED;
                _send_component_information_basic(chan);
                _send_command_ack();
            }
        break;
        default:
            cmdAckResponse.result = MAV_RESULT_UNSUPPORTED;
            _send_command_ack();

            break;
    }
}

void MavlinkHandler::_received_param_ext_callback(mavlink_channel_t chan, const mavlink_message_t& msg){
	mavlink_param_ext_set_t param;
	mavlink_msg_param_ext_set_decode(&msg, &param);

	if (param.target_system    != Manager::GetInstance().GetSysid() ||
	    param.target_component != compid) return;

    cmdExtResponse.target         = {msg.sysid, msg.compid};
    cmdExtResponse.target_channel = chan;
    NetworkConfig_t& netConfig_ = StorageNetworkParam::GetInstance().GetConfig();
    bool changed = StorageNetworkParam::GetInstance().SetParam(
        param.param_id, (const char*)param.param_value);
	// ── Test WWDG (Watchdog) ────────────────────────────────────────────────
	if (!changed && strcmp(param.param_id, "TEST_WWDG") == 0) {
	    int val;
	    auto result = std::from_chars(
	        (const char*)param.param_value,
	        (const char*)param.param_value + strlen((const char*)param.param_value),
	        val);
	    
	    if (result.ec == std::errc() && val == 1) {
	        // Trigger HardFault bằng cách dereference null pointer
	        LOGW << "TEST_WWDG triggered! Dereferencing nullptr to trigger HardFault..." << LEND;
	        g_os_delay(50);  // Small delay để ACK được gửi đi
	        
	        // Dereference null pointer -> HardFault -> Reset
	        volatile uint32_t* null_ptr = (volatile uint32_t*)0x00000000;
	        *null_ptr = 0xDEADBEEF;  // HardFault! MCU reset
	    }
	}
	else if (strcmp(param.param_id, "RESET_UART") == 0) {
        int val;
        auto result = std::from_chars(
            (const char*)param.param_value,
            (const char*)param.param_value + strlen((const char*)param.param_value),
            val);
        
        if (result.ec == std::errc() && val == 1) {
            //LOGW << "RESET_UART triggered! Resetting UART4..." << LEND;
            g_os_delay(50);  // Small delay để ACK được gửi đi
            
            // Reset UART4
            ComPortManager::GetInstance().Uart4().Reset();
            changed = true;
        }
    }

    if (!changed) {
        _send_param_ext_ack(param.param_id, (const char*)param.param_value, PARAM_ACK_VALUE_UNSUPPORTED);
        return;
    }

//	// Lưu flash
	if (!StorageNetworkParam::GetInstance().Save()) {
		_send_param_ext_ack(param.param_id,param.param_value, PARAM_ACK_IN_PROGRESS);
	    return;
	}

	// Apply không reset
    ComPortManager::GetInstance().ApplyNetworkConfig(netConfig_);

	LOGI << "Param ID: " << param.param_id << " Value: " << param.param_value << LEND;
	_send_param_ext_ack(param.param_id, param.param_value, PARAM_ACK_ACCEPTED);
}

void MavlinkHandler::_received_param_ext_request_read_callback(mavlink_channel_t chan, const mavlink_message_t& msg) {

    mavlink_param_ext_request_read_t request;
    mavlink_msg_param_ext_request_read_decode(&msg, &request);
    cmdExtResponse.target         = {msg.sysid, msg.compid};
    cmdExtResponse.target_channel = chan;
	char param_id_safe[17] = {};
	strncpy(param_id_safe, request.param_id, 16);

    if (request.target_system != Manager::GetInstance().GetSysid() ||
        request.target_component != _own_comp_id) {
        return;
    }

    auto it = std::find_if(param_list.begin(), param_list.end(),
        [&](const char* name) {
            return strncmp(name, param_id_safe, 16) == 0;
        });

    if (it != param_list.end()) {
        _send_param_value(param_id_safe, PARAM_ACK_ACCEPTED);
    } else {
    	LOGE<<"Param id: "<< param_id_safe << " request FAIL"<<LEND;
    	 _send_param_ext_ack(param_id_safe,"", PARAM_ACK_VALUE_UNSUPPORTED);
    }
}
void MavlinkHandler::_received_param_ext_request_list_callback(mavlink_channel_t chan, const mavlink_message_t& msg) {
    // Không hỗ trợ đọc param, trả về NACK
    mavlink_param_ext_request_list_t request;
    mavlink_msg_param_ext_request_list_decode(&msg, &request);
    cmdExtResponse.target         = {msg.sysid, msg.compid};
    cmdExtResponse.target_channel = chan;

    if (request.target_system != Manager::GetInstance().GetSysid() ||
        request.target_component != _own_comp_id) {
        return;
    }
    
    for (const char* id : param_list) {
        _send_param_value(id, PARAM_ACK_ACCEPTED);
        g_os_delay(50);
    }
}
/* ---------------------------------------------------------------------------
 * Private helpers
 * ---------------------------------------------------------------------------*/



/// @brief Main state machine
StatusEnum MavlinkHandler::_mavlinkTaskFunc()
{
   
   
    ConnectAllPorts();

    
  while(1) {
    
    SendPeriodicMsgs();
    g_os_delay(10);
  }

}

