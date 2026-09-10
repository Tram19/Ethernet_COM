#ifndef __COM_PORT_MANAGER_HPP__
#define __COM_PORT_MANAGER_HPP__

#include "Modules/Ethernet_COM/Ethernet_COM.hpp"
#include "RTOS/FreeRTOS/Inc/free_rtos_mutex.hpp"
#include "Driver/Mavlink/Inc/driver_mavlink_serial_com_port.hpp"
#include "StorageParam/StorageParam.hpp"
#include <span>
#include "wwdg.h"


class ComPortManager {
public:
    ComPortManager()
        : _comm0(MAVLINK_COMM_0, _uart3),
          _comm_drone(MAVLINK_COMM_2, _ethernet),
          _comm_som(MAVLINK_COMM_3, _ethernet),
          _comm1(MAVLINK_COMM_1, _uart4) {}

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
            _comm1.Open() != OK) {
            return ERROR_HAL;
        }


        _is_open = true;
        return OK;
    }

    StatusEnum Close() {
        if (!_is_open) {
            return ERROR_NOT_EXIST;
        }
        _comm_som.Close();
        _comm_drone.Close();
        _comm0.Close();
        _comm1.Close();
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
    Driver::Mavlink::SerialCOMPort& GlinkComm0() { return _comm1; }
    Ethernet_COM& Ethernet() { return _ethernet; }

private:
    HW::Peripheral::Uart _uart3{UART_COM3, 2048};
    HW::Peripheral::Uart _uart4{UART_COM4, 2048};
    HW::Peripheral::Spi _spi2{SPI_COM2};
    Ethernet_COM _ethernet{_spi2};
    Driver::Mavlink::SerialCOMPort _comm0;
    Driver::Mavlink::UdpCOMPort _comm_drone;
    Driver::Mavlink::UdpCOMPort _comm_som;
    Driver::Mavlink::SerialCOMPort _comm1;
    bool _is_open{false};
};

#endif /* __COM_PORT_MANAGER_HPP__ */
