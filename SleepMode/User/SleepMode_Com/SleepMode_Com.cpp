/*
 * SleepMode_Com.cpp
 *
 *  Created on: May 13, 2026
 *      Author: USER
 */
#include "SleepMode_Com.hpp"
#include "Modules/Ethernet_COM/Ethernet_COM.hpp"
#include "StorageParam/StorageParam.hpp"
#include "Hardware/Stm32U5/Inc/hardware_u5_peripheral_io.hpp"
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

HW::Peripheral::I2c		    _i2c1;
IOExpander 			 	    _ioexpander;
PowerManager 			    _powermanager;

enum class AppState : uint8_t {
    WAIT_AIRCRAFT,
    WAIT_GIMBAL,
    WAIT_CAMERA,
    GET_INFOR,
    READY,
    ERROR,
};
static AppState _app_state{AppState::WAIT_AIRCRAFT};

/* ---------------------------------------------------------------------------
 * Open / Close
 * ---------------------------------------------------------------------------*/
StatusEnum SleepMode_Com::Open()
{
    if (isOpen) {
        LOGE<<"Trying to open an already open sleep mode task"<<LEND;
        return ERROR_EXIST;
    }
    HAL_WWDG_Refresh(&hwwdg);

	_i2c1.deviceAddress = PI4IOE_ADDR_LOW;
	_i2c1.id 			= I2C_COM1;
	_i2c1.Open();

	HAL_WWDG_Refresh(&hwwdg);

	_ioexpander.pi2c    = (&_i2c1);
	_ioexpander.Open();

	HAL_WWDG_Refresh(&hwwdg);

	_powermanager.pExpander = &_ioexpander;
	_powermanager.Open();


	HAL_WWDG_Refresh(&hwwdg);
    
    // Main task
    if (_Sleep_Main_Task.Open() != OK) {
        LOGE << "Main task Open FAIL" <<LEND;
        return ERROR_HAL;
    }
    LOGI<<"Sleep mode main task Open"<<LEND;
    isOpen = true;
    _som_state = SOM_IDLE;
    time   = TIME_MS;

    return OK;
}

StatusEnum SleepMode_Com::Close()
{
    if (!isOpen) {
        LOGE<<"Trying to close an already closed bootloader"<<LEND;
        return ERROR_NOT_EXIST;
    }
    if (_Sleep_Main_Task.Close() != OK) {
        LOGE << "Main task Close FAIL" <<LEND;
        return ERROR_HAL;
    }
    
    isOpen = false;
    return OK;
}

StatusEnum SleepMode_Com::RequestPower(PowerAction action, AckCallback callback, void* context) {
    if (callback == nullptr) {
        return ERROR_EXIST;
    }
    _pending_ack = {callback, context, action};
    _Sleep_Main_Task.Set(action == PowerAction::Sleep
        ? SOM_SLEEP_CMD_FLAG
        : SOM_ON_CMD_FLAG);
    return OK;
}

StatusEnum SleepMode_Com::ForceWakeUp()
{
    const StatusEnum result = _powermanager.WakeUpSom();
    if (result == OK) {
        _som_state = SOM_ON;
    }
    return result;
}

StatusEnum SleepMode_Com::ForcePowerOn()
{
    const StatusEnum result = _powermanager.PowerOn();
    if (result == OK) {
        _som_state = SOM_ON;
    }
    return result;
}

StatusEnum SleepMode_Com::ForcePowerOff()
{
    const StatusEnum result = _powermanager.PowerOff();
    if (result == OK) {
        Switch_UART_MCU();
        _som_state = SOM_OFF;
    }
    return result;
}
StatusEnum SleepMode_Com::ForceSleep()
{
    const StatusEnum result = _powermanager.SleepSom();
    if (result == OK) {
        Switch_UART_MCU();
        _som_state = SOM_SLEEP;
    }
    return result;
}
StatusEnum SleepMode_Com::ForceReboot()
{
    const StatusEnum result = _powermanager.PowerOff();
    if (result == OK) {
        g_os_delay(1000);
        const StatusEnum result2 = _powermanager.PowerOn();
        if (result2 == OK) {
            _som_state = SOM_IDLE;
        }
        return result2;
    }
    return result;
}
IOExpander& SleepMode_Com::GetIOExpander()
{
    return _ioexpander;
}

/* ---------------------------------------------------------------------------
 * Private helpers
 * ---------------------------------------------------------------------------*/



/// @brief Main state machine
StatusEnum SleepMode_Com::_mainTaskFunc()
{
    g_os_delay(1);
    if (!__HAL_RCC_GET_FLAG(RCC_FLAG_WWDGRST)) _powermanager.PowerOn();
    g_os_delay(1);
    g_os_delay(10);



  while(1) {
    StorageManager::GetInstance().Process();
    _powermanager.ReadCurrent12vSys();

    uint32_t now = osKernelGetTickCount();
    if ((now - _last_pw_monitor_ms) >= 3000) {
        LOGS<<"PW : "<<_powermanager.GetCurrent12vSys()<<"mA"<<LEND; 
        _last_pw_monitor_ms = now;
    }

    uint32_t rs = _Sleep_Main_Task.Wait(SOM_SLEEP_CMD_FLAG | SOM_HB_FLAG | SOM_ON_CMD_FLAG, 1);
    if ((int32_t)rs >= 0)
    {
        // --- Nhận heartbeat từ SOM ---
        if (rs & SOM_HB_FLAG)
        {
               _handle_som_heartbeat();
        }

        // --- Nhận lệnh sleep ---
        if (rs & SOM_SLEEP_CMD_FLAG)
        {
            if (_som_state == SOM_IDLE || _som_state == SOM_ON)
            {
                Switch_UART_MCU();
                _som_state = SOM_SLEEP;
            }
            else if (_som_state == SOM_SLEEP || _som_state == SOM_WAIT_POWEROFF)
            {
                LOGI << "SOM sleep already in progress" << LEND;
                _notify_ack(AckResult::InProgress);
            }
            else
            {
                LOGE << "Cannot sleep SOM in state " << static_cast<int>(_som_state) << LEND;
                _notify_ack(AckResult::Failed);
            }
        }

        // --- Nhận lệnh on ---
        if (rs & SOM_ON_CMD_FLAG)
        {
            if (_som_state == SOM_IDLE || _som_state == SOM_OFF)
            {
                _som_state = SOM_PWON;
            }
            else if (_som_state == SOM_PWON || _som_state == SOM_WAIT_HEARTBEAT ||
                     _som_state == SOM_WAIT_WAKEUP)
            {
                LOGI << "SOM wakeup already in progress" << LEND;
                _notify_ack(AckResult::InProgress);
            }
            else
            {
                _notify_ack(AckResult::Failed);
            }
        }
    }

    // --- State machine xử lý ---
    switch (_som_state)
    {
        case SOM_IDLE:
        {
        	//LOGI<<"SOM state idle"<<LEND;
        }
        break;

        case SOM_ON:
            break;

        case SOM_OFF:
            break;
        case SOM_PWON:
        {
        	LOGW<<"PW ON SOM"<<LEND;
            _wakeup_started_ms = osKernelGetTickCount();
            _som_heartbeat_seen = false;
            const StatusEnum result = _powermanager.PowerOn();
            LOGI << "PowerOn result: " << static_cast<int>(result) << LEND;
            if (result == OK) {
                _notify_ack(AckResult::InProgress);
                _som_state = SOM_WAIT_WAKEUP;
            } else {
                _som_state = SOM_ERROR;
            }
        }
        break;
        case SOM_WAIT_HEARTBEAT:
        {
            // Power rails are on, but SOM must send heartbeat before wake ACK.
        }
        break;
        case SOM_WAIT_WAKEUP:
        {
            const uint32_t now = osKernelGetTickCount();
            if ((now - _wakeup_started_ms) <= SOM_WAKE_TIMEOUT_MS) {
                _powermanager.ReadCurrent12vSys();
                const bool current_is_high =
                    _powermanager.GetCurrent12vSys() > (SOM_CURRENT_OFF_THRESHOLD_mA*2);

                if (_som_heartbeat_seen && current_is_high) {
                    LOGI << "SOM wakeup confirmed" << LEND;
                    _som_state = SOM_DONE;
                }
            }
            else {
                LOGE << "SOM wakeup timeout" << LEND;
                _som_state = SOM_ERROR;
            }
        }
        break;
        case SOM_SLEEP:
        {
        	LOGI<<"Som state sleep"<<LEND;
			_notify_ack(AckResult::InProgress);
			_poweroff_started_ms = osKernelGetTickCount();
			_poweroff_started = false;
			_som_state = SOM_WAIT_POWEROFF;
        }
        break;

        case SOM_WAIT_POWEROFF:
        {
            const uint32_t now = osKernelGetTickCount();
            uint32_t ack_in_progress =0;

            if ((now - _poweroff_started_ms) >= SOM_SLEEP_TIMEOUT_MS){
                const bool som_is_sleeping = 
                    _som_heartbeat_seen &&
                    (now - _last_som_heartbeat_ms) >= SOM_HEARTBEAT_TIMEOUT_MS;

                _powermanager.ReadCurrent12vSys();
                const bool current_is_off =
                    _powermanager.GetCurrent12vSys() < SOM_CURRENT_OFF_THRESHOLD_mA;

                if (som_is_sleeping && current_is_off && !_poweroff_started) {
                    LOGW << "SOM sleep confirmed, powering off auxiliary rails" << LEND;
                    const StatusEnum result = _powermanager.SleepSom();
                    LOGI << "PowerOff result: " << static_cast<int>(result) << LEND;
                    if (result != OK) {
                        LOGE<<"PowerOff failed"<<LEND;
                        _som_state = SOM_ERROR;
                        break;
                    }
                    _poweroff_started = true;
                    _som_state = SOM_DONE;
                }
                else {
                    LOGE << "SOM sleep timeout" << LEND;
                    _som_state = SOM_ERROR;
                }
            }
        }
        break;

        case SOM_DONE:
        {
            
            _notify_ack(AckResult::Accepted);
            g_os_delay(100);
        }
        break;

        case SOM_ERROR:
        {
            _notify_ack(AckResult::Failed);
        }
        break;

        default:
            break;
    }
}

}

