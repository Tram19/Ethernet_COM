 
/** 
  ******************************************************************************
  * @file    hardware_u5_peripheral_uart.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Nov 25, 2024
  * @brief   This file contains all the functions prototypes for the hardware_u5_peripheral_uart.cpp 
  *          firmware library.
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2018 Gremsy. All rights reserved.
  *
  * The information contained herein is confidential
  * property of Company. The use, copying, transfer or 
  * disclosure of such information is prohibited except
  * by express written agreement with Company.
  *
  ******************************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_UART_HPP__
#define __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_UART_HPP__

/* Includes ------------------------------------------------------------------*/
#include <Hardware/Stm32U5/Inc/hardware_u5_peripheral_config.hpp>
#include <Hardware/hardware.hpp>
#include <mutex>
#include <atomic>
#if defined STM32U5 || defined STM32H5

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
static constexpr size_t TX_STREAM_SIZE = 4096;
/* Exported class ------------------------------------------------------------*/

/// @brief
class HW::Peripheral::Uart : public HW::Base::Uart {

public:

    /// TYPEDEF ________________________________________________________________
    Uart() = default;
    Uart(UartIdEnum _id) : HW::Base::Uart(_id){};
    Uart(UartIdEnum _id, uint16_t _txBufferSize) : HW::Base::Uart(_id), txBufferSize(_txBufferSize){};
    /// PARRAM _________________________________________________________________
    uint16_t                        txBufferSize{0};



    /** @brief bien su dung private */
#ifdef HAL_UART_MODULE_ENABLED
    UART_HandleTypeDef* __handle;
#endif

    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________

    /** @brief ham open hardware
     *  @param none
     *  @return trang thai qua trinh open
     * */
    StatusEnum Open(void);

    /** @brief ham close hardware
     *  @param none
     *  @return trang thai qua trinh close
     * */
    StatusEnum Close(void);

    /** @brief ham truyen gia tri
     *  @param[in] str chuoi can truyen
     *  @return trang thai qua trinh truyen
     * */

    StatusEnum Reset(void);
    StatusEnum Write(Type::String str);

    /** @brief ham truyen gia tri
     *  @param[in] buf dia chi mang truyen
     *  @param[in] len do dang mang truyen
     *  @return trang thai qua trinh truyen
     * */
    StatusEnum Write(uint8_t* buff, int len);
    /**
     * @brief UART transmit data
     *
     * @param[in] data  Data buffer to transmit.
     *                  std::span is a non-owning view,
     *                  no memory allocation or copy is performed.
     *                  Buffer must remain valid during transmission
     *                  (especially when using DMA/Interrupt).
     *
     * @return StatusEnum
     *         - OK        : Transmission successful
     *         - BUSY      : UART is not opened
     *         - ERROR_HAL : HAL error or TX timeout
     */
    StatusEnum Write(std::span<const uint8_t> data);
    StatusEnum Write(const char* str);
    /** @breif ham get pin tx
     * */
    PinEnum GetTxPin(void){
        return txPin;
    }

    /** @breif ham get pin rx
     * */
    PinEnum GetRxPin(void){
        return rxPin;
    }

    /// PRIVATE ________________________________________________________________

    /** @brief ham xu ly loi
     *  @param none
     *  @return none
     * */
    void __ErrorProcess(void);

    /** @brief ham xu ly du lieu nhan duoc
     *  @param none
     *  @return none
     * */
    void __RXProcess(void);

    /** @brief ham xu ly du lieu truyen di
     *  @param none
     *  @return none
     * */
    void __TXITCpltProcess(void);

    /** @brief ham xu ly du lieu truyen di
     *  @param none
     *  @return none
     * */
    void __RXITCpltProcess(void);

protected:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________



    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


private:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________

    /// RX
    uint8_t*                            rxBuff       = nullptr;
    uint8_t*                            rxBuffTemp   = nullptr;
    int                                 rxBuffPtrOld = 0;
    RTOS::Task                          rxTask;

    /// TX — thay RTOS::Event txEvent bằng stream buffer + dedicated task
    static constexpr size_t             TX_SCRATCH_SIZE = 4096;

    RTOS::StreamBufferStatic<TX_STREAM_SIZE>    _txStreamBuf;
    RTOS::TaskStatic<2048>                      _txTask{PRIORITY_HIGH, [this](){ _TxRunLoop(); }, "UartTx"};
    uint8_t                                     _txScratch[TX_SCRATCH_SIZE]{}; // ← thêm mới

    /// TX internal loop — chạy trong _txTask
    void _TxRunLoop(void);                                           // ← thêm mới
    volatile uint32_t errorPending{0};
    bool _uart_debug{false};
};

/* Exported functions --------------------------------------------------------*/

#endif /** STM32FU5 */
#endif /* __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_UART_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
