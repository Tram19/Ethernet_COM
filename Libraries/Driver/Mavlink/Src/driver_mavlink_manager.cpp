/**
 ******************************************************************************
 * @file    driver_mavlink_manager.cpp
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

#include "Driver/Mavlink/Inc/driver_mavlink_manager.hpp"

#if (CONFIG_MAVLINK == 1)

# include "Driver/Mavlink/Inc/driver_mavlink_component.hpp"
# include "Driver/Mavlink/Inc/driver_mavlink_com_port.hpp"


/* Private typedef -----------------------------------------------------------*/

using namespace Driver::Mavlink;

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

Manager Manager::instance;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/**
 * @brief Add a component to the manager
 *
 * @param _component
 * @return etl::tuple<StatusEnum, ComponentIndexEnum>
 */
etl::tuple<StatusEnum, ComponentIndexEnum> Manager::AddComponent(Component &_component) {
    for (uint8_t i = 0; i < MAV_COMP_COUNT; i++) {
        if (components[i] == nullptr) {
            components[i] = &_component;
            return etl::make_tuple(OK, static_cast<ComponentIndexEnum>(i));
        }
    }

    LOGE << ("Cannot add mavlink component");
    return etl::make_tuple(ERROR_HAL, MAV_COMP_COUNT);
}

/**
 * @brief Remove a component from the manager
 *
 * @param index
 * @return StatusEnum
 */
StatusEnum Manager::RemoveComponent(ComponentIndexEnum index) {
    if (components[index] == nullptr) {
        LOGE << ("Try to remove non existent mavlink component at MAV_COMP_{0}") << static_cast<uint8_t>(index);
        return ERROR_NOT_EXIST;
    }

    components[index] = nullptr;
    return OK;
}

/**
 * @brief Add a COM port to the manager
 *
 * @param _comPort
 * @param port
 * @return StatusEnum
 */
StatusEnum Manager::AddCOMPort(COMPortBase &_comPort, mavlink_channel_t port) {
    if (comPorts[port] != nullptr) {
        LOGE << ("Try to add new mavlink COM port to already existent port at MAVLINK_COMM_{0}") <<
                         static_cast<uint8_t>(port);
        return ERROR_EXIST;
    }

    comPorts[port] = &_comPort;
    return OK;
}

/**
 * @brief Remove a COM port from the manager
 *
 * @param _comPort
 * @return StatusEnum
 */
StatusEnum Manager::RemoveCOMPort(mavlink_channel_t port) {
    if (comPorts[port] == nullptr) {
        LOGE << ("Try to remove non existent mavlink COM port at MAVLINK_COMM_{0}") << static_cast<uint8_t>(port);
        return ERROR_NOT_EXIST;
    }

    comPorts[port] = nullptr;
    return OK;
}

/**
 * Open driver mavlink manager
 */
StatusEnum Manager::Open(){

    if(_txQueue.Open()!=OK){
    	LOGI<<"TX Queue Open Fail "<<LEND;
    return ERROR_HAL;
    }

    if(_txTask.Open()!=OK){
    	LOGI<<"TX Task Open Fail "<<LEND;
    return ERROR_HAL;
    }
    return OK;

}
/**
 * @brief Callback when parse a message successfully
 *
 * @param msg
 */
void Manager::ParseMessageCallback(mavlink_channel_t port, const mavlink_message_t &msg) {
    auto [isProcessLocal, componentIndex] = CheckAndForward(port, msg);

    if (!isProcessLocal) {
        return;
    }

    // Check if process msg for all component or individual
    if (componentIndex == MAV_COMP_ALL) {
        for (auto component : components) {
            if (component != nullptr) {
                component->ReceiveMsgCallback(port, msg);
            }
        }
    } else {
        if (components[componentIndex] != nullptr) {
            components[componentIndex]->ReceiveMsgCallback(port, msg);
        }
    }
}

/**
 * @brief Check if the message should be forwarded to other ports or processed locally
 *
 * @param port
 * @param msg
 * @return etl::tuple<bool, ComponentIndexEnum> true if the message should be processed locally, and index of the
 * component to which the message should be processed
 */
etl::tuple<bool, ComponentIndexEnum> Manager::CheckAndForward(mavlink_channel_t port, const mavlink_message_t &msg) {
    // Not handle self msg
    for (auto component : components) {
        if (component != nullptr) {
            if (sysid == msg.sysid && component->GetCompid() == msg.compid) {
                return etl::make_tuple(false, MAV_COMP_COUNT);
            }
        }
    }
    //LOGI<<"Fwd message " << msg.msgid << " from compid " << msg.compid<< LEND;
    int8_t routeId = LearnRoute(port, msg);

    // Handle private message
    if (routeId >= 0 && routes[routeId]->isPrivate) {
        return etl::make_tuple(true, MAV_COMP_ALL);
    }

    // Dont forward RADIO message
    if (msg.msgid == MAVLINK_MSG_ID_RADIO_STATUS) {
        return etl::make_tuple(false, MAV_COMP_COUNT);
    }

    // ADSB packets are not forwarded, they have their own stream rate
    if (msg.msgid == MAVLINK_MSG_ID_ADSB_VEHICLE) {
        return etl::make_tuple(false, MAV_COMP_COUNT);
    }

    // Forward heartbeat to other ports and process locally
    if (msg.msgid == MAVLINK_MSG_ID_HEARTBEAT) {
        //LOGS<<"heartbeat from: "<< msg.compid<<" port: "<<port<<LEND;
        HandleHeartbeat(msg);
        HandleBroadcast(port, msg);
        return etl::make_tuple(true, MAV_COMP_ALL);
    }

    auto [isBroadcast, target] = GetTargets(msg);

    if (isBroadcast) {
        HandleBroadcast(port, msg);
        return etl::make_tuple(true, MAV_COMP_ALL);
    }

    bool broadcastComp = target.compid == MAV_COMP_ID_ALL;
    bool matchSystem   = target.sysid == sysid;

    bool isProcessLocal = false;

    if (matchSystem) {
        if (broadcastComp) {
            // Match system and broadcast component, process locally to all local components
            isProcessLocal = true;

        } else {
            // Scan for matching individual component
            for (uint8_t i = 0; i < MAV_COMP_COUNT; i++) {
                if (components[i] != nullptr) {
                    if (target.compid == components[i]->GetCompid()) {
                        return etl::make_tuple(true, static_cast<ComponentIndexEnum>(i));
                    }
                }
            }
        }
    }

    bool sent2Chan[NUM_COM_PORTS] = {false};  // Flag notify channel has sent message

    // Scan all routes to forward message to appropriate channels
    for (uint8_t i = 0; i < numRoutes; i++) {
        // Check if match sysid and compid
        if (target.sysid == routes[i]->component.sysid &&
            (broadcastComp || target.compid == routes[i]->component.compid)) {
            // Not send back to source port and not send to a port twice
            if (routes[i]->port != port && !sent2Chan[routes[i]->port]) {
                // Forward message to this port
                SendMessage(routes[i]->port, msg);
                sent2Chan[routes[i]->port] = true;
            }
        }
    }

    if (isProcessLocal) {
        return etl::make_tuple(true, MAV_COMP_ALL);
    }

    return etl::make_tuple(false, MAV_COMP_COUNT);
}

/**
 * @brief Learn new route to the manager
 *
 * @param port
 * @param msg
 * @return int8_t route index where msg came from
 */
int8_t Manager::LearnRoute(mavlink_channel_t port, const mavlink_message_t &msg) {
    // Dont learn route from broadcast system
    if (msg.sysid == 0) {
        return -1;
    }

    // Dont learn route from self
    for (auto component : components) {
        if (component != nullptr) {
            if (sysid == msg.sysid && component->GetCompid() == msg.compid) {
                return -1;
            }
        }
    }

    // Dont learn route fromt our system and broadcast. We should broadcast this msg and also process locally
    if (msg.sysid == sysid && msg.compid == MAV_COMP_ID_ALL) {
        return -1;
    }

    uint8_t i = 0;

    // Scan all routes to check if we already have this route
    for (i = 0; i < numRoutes; i++) {
        if (routes[i]->component.sysid == msg.sysid && routes[i]->component.compid == msg.compid &&
            routes[i]->port == port) {
            // Get mavtype from msg if not set
            if (routes[i]->mavtype == 0 && msg.msgid == MAVLINK_MSG_ID_HEARTBEAT) {
                routes[i]->mavtype = mavlink_msg_heartbeat_get_type(&msg);
            }
            return i;
        }
    }

    if (i < numRoutes) {
        return i;
    }

    if (numRoutes == MAX_ROUTES) {
        LOGI << ("Max mavlink routes reached")<<LEND;
        return -1;
    }

    // Add new route
    routes[i] = new Route();

    if (routes[i] == nullptr) {
        LOGE << ("Cannot allocate memory for new mavlink route")<<LEND;
        return -1;
    }

    routes[i]->component.sysid  = msg.sysid;
    routes[i]->component.compid = msg.compid;
    routes[i]->port             = port;

    if (msg.msgid == MAVLINK_MSG_ID_HEARTBEAT) {
        routes[i]->mavtype = mavlink_msg_heartbeat_get_type(&msg);
    } else {
        routes[i]->mavtype = 0;
    }

    ++numRoutes;

    return i;
}

/**
 * @brief Handle broadcast msg, forward to other ports
 *
 * @param port
 * @param msg
 */
void Manager::HandleBroadcast(mavlink_channel_t port, const mavlink_message_t &msg) {
    // Forward message to other ports
    for (uint8_t i = 0; i < NUM_COM_PORTS; i++) {
        if (i != port) {
            SendMessage(static_cast<mavlink_channel_t>(i), msg);
        }
    }
}

/**
 * @brief Get the Targets of the message
 *
 * @param msg
 * @return etl::tuple<bool, mavlink_system_t> true if msg is broadcast, and target system
 */
etl::tuple<bool, mavlink_system_t> Manager::GetTargets(const mavlink_message_t &msg) {
    const mavlink_msg_entry_t *msgEntry = mavlink_get_msg_entry(msg.msgid);

    if (msgEntry == nullptr) {
        return etl::make_tuple<bool, mavlink_system_t>(true, {0, 0});
    }

    uint8_t sysid = 0;

    if (msgEntry->flags & MAV_MSG_ENTRY_FLAG_HAVE_TARGET_SYSTEM) {
        sysid = _MAV_RETURN_uint8_t(&msg, msgEntry->target_system_ofs);
    } else {
        // Cannot get target sysid, assume broadcast
        return etl::make_tuple<bool, mavlink_system_t>(true, {0, 0});
    }

    uint8_t compid = 0;

    if (msgEntry->flags & MAV_MSG_ENTRY_FLAG_HAVE_TARGET_COMPONENT) {
        compid = _MAV_RETURN_uint8_t(&msg, msgEntry->target_component_ofs);
    }

    return etl::make_tuple<bool, mavlink_system_t>(sysid == 0 && compid == MAV_COMP_ID_ALL, {sysid, compid});
}

/**
 * @brief Handle heartbeat msg
 *
 * @param msg
 */
void Manager::HandleHeartbeat(const mavlink_message_t &msg) {
    // We dont care about other heartbeats because we are master
    if (isMaster) {
        return;
    }

    // We are slave, check if we are initialized yet
    if (isInitialized) {
        // Done
        return;
    }

    // Check if this heartbeat is from master candidate
    if (!IsMasterCandidate(msg)) {
        return;
    }

    // This is master candidate, adapt master sysid and handle 1st connection
    SetSysid(msg.sysid);
}

/**
 * @brief Check if the message is from a master candidate
 *
 * @param msg
 * @return true
 * @return false
 */
bool Manager::IsMasterCandidate(const mavlink_message_t &msg) {
    // Check if autopilot by autopilot field
    if (mavlink_msg_heartbeat_get_autopilot(&msg) != MAV_AUTOPILOT_INVALID) {
        return true;
    }

    uint8_t mavtype = mavlink_msg_heartbeat_get_type(&msg);

    // Check for some mavtype, ignore type autopilot because already checked by autopilot field
    if (mavtype == MAV_TYPE_ONBOARD_CONTROLLER || mavtype == MAV_TYPE_GIMBAL || mavtype == MAV_TYPE_CAMERA) {
        return true;
    }

    return false;
}

/**
 * @brief Reset the system
 *
 * This function resets all the internal state of the manager.
 * This will reset all the components and the system id.
 */
void Manager::ResetSystem() {
    sysid         = DEFAULT_SYSID;
    isInitialized = false;

    // Rset all components
    for (auto component : components) {
        if (component != nullptr) {
            component->Reset();
        }
    }
}

/**
 * @brief Send a buffer to a MAVLink channel
 *
 * This function sends a MAVLink buffer to a given channel. The buffer is
 * expected to contain a complete MAVLink message. The message is sent
 * directly to the channel without any additional processing.
 *
 * @param port    The port to send the message on
 * @param buff    The buffer to send
 */
bool Manager::SendBuff(mavlink_channel_t port, std::span<const uint8_t> buff) {
    if (comPorts[port] != nullptr) {
        return comPorts[port]->SinkIt(buff);
    }

    return false;
}

/**
 * @brief Send a message to a MAVLink channel
 *
 * This function sends a MAVLink message to a given channel. The message is
 * expected to contain a complete MAVLink message. The message is sent
 * directly to the channel without any additional processing.
 *
 * @param port    The port to send the message on
 * @param msg     The message to send
 */
bool Manager::SendMessage(mavlink_channel_t port, const mavlink_message_t &msg) {
    if (!_txQueue.IsOpen()) return false;
    return (_txQueue.Emplace(port, msg) == osOK);
}

StatusEnum Manager::_txTaskFunc() {
        TxRequest req{};
        for (;;) {
            if (_txQueue.Get(&req, osWaitForever) != osOK) continue;

            if (comPorts[req.port] != nullptr) {
                uint16_t len = mavlink_msg_to_send_buffer(sendBuff.data(), &req.message);
                comPorts[req.port]->SinkIt(std::span<const uint8_t>{sendBuff.data(), len});

        }
        }
    }


extern "C" {

void _mavlink_send_uart(mavlink_channel_t chan, const char *buf, uint16_t len) {
    Manager::GetInstance().SendBuff(chan, {reinterpret_cast<const uint8_t *>(buf), len});
}

void _mavlink_resend_uart(mavlink_channel_t chan, const mavlink_message_t *msg) {

    uint8_t ck[2];

    ck[0] = (uint8_t)(msg->checksum & 0xFF);
    ck[1] = (uint8_t)(msg->checksum >> 8);
    // XXX use the right sequence here

    uint8_t header_len;
    uint8_t signature_len;

    // Bộ đệm tạm thời chứa trọn vẹn gói tin
    uint8_t buf[280]; 
    uint16_t total_len = 0;

    if (msg->magic == MAVLINK_STX_MAVLINK1) {
        header_len    = MAVLINK_CORE_HEADER_MAVLINK1_LEN + 1;
        signature_len = 0;
        MAVLINK_START_UART_SEND(chan, header_len + msg->len + 2 + signature_len);
        // we can't send the structure directly as it has extra mavlink2 elements in it
        uint8_t buf[MAVLINK_CORE_HEADER_MAVLINK1_LEN + 1];
        buf[0] = msg->magic;
        buf[1] = msg->len;
        buf[2] = msg->seq;
        buf[3] = msg->sysid;
        buf[4] = msg->compid;
        buf[5] = msg->msgid & 0xFF;
        _mavlink_send_uart(chan, (const char *)buf, header_len);
    } else {
        header_len    = MAVLINK_CORE_HEADER_LEN + 1;
        signature_len = (msg->incompat_flags & MAVLINK_IFLAG_SIGNED) ? MAVLINK_SIGNATURE_BLOCK_LEN : 0;
        MAVLINK_START_UART_SEND(chan, header_len + msg->len + 2 + signature_len);
        uint8_t buf[MAVLINK_CORE_HEADER_LEN + 1];
        buf[0] = msg->magic;
        buf[1] = msg->len;
        buf[2] = msg->incompat_flags;
        buf[3] = msg->compat_flags;
        buf[4] = msg->seq;
        buf[5] = msg->sysid;
        buf[6] = msg->compid;
        buf[7] = msg->msgid & 0xFF;
        buf[8] = (msg->msgid >> 8) & 0xFF;
        buf[9] = (msg->msgid >> 16) & 0xFF;
        _mavlink_send_uart(chan, (const char *)buf, header_len);
    }
    _mavlink_send_uart(chan, _MAV_PAYLOAD(msg), msg->len);
    _mavlink_send_uart(chan, (const char *)ck, 2);
    if (signature_len != 0) {
        _mavlink_send_uart(chan, (const char *)msg->signature, MAVLINK_SIGNATURE_BLOCK_LEN);
    }
    MAVLINK_END_UART_SEND(chan, header_len + msg->len + 2 + signature_len);
}

} /* extern "C" */

#endif /* CONFIG_MAVLINK */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
