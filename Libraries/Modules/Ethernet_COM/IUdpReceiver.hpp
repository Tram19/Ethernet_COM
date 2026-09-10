#pragma once
#include <cstdint>



// Ethernet_COM gọi vào đây khi có UDP payload
// UdpCOMPort sẽ implement interface này
class IUdpReceiver {
public:
    virtual ~IUdpReceiver() = default;
    virtual void OnUdpReceived(const uint8_t* data, uint16_t len) = 0;
};

