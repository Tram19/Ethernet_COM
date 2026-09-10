/**
  ******************************************************************************
  * @file    hardware_u5_peripheral_i2c.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    2024
  * @brief   STM32U5 I2C peripheral driver — Polling / IT / DMA + FreeRTOS
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2024 Gremsy. All rights reserved.
  ******************************************************************************
*/

#ifndef __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_I2C_HPP__
#define __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_I2C_HPP__

/* Includes ------------------------------------------------------------------*/
#include <Hardware/Stm32U5/Inc/hardware_u5_peripheral_config.hpp>

#if defined(STM32U5)

#include "cmsis_os.h"   ///< FreeRTOS CMSIS-RTOS v2
//#include "stm32u5xx_hal_conf.h"
/* Exported types ------------------------------------------------------------*/
#ifdef HAL_I2C_MODULE_ENABLED
struct __I2C_HandleTypeDef;
typedef struct __I2C_HandleTypeDef I2C_HandleTypeDef;
#endif

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/// @brief STM32U5 I2C peripheral driver — Polling / IT / DMA
///
/// IT / DMA mode: transfer is kicked non-blocking, then the caller task blocks
/// on a binary semaphore until the HAL callback gives it back.
/// No volatile state flags — semaphore IS the synchronisation primitive.
///
/// Inherits Open / Close / IsOpen  from HW::Base::Common
/// Inherits id, deviceAddress      from HW::Base::I2c
class HW::Peripheral::I2c : public HW::Base::I2c
{

public:

    /// TYPEDEF ________________________________________________________________

    typedef enum
    {
        MODE_POLLING = 0,
        MODE_IT,
        MODE_DMA,

    } TransferModeEnum;

    /// PARAM __________________________________________________________________

#ifdef HAL_I2C_MODULE_ENABLED
    I2C_HandleTypeDef*  __handle{nullptr};
#endif

    TransferModeEnum    transferMode{MODE_POLLING};

    /// OPERATOR _______________________________________________________________

    /// @brief Binary semaphores — given by HAL callback, taken by WaitXxxDone()
    osSemaphoreId_t     __semTx{nullptr};
    osSemaphoreId_t     __semRx{nullptr};


    /// @brief Error flag set by ErrorCallback so WaitXxxDone() can return ERROR_HAL
    volatile bool       __txError{false};
    volatile bool       __rxError{false};
    /// FUNCTION _______________________________________________________________

    StatusEnum Open(void);
    StatusEnum Close(void);

    /** @brief Write one byte to a slave register */
    StatusEnum SendMem(uint16_t memAddress, uint8_t data) override;

    /** @brief Read one byte from a slave register */
    StatusEnum GetMem(uint16_t memAddress, uint8_t* pdata) override;

    /** @brief Master Tx — write buffer to slave */
    StatusEnum Write(uint8_t* buff, int len) override;

    /** @brief Master Rx — read buffer from slave */
    StatusEnum Read(uint8_t* buff, int len) override;

    StatusEnum Reset() override;

protected:

    /// TYPEDEF ________________________________________________________________


    /// PARAM __________________________________________________________________


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


private:

    /// TYPEDEF ________________________________________________________________


    /// PARAM __________________________________________________________________

#ifdef HAL_I2C_MODULE_ENABLED
    I2cConfigType*      paramConfig{nullptr};
#endif



    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________

    // ---- Polling ------------------------------------------------------------
    StatusEnum SendMem_Polling(uint16_t memAddress, uint8_t data);
    StatusEnum GetMem_Polling(uint16_t memAddress, uint8_t* pdata);
    StatusEnum Write_Polling(uint8_t* buff, int len);
    StatusEnum Read_Polling(uint8_t* buff, int len);

    // ---- IT -----------------------------------------------------------------
    StatusEnum SendMem_IT(uint16_t memAddress, uint8_t data);
    StatusEnum GetMem_IT(uint16_t memAddress, uint8_t* pdata);
    StatusEnum Write_IT(uint8_t* buff, int len);
    StatusEnum Read_IT(uint8_t* buff, int len);

    // ---- DMA ----------------------------------------------------------------
    StatusEnum SendMem_DMA(uint16_t memAddress, uint8_t data);
    StatusEnum GetMem_DMA(uint16_t memAddress, uint8_t* pdata);
    StatusEnum Write_DMA(uint8_t* buff, int len);
    StatusEnum Read_DMA(uint8_t* buff, int len);

    // ---- Wait (IT + DMA shared) ---------------------------------------------

    /** @brief Block current task until Tx callback gives semaphore or timeout */
    StatusEnum WaitTxDone(uint32_t timeoutMs);

    /** @brief Block current task until Rx callback gives semaphore or timeout */
    StatusEnum WaitRxDone(uint32_t timeoutMs);

    // ---- Friend callbacks ---------------------------------------------------
#ifdef HAL_I2C_MODULE_ENABLED
    friend void hardware_u5_peripheral_i2c_MasterTxCpltCallback(I2C_HandleTypeDef*);
    friend void hardware_u5_peripheral_i2c_MasterRxCpltCallback(I2C_HandleTypeDef*);
    friend void hardware_u5_peripheral_i2c_MemTxCpltCallback(I2C_HandleTypeDef*);
    friend void hardware_u5_peripheral_i2c_MemRxCpltCallback(I2C_HandleTypeDef*);
    friend void hardware_u5_peripheral_i2c_ErrorCallback(I2C_HandleTypeDef*);
#endif

};

/* Exported functions --------------------------------------------------------*/

#endif /* STM32U5 */
#endif /* __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_I2C_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
