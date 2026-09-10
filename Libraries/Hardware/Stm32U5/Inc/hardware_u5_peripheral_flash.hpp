/**
  ******************************************************************************
  * @file    hardware_u5_peripheral_flash.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    June 4, 2026
  * @brief   Flash Peripheral Driver for STM32U5 series.
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2026 Gremsy. All rights reserved.
  *
  ******************************************************************************
*/

#ifndef __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_FLASH_HPP__
#define __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_FLASH_HPP__

#include "hardware_u5_peripheral_config.hpp"

#if defined STM32U5 || defined STM32H5

/* Exported macro ------------------------------------------------------------*/
#define BANK1_START_ADDRESS 					0x08000000
#define BANK2_START_ADDRESS 					0x08040000
#define BANK1_END_ADDRESS                     	0x0803FFFF
#define BANK2_END_ADDRESS                     	0x0807FFFF

#define PARAM_ADDRESS                           0x0807E000 /* Last 8KB of Bank2 */
#define PAGE_SIZE     							8 /*8kb*/
#define NUM_PAGE      							32

class HW::Peripheral::Flash : public HW::Base::Flash {
public:
    StatusEnum Open(void);
    StatusEnum Close(void);

    StatusEnum Earse(void);
    
    uint32_t GetSize();

    StatusEnum Write(uint32_t add, uint8_t* buff, int len);
    StatusEnum Write(uint32_t add, const char* str);
    StatusEnum Write(uint32_t add, uint8_t data);
    StatusEnum Write(uint32_t add, uint16_t data);
    StatusEnum Write(uint32_t add, uint32_t data);
    StatusEnum Write(uint32_t add, uint64_t data);

    StatusEnum Read(uint32_t add, uint8_t* buff, int len);
    StatusEnum Read(uint32_t add, uint8_t* pdata);
    StatusEnum Read(uint32_t add, uint16_t* pdata);
    StatusEnum Read(uint32_t add, uint32_t* pdata);
    StatusEnum Read(uint32_t add, uint64_t* pdata);

    uint32_t writeFlashBytestWithoutLock(uint32_t startAddress, const uint8_t * data, uint32_t bytes);
    bool eraseFlash(uint32_t bytes, uint32_t startAddress);

private:
    SectorConfigType* paramConfig;

    /*
        * @brief Erase flash memory starting from a specific address for a given number of bytes.
        *
        * @param bytes The number of bytes to erase.
        * @param startAddress The starting address from which to begin erasing.
        * @return true if the erase operation was successful, false otherwise.   
    */

    FLASH_EraseInitTypeDef EraseInitStruct;
};

#endif /* STM32U5 */
#endif /* __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_FLASH_HPP__ */
