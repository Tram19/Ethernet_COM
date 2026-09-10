/**
  ******************************************************************************
  * @file           : enc28j60.c
  * @brief          : ENC28J60 driver
  * @author         : MicroTechnics (microtechnics.ru)
  ******************************************************************************
  */



#include <gremsy_include.hpp>

#ifdef GREMSY_MODULE_ETHERNET

/* Includes ------------------------------------------------------------------*/

#include <Modules/Ethernet_COM/common.hpp>
#include <Modules/ENC28J60/enc28j60.hpp>



/* Declarations and definitions ----------------------------------------------*/
/*
The source address field is a 6-byte field filled with the
MAC address of the node which created the Ethernet
packet. Users of the ENC28J60 must generate a
unique MAC address for each controller used. 
*/
uint8_t macAddr[MAC_ADDRESS_BYTES_NUM] = {0x00, 0x17, 0x22, 0xED, 0xA5, 0x01};
uint8_t ipAddr[IP_ADDRESS_BYTES_NUM] = {192, 168, 16, 227};

uint8_t commandOpCodes[COMMANDS_NUM] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x07};
static ENC28J60_RegBank curBank = BANK_0;
static uint16_t curPtr = ENC28J60_RX_BUF_START;

#ifdef GREMSY_SPI_ENABLE
/* Functions -----------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
ENC28J60::ENC28J60(HW::Peripheral::Spi& spi) : spi_(spi)
{
}

void ENC28J60::SetCS(ENC28J60_CS_State state)
{
  if (state == CS_LOW)
  {
    spi_.Lock();
  }
  HAL_GPIO_WritePin(ETH_SPI_CS_GPIO_Port, ETH_SPI_CS_Pin, (GPIO_PinState)state);
  if (state == CS_HIGH)
  {
    spi_.Unlock();
  }
}



/*----------------------------------------------------------------------------*/
void ENC28J60::WriteBytes(uint8_t* data, uint16_t size)
{
  //HAL_StatusTypeDef res = HAL_SPI_Transmit(&hspi2, data, size, ENC28J60_SPI_TIMEOUT);

  StatusEnum ret = spi_.Send(std::span<const uint8_t>{data, size});
}



/*----------------------------------------------------------------------------*/
void ENC28J60::WriteByte(uint8_t data)
{
  HAL_StatusTypeDef res = HAL_SPI_Transmit(&hspi2, &data, 1, ENC28J60_SPI_TIMEOUT);
}


//static uint8_t txData = 0x00;
//static uint8_t rxData = 0x00;
/*----------------------------------------------------------------------------*/
uint8_t ENC28J60::ReadByte()
{

	uint8_t txData = 0x00;
	uint8_t rxData = 0x00;
//  StatusEnum ret = spi_.Read(&txData, &rxData, 1);
   HAL_StatusTypeDef res = HAL_SPI_TransmitReceive(&hspi2, &txData, &rxData, 1, ENC28J60_SPI_TIMEOUT);
  // HAL_StatusTypeDef res = HAL_SPI_TransmitReceive_DMA(&hspi2, &txData, &rxData, 1);
  return rxData;
}



/*----------------------------------------------------------------------------*/
ENC28J60_RegType ENC28J60::getRegType(uint8_t reg)
{
  ENC28J60_RegType type = (ENC28J60_RegType)((reg & ENC28J60_REG_TYPE_MASK) >> ENC28J60_REG_TYPE_OFFSET);
  return type;
}



/*----------------------------------------------------------------------------*/
ENC28J60_RegBank ENC28J60::getRegBank(uint8_t reg)
{
  ENC28J60_RegBank bank = (ENC28J60_RegBank)((reg & ENC28J60_REG_BANK_MASK) >> ENC28J60_REG_BANK_OFFSET);
  return bank;
}



/*----------------------------------------------------------------------------*/
uint8_t ENC28J60::getRegAddr(uint8_t reg)
{
  uint8_t addr = (reg & ENC28J60_REG_ADDR_MASK);
  return addr;
}



/*----------------------------------------------------------------------------*/
void ENC28J60::WriteCommand(ENC28J60_Command command, uint8_t argData)
{
  uint8_t data = 0;
  data = (commandOpCodes[command] << ENC28J60_OP_CODE_OFFSET) | argData;
  WriteByte(data);
}



/*----------------------------------------------------------------------------*/
void ENC28J60::CheckBank(uint8_t reg)
{
  uint8_t regAddr = getRegAddr(reg);  
  if (regAddr < ENC28J60_COMMON_REGS_ADDR)
  {
    ENC28J60_RegBank regBank = getRegBank(reg);    
    if (curBank != regBank)
    {
      uint8_t econ1Addr = getRegAddr(ECON1);
      
      // Clear bank bits
      SetCS(CS_LOW); 
      WriteCommand(BIT_FIELD_CLEAR, econ1Addr);
      WriteByte(ECON1_BSEL1_BIT | ECON1_BSEL0_BIT);
      SetCS(CS_HIGH);
      
      // Set bank bits
      SetCS(CS_LOW); 
      WriteCommand(BIT_FIELD_SET, econ1Addr);
      WriteByte(regBank);
      SetCS(CS_HIGH);
      
      curBank = regBank;
    }
  }
}



/*----------------------------------------------------------------------------*/
void ENC28J60::BitFieldSet(uint8_t reg, uint8_t regData)
{
  uint8_t regAddr = getRegAddr(reg);
  CheckBank(reg);
  
  SetCS(CS_LOW); 
  WriteCommand(BIT_FIELD_SET, regAddr);
  WriteByte(regData);
  SetCS(CS_HIGH);
}



/*----------------------------------------------------------------------------*/
void ENC28J60::BitFieldClear(uint8_t reg, uint8_t regData)
{
  uint8_t regAddr = getRegAddr(reg);
  CheckBank(reg);
  
  SetCS(CS_LOW); 
  WriteCommand(BIT_FIELD_CLEAR, regAddr);
  WriteByte(regData);
  SetCS(CS_HIGH);
}



/*----------------------------------------------------------------------------*/
uint8_t ENC28J60::ReadControlReg(uint8_t reg)
{
  uint8_t data = 0;
  ENC28J60_RegType regType = getRegType(reg);
  uint8_t regAddr = getRegAddr(reg);
  CheckBank(reg);
  
  SetCS(CS_LOW);
  WriteCommand(READ_CONTROL_REG, regAddr);

  if (regType == MAC_MII_REG)
  {
	  ENC28J60::ReadByte();
  }
  data = ENC28J60::ReadByte();
  
  SetCS(CS_HIGH);
  return data;
}

uint8_t ENC28J60::GetRevisionID()
{
  uint8_t revid = 0;
    // Đọc toàn bộ 8-bit của thanh ghi EREVID
    revid = ReadControlReg(EREVID);
    
    // Theo Datasheet, ID chỉ nằm ở 5 bit cuối (bit 4:0), 3 bit đầu luôn là 0
    // Ta dùng phép mask & 0x1F để lấy chính xác giá trị ID
    return (revid & 0x1F); 
}

/*
Nhiễu xuyên âm (Crosstalk) hoặc gai điện áp (glitch) vào chân CS của ENC28J60.
Con ENC vô tình tưởng đó là các lệnh ghi đè thanh ghi và làm hỏng Con trỏ ghi nội bộ 
(Internal Write Pointer - ERXWRPT).
*/
void ENC28J60::RescueRx()
{
    std::lock_guard<RTOS::StaticMutex> lock(_encmutex); // Đảm bảo an toàn bus SPI
    
    // 1. Tạm dừng khối nhận gói tin để tránh ghi đè dữ liệu mới
    BitFieldClear(ECON1, ECON1_RXEN_BIT);

    // 2. Hardware Reset riêng khối logic của bộ nhận (Receive Only Reset)
    // Lệnh này xả phần cứng khối RX mà không làm ảnh hưởng tới khối TX hay cấu hình MAC [1].
    BitFieldSet(ECON1, ECON1_RXRST_BIT);
    BitFieldClear(ECON1, ECON1_RXRST_BIT);

    // 3. Xóa sạch đếm gói tin (Flush EPKTCNT)
    // Việc Reset RX Logic ở trên không tự xóa biến đếm gói tin. Ta phải trừ thủ công về 0 [2].
    uint8_t pktCnt = ReadControlReg(EPKTCNT);
    while (pktCnt > 0) {
        BitFieldSet(ECON2, ECON2_PKTDEC_BIT);
        pktCnt--;
    }

    // 4. BÍ QUYẾT: Ép phần cứng Reset con trỏ Ghi nội bộ (Internal Write Pointer)
    // Theo Datasheet, mỗi khi ta lập trình lại ERXST, phần cứng sẽ tự động 
    // cập nhật bản sao nội bộ của ERXWRPT về bằng đúng ERXST [3]. 
    // Đây là bước sống còn để sửa lỗi con trỏ ghi bị IMU làm sai lệch.
    WriteControlRegPair(ERXSTL, ENC28J60_RX_BUF_START);
    WriteControlRegPair(ERXNDL, ENC28J60_RX_BUF_END);

    // 5. Cập nhật lại con trỏ Đọc (Bắt buộc phải là số LẺ theo Erratum 13)
    WriteControlRegPair(ERXRDPTL, ENC28J60_RX_BUF_END); 
    
    // 6. Reset lại biến theo dõi gói tin của phần mềm
    curPtr = ENC28J60_RX_BUF_START;

    // 7. Kích hoạt lại khối nhận
    BitFieldSet(ECON1, ECON1_RXEN_BIT);
}


/*----------------------------------------------------------------------------*/
void ENC28J60::WriteControlReg(uint8_t reg, uint8_t regData)
{
  uint8_t regAddr = getRegAddr(reg);
  CheckBank(reg);
  
  SetCS(CS_LOW); 
  WriteCommand(WRITE_CONTROL_REG, regAddr);
  WriteByte(regData);
  SetCS(CS_HIGH);
}



/*----------------------------------------------------------------------------*/
void ENC28J60::WriteControlRegPair(uint8_t reg, uint16_t regData)
{
  WriteControlReg(reg, (uint8_t)regData);
  WriteControlReg(reg + 1, (uint8_t)(regData >> 8));
}



/*----------------------------------------------------------------------------*/
uint16_t ENC28J60::ReadControlRegPair(uint8_t reg)
{
  uint16_t data = 0;
  data = (uint16_t)ReadControlReg(reg) | ((uint16_t)ReadControlReg(reg + 1) << 8);
  return data;
}


// Ghi vao bo nho dem SRAM 8-kbyte 
/*----------------------------------------------------------------------------*/
void ENC28J60::WriteBufferMem(uint8_t *data, uint16_t size)
{
  SetCS(CS_LOW); 
  WriteCommand(WRITE_BUFFER_MEM, ENC28J60_BUF_COMMAND_ARG);
  WriteBytes(data, size);
  SetCS(CS_HIGH);
}


static uint8_t txDummy[ENC28J60_MAX_FRAME_LEN] = {0};

/*----------------------------------------------------------------------------*/
bool ENC28J60::ReadBufferMem(uint8_t *data, uint16_t size)
{
  SetCS(CS_LOW); 
  WriteCommand(READ_BUFFER_MEM, ENC28J60_BUF_COMMAND_ARG);
  
//  for (uint16_t i = 0; i < size; i++)
//  {
//    *data = ReadByte();
//    data++;
//  }
  StatusEnum ret = spi_.Read(txDummy, data, size);
  if (ret != OK) {
      LOGE << "ReadBufferMem fail size= " << size <<"ret "<< ret << LEND;
      SetCS(CS_HIGH);
      return false;
  }

  SetCS(CS_HIGH);
  return true;
}



/*----------------------------------------------------------------------------*/
void ENC28J60::SystemReset()
{
  SetCS(CS_LOW);
  WriteCommand(SYSTEM_RESET, ENC28J60_RESET_COMMAND_ARG);
  SetCS(CS_HIGH);
  
  curBank = BANK_0;
  g_os_delay(100);//WAIT FOR OST, but the internal clock of the ENC28J60 is not stable yet after reset (base on silicon errata ) 
}



/*----------------------------------------------------------------------------*/
uint16_t ENC28J60::ReadPhyReg(uint8_t reg)
{
  uint16_t data = 0;
  uint8_t regAddr = getRegAddr(reg);
  
  WriteControlReg(MIREGADR, regAddr);
  BitFieldSet(MICMD, MICMD_MIIRD_BIT);
  
  while((ReadControlReg(MISTAT) & MISTAT_BUSY_BIT) != 0);
  
  BitFieldClear(MICMD, MICMD_MIIRD_BIT);
  data = ReadControlRegPair(MIRDL);
  
  return data;
}



/*----------------------------------------------------------------------------*/
void ENC28J60::StartReceiving()
{
  BitFieldSet(ECON1, ECON1_RXEN_BIT);
}



/*----------------------------------------------------------------------------*/
void ENC28J60::WritePhyReg(uint8_t reg, uint16_t regData)
{
  uint8_t regAddr = getRegAddr(reg);
  
  WriteControlReg(MIREGADR, regAddr);
  WriteControlRegPair(MIWRL, regData);
  
  while((ReadControlReg(MISTAT) & MISTAT_BUSY_BIT) != 0);
}



/*----------------------------------------------------------------------------*/
bool ENC28J60::Init()
{
  // HAL_GPIO_WritePin(ENC28J60_RESET_PORT, ENC28J60_RESET_PIN, GPIO_PIN_RESET);
  // HAL_Delay(50);
  // HAL_GPIO_WritePin(ENC28J60_RESET_PORT, ENC28J60_RESET_PIN, GPIO_PIN_SET);
  // HAL_Delay(50);
  spi_.Open();
  g_os_delay(500);
  SystemReset();

  // Rx/Tx buffers
  WriteControlRegPair(ERXSTL, ENC28J60_RX_BUF_START);
  WriteControlRegPair(ERXNDL, ENC28J60_RX_BUF_END);
  
  WriteControlRegPair(ERDPTL, ENC28J60_RX_BUF_START);
  
  //ERXFCON register is not used because we want to receive all frames, including runt frames (less than 64 bytes)
  // Default value : 10100001  : bit 0 = 1 (BCEN) and bit 6 = 0 (ANDOR) accept Broadcast 

  // MAC address 
  WriteControlReg(MAADR1, macAddr[0]);
  WriteControlReg(MAADR2, macAddr[1]);
  WriteControlReg(MAADR3, macAddr[2]);
  WriteControlReg(MAADR4, macAddr[3]);
  WriteControlReg(MAADR5, macAddr[4]);
  WriteControlReg(MAADR6, macAddr[5]);
  
  WriteControlReg(MACON1, MACON1_TXPAUS_BIT | MACON1_RXPAUS_BIT | MACON1_MARXEN_BIT);
  WriteControlReg(MACON3, MACON3_PADCFG0_BIT | MACON3_TXCRCEN_BIT | MACON3_FRMLNEN_BIT);
    
  WriteControlRegPair(MAIPGL, ENC28J60_NBB_PACKET_GAP);
  WriteControlReg(MABBIPG, ENC28J60_BB_PACKET_GAP);
  
  WriteControlRegPair(MAMXFLL, ENC28J60_FRAME_DATA_MAX);
  
  // PHY resisters
  WritePhyReg(PHCON2, PHCON2_HDLDIS_BIT);
  
  StartReceiving();

  return true;
}



/*----------------------------------------------------------------------------*/
uint16_t ENC28J60::ReceiveFrame(ENC28J60_Frame* frame)
{  
  std::lock_guard<RTOS::StaticMutex> lock(_encmutex);
  uint16_t dataSize = 0;
  uint8_t packetsNum = ReadControlReg(EPKTCNT);//Kiem tra so luong goi tin dang cho
  
  if (packetsNum > 0)
  {
    WriteControlRegPair(ERDPTL, curPtr); //Gan dia chi con tro doc du lieu trong bo nho dem
    
    if (!ReadBufferMem((uint8_t*)frame, ENC28J60_HEADER_SIZE)) // doc header cua goi tin 
    {
      RescueRx();
      return 0;
    }
    
    curPtr = frame->nextPtr;
    // Kiem tra goi tin co hop le khong, neu hop le thi doc du lieu va checksum
    if ((frame->status & ENC28J60_FRAME_RX_OK_MASK) != 0) 
    {
      dataSize = frame->length - ENC28J60_CRC_SIZE;
      
      if (dataSize > ENC28J60_FRAME_DATA_MAX)
      {
        dataSize = ENC28J60_FRAME_DATA_MAX;
      }
      
      // Doc du lieu va checksum cua goi tin
      if (!ReadBufferMem((uint8_t*)&(frame->data[0]), dataSize))
      {
        RescueRx();
        return 0;
      }
      if (!ReadBufferMem((uint8_t*)&(frame->checkSum), ENC28J60_CRC_SIZE))
      {
        RescueRx();
        return 0;
      }
    }
    
    //cap nhat dia chi con tro doc du lieu tiep theo va giam so luong goi tin trong bo nho dem
    uint16_t nextPtr = frame->nextPtr - 1;
    if (nextPtr > ENC28J60_RX_BUF_END)
    {
      nextPtr = ENC28J60_RX_BUF_END;
    }
    
    WriteControlRegPair(ERXRDPTL, nextPtr);
    BitFieldSet(ECON2, ECON2_PKTDEC_BIT);
  }
  
  return dataSize;
}



/*----------------------------------------------------------------------------*/
StatusEnum ENC28J60::TransmitFrame(uint8_t *data, uint16_t size)
{
  std::lock_guard<RTOS::StaticMutex> lock(_encmutex);
  constexpr uint32_t TIMEOUT_MS = 10;
  uint32_t           deadline   = osKernelGetTickCount() + TIMEOUT_MS;
  //ECON1_TXRTS_BIT kiem tra qua trinh truyen du lieu truoc do da hoan tat chua
  while((ReadControlReg(ECON1) & ECON1_TXRTS_BIT) != 0)
  {
    if((ReadControlReg(EIR) & EIR_TXERIF_BIT) != 0)
    {
      BitFieldSet(ECON1, ECON1_TXRST_BIT);
      BitFieldClear(ECON1, ECON1_TXRST_BIT);
      return ERROR_HAL;
    }
    if (osKernelGetTickCount() >= deadline)
    {
      BitFieldSet(ECON1, ECON1_TXRST_BIT);
      BitFieldClear(ECON1, ECON1_TXRST_BIT);
      return ERROR_TIMEOUT;
    }
    g_os_delay(1);
  }

  //Lệnh này gán địa chỉ bắt đầu của vùng đệm phát dữ liệu (Transmit Buffer) bằng cách ghi vào thanh ghi EWRPTL/EWRPTH. Sau đó, dữ liệu được ghi vào bộ đệm này thông qua lệnh WriteBufferMem. Cuối cùng, các thanh ghi ETXSTL/ETXSTH và ETXNDL/ETXNDH được thiết lập để xác định phạm vi của dữ liệu cần truyền, và lệnh BitFieldSet được sử dụng để kích hoạt quá trình truyền dữ liệu bằng cách đặt bit ECON1_TXRTS_BIT trong thanh ghi ECON1.
  WriteControlRegPair(EWRPTL, ENC28J60_TX_BUF_START);
  
  //Sử dụng cấu hình mặc định đã được cài đặt trong thanh ghi MACON3
  uint8_t controlByte = 0x00;
  WriteBufferMem(&controlByte, 1);
  // Ghi dữ liệu thực tế của gói tin
  WriteBufferMem(data, size);

  //gán địa chỉ cho con trỏ Bắt đầu (ETXST) và Kết thúc (ETXND) để phần cứng biết chính xác khối dữ liệu cần phát nằm ở đâu
  WriteControlRegPair(ETXSTL, ENC28J60_TX_BUF_START);
  WriteControlRegPair(ETXNDL, ENC28J60_TX_BUF_START + size);

  //Kích hoạt phát sóng phần cứng
  BitFieldSet(ECON1, ECON1_TXRTS_BIT);
  return OK;
}


#endif //GREMSY_SPI_ENABLE
/*----------------------------------------------------------------------------*/

#endif // GREMSY_MODULE_ETHERNET
