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

#ifndef __SLEEPMODE_COM_HPP__
#define __SLEEPMODE_COM_HPP__

#include "main.h"
#include "gremsy.hpp"
#include "gremsy_include.hpp"
#include "Driver/Mavlink/Inc/driver_mavlink.hpp"
#include "ComPortManager/ComPortManager.hpp"
#include "Modules/SignalClone/Inc/module_signal_clone.hpp"
#include "RTOS/FreeRTOS/Inc/free_rtos_mutex.hpp"
using namespace Driver::Mavlink;

class IOExpander;





/* Exported macro ------------------------------------------------------------*/
#define SOM_SLEEP_CMD_FLAG      0x01
#define SOM_HB_FLAG             0x02
#define SOM_ON_CMD_FLAG			0x04
#define SOM_SWITCH_GPIO_PORT    GPIOC
#define SOM_SWITCH_GPIO_PIN     GPIO_PIN_13
#define SOM_SLEEP_TIMEOUT_MS    15000U
#define SOM_WAKE_TIMEOUT_MS     20000U
#define SOM_HEARTBEAT_TIMEOUT_MS 3000U
#define SOM_CURRENT_OFF_THRESHOLD_mA 50U

/* Exported class ------------------------------------------------------------*/

/// @brief Power manager for the SOM sleep and wake state machine.
class SleepMode_Com 
{
public:

    enum class PowerAction : uint8_t {
        Sleep,
        Wake,
        PW_OFF,
        PW_ON,
    };
    enum class AckResult : uint8_t {
        InProgress,
        Accepted,
        Failed,
    };
    using AckCallback = bool (*)(void* context, PowerAction action, AckResult result);
    IOExpander& GetIOExpander();
    SleepMode_Com() = default;
    SleepMode_Com(SleepMode_Com const&)            = delete;
    SleepMode_Com& operator=(SleepMode_Com const&) = delete;
    /// TYPEDEF ________________________________________________________________
    enum SOM_STATE : uint8_t
    {
        SOM_IDLE    = 0,
        SOM_ON      = 1,
        SOM_SLEEP   = 2,
        SOM_PWOFF   = 3,
		SOM_PWON	= 4,
        SOM_DONE    = 5,
        SOM_ERROR   = 6,
        SOM_OFF     = 7,
        SOM_WAIT_HEARTBEAT = 8,
        SOM_WAIT_POWEROFF = 9,
        SOM_WAIT_WAKEUP = 10,
    };
    /// FUNCTION _______________________________________________________________
    /**
     * @brief Task process.
     */
   // static SleepMode_Com& GetInstance() { return instance; }

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

    StatusEnum  _mainTaskFunc();
    StatusEnum RequestPower(PowerAction action, AckCallback callback, void* context);
    StatusEnum ForceWakeUp();
    StatusEnum ForcePowerOn();
    StatusEnum ForcePowerOff();
    StatusEnum ForceSleep();
    StatusEnum ForceReboot();
    void Heartbeat_Som() {
        _last_som_heartbeat_ms = osKernelGetTickCount();
        _som_heartbeat_seen = true;
        _Sleep_Main_Task.Set(SOM_HB_FLAG);
    }

    void Switch_UART_SOM(){
        if(SWITCH_UART_SOM == 1){
            HAL_GPIO_WritePin(GPIOC, SEL_MAV_Pin, GPIO_PIN_SET);
        }
    }
    void Switch_UART_MCU(){
        if(SWITCH_UART_SOM == 1){
            HAL_GPIO_WritePin(GPIOC, SEL_MAV_Pin, GPIO_PIN_RESET);
        }
    }

private:
    struct PendingAck {
        AckCallback callback{nullptr};
        void* context{nullptr};
        PowerAction action{PowerAction::Sleep};
    };

    bool _notify_ack(AckResult result) {
        if (_pending_ack.callback == nullptr) {
            return false;
        }
        const PendingAck pending = _pending_ack;
        if (result != AckResult::InProgress) {
            _pending_ack = {};
            _som_state = result == AckResult::Accepted
                ? (pending.action == PowerAction::Sleep ? SOM_OFF : SOM_ON)
                : (pending.action == PowerAction::Sleep ? SOM_ON : SOM_OFF);
        }
        if (pending.callback != nullptr) {
            return pending.callback(pending.context, pending.action, result);
        }
        return false;
    }

    void _handle_som_heartbeat() {
        if (_som_state == SOM_IDLE || _som_state == SOM_WAIT_HEARTBEAT ||
            _som_state == SOM_WAIT_WAKEUP) {
            if (_pending_ack.action == PowerAction::Wake) {
                bool ack_sent = _notify_ack(AckResult::Accepted);
                if (ack_sent) {
                    g_os_delay(100);
                }
            } else {
                _som_state = SOM_ON;
            }
            LOGW<<"switch uart to som"<<LEND;
            Switch_UART_SOM();
        }
    }

    /// PARAM __________________________________________________________________
    HW::Peripheral::Time time;


    SOM_STATE           _som_state{SOM_IDLE};
    PendingAck          _pending_ack{};
    bool                isOpen{false};
    uint32_t _last_som_heartbeat_ms{0};
    uint32_t _last_pw_monitor_ms = 0;
    uint32_t _poweroff_started_ms{0};
    uint32_t _wakeup_started_ms{0};
    bool _poweroff_started{false};
    bool _som_heartbeat_seen{false};

    /// FUNCTION _______________________________________________________________

 
    static constexpr uint32_t MAVLINKCOM_TASK_STACK_SIZE = 4096U;

    RTOS::TaskStatic<MAVLINKCOM_TASK_STACK_SIZE> _Sleep_Main_Task{
        PRIORITY_NORMAL,
        [this]() { this->_mainTaskFunc(); },
        "mavlink com task"
    };    
};

#endif /* __SLEEPMODE_COM_HPP__ */
