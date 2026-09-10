/**
 ******************************************************************************
 * @file    driver_mavlink_component.hpp
 * @author  Gremsy Team
 * @version V2.0.0
 * @date    Nov 16, 2024
 * @brief   This file contains all the functions prototypes for the driver_mavlink_component.cpp
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

#ifndef __DRIVER_MAVLINK_INC_DRIVER_MAVLINK_COMPONENT_HPP__
# define __DRIVER_MAVLINK_INC_DRIVER_MAVLINK_COMPONENT_HPP__

/* Includes ------------------------------------------------------------------*/

# include "driver_mavlink_include.hpp"

# if (CONFIG_MAVLINK == 1)

#  include "etl/unordered_map.h"
#  include "etl/vector.h"


/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

class Driver::Mavlink::Component {
public:

    using MsgCallbackSignature = void(mavlink_channel_t chan, const mavlink_message_t &msg);
    using SendMsgSignature     = bool(mavlink_channel_t chan);

    using MsgCallback = stdext::inplace_function<MsgCallbackSignature>;
    using SendMsgFunc = stdext::inplace_function<SendMsgSignature>;

    /**
     * @brief Receive new msg callback
     *
     * @param port
     * @param msg
     */
    void ReceiveMsgCallback(mavlink_channel_t port, const mavlink_message_t &msg);

    /**
     * @brief Resets the component.
     */
    virtual void Reset() = 0;

    /**
     * @brief Gets the component id.
     *
     * @return uint8_t
     */
    uint8_t GetCompid() const { return compid; }

protected:

    struct PeriodicSendMsg {
        PeriodicSendMsg(uint8_t _msgid, SendMsgFunc &&_sendFunc, uint32_t _interval_ms)
            : msgid(_msgid),
              sendFunc(_sendFunc),
              interval_ms(_interval_ms) {}

        uint8_t                                           msgid;
        SendMsgFunc                                       sendFunc;
        uint32_t                                          interval_ms;
        std::array<uint32_t, NUM_COM_PORTS>               lastSendTime {};
    };

    Component(uint8_t _compid, etl::iunordered_map<uint8_t, MsgCallback> &_msgCallbacks,
              etl::ivector<PeriodicSendMsg> &_periodicSendMsgs)
        : compid(_compid),
          msgCallbacks(_msgCallbacks),
          periodicSendMsgs(_periodicSendMsgs) {}

    virtual ~Component() {}

    /**
     * @brief Opens the component.
     */
    virtual StatusEnum Open();

    /**
     * @brief Closes the component.
     */
    virtual StatusEnum Close();

    /**
     * @brief Connects all communication ports.
     *
     * This function iterates over all available communication ports and sets their
     * status to connected. It ensures that each port in the `connectedPort` array
     * is marked as true, indicating that all ports are active and ready for use.
     */
    void ConnectAllPorts() {
        for (auto &port : connectedPort) {
            port = true;
        }
    }

    /**
     * @brief Disconnects all communication ports.
     *
     * This function iterates over all available communication ports and sets their
     * status to disconnected. It ensures that each port in the `connectedPort` array
     * is marked as false, indicating that all ports are inactive and will not be used.
     */
    void DisconnectAllPorts() {
        for (auto &port : connectedPort) {
            port = false;
        }
    }

    /**
     * @brief Connect component to a port
     *
     * @param port
     */
    void ConnectPort(mavlink_channel_t port) { connectedPort[port] = true; }

    void DisconnectPort(mavlink_channel_t port) { connectedPort[port] = false; }

    /**
     * @brief Add msg callback
     *
     * @param msgid
     * @param callback
     * @return StatusEnum
     */
    StatusEnum AddMsgCallback(uint8_t msgid, MsgCallback &&callback) {
        if (msgCallbacks.find(msgid) != msgCallbacks.end()) {
            LOGE << "Try to add new mavlink msg callback for component {" << compid << "} to already existent msgid { "<< msgid << "}";
            return ERROR_EXIST;
        }

        if (msgCallbacks.available() == 0) {
            LOGE << "Mavlink msg callback table of component {" << compid << "} is full. Current capacity {" << static_cast <uint32_t> (periodicSendMsgs.capacity()) << "}";
            return ERROR_HAL;
        }

        msgCallbacks.insert(etl::pair(msgid, callback));
        return OK;
    }

    /**
     * @brief Add periodic send msg
     *
     * @param msgid
     * @param sendFunc
     * @param interval default 100ms
     * @return StatusEnum
     */
    StatusEnum AddPeriodicSendMsg(uint8_t msgid, SendMsgFunc &&sendFunc, uint32_t interval_ms = 100) {
        // Scan if we already have this msgid
        for (auto periodicSendMsg : periodicSendMsgs) {
            if (periodicSendMsg.msgid == msgid) {
                LOGE <<
                    "Try to add new mavlink periodic send msg for component {" << compid << "} to already existent msgid {" << msgid << "}";
                return ERROR_EXIST;
            }
        }

        if (periodicSendMsgs.available() == 0) {
            LOGE << "Mavlink periodic send msg table of component {" << compid << "} is full. Current capacity {" << static_cast <uint32_t> (periodicSendMsgs.capacity()) << "}";
            return ERROR_HAL;
        }

        periodicSendMsgs.push_back(PeriodicSendMsg(msgid, std::move(sendFunc), interval_ms));

        // Sort by interval
        std::sort(periodicSendMsgs.begin(), periodicSendMsgs.end(),
                  [](const PeriodicSendMsg &a, const PeriodicSendMsg &b) { return a.interval_ms < b.interval_ms; });

        return OK;
    }

    /**
     * @brief Send periodic msgs, this function need to be called periodically, example in component task process
     *
     */
    void SendPeriodicMsgs();

    uint8_t compid;

    bool connectedPort[NUM_COM_PORTS] = {false};

private:

    static constexpr float MSG_INTERVAL_ADJUSTMENT = 0.98f;

    etl::iunordered_map<uint8_t, MsgCallback> &msgCallbacks;
    etl::ivector<PeriodicSendMsg>             &periodicSendMsgs;

    ComponentIndexEnum componentIndex = MAV_COMP_COUNT;
    bool               isOpen         = false;
};

/* Exported functions --------------------------------------------------------*/

# endif /* CONFIG_MAVLINK */

#endif /* __DRIVER_MAVLINK_INC_DRIVER_MAVLINK_COMPONENT_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
