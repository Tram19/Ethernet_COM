/**
  ******************************************************************************
  * @file    hardware_u5_peripheral_flash.cpp
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

#include "Hardware/Stm32U5/Inc/hardware_u5_peripheral_flash.hpp"
#include <string.h>

#ifdef STM32U5

bool HW::Peripheral::Flash::eraseFlash(uint32_t bytes, uint32_t startAddress)
{
    if(startAddress < BANK1_START_ADDRESS || startAddress > BANK2_END_ADDRESS) return false;
    uint32_t startBank = (startAddress < BANK2_START_ADDRESS) ? BANK1_START_ADDRESS : BANK2_START_ADDRESS;
    uint32_t offset = startAddress - startBank;
    float nPage = uint32_t((float)bytes / (8.0f* 1024.f)) + 1;
    uint32_t sPage = uint32_t((float)offset /  (8.0f * 1024.f));
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.Banks     = (startAddress < BANK2_START_ADDRESS) ? FLASH_BANK_1 : FLASH_BANK_2;
    EraseInitStruct.Page      = sPage;
    EraseInitStruct.NbPages   = nPage;
    uint32_t SectorError;
    HAL_FLASH_Unlock();
    HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError);
    HAL_FLASH_Lock();
    return true;
}


StatusEnum HW::Peripheral::Flash::Earse(void)
{
    if(isOpen == true)
    {
        uint32_t pageError = 0;
        FLASH_EraseInitTypeDef ins = {0};
        StatusEnum status;

        ins.TypeErase = FLASH_TYPEERASE_PAGES;
        ins.Banks     = FLASH_BANK_1;
        ins.NbPages   = 1;
        ins.Page      = paramConfig->sector;

        HAL_FLASH_Unlock();
        status = (StatusEnum)HAL_FLASHEx_Erase(&ins, &pageError);
        HAL_FLASH_Lock();

        return status;
    }

    return ERROR_NOT_EXIST;
}

uint32_t HW::Peripheral::Flash::writeFlashBytestWithoutLock(uint32_t startAddress, const uint8_t * data, uint32_t bytes)
{
    uint32_t offset = 0;
    HAL_StatusTypeDef status = HAL_ERROR;
    //kiem tra last packet
    HAL_FLASH_Unlock();
    if( bytes > 16 && bytes <= 128)
    {
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BURST, startAddress + offset, (uint32_t)(data + offset));
        HAL_FLASH_Lock();
        if (status != HAL_OK)
        {
            return 0;
        }
        return bytes;
    }
    if(bytes <= 16)
    {
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_QUADWORD, startAddress + offset, (uint32_t)(data + offset));
        HAL_FLASH_Lock();
        if (status != HAL_OK)
        {
            return 0;
        }
        return bytes;
    }

    while(bytes > 0)
    {
        if (bytes >= 128) {
            status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BURST, startAddress + offset, (uint32_t)(data + offset));
            if (status != HAL_OK)
            {
                HAL_FLASH_Lock();
                return offset;
            }
            status = FLASH_WaitForLastOperation(1000);
            if (status != HAL_OK)
            {
                HAL_FLASH_Lock();
                return offset;
            }
            offset += 128;
            bytes  -= 128;
        } else if(bytes < 128 && bytes>= 16){
            status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_QUADWORD, startAddress + offset, (uint32_t)(data + offset));
            if (status != HAL_OK)
            {
                HAL_FLASH_Lock();
                return offset;
            }
            status = FLASH_WaitForLastOperation(1000);
            if (status != HAL_OK)
            {
                HAL_FLASH_Lock();
                return offset;
            }
            offset += 16;
            bytes  -= 16;
        }
        else
        {
            HAL_FLASH_Lock();
            return offset;
        }
    }
    HAL_FLASH_Lock();
    return offset;
}


StatusEnum HW::Peripheral::Flash::Write(uint32_t add, uint8_t* buff, int len)
{
    if(isOpen == true)
    {
        StatusEnum status = OK;
        uint32_t startAdd = paramConfig->add + add;

        if((add + len) > paramConfig->size)
        {
            return ERROR_NOT_EXIST;
        }

        HAL_FLASH_Unlock();

        uint32_t currAdd = startAdd;
        int bytesRemaining = len;
        uint8_t* srcPtr = buff;

        while(bytesRemaining > 0)
        {
            uint32_t alignedBlockAdd = currAdd & ~0x0FU;
            uint32_t qw_buf[4];

            qw_buf[0] = *(uint32_t*)(alignedBlockAdd);
            qw_buf[1] = *(uint32_t*)(alignedBlockAdd + 4);
            qw_buf[2] = *(uint32_t*)(alignedBlockAdd + 8);
            qw_buf[3] = *(uint32_t*)(alignedBlockAdd + 12);

            uint8_t* qw_byte_buf = (uint8_t*)qw_buf;
            uint32_t offsetInBlock = currAdd - alignedBlockAdd;
            uint32_t bytesToCopy = 16 - offsetInBlock;
            if(bytesToCopy > (uint32_t)bytesRemaining)
            {
                bytesToCopy = bytesRemaining;
            }

            for(uint32_t i = 0; i < bytesToCopy; i++)
            {
                qw_byte_buf[offsetInBlock + i] = srcPtr[i];
            }

            status = (StatusEnum)HAL_FLASH_Program(FLASH_TYPEPROGRAM_QUADWORD, alignedBlockAdd, (uint32_t)qw_buf);
            if(status != OK)
            {
                HAL_FLASH_Lock();
                return status;
            }

            currAdd += bytesToCopy;
            srcPtr += bytesToCopy;
            bytesRemaining -= bytesToCopy;
        }

        HAL_FLASH_Lock();
        return status;
    }

    return ERROR_NOT_EXIST;
}

StatusEnum HW::Peripheral::Flash::Write(uint32_t add, const char* str)
{
    return Write(add, (uint8_t*)str, strlen(str));
}

StatusEnum HW::Peripheral::Flash::Write(uint32_t add, uint8_t data)
{
    return Write(add, &data, 1);
}

StatusEnum HW::Peripheral::Flash::Write(uint32_t add, uint16_t data)
{
    return Write(add, (uint8_t*)&data, 2);
}

StatusEnum HW::Peripheral::Flash::Write(uint32_t add, uint32_t data)
{
    return Write(add, (uint8_t*)&data, 4);
}

StatusEnum HW::Peripheral::Flash::Write(uint32_t add, uint64_t data)
{
    return Write(add, (uint8_t*)&data, 8);
}

StatusEnum HW::Peripheral::Flash::Read(uint32_t add, uint8_t* buff, int len)
{
    if(isOpen == true)
    {
        if((add + len) > paramConfig->size)
        {
            return ERROR_NOT_EXIST;
        }

        uint32_t startAdd = paramConfig->add + add;
        for(int i = 0; i < len; i++)
        {
            buff[i] = *(uint8_t*)(startAdd + i);
        }

        return OK;
    }

    return ERROR_NOT_EXIST;
}

StatusEnum HW::Peripheral::Flash::Read(uint32_t add, uint8_t* pdata)
{
    return Read(add, pdata, 1);
}

StatusEnum HW::Peripheral::Flash::Read(uint32_t add, uint16_t* pdata)
{
    return Read(add, (uint8_t*)pdata, 2);
}

StatusEnum HW::Peripheral::Flash::Read(uint32_t add, uint32_t* pdata)
{
    return Read(add, (uint8_t*)pdata, 4);
}

StatusEnum HW::Peripheral::Flash::Read(uint32_t add, uint64_t* pdata)
{
    return Read(add, (uint8_t*)pdata, 8);
}

uint32_t HW::Peripheral::Flash::GetSize()
{
    if(isOpen == true)
    {
        return paramConfig->size;
    }
    return 0;
}

StatusEnum HW::Peripheral::Flash::Open(void)
{
    if(!IS_SECTOR_ID(id))
    {
        return ERROR_NOT_EXIST;
    }

    if(isOpen == true)
    {
        return ERROR_EXIST;
    }

    paramConfig = (SectorConfigType*)&HARDWARE_SECTOR_CONFIG[id];
    isOpen = true;

    return OK;
}

StatusEnum HW::Peripheral::Flash::Close(void)
{
    if(!IS_SECTOR_ID(id))
    {
        return ERROR_NOT_EXIST;
    }

    if(isOpen == false)
    {
        return ERROR_EXIST;
    }

    isOpen = false;
    return OK;
}

#endif /** STM32U5 */
