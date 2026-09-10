/**
  ******************************************************************************
  * @file    hardware_u5_peripheral_i2c.cpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    2024
  * @brief   STM32U5 I2C peripheral driver — Polling / IT / DMA + FreeRTOS
  *
  *  Synchronisation strategy (IT / DMA)
  *  ────────────────────────────────────
  *  - Open() creates two binary semaphores: __semTx, __semRx (initial count = 0)
  *  - Write_IT / Write_DMA / SendMem_IT / SendMem_DMA kick the HAL API then
  *    call WaitTxDone() → osSemaphoreAcquire(__semTx, timeout) → task blocks
  *  - HAL callback (ISR context) calls osSemaphoreRelease(__semTx) → task unblocks
  *  - Error path: ErrorCallback sets __txError / __rxError then releases both
  *    semaphores so WaitXxxDone() unblocks and returns ERROR_HAL
  *
  *  No volatile XferState flags needed — the semaphore count IS the state.
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2024 Gremsy. All rights reserved.
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "Hardware/Stm32U5/Inc/hardware_u5_peripheral_i2c.hpp"

#ifdef STM32U5

/* Private define ------------------------------------------------------------*/

#define I2C_POLL_TIMEOUT_MS     1000U   ///< Default timeout for all transfers

/* Private variables ---------------------------------------------------------*/

#ifdef HAL_I2C_MODULE_ENABLED

#if !defined(USE_HAL_I2C_REGISTER_CALLBACKS) || (USE_HAL_I2C_REGISTER_CALLBACKS != 1U)
#error "hardware_u5_peripheral_i2c requires USE_HAL_I2C_REGISTER_CALLBACKS = 1U in stm32u5xx_hal_conf.h"
#endif

HW::Peripheral::I2c* __gp_hardware_peripheral_i2c[I2C_COUNT];

/* Private functions — instance lookup ---------------------------------------*/

static HW::Peripheral::I2c* hardware_u5_peripheral_i2c_get_pinstance(I2C_HandleTypeDef* hi2c)
{
    for (int i = 0; i < I2C_COUNT; i++)
    {
        if (__gp_hardware_peripheral_i2c[i] != nullptr)
            if (__gp_hardware_peripheral_i2c[i]->__handle == hi2c)
                return __gp_hardware_peripheral_i2c[i];
    }
    return nullptr;
}

/* HAL Callbacks (called from ISR) -------------------------------------------*/

static void hardware_u5_peripheral_i2c_ErrorCallback(I2C_HandleTypeDef* hi2c)
{
    HW::Peripheral::I2c* p = hardware_u5_peripheral_i2c_get_pinstance(hi2c);
    if (p == nullptr) return;

    /// Mark error then unblock both wait functions
    p->__txError = true;
    p->__rxError = true;

    osSemaphoreRelease(p->__semTx);
    osSemaphoreRelease(p->__semRx);
    //p->Reset();
}

static void hardware_u5_peripheral_i2c_MasterTxCpltCallback(I2C_HandleTypeDef* hi2c)
{
    HW::Peripheral::I2c* p = hardware_u5_peripheral_i2c_get_pinstance(hi2c);
    if (p != nullptr) osSemaphoreRelease(p->__semTx);
}

static void hardware_u5_peripheral_i2c_MasterRxCpltCallback(I2C_HandleTypeDef* hi2c)
{
    HW::Peripheral::I2c* p = hardware_u5_peripheral_i2c_get_pinstance(hi2c);
    if (p != nullptr) osSemaphoreRelease(p->__semRx);
}

static void hardware_u5_peripheral_i2c_MemTxCpltCallback(I2C_HandleTypeDef* hi2c)
{
    HW::Peripheral::I2c* p = hardware_u5_peripheral_i2c_get_pinstance(hi2c);
    if (p != nullptr) osSemaphoreRelease(p->__semTx);
}

static void hardware_u5_peripheral_i2c_MemRxCpltCallback(I2C_HandleTypeDef* hi2c)
{
    HW::Peripheral::I2c* p = hardware_u5_peripheral_i2c_get_pinstance(hi2c);
    if (p != nullptr) osSemaphoreRelease(p->__semRx);
}

#endif /* HAL_I2C_MODULE_ENABLED */

/*==============================================================================
  WAIT HELPERS  (shared by IT and DMA)
==============================================================================*/

StatusEnum HW::Peripheral::I2c::WaitTxDone(uint32_t timeoutMs)
{
#ifdef HAL_I2C_MODULE_ENABLED
    osStatus_t status = osSemaphoreAcquire(__semTx, timeoutMs);

    if (status == osErrorTimeout)
    {
        HAL_I2C_Master_Abort_IT(__handle, deviceAddress);
        return ERROR_NOT_EXIST;
    }

    if (__txError)
    {
        __txError = false;
        return ERROR_HAL;
    }

    return OK;
#endif
    return ERROR_NOT_EXIST;
}

StatusEnum HW::Peripheral::I2c::WaitRxDone(uint32_t timeoutMs)
{
#ifdef HAL_I2C_MODULE_ENABLED
    osStatus_t status = osSemaphoreAcquire(__semRx, timeoutMs);

    if (status == osErrorTimeout)
    {
        HAL_I2C_Master_Abort_IT(__handle, deviceAddress);
        return ERROR_NOT_EXIST;
    }

    if (__rxError)
    {
        __rxError = false;
        return ERROR_HAL;
    }

    return OK;
#endif
    return ERROR_NOT_EXIST;
}

/*==============================================================================
  POLLING
==============================================================================*/

StatusEnum HW::Peripheral::I2c::SendMem_Polling(uint16_t memAddress, uint8_t data)
{
#ifdef HAL_I2C_MODULE_ENABLED
    return (StatusEnum)HAL_I2C_Mem_Write(
        __handle, deviceAddress,
        memAddress, I2C_MEMADD_SIZE_8BIT,
        &data, 1U, I2C_POLL_TIMEOUT_MS);
#endif
    return ERROR_NOT_EXIST;
}

StatusEnum HW::Peripheral::I2c::GetMem_Polling(uint16_t memAddress, uint8_t* pdata)
{
#ifdef HAL_I2C_MODULE_ENABLED
    if (pdata == nullptr) return ERROR_NOT_EXIST;
    return (StatusEnum)HAL_I2C_Mem_Read(
        __handle, deviceAddress,
        memAddress, I2C_MEMADD_SIZE_8BIT,
        pdata, 1U, I2C_POLL_TIMEOUT_MS);
#endif
    return ERROR_NOT_EXIST;
}

StatusEnum HW::Peripheral::I2c::Write_Polling(uint8_t* buff, int len)
{
#ifdef HAL_I2C_MODULE_ENABLED
    if (buff == nullptr || len <= 0) return ERROR_NOT_EXIST;
    return (StatusEnum)HAL_I2C_Master_Transmit(
        __handle, deviceAddress,
        buff, (uint16_t)len, I2C_POLL_TIMEOUT_MS);
#endif
    return ERROR_NOT_EXIST;
}

StatusEnum HW::Peripheral::I2c::Read_Polling(uint8_t* buff, int len)
{
#ifdef HAL_I2C_MODULE_ENABLED
    if (buff == nullptr || len <= 0) return ERROR_NOT_EXIST;
    return (StatusEnum)HAL_I2C_Master_Receive(
        __handle, deviceAddress,
        buff, (uint16_t)len, I2C_POLL_TIMEOUT_MS);
#endif
    return ERROR_NOT_EXIST;
}

/*==============================================================================
  IT
==============================================================================*/

StatusEnum HW::Peripheral::I2c::SendMem_IT(uint16_t memAddress, uint8_t data)
{
#ifdef HAL_I2C_MODULE_ENABLED
    static uint8_t txBuf;   ///< Static — must stay valid until MemTxCplt ISR fires
    txBuf = data;

    __txError = false;

    if (HAL_I2C_Mem_Write_IT(__handle, deviceAddress,
            memAddress, I2C_MEMADD_SIZE_8BIT,
            &txBuf, 1U) != HAL_OK)  return ERROR_HAL;

    return WaitTxDone(I2C_POLL_TIMEOUT_MS);
#endif
    return ERROR_NOT_EXIST;
}

StatusEnum HW::Peripheral::I2c::GetMem_IT(uint16_t memAddress, uint8_t* pdata)
{
#ifdef HAL_I2C_MODULE_ENABLED
    if (pdata == nullptr) return ERROR_NOT_EXIST;

    __rxError = false;

    if (HAL_I2C_Mem_Read_IT(__handle, deviceAddress,
            memAddress, I2C_MEMADD_SIZE_8BIT,
            pdata, 1U) != HAL_OK)   return ERROR_HAL;

    return WaitRxDone(I2C_POLL_TIMEOUT_MS);
#endif
    return ERROR_NOT_EXIST;
}

StatusEnum HW::Peripheral::I2c::Write_IT(uint8_t* buff, int len)
{
#ifdef HAL_I2C_MODULE_ENABLED
    if (buff == nullptr || len <= 0) return ERROR_NOT_EXIST;

    __txError = false;

    if (HAL_I2C_Master_Transmit_IT(__handle, deviceAddress,
            buff, (uint16_t)len) != HAL_OK) return ERROR_HAL;

    return WaitTxDone((uint32_t)(len + 100));
#endif
    return ERROR_NOT_EXIST;
}

StatusEnum HW::Peripheral::I2c::Read_IT(uint8_t* buff, int len)
{
#ifdef HAL_I2C_MODULE_ENABLED
    if (buff == nullptr || len <= 0) return ERROR_NOT_EXIST;

    __rxError = false;

    if (HAL_I2C_Master_Receive_IT(__handle, deviceAddress,
            buff, (uint16_t)len) != HAL_OK) return ERROR_HAL;

    return WaitRxDone((uint32_t)(len + 100));
#endif
    return ERROR_NOT_EXIST;
}

/*==============================================================================
  DMA
==============================================================================*/

StatusEnum HW::Peripheral::I2c::SendMem_DMA(uint16_t memAddress, uint8_t data)
{
#ifdef HAL_I2C_MODULE_ENABLED
    static uint8_t txBuf;   ///< Static — must stay valid until MemTxCplt ISR fires
    txBuf = data;

    __txError = false;

    if (HAL_I2C_Mem_Write_DMA(__handle, deviceAddress,
            memAddress, I2C_MEMADD_SIZE_8BIT,
            &txBuf, 1U) != HAL_OK)  return ERROR_HAL;

    return WaitTxDone(I2C_POLL_TIMEOUT_MS);
#endif
    return ERROR_NOT_EXIST;
}

StatusEnum HW::Peripheral::I2c::GetMem_DMA(uint16_t memAddress, uint8_t* pdata)
{
#ifdef HAL_I2C_MODULE_ENABLED
    if (pdata == nullptr) return ERROR_NOT_EXIST;

    __rxError = false;

    if (HAL_I2C_Mem_Read_DMA(__handle, deviceAddress,
            memAddress, I2C_MEMADD_SIZE_8BIT,
            pdata, 1U) != HAL_OK)   return ERROR_HAL;

    return WaitRxDone(I2C_POLL_TIMEOUT_MS);
#endif
    return ERROR_NOT_EXIST;
}

StatusEnum HW::Peripheral::I2c::Write_DMA(uint8_t* buff, int len)
{
#ifdef HAL_I2C_MODULE_ENABLED
    if (buff == nullptr || len <= 0) return ERROR_NOT_EXIST;

    __txError = false;

    if (HAL_I2C_Master_Transmit_DMA(__handle, deviceAddress,
            buff, (uint16_t)len) != HAL_OK) return ERROR_HAL;

    return WaitTxDone((uint32_t)(len + 100));
#endif
    return ERROR_NOT_EXIST;
}

StatusEnum HW::Peripheral::I2c::Read_DMA(uint8_t* buff, int len)
{
#ifdef HAL_I2C_MODULE_ENABLED
    if (buff == nullptr || len <= 0) return ERROR_NOT_EXIST;

    __rxError = false;

    if (HAL_I2C_Master_Receive_DMA(__handle, deviceAddress,
            buff, (uint16_t)len) != HAL_OK) return ERROR_HAL;

    return WaitRxDone((uint32_t)(len + 100));
#endif
    return ERROR_NOT_EXIST;
}

/*==============================================================================
  PUBLIC API — dispatch to active mode
==============================================================================*/

StatusEnum HW::Peripheral::I2c::SendMem(uint16_t memAddress, uint8_t data)
{
    switch (transferMode)
    {
        case MODE_POLLING:  return SendMem_Polling(memAddress, data);
        case MODE_IT:       return SendMem_IT(memAddress, data);
        case MODE_DMA:      return SendMem_DMA(memAddress, data);
        default:            return ERROR_NOT_EXIST;
    }
}

StatusEnum HW::Peripheral::I2c::GetMem(uint16_t memAddress, uint8_t* pdata)
{
    switch (transferMode)
    {
        case MODE_POLLING:  return GetMem_Polling(memAddress, pdata);
        case MODE_IT:       return GetMem_IT(memAddress, pdata);
        case MODE_DMA:      return GetMem_DMA(memAddress, pdata);
        default:            return ERROR_NOT_EXIST;
    }
}

StatusEnum HW::Peripheral::I2c::Write(uint8_t* buff, int len)
{
    switch (transferMode)
    {
        case MODE_POLLING:  return Write_Polling(buff, len);
        case MODE_IT:       return Write_IT(buff, len);
        case MODE_DMA:      return Write_DMA(buff, len);
        default:            return ERROR_NOT_EXIST;
    }
}

StatusEnum HW::Peripheral::I2c::Read(uint8_t* buff, int len)
{
    switch (transferMode)
    {
        case MODE_POLLING:  return Read_Polling(buff, len);
        case MODE_IT:       return Read_IT(buff, len);
        case MODE_DMA:      return Read_DMA(buff, len);
        default:            return ERROR_NOT_EXIST;
    }
}

StatusEnum HW::Peripheral::I2c::Reset()
{
#ifdef HAL_I2C_MODULE_ENABLED
    HAL_I2C_DeInit(__handle);

    if (__handle->Instance == I2C1) {
        __HAL_RCC_I2C1_FORCE_RESET();
        __HAL_RCC_I2C1_RELEASE_RESET();
    } else if (__handle->Instance == I2C2) {
        __HAL_RCC_I2C2_FORCE_RESET();
        __HAL_RCC_I2C2_RELEASE_RESET();
    }

    return (HAL_I2C_Init(__handle) == HAL_OK) ? OK : ERROR_HAL;
#endif
    return ERROR_NOT_EXIST;
}
/*==============================================================================
  Open / Close
==============================================================================*/

StatusEnum HW::Peripheral::I2c::Open(void)
{
#ifdef HAL_I2C_MODULE_ENABLED

    if (IS_I2C_ID(id) == false) return ERROR_NOT_EXIST;
    if (isOpen == true)         return ERROR_EXIST;

    /// --- Create semaphores (binary: max count = 1, initial count = 0) ---
    __semTx = osSemaphoreNew(1U, 0U, nullptr);
    __semRx = osSemaphoreNew(1U, 0U, nullptr);

    if (__semTx == nullptr || __semRx == nullptr) return ERROR_HAL;

    /// --- Load config ---
    paramConfig = (I2cConfigType*)&HARDWARE_PERIPHERAL_I2C[id];
    __handle    = paramConfig->handle;

    /// --- De-init → re-init ---
    if (HAL_I2C_DeInit(__handle) != HAL_OK) return ERROR_HAL;

    /// NOTE: Timing = 400 kHz @ PCLK1 160 MHz. Recalculate in CubeMX for your clock.
    __handle->Init.Timing           = 0x00C0EAFF;
    __handle->Init.OwnAddress1      = 0;
    __handle->Init.AddressingMode   = I2C_ADDRESSINGMODE_7BIT;
    __handle->Init.DualAddressMode  = I2C_DUALADDRESS_DISABLE;
    __handle->Init.OwnAddress2      = 0;
    __handle->Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    __handle->Init.GeneralCallMode  = I2C_GENERALCALL_DISABLE;
    __handle->Init.NoStretchMode    = I2C_NOSTRETCH_DISABLE;

    if (HAL_I2C_Init(__handle) != HAL_OK)                               return ERROR_HAL;
    if (HAL_I2CEx_ConfigAnalogFilter(__handle,
            I2C_ANALOGFILTER_ENABLE) != HAL_OK)                         return ERROR_HAL;

    /// --- Register callbacks ---
    if (HAL_I2C_RegisterCallback(__handle, HAL_I2C_MASTER_TX_COMPLETE_CB_ID,
            hardware_u5_peripheral_i2c_MasterTxCpltCallback) != HAL_OK) return ERROR_HAL;

    if (HAL_I2C_RegisterCallback(__handle, HAL_I2C_MASTER_RX_COMPLETE_CB_ID,
            hardware_u5_peripheral_i2c_MasterRxCpltCallback) != HAL_OK) return ERROR_HAL;

    if (HAL_I2C_RegisterCallback(__handle, HAL_I2C_MEM_TX_COMPLETE_CB_ID,
            hardware_u5_peripheral_i2c_MemTxCpltCallback) != HAL_OK)    return ERROR_HAL;

    if (HAL_I2C_RegisterCallback(__handle, HAL_I2C_MEM_RX_COMPLETE_CB_ID,
            hardware_u5_peripheral_i2c_MemRxCpltCallback) != HAL_OK)    return ERROR_HAL;

    if (HAL_I2C_RegisterCallback(__handle, HAL_I2C_ERROR_CB_ID,
            hardware_u5_peripheral_i2c_ErrorCallback) != HAL_OK)        return ERROR_HAL;

    /// --- Finalise ---
    isOpen                              = true;
    __gp_hardware_peripheral_i2c[id]    = this;

    return OK;

#endif
    return ERROR_NOT_EXIST;
}

StatusEnum HW::Peripheral::I2c::Close(void)
{
#ifdef HAL_I2C_MODULE_ENABLED

    if (IS_I2C_ID(id) == false) return ERROR_NOT_EXIST;
    if (isOpen == false)        return ERROR_EXIST;

    HAL_I2C_UnRegisterCallback(__handle, HAL_I2C_MASTER_TX_COMPLETE_CB_ID);
    HAL_I2C_UnRegisterCallback(__handle, HAL_I2C_MASTER_RX_COMPLETE_CB_ID);
    HAL_I2C_UnRegisterCallback(__handle, HAL_I2C_MEM_TX_COMPLETE_CB_ID);
    HAL_I2C_UnRegisterCallback(__handle, HAL_I2C_MEM_RX_COMPLETE_CB_ID);
    HAL_I2C_UnRegisterCallback(__handle, HAL_I2C_ERROR_CB_ID);

    if (HAL_I2C_DeInit(__handle) != HAL_OK) return ERROR_HAL;

    /// --- Delete semaphores ---
    osSemaphoreDelete(__semTx);
    osSemaphoreDelete(__semRx);
    __semTx = nullptr;
    __semRx = nullptr;

    __gp_hardware_peripheral_i2c[id]    = nullptr;
    isOpen                              = false;

    return OK;

#endif
    return ERROR_NOT_EXIST;
}

#endif /* STM32U5 */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
