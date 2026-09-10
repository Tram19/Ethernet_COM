/**
 ******************************************************************************
 * @file    driver_mavlink_component.cpp
 * @author  Gremsy Team
 * @version V2.0.0
 * @date    Nov 18, 2024
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

#include "Driver/Mavlink/Inc/driver_mavlink_component.hpp"

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
 * @brief Opens the component.
 */
StatusEnum Component::Open() {
    if (isOpen) {
        return ERROR_EXIST;
    }

    auto [ret, index] = Manager::GetInstance().AddComponent(*this);

    if (ret != OK) {
        return ret;
    }

    if (Manager::GetInstance().Open()!=OK) LOGE<< "Mavlink manager open fail"<<LEND;

    componentIndex = index;
    isOpen         = true;
    return OK;
}

/**
 * @brief Closes the component.
 */
StatusEnum Component::Close() {
    if (!isOpen) {
        return ERROR_NOT_EXIST;
    }

    return Manager::GetInstance().RemoveComponent(componentIndex);
}

/**
 * @brief Receive new msg callback
 *
 * @param port
 * @param msg
 */
void Component::ReceiveMsgCallback(mavlink_channel_t port, const mavlink_message_t& msg) {
    auto it = msgCallbacks.find(msg.msgid);
    if (it != msgCallbacks.end()) {
        it->second(port, msg);
    }
}

/**
 * @brief Send periodic msgs, this function need to be called periodically, example in component task process
 *
 */
void Component::SendPeriodicMsgs() {
    // Only send if manager is initialized
    if (!Manager::GetInstance().IsInitialized()) {
        return;
    }

    uint32_t now = HW::Peripheral::Time::GetTick();

    // Iterate over all ports
    for (uint8_t port = MAVLINK_COMM_0; port < NUM_COM_PORTS; port++) {
        // Only send if port is connected
        if (connectedPort[port]) {
            for (auto& periodicSendMsg : periodicSendMsgs) {
                // Check if now is time to send
                if ((now - periodicSendMsg.lastSendTime[port]) >= (uint32_t)(periodicSendMsg.interval_ms*1000)) { 
                     // 2 so uint32 tru nhau neu ra hieu so la so am thi se tu dong cong them 2^32 vao=> khi now bi tran ve 0 thi phep tinh la:
                     // 0 - 4294967295 (2^32-1) = -4294967295 => tu dong cong them 2^32 => 1 
                     // now tang len periodicSendMsg.interval_ms*1000 => now- lastSendTime[port] = periodicSendMsg.interval_ms*1000 - (2^32 -1) + 2^32 = periodicSendMsg.interval_ms*1000 + 1  => phep tinh dung
                    // Update last send time if send successfully
                    if (periodicSendMsg.sendFunc(static_cast<mavlink_channel_t>(port))) {
                        periodicSendMsg.lastSendTime[port] = now;
                    } else {
                        // Cannot send more in this port, move to next
                        break;
                    }
                }
            }
        }
    }
}

#endif /* CONFIG_MAVLINK */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
