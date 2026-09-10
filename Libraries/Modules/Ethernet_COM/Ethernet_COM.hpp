/**
 * @file Ethernet_COM.hpp
 * @brief Ethernet communication module interface
 * @author Gremsy Team
 * @date May 28, 2026
 */

#ifndef ENTHERNET_COM_ENTHERNET_COM_HPP_
#define ENTHERNET_COM_ENTHERNET_COM_HPP_

#include <gremsy_include.hpp>

#ifdef GREMSY_MODULE_ETHERNET
#include <Modules/ENC28J60/enc28j60.hpp>
#include <Modules/Ethernet_COM/common.hpp>
#include <Modules/Ethernet_COM/IUdpReceiver.hpp>
#include <mutex>
/* Function declarations */

#define ETH_FRAME_TYPE_ARP                                      0x0806
#define ETH_FRAME_TYPE_IP                                       0x0800

#define ARP_OP_CODE_REQUEST                                     0x0001
#define ARP_OP_CODE_RESPONSE                                    0x0002

#define ICMP_FRAME_TYPE_ECHO_REQUEST	                        0x08
#define ICMP_FRAME_TYPE_ECHO_REPLY	                            0x00

#define IP_FRAME_PROTOCOL_ICMP                                  0x01
#define IP_FRAME_PROTOCOL_TCP                                   0x06
#define IP_FRAME_PROTOCOL_UDP                                   0x11
#define UDP_DEMO_PORT                                           14550


typedef struct ETH_Frame
{
  uint8_t destMacAddr[MAC_ADDRESS_BYTES_NUM];
  uint8_t srcMacAddr[MAC_ADDRESS_BYTES_NUM];
  uint16_t etherType;
  uint8_t data[];
} ETH_Frame;

typedef struct ARP_Frame
{
  uint16_t hType;
  uint16_t pType;
  uint8_t hLen;
  uint8_t pLen;
  uint16_t opCode;
  uint8_t srcMacAddr[MAC_ADDRESS_BYTES_NUM];
  uint8_t srcIpAddr[IP_ADDRESS_BYTES_NUM];
  uint8_t destMacAddr[MAC_ADDRESS_BYTES_NUM];
  uint8_t destIpAddr[IP_ADDRESS_BYTES_NUM];
} ARP_Frame;

typedef struct __attribute__((packed)) IP_Frame {
  uint8_t  verHeaderLen;
  uint8_t  diffServices;
  uint16_t len;
  uint16_t fragId;
  uint16_t fragOffset;
  uint8_t  timeToLive;
  uint8_t  protocol;
  uint16_t checkSum;
  uint8_t  srcIpAddr[4];
  uint8_t  destIpAddr[4];
  uint8_t  data[]; // Flexible array member
} IP_Frame;

typedef struct ICMP_EchoFrame
{
  uint8_t type;
  uint8_t code;
  uint16_t checkSum;
  uint16_t id;
  uint16_t seqNum;
  uint8_t data[];
} ICMP_EchoFrame;

typedef struct UDP_Frame
{
  uint16_t srcPort;
  uint16_t destPort;
  uint16_t len;
  uint16_t checkSum;
  uint8_t data[];
} UDP_Frame;

enum class UdpMode : uint8_t {
        CLIENT = 0,   // biết IP đích trước, gửi thẳng
        SERVER = 1,   // học IP từ packet nhận được
        BROADCAST = 2 // gửi đến broadcast address, không cần biết IP đích
};

bool _ParseIpString(const char* str, uint8_t ip[4]);
class Ethernet_COM {
public:
/**
 * @brief Initialize Ethernet communication module
 * Call this once at startup to initialize the ENC28J60 chip
 */

explicit Ethernet_COM(HW::Peripheral::Spi& spi);
void Open(void);
void Close(void);

// UdpCOMPort gọi lúc khởi tạo để đăng ký nhận UDP
bool RegisterUdpReceiver(uint16_t port, IUdpReceiver* receiver);
void UnregisterUdpReceiver(uint16_t port);

// UdpCOMPort::SinkIt() gọi để gửi MAVLink packet ra ngoài
// destIp/destPort lấy từ config (fixed/broadcast)
StatusEnum SendUdp(const uint8_t destIp[4], uint16_t listenPort, UdpMode udpmode,
               const uint8_t* data, uint16_t len);

/**
 * @brief Ethernet communication task
 * Call this periodically from main loop or RTOS task
 * Processes incoming frames and handles network protocols
 */
StatusEnum EthernetMainTask();

/**
 * @brief Send raw data over Ethernet to a specific IP address
 * @param destIp Destination IP address (4 bytes array)
 * @param data Data buffer to send
 * @param length Length of data in bytes
 */
void Ethernet_COM_SendData(uint8_t* destIp, uint8_t* data, uint16_t length);

static constexpr uint32_t ETHERNET_TASK_STACK_SIZE = 5048U;

RTOS::TaskStatic<ETHERNET_TASK_STACK_SIZE> _Ethernet_Main_Task{
        PRIORITY_NORMAL,
        [this]() { this->EthernetMainTask(); },
        "ethernet com task"
};

void setOwnIp(const uint8_t ip[4]) {
    memcpy(_ownIpAddr, ip, 4);
}

// Cho phép tầng trên set cứng nếu cần
void SetPeer(uint16_t localPort, const uint8_t ip[4], uint16_t peerPort) {
    std::lock_guard<RTOS::StaticMutex> lock(_mutex);
    for (uint8_t i = 0; i < udpReceiverCount_; i++) {
        if (udpReceivers_[i].port == localPort) {
            memcpy(udpReceivers_[i].peerIp, ip, 4);
            udpReceivers_[i].peerPort   = peerPort;
            udpReceivers_[i].peerKnown  = true;
            udpReceivers_[i].peerLocked = true;
            break;
        }
    }
}
void UnlockPeer(uint16_t localPort) {
    std::lock_guard<RTOS::StaticMutex> lock(_mutex);
    for (uint8_t i = 0; i < udpReceiverCount_; i++) {
        if (udpReceivers_[i].port == localPort) {
            udpReceivers_[i].peerLocked = false;
            break;
        }
    }
}

private:

    void     ETH_Process();
    void 	 ETH_Response(ETH_Frame* ethFrame, uint16_t len);

    uint16_t ARP_Process(ARP_Frame* frame, uint16_t len);
    uint16_t IP_Process(IP_Frame* frame, uint16_t len);
    uint16_t IP_CalcCheckSum(uint8_t* data, uint16_t len);
    uint16_t ICMP_Process(ICMP_EchoFrame* frame, uint16_t len);
    uint16_t UDP_Dispatch(IP_Frame* ipFrame, UDP_Frame* frame, uint16_t len);

    ENC28J60             enc_;       // owns driver
    RTOS::Task           task_;

    static constexpr uint8_t MAX_UDP_RECEIVERS = 4;
    struct UdpEntry {
        uint16_t      port = 0;             // Local port board to listen on
        IUdpReceiver* receiver = nullptr;
        uint8_t       peerIp[4] = {};       // Peer IP address to send to (learned from incoming packet or set by user)
        uint16_t      peerPort = 0;         // Peer port to send to (learned from incoming packet or set by user)
        bool          peerKnown = false;
        bool          peerLocked = false;
    };
    UdpEntry udpReceivers_[MAX_UDP_RECEIVERS] = {};
    uint8_t  udpReceiverCount_ = 0;

    ENC28J60_Frame rxFrame_;
    uint16_t listenPort_ = UDP_DEMO_PORT;

    // Network config
    uint8_t ownMac_[6] = {};
    uint8_t _ownIpAddr[4] = {};
    uint8_t destMac_[6] = {};

    mutable RTOS::StaticMutex _mutex;
};
#endif /* GREMSY_MODULE_ETHERNET */
#endif /* ENTHERNET_COM_ENTHERNET_COM_HPP_ */
