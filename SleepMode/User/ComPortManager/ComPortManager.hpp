#ifndef __COM_PORT_MANAGER_HPP__
#define __COM_PORT_MANAGER_HPP__

#include "Modules/Ethernet_COM/Ethernet_COM.hpp"
#include "RTOS/FreeRTOS/Inc/free_rtos_mutex.hpp"
#include "Driver/Mavlink/Inc/driver_mavlink_serial_com_port.hpp"
#include "Driver/gLink/Inc/driver_glink.hpp"
#include "StorageParam/StorageParam.hpp"
#include "ImuHandler/ImuHandler.hpp"
#include <span>
#include "wwdg.h"


class ComPortManager {
public:
    ComPortManager()
        : _comm0(MAVLINK_COMM_0, _uart3),
          _comm_drone(MAVLINK_COMM_2, _ethernet),
          _comm_som(MAVLINK_COMM_3, _ethernet),
          _glink_comm0(Driver::GLink::GLINK_COMM_0, _uart4) {}

    ComPortManager(const ComPortManager&) = delete;
    ComPortManager& operator=(const ComPortManager&) = delete;

    static ComPortManager& GetInstance() {
        static ComPortManager instance;
        return instance;
    }

    StatusEnum Open() {
        if (_is_open) {
            return ERROR_EXIST;
        }
        _ethernet.Open();
        _uart4.baudrate = 921600;
        _uart4.txBufferSize = 2048;
        _uart4.readTaskSize = 4096;
        if (_comm0.Open() != OK ||
            _comm_drone.Open() != OK ||
            _comm_som.Open() != OK ||
            _glink_comm0.Open() != OK) {
            return ERROR_HAL;
        }

        _imu_cs.pin = g_hardware_convert_gpio_to_pin_enum(GPIOA, GPIO_PIN_8);
        _imu_cs.levelActive = false;
        _imu_cs.Open();
        _imu_handler.BindImu(&_spi2, &_imu_cs);
        _imu_handler.Open();

        _is_open = true;
        return OK;
    }

    StatusEnum Close() {
        if (!_is_open) {
            return ERROR_NOT_EXIST;
        }
        _imu_handler.Close();
        _imu_cs.Close();
        _comm_som.Close();
        _comm_drone.Close();
        _comm0.Close();
        _glink_comm0.Close();
        _ethernet.Close();
        _is_open = false;
        return OK;
    }

    void ApplyNetworkConfig(const NetworkConfig_t& config) {
        _comm_drone.SetDestIp(config.droneIp);
        _comm_drone.SetDestPort(config.dronePort);
        _comm_drone.SetUdpMode(config.udpModeDrone);
        _comm_som.SetDestIp(config.somIp);
        _comm_som.SetDestPort(config.somPort);
        _comm_som.SetUdpMode(config.udpModeDevice);
        _ethernet.setOwnIp(config.ownIp);
    }

    HW::Peripheral::Uart& Uart4() { return _uart4; }
    HW::Peripheral::Spi& Spi2() { return _spi2; }
    HW::Base::Spi& Spi2Base() { return _spi2; }
    Driver::Mavlink::SerialCOMPort& Comm0() { return _comm0; }
    Driver::Mavlink::UdpCOMPort& Drone() { return _comm_drone; }
    Driver::Mavlink::UdpCOMPort& Som() { return _comm_som; }
    Driver::GLink::SerialCOMPort& GlinkComm0() { return _glink_comm0; }
    Ethernet_COM& Ethernet() { return _ethernet; }
    ImuHandler& Imu() { return _imu_handler; }

private:
    HW::Peripheral::Uart _uart3{UART_COM3, 2048};
    HW::Peripheral::Uart _uart4{UART_COM4, 2048};
    HW::Peripheral::Spi _spi2{SPI_COM2};
    Ethernet_COM _ethernet{_spi2};
    Driver::Mavlink::SerialCOMPort _comm0;
    Driver::Mavlink::UdpCOMPort _comm_drone;
    Driver::Mavlink::UdpCOMPort _comm_som;
    Driver::GLink::SerialCOMPort _glink_comm0;
    ImuHandler _imu_handler;
    HW::Peripheral::Output _imu_cs;
    bool _is_open{false};
};

#endif /* __COM_PORT_MANAGER_HPP__ */
