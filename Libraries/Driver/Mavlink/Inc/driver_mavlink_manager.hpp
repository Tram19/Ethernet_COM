/**
 ******************************************************************************
 * @file    driver_mavlink_manager.hpp
 * @author  Gremsy Team
 * @version V2.0.0
 * @date    Nov 18, 2024
 * @brief   This file contains all the functions prototypes for the driver_mavlink_manager.cpp
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

#ifndef __DRIVER_MAVLINK_INC_DRIVER_MAVLINK_MANAGER_HPP__
# define __DRIVER_MAVLINK_INC_DRIVER_MAVLINK_MANAGER_HPP__

/* Includes ------------------------------------------------------------------*/

# include "driver_mavlink_include.hpp"

# if (CONFIG_MAVLINK == 1)

//#  include <tuple>
//#  include <span>
#include "etl/tuple.h"
#include "etl/span.h"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

class Driver::Mavlink::Manager {
public:

    /**
     * @brief Configure whole system as a master
     *
     * @param[in] _sysid System ID of this node
     *
     * This function will reset the MAVLink system and set the system ID to the given value.
     * It will also set the isMaster flag to true.
     */
    void ConfigMaster(uint8_t _sysid) {
        ResetSystem();
        SetSysid(_sysid);
        isMaster = true;
    }

    /**
     * @brief Get the Instance object
     *
     * @return Manager&
     */
    static Manager &GetInstance() { return instance; }

    /**
     * @brief Add a component to the manager
     *
     * @param _component
     * @return etl::tuple<StatusEnum, ComponentIndexEnum>
     */
    etl::tuple<StatusEnum, ComponentIndexEnum> AddComponent(Component &_component);

    /**
     * @brief Remove a component from the manager
     *
     * @param index
     * @return StatusEnum
     */
    StatusEnum RemoveComponent(ComponentIndexEnum index);

    /**
     * @brief Add a COM port to the manager
     *
     * @param _comPort
     * @param port
     * @return StatusEnum
     */
    StatusEnum AddCOMPort(COMPortBase &_comPort, mavlink_channel_t port);

    /**
     * @brief Remove a COM port from the manager
     *
     * @param _comPort
     * @return StatusEnum
     */
    StatusEnum RemoveCOMPort(mavlink_channel_t port);

    /**
     * @brief driver manager open
     *
     */
    StatusEnum Open();

    /**
     * @brief Callback when parse a message successfully
     *
     * @param msg
     */
    void ParseMessageCallback(mavlink_channel_t port, const mavlink_message_t &msg);

    /**
     * @brief Get the Sysid of whole system
     *
     * @return uint8_t
     */
    uint8_t GetSysid() const { return sysid; }

    /**
     * @brief Reset the system
     *
     * This function resets all the internal state of the manager.
     * This will reset all the components and the system id.
     */
    void ResetSystem();

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
    bool SendBuff(mavlink_channel_t port, std::span<const uint8_t> buff);

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
    bool SendMessage(mavlink_channel_t port, const mavlink_message_t &msg);


    /**
     * @brief Check if the manager is initialized
     *
     * @return true
     * @return false
     */
    bool IsInitialized() const { return isInitialized; }


private:

    struct Route {
        mavlink_system_t  component;
        mavlink_channel_t port;
        uint8_t           mavtype;
        bool              isPrivate;
    };

    struct TxRequest {
    mavlink_channel_t port;
    mavlink_message_t message;
    };

    Manager() = default;

    ~Manager() {
        for (auto route : routes) {
            if (route != nullptr) {
                delete route;
            }
        }
    }

    // Disable copy constructor and assignment operator
    Manager(const Manager &)            = delete;
    Manager &operator=(const Manager &) = delete;

    /**
     * @brief Check if the message should be forwarded to other ports or processed locally
     *
     * @param port
     * @param msg
     * @return etl::tuple<bool, ComponentIndexEnum> true if the message should be processed locally, and index of the
     * component to which the message should be processed
     */
    etl::tuple<bool, ComponentIndexEnum> CheckAndForward(mavlink_channel_t port, const mavlink_message_t &msg);

    /**
     * @brief Learn new route to the manager
     *
     * @param port
     * @param msg
     * @return int8_t route index where msg came from
     */
    int8_t LearnRoute(mavlink_channel_t port, const mavlink_message_t &msg);

    /**
     * @brief Handle broadcast msg, forward to other ports
     *
     * @param port
     * @param msg
     */
    void HandleBroadcast(mavlink_channel_t port, const mavlink_message_t &msg);

    /**
     * @brief Get the Targets of the message
     *
     * @param msg
     * @return etl::tuple<bool, mavlink_system_t> true if msg is broadcast, and target system
     */
    etl::tuple<bool, mavlink_system_t> GetTargets(const mavlink_message_t &msg);

    /**
     * @brief Handle heartbeat msg
     *
     * @param msg
     */
    void HandleHeartbeat(const mavlink_message_t &msg);

    StatusEnum SetSysid(uint8_t _sysid) {
        if (isInitialized) {
            return ERROR_EXIST;
        }

        sysid         = _sysid;
        isInitialized = true;
        return OK;
    }

    /**
     * @brief Check if the message is from a master candidate
     *
     * @param msg
     * @return true
     * @return false
     */
    bool IsMasterCandidate(const mavlink_message_t &msg);

    static constexpr uint8_t MAX_ROUTES    = 10;
    static constexpr uint8_t DEFAULT_SYSID = 1;

    static Manager instance;

    // Default sysid is 1, based on isMaster flag
    uint8_t sysid         = DEFAULT_SYSID;
    bool    isInitialized = false;  // Flag to indicate if the sysid has been initialized

    // Flag to indicate if the system is master or slave. If master the user have to ConfigMaster() with the sysid.
    // Otherwise it is default to slave, which will adapte to the sysid of the autopilot or other control components
    bool isMaster = false;

    std::array<uint8_t, MAVLINK_MAX_PACKET_LEN> sendBuff;

    std::array<Component *, MAV_COMP_COUNT>  components;
    std::array<COMPortBase *, NUM_COM_PORTS> comPorts;

    std::array<Route *, MAX_ROUTES> routes;
    uint8_t                         numRoutes = 0;

    //--- Tx message queue and task---------------------------------
    StatusEnum _txTaskFunc();

    RTOS::TaskStatic<2048> _txTask{
        PRIORITY_NORMAL,
        [this]() { this->_txTaskFunc(); },
        "ManagerTX"
    };

    static constexpr uint8_t MAX_TX_MESSAGES = 8;
    RTOS::MessageQueueStatic<TxRequest, MAX_TX_MESSAGES> _txQueue{"TXQUEUE"};
    
    //--------------------------------------------------------------


};

/* Exported functions --------------------------------------------------------*/

# endif /* CONFIG_MAVLINK */

#endif /* __DRIVER_MAVLINK_INC_DRIVER_MAVLINK_MANAGER_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
