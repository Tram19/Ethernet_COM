
/**
 * @file Ethernet_COM.cpp
 * @brief Ethernet communication module implementation
 * @author Gremsy Team
 * @date May 28, 2026
 * 
 * This module provides high-level Ethernet communication interface
 * using ENC28J60 Ethernet chip connected via SPI to STM32U5 MCU
 */

#include <Modules/Ethernet_COM/Ethernet_COM.hpp>
#include <Modules/ENC28J60/enc28j60.hpp>
#include <Modules/Ethernet_COM/common.hpp>

#ifdef GREMSY_MODULE_ETHERNET

/* Private variables ----------------------------------------------------------*/

// Buffer for receiving Ethernet frames
static ENC28J60_Frame eth_rx_frame;

// Flag to track initialization state
static bool ethernet_initialized = false;
uint32_t _last_hb_eth_log_ms = 0;
/* Public functions -----------------------------------------------------------*/

// ── Helper parse "string" → uint8_t[4] ────────────────────────────
bool _ParseIpString(const char* str, uint8_t ip[4]) {
    int a, b, c, d;
    if (sscanf(str, "%d.%d.%d.%d", &a, &b, &c, &d) != 4) return false;
    if (a < 0 || a > 255 || b < 0 || b > 255 ||
        c < 0 || c > 255 || d < 0 || d > 255) return false;
    ip[0] = a; ip[1] = b; ip[2] = c; ip[3] = d;
    return true;
}
/**
 * @brief Initialize Ethernet communication module
 * 
 * This function initializes the ENC28J60 Ethernet chip via SPI and
 * configures network parameters (MAC address, IP address, subnet mask).
 * 
 * Call this function once during system startup before any Ethernet
 * communication is attempted.
 */
Ethernet_COM::Ethernet_COM(HW::Peripheral::Spi& spi)
    : enc_(spi)
{
}
void Ethernet_COM::Open(void)
{
  if (ethernet_initialized) {
    return;  // Already initialized
  }
 if (memcmp(_ownIpAddr, "\0\0\0\0", 4) == 0) {
     // IP address not set, use default
     memcpy(_ownIpAddr, ipAddr, IP_ADDRESS_BYTES_NUM);
      LOGW << "Own IP not set, using default: "
            << (int)_ownIpAddr[0] << "." << (int)_ownIpAddr[1] << "."
            << (int)_ownIpAddr[2] << "." << (int)_ownIpAddr[3] << LEND;
 }

 LOGI << "Initializing Ethernet COM with IP: "
      << (int)_ownIpAddr[0] << "." << (int)_ownIpAddr[1] << "."
      << (int)_ownIpAddr[2] << "." << (int)_ownIpAddr[3] << LEND;
  // Initialize ENC28J60 Ethernet chip
  if (!enc_.Init()) {
      LOGS << "Failed to initialize ENC28J60" << LEND;
      return;
  }
  LOGS << "ENC28J60 initialized successfully" << LEND;
  
  _Ethernet_Main_Task.Open();
  
  _mutex.Open();
  // Mark as initialized
  ethernet_initialized = true;
}

void Ethernet_COM::Close(void)
{
  if (!ethernet_initialized) {
    return;
  }
  LOGI<<"CLOSE ETHERNET COM"<<LEND;
  _Ethernet_Main_Task.Close();
  _mutex.Close();
  ethernet_initialized = false;
}

/**
 * @brief Ethernet communication task
 * 
 * This function processes incoming Ethernet frames and handles network
 * protocols. Call periodically from main loop (every 10-100ms).
 * 
 * Supports: ARP, IP, ICMP protocols
 */
StatusEnum Ethernet_COM::EthernetMainTask()
{
	while(1)
	{
		  if (!ethernet_initialized) {
			  g_os_delay(1000);
			  continue;
		  }

		  ETH_Process();

      uint32_t now = osKernelGetTickCount();
      if ((now - _last_hb_eth_log_ms) >= 5000U) {
        // LOGS<<"enc ID: "<<enc_.GetRevisionID()<<LEND;
        _last_hb_eth_log_ms = now;
      }

		  g_os_delay(10);
	}

	return OK;
}

void Ethernet_COM::ETH_Process() {
  uint16_t responseSize = 0;
  uint16_t requestSize = enc_.ReceiveFrame(&rxFrame_);

  if (requestSize > 0)
  {
    ETH_Frame* ethFrame = (ETH_Frame*)rxFrame_.data;
    uint16_t etherType = ntohs(ethFrame->etherType);
    uint16_t ethDataLen = requestSize - sizeof(ETH_Frame);
    
    // ARP protocol
    if (etherType == ETH_FRAME_TYPE_ARP)
    {
      responseSize = ARP_Process((ARP_Frame*)ethFrame->data, ethDataLen); // dùng để chuyển đổi IP thành MAC, trả lời các yêu cầu ARP (khi may tinh ping toi) từ các thiết bị khác trong mạng LAN, và duy trì bảng ARP để lưu trữ thông tin về các thiết bị đã biết trong mạng.
    }
    
    // IP protocol
    else if (etherType == ETH_FRAME_TYPE_IP)
    {
      responseSize = IP_Process((IP_Frame*)ethFrame->data, ethDataLen); // dùng để xử lý các gói tin IP, bao gồm ICMP (ping), UDP, TCP
    }

    if (responseSize > 0)
    {
      ETH_Response(ethFrame, responseSize);
    }
  }
}

/*----------------------------------------------------------------------------*/
void Ethernet_COM::ETH_Response(ETH_Frame* ethFrame, uint16_t len)
{
  std::lock_guard<RTOS::StaticMutex> lock(_mutex);
  memcpy(ethFrame->destMacAddr, ethFrame->srcMacAddr, MAC_ADDRESS_BYTES_NUM);
  memcpy(ethFrame->srcMacAddr, macAddr, MAC_ADDRESS_BYTES_NUM);
  
  // len: data length, not including Ethernet header
  // Total frame length = Ethernet header (14 bytes) + data length
  enc_.TransmitFrame((uint8_t*)ethFrame, len + sizeof(ETH_Frame));
}


//--- Xử lý ARP request và tạo ARP reply
uint16_t Ethernet_COM::ARP_Process(ARP_Frame* arpFrame, uint16_t frameLen)
{
  uint16_t newFrameLen = 0;
  
  if (memcmp(arpFrame->destIpAddr, _ownIpAddr, IP_ADDRESS_BYTES_NUM) == 0)
  {
    if (arpFrame->opCode == ntohs(ARP_OP_CODE_REQUEST))
    {
      memcpy(arpFrame->destMacAddr, arpFrame->srcMacAddr, MAC_ADDRESS_BYTES_NUM);
      memcpy(arpFrame->srcMacAddr, macAddr, MAC_ADDRESS_BYTES_NUM);
      
      memcpy(arpFrame->destIpAddr, arpFrame->srcIpAddr, IP_ADDRESS_BYTES_NUM);
      memcpy(arpFrame->srcIpAddr, _ownIpAddr, IP_ADDRESS_BYTES_NUM);
      
      arpFrame->opCode = htons(ARP_OP_CODE_RESPONSE);
      newFrameLen = frameLen;
    }
  }
  
  return newFrameLen;
}

/*----------------------------------------------------------------------------*/
uint16_t Ethernet_COM::IP_CalcCheckSum(uint8_t* data, uint16_t len)
{
  uint32_t res = 0;
  uint16_t* ptr = (uint16_t*)data;

  while (len > 1) 
  {
    res += *ptr;
    ptr++;
    len -= 2;
  }
  
  if (len > 0) 
  {
    res += *(uint8_t*)ptr;
  }
  
  while (res > 0xffff)
  {
    res = (res >> 16) + (res & 0xFFFF);
  }
  
  return ~((uint16_t)res);
}


/*----------------------------------------------------------------------------*/
uint16_t Ethernet_COM::IP_Process(IP_Frame* ipFrame, uint16_t frameLen)
{
  uint16_t newFrameLen = 0;
  
  if (memcmp(ipFrame->destIpAddr, _ownIpAddr, IP_ADDRESS_BYTES_NUM) == 0)
  {             
    uint16_t rxCheckSum = ipFrame->checkSum;
    ipFrame->checkSum = 0;
    uint16_t calcCheckSum = IP_CalcCheckSum((uint8_t*)ipFrame, sizeof(IP_Frame));
    
    if (rxCheckSum == calcCheckSum)
    {   
      uint16_t dataLen = frameLen - sizeof(IP_Frame);
      uint16_t newDataLen = 0;
      // ICMP: xử lý ping request và tạo ping reply
      if (ipFrame->protocol == IP_FRAME_PROTOCOL_ICMP)
      {
        newDataLen = ICMP_Process((ICMP_EchoFrame*)ipFrame->data, dataLen);
		    newFrameLen = newDataLen + sizeof(IP_Frame);
      	ipFrame->len = htons(newFrameLen);
      
      	ipFrame->fragId = 0;
      	ipFrame->fragOffset = 0;
      
      	//  Đảo địa chỉ để gửi phản hồi
      	memcpy(ipFrame->destIpAddr, ipFrame->srcIpAddr, IP_ADDRESS_BYTES_NUM);
      	memcpy(ipFrame->srcIpAddr, _ownIpAddr, IP_ADDRESS_BYTES_NUM);
      
      	ipFrame->checkSum = IP_CalcCheckSum((uint8_t*)ipFrame, sizeof(IP_Frame));
		    return newFrameLen;
      }
      //UDP process 
      if (ipFrame->protocol == IP_FRAME_PROTOCOL_UDP)
      {
        UDP_Frame* udpFrame = (UDP_Frame*)ipFrame->data;
        // UDP: dispatch tới receiver, cập nhật peer IP/port per-port
        UDP_Dispatch(ipFrame, udpFrame, dataLen);
        return 0; 
      }
	  return 0;
    }
  } 
  return 0;
}
/*----------------------------------------------------------------------------*/
uint16_t Ethernet_COM::ICMP_Process(ICMP_EchoFrame* icmpFrame, uint16_t frameLen)
{
  uint16_t newFrameLen = 0;
  
  uint16_t rxCheckSum = icmpFrame->checkSum;
  icmpFrame->checkSum = 0;
  uint16_t calcCheckSum = IP_CalcCheckSum((uint8_t*)icmpFrame, frameLen);
  
  if (rxCheckSum == calcCheckSum)
  {
    if (icmpFrame->type == ICMP_FRAME_TYPE_ECHO_REQUEST)
    {
      icmpFrame->type = ICMP_FRAME_TYPE_ECHO_REPLY;
      icmpFrame->checkSum = IP_CalcCheckSum((uint8_t*)icmpFrame, frameLen);
      newFrameLen = frameLen;
    }
  }
  
  return newFrameLen;
}
// ── UDP dispatch ────────────────────────────────────────────────────
uint16_t Ethernet_COM::UDP_Dispatch(IP_Frame* ipFrame, UDP_Frame* udpFrame, uint16_t frameLen) {
    uint16_t dstPort    = ntohs(udpFrame->destPort);
    uint16_t srcPort    = ntohs(udpFrame->srcPort);
    uint16_t len        = ntohs(udpFrame->len);
    uint16_t payloadLen = len - sizeof(UDP_Frame);

    for (uint8_t i = 0; i < udpReceiverCount_; i++) {
        if (udpReceivers_[i].port == dstPort) {
            if (!udpReceivers_[i].peerLocked) {
                memcpy(udpReceivers_[i].peerIp, ipFrame->srcIpAddr, 4);
                udpReceivers_[i].peerPort   = srcPort;
                udpReceivers_[i].peerKnown  = true;
            }
            if (udpReceivers_[i].receiver != nullptr) {
                udpReceivers_[i].receiver->OnUdpReceived(udpFrame->data, payloadLen);
            }
            break;
        }
    }
    return 0; 
}

// ── Send UDP ─────────────────────────────────────────────────────
StatusEnum Ethernet_COM::SendUdp(const uint8_t destIp[4], uint16_t listenPort, UdpMode udpmode,
                                  const uint8_t* payload, uint16_t payloadLen)
{
	std::lock_guard<RTOS::StaticMutex> lock(_mutex);
    uint8_t _destIpAddr[4];
    uint16_t _destPortAddr;
    // ── Tính kích thước từng layer ────────────────────────────────────────
    uint16_t udpLen  = sizeof(UDP_Frame)  + payloadLen;
    uint16_t ipLen   = sizeof(IP_Frame)   + udpLen;
    uint16_t ethLen  = sizeof(ETH_Frame)  + ipLen;

    // ── Dùng static buffer (không dùng heap trên embedded) ───────────────
    static uint8_t txBuf[ENC28J60_MAX_FRAME_LEN];
    if (ethLen > sizeof(txBuf)) return ERROR_MEMORY;

    // ── Pointer vào từng layer trong buffer ──────────────────────────────
    ETH_Frame* ethFrame = (ETH_Frame*)txBuf;
    IP_Frame*  ipFrame  = (IP_Frame*)ethFrame->data;
    UDP_Frame* udpFrame = (UDP_Frame*)ipFrame->data;

    // ── Set UDP Mode ──────────────────────────────────────────────────────
    switch (udpmode) {
        case UdpMode::CLIENT:  // manual: caller chỉ định
            memcpy(_destIpAddr, destIp, 4);
            _destPortAddr = listenPort;
            break;     

        case UdpMode::SERVER: { // auto: reply về peer vừa nhận của CỔNG NÀY
            const UdpEntry* entry = nullptr;
            for (uint8_t i = 0; i < udpReceiverCount_; i++) {
                if (udpReceivers_[i].port == listenPort) {
                    entry = &udpReceivers_[i];
                    break;
                }
            }
            if (entry == nullptr || !entry->peerKnown) {
                return ERROR_EXIST;
            }
            memcpy(_destIpAddr, entry->peerIp, 4);
            _destPortAddr = entry->peerPort;
            break;
        }

        case UdpMode::BROADCAST:  // broadcast
            static const uint8_t bcast[] = {192,168,16,255};
            memcpy(_destIpAddr, bcast, 4);
            _destPortAddr = listenPort;
            break;
    }

    // ── ETH header ────────────────────────────────────────────────────────
    // destMac: dùng broadcast FF:FF:FF:FF:FF:FF hoặc lưu MAC từ packet nhận
    memset(ethFrame->destMacAddr, 0xFF, 6);
    memcpy(ethFrame->srcMacAddr,  macAddr, 6);       // MAC của STM32, cấu hình sẵn
    ethFrame->etherType = htons(ETH_FRAME_TYPE_IP);

    // ── IP header ─────────────────────────────────────────────────────────
    ipFrame->verHeaderLen   = 0x45;                   // IPv4, header 20 byte
    ipFrame->diffServices       = 0;
    ipFrame->len       = htons(ipLen);
    ipFrame->fragId    = 0;
    ipFrame->fragOffset= 0;
    ipFrame->timeToLive       = 128;
    ipFrame->protocol  = IP_FRAME_PROTOCOL_UDP;
    ipFrame->checkSum  = 0;
    memcpy(ipFrame->srcIpAddr,  _ownIpAddr,  4);     // IP của STM32
    memcpy(ipFrame->destIpAddr, _destIpAddr,  4);
    ipFrame->checkSum  = IP_CalcCheckSum((uint8_t*)ipFrame, sizeof(IP_Frame));

    // ── UDP header ────────────────────────────────────────────────────────
    udpFrame->srcPort  = htons(listenPort);          // port STM32 lắng nghe
    udpFrame->destPort = htons(_destPortAddr);
    udpFrame->len      = htons(udpLen);
    udpFrame->checkSum = 0;                      // optional cho IPv4

    // ── Payload ───────────────────────────────────────────────────────────
    memcpy(udpFrame->data, payload, payloadLen);

   // LOGI<<"Send UDP to : "<<_destIpAddr[0]<<"."<<_destIpAddr[1]<<"."<<_destIpAddr[2]<<"."<<_destIpAddr[3] << LEND;
    // ── Gửi qua ENC28J60 ─────────────────────────────────────────────────
    return enc_.TransmitFrame(txBuf, ethLen);
}
// ── Register ──────────────────────────────────────────────────────────────
bool Ethernet_COM::RegisterUdpReceiver(uint16_t port, IUdpReceiver* receiver) {
    // Kiểm tra port đã đăng ký chưa
    for (uint8_t i = 0; i < udpReceiverCount_; i++) {
        if (udpReceivers_[i].port == port) {
            LOGE << "Port " << port << " already registered: "<< udpReceivers_[i].port<< LEND;
            return false;
        }
    }
    if (udpReceiverCount_ >= MAX_UDP_RECEIVERS) {
        LOGE << "UDP receiver table full" << LEND;
        return false;
    }
    udpReceivers_[udpReceiverCount_++] = { port, receiver };
    return true;
}

void Ethernet_COM::UnregisterUdpReceiver(uint16_t port) {
    for (uint8_t i = 0; i < udpReceiverCount_; i++) {
        if (udpReceivers_[i].port == port) {
            // Dịch các phần tử phía sau lên để lấp chỗ trống
            for (uint8_t j = i; j < udpReceiverCount_ - 1; j++) {
                udpReceivers_[j] = udpReceivers_[j + 1];
            }
            // Xóa slot cuối
            udpReceivers_[udpReceiverCount_ - 1] = { 0, nullptr };
            udpReceiverCount_--;
            return;
        }
    }
}

#endif /* GREMSY_MODULE_ETHERNET */

/******************************************************************************/
