 
/** 
  ******************************************************************************
  * @file    hardware_u5_peripheral_uart.cpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Nov 25, 2024
  * @brief   
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2024 Gremsy.  
  * All rights reserved.
  *
  * The information contained herein is confidential
  * property of Company. The use, copying, transfer or 
  * disclosure of such information is prohibited except
  * by express written agreement with Company.
  *
  ******************************************************************************
*/ 

/* Includes ------------------------------------------------------------------*/

#include "Hardware/Stm32U5/Inc/hardware_u5_peripheral_uart.hpp"

#if defined STM32U5 || defined STM32H5
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

///
#define TAG     "UART"

///
#define HW_UART_READ_BUFFTSIZE              254
#define HW_UART_READ_BUFFTEMP_SIZE          254
#define HW_UART_READ_TIME                   10      //ms

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

///
#ifdef HAL_UART_MODULE_ENABLED

#if !defined(USE_HAL_UART_REGISTER_CALLBACKS) || (USE_HAL_UART_REGISTER_CALLBACKS != 1U)
#error "hardware_u5_peripheral_uart requires USE_HAL_UART_REGISTER_CALLBACKS = 1U in stm32u5xx_hal_conf.h"
#endif

static HW::Peripheral::Uart*    __gp_hardware_peripheral_uart[UART_COUNT];
static RTOS::StreamBufferStatic<TX_STREAM_SIZE> _txBufPool[UART_COUNT - 1];
static bool     _txBufPoolUsed[UART_COUNT - 1]{0};
#endif

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

#ifdef HAL_UART_MODULE_ENABLED
/** @brief function check and get address of UART peripheral
 *  @param[in] huart variable address of UART handler
 *  @return address class UART
 * */
static HW::Peripheral::Uart* hardware_u5_peripheral_uart_get_pinstance(struct __UART_HandleTypeDef *huart)
{

    for(int i = 0; i < UART_COUNT; i++)
    {
        if(__gp_hardware_peripheral_uart[i] != 0)
        {
            if(__gp_hardware_peripheral_uart[i]->__handle == huart)    return __gp_hardware_peripheral_uart[i];
        }

    }

    return 0;
}

/** @brief function callback when UART error occurred
 *  @param[in] huart variable address of UART handler
 *  @return none
 * */
void hardware_u5_peripheral_uart_error_callback(struct __UART_HandleTypeDef *huart)
{
    HW::Peripheral::Uart* puart = hardware_u5_peripheral_uart_get_pinstance(huart);

    if(puart)
    {
        puart->__ErrorProcess();
    }
}

/** @brief function callback when UART transmit complete
 *  @param[in] huart variable address of UART Handler
 *  @return none
 * */
void hardware_u5_peripheral_uart_tx_cplt_callback(struct __UART_HandleTypeDef *huart)
{
    HW::Peripheral::Uart* puart = hardware_u5_peripheral_uart_get_pinstance(huart);

    if(puart)
    {
        puart->__TXITCpltProcess();
    }
}

/** @brief function callback when UART transmit complete
 *  @param[in] huart variable address of UART Handler
 *  @return none
 * */
void hardware_u5_peripheral_uart_rx_cplt_callback(struct __UART_HandleTypeDef *huart)
{
    HW::Peripheral::Uart* puart = hardware_u5_peripheral_uart_get_pinstance(huart);

    if(puart)
    {
        puart->__RXITCpltProcess();
    }
}

/** @brief task process receiver
 *  @param[in] arg variable address of object
 *  @return none
 * */
static void hardware_u5_peripheral_uart_rx_task(void* arg)
{
    HW::Peripheral::Uart* puart = (HW::Peripheral::Uart*)arg;
    puart->__RXProcess();
}

/** @brief ham xu lu loi
 *  @param none
 *  @return none
 * */
static uint32_t errcode = 0;
void HW::Peripheral::Uart::__ErrorProcess(void)
{
    // if(funcErrorCallback)
    // {
    //     ptrFuncErrorCallbackArg->code = __handle->ErrorCode;
    //     funcErrorCallback(ptrFuncErrorCallbackArg);
    // }

//    HAL_UART_DMAStop(__handle);

    if (__handle->ErrorCode & HAL_UART_ERROR_PE)
    {
        __HAL_UART_CLEAR_PEFLAG(__handle);
    }
    if (__handle->ErrorCode & HAL_UART_ERROR_NE)
    {
        __HAL_UART_CLEAR_NEFLAG(__handle);
    }
    if (__handle->ErrorCode & HAL_UART_ERROR_FE)
    {
        __HAL_UART_CLEAR_FEFLAG(__handle);
    }
    if (__handle->ErrorCode & HAL_UART_ERROR_ORE)
    {
        __HAL_UART_CLEAR_OREFLAG(__handle);
    }

    errorPending = __handle->ErrorCode; 
    errcode = __handle->ErrorCode; 
    //ISR chỉ nên làm tối thiểu (clear flag, có thể set 1 flag/notify)
    // còn xử lý thật (restart DMA, reset buffer) nên đẩy ra 1 task riêng hoặc kết hợp vào chính rxTask đã có sẵn.
    // Cách này vừa tránh giữ ISR quá lâu (an toàn hệ thống), vừa cho phép bạn đặt critical section rõ ràng,
}

/** @brief ham xu ly du lieu nhan duoc
 *  @param none
 *  @return none
 * */

void HW::Peripheral::Uart::__RXProcess(void)
{
    while(1)
    {
        if (errorPending != 0)
        {
            uint32_t code = errorPending;
            errorPending = 0;

            LOGS << "UART[" << (int)id << "] error handled, code=" << code << LEND;

            if (code!=0)
            {
                if (__handle->hdmarx)
                {
                    memset(rxBuff, 0, readBufferSize);
                    rxBuffPtrOld = 0;
                    HAL_UART_Receive_DMA(__handle, rxBuff, readBufferSize);
                }
                else
                {
                    memset(rxBuff, 0, readBufferSize);
                    rxBuffPtrOld = 0;
                    HAL_UART_Receive_IT(__handle, rxBuff, readBufferSize);
                }
            }
            }
        if (_uart_debug)
        {
            if (errcode != 0)
            {
                static uint32_t last_log_time = 0;
                uint32_t now = osKernelGetTickCount();
                if ((now - last_log_time) >= 5000U) // throttle log every 5 seconds
                {
                    last_log_time = now;
                LOGS << "UART[" << (int)id << "] error code=" << errcode << LEND;
                }
            }
        }
        int cnt_dma, cnt, n;

        if(__handle->hdmarx)
        {
            cnt_dma = __HAL_DMA_GET_COUNTER(__handle->hdmarx);
        }
        else
        {
            cnt_dma = __handle->RxXferCount;
        }

        cnt     = readBufferSize - cnt_dma;
        n       = cnt - rxBuffPtrOld;

        if (_uart_debug && id == 3)
        {
            static uint32_t loopCounter = 0;
            loopCounter++;
        
            LOGS << "U3 #" << loopCounter
                 << " cntDma=" << cnt_dma
                 << " cnt=" << cnt
                 << " ptrOld=" << rxBuffPtrOld
                 << " n=" << n
                 << " state=" << (int)__handle->RxState
                 << " err=" << __handle->ErrorCode
                 << LEND;
        }
        uint8_t* logDataPtr = nullptr;
        int logDataSize = 0;

        if(n < 0)
        {
            int i, j;
            n += readBufferSize;

            if(funcPacketDataReadyCallback)
            {
                if(n > HW_UART_READ_BUFFTEMP_SIZE)
                {
                    n = HW_UART_READ_BUFFTEMP_SIZE;
                    cnt = rxBuffPtrOld + n;
                }
            }

            for(i = 0; i < n; i++)
            {
                j = rxBuffPtrOld + i;
                if(j >= readBufferSize) j -= readBufferSize;

                if(funcByteDataReadyCallback && ptrFuncByteDataReadyCallbackArg != nullptr)
                {
                    ptrFuncByteDataReadyCallbackArg->c       = rxBuff[j];
                    ptrFuncByteDataReadyCallbackArg->id      = id;
                    ptrFuncByteDataReadyCallbackArg->sender  = (uint32_t)sender;
                    funcByteDataReadyCallback(ptrFuncByteDataReadyCallbackArg);
                }

                rxBuffTemp[i] = rxBuff[j];
            }

            if(funcPacketDataReadyCallback)
            {
                ptrFuncPacketDataReadyCallbackArg->dataSize  = n;
                ptrFuncPacketDataReadyCallbackArg->id        = id;
                ptrFuncPacketDataReadyCallbackArg->sender    = (uint32_t)sender;
                funcPacketDataReadyCallback(ptrFuncPacketDataReadyCallbackArg, rxBuffTemp);
            }

            if(lambdaPacketReadyCallback)
            {
                lambdaPacketReadyCallback(rxBuffTemp, n);
            }

            logDataPtr = rxBuffTemp;
            logDataSize = n;

            rxBuffPtrOld = cnt;
        }
        else if(n > 0)
        {
            if(funcByteDataReadyCallback)
            {
                int i, j;

                for(i = 0; i < n; i++)
                {
                    j = rxBuffPtrOld + i;
                    ptrFuncByteDataReadyCallbackArg->c       = rxBuff[j];
                    ptrFuncByteDataReadyCallbackArg->id      = id;
                    ptrFuncByteDataReadyCallbackArg->sender  = (uint32_t)sender;
                    funcByteDataReadyCallback(ptrFuncByteDataReadyCallbackArg);
                }
            }

            if(funcPacketDataReadyCallback)
            {
                ptrFuncPacketDataReadyCallbackArg->dataSize  = n;
                ptrFuncPacketDataReadyCallbackArg->id        = id;
                ptrFuncPacketDataReadyCallbackArg->sender    = (uint32_t)sender;
                funcPacketDataReadyCallback(ptrFuncPacketDataReadyCallbackArg, &rxBuff[rxBuffPtrOld]);
            }

            if(lambdaPacketReadyCallback)
            {
                lambdaPacketReadyCallback(&rxBuff[rxBuffPtrOld], n);
            }

            logDataPtr = &rxBuff[rxBuffPtrOld];
            logDataSize = n;

            rxBuffPtrOld = cnt;
        }

// ================= LOG DEBUG THROTTLE 5 GIÂY (GOM CHUỖI IN 1 LẦN) =================
        if (_uart_debug && id == 2 && logDataSize > 0 && logDataPtr != nullptr)
        {
            static uint32_t last_uart3_log_ms = 0;
            uint32_t now = osKernelGetTickCount();

            if ((now - last_uart3_log_ms) >= 1000U)
            {
                last_uart3_log_ms = now;

                char hexBuffer[128] = {0};
                char strBuffer[128] = {0};
                
                int hexPos = 0;
                int strPos = 0;

                for (int idx = 0; idx < logDataSize; idx++)
                {
                    uint8_t ch = logDataPtr[idx];

                    // 1. Gom chuỗi HEX (ví dụ: "FD 01 02 ")
                    if (hexPos < (sizeof(hexBuffer) - 4)) {
                        hexPos += snprintf(&hexBuffer[hexPos], sizeof(hexBuffer) - hexPos, "%02X ", ch);
                    }

                    // 2. Gom chuỗi STR (hiển thị ký tự đọc được)
                    if (strPos < (sizeof(strBuffer) - 3)) {
                        if (ch >= 32 && ch <= 126) {
                            strBuffer[strPos++] = (char)ch;
                        } else if (ch == '\r') {
                            strBuffer[strPos++] = '\\';
                            strBuffer[strPos++] = 'r';
                        } else if (ch == '\n') {
                            strBuffer[strPos++] = '\\';
                            strBuffer[strPos++] = 'n';
                        } else {
                            strBuffer[strPos++] = '.';
                        }
                    }
                }

                // In đúng 1 lần duy nhất cho mỗi loại log
                LOGS << "UART[2] RX (" << logDataSize << "B) HEX: " << hexBuffer << LEND;
               // LOGS << "UART[2] RX STR: " << strBuffer << LEND;
            }
        }
    
        // =========================================================================

        rxTask.Sleep();
    }
}

/** @brief ham xu ly du lieu truyen trong ngat
 *  @param none
 *  @return none
 * */
void HW::Peripheral::Uart::__TXITCpltProcess(void)
{
    if(!_txTask.IsOpen()) return;

    BaseType_t woken = pdFALSE;
    _txTask.NotifyFromISR(&woken);
    portYIELD_FROM_ISR(woken);
}

/** @brief ham xu ly du lieu nhan trong ngat
 *  @param none
 *  @return none
 * */
void HW::Peripheral::Uart::__RXITCpltProcess(void)
{
    /** @brief cap phat dia chi mang va do lon mang cho dma rx uart */
    if(__handle->hdmarx)
    {
        if(__handle->hdmarx->Init.Mode == DMA_NORMAL)
        {
            HAL_UART_Receive_DMA(__handle, rxBuff, readBufferSize);
        }
    }
    else
    {
        HAL_UART_Receive_IT(__handle, rxBuff, readBufferSize);
    }
}

/* ---------------------------------------------------------------------------*/
/*  TX RunLoop — ← thêm mới, chạy trong _txTask                               */
/* ---------------------------------------------------------------------------*/

void HW::Peripheral::Uart::_TxRunLoop(void)
{
    g_os_delay(1);
    if(!_txStreamBuf.IsOpen()) {

    	LOGI<< "ID: " << id << "   _txStreamBuf not open 123!\n";
        return;
    }

    for(;;)
    {

        uint16_t rxLen = _txStreamBuf.Receive(_txScratch); // lay du lieu tu tx tream buffer (fifo), copy du lieu vao txScratch

        if(rxLen == 0) continue;


        if(__handle->hdmatx)
        {
            HAL_UART_Transmit_DMA(__handle, _txScratch, rxLen);
        }
        else
        {
            HAL_UART_Transmit_IT(__handle, _txScratch, rxLen);
        }

        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);


    }
}

#endif /* HAL_UART_MODULE_ENABLED */

/* Public functions ----------------------------------------------------------*/

StatusEnum HW::Peripheral::Uart::Write(Type::String str)
{
#ifdef HAL_UART_MODULE_ENABLED
    if(!isOpen) return BUSY;
    std::span<const uint8_t> data(
        reinterpret_cast<const uint8_t*>(str.array), str.length);
    return Write(data);   
#else
    return OK;
#endif
}

/** @brief ham truyen gia tri
 *  @param[in] buf dia chi mang truyen
 *  @param[in] len do dang mang truyen
 *  @return trang thai qua trinh truyen
 * */
StatusEnum HW::Peripheral::Uart::Write(uint8_t* buff, int len)
{
#ifdef HAL_UART_MODULE_ENABLED
    if(!isOpen) return BUSY;
    return Write(std::span<const uint8_t>(buff, static_cast<size_t>(len)));
#else
    return OK;
#endif
}

StatusEnum HW::Peripheral::Uart::Write(std::span<const uint8_t> data)
{
#ifdef HAL_UART_MODULE_ENABLED
    if(!isOpen)       return BUSY;
    if(data.empty())  return OK;
    if(data.size() > TX_STREAM_SIZE / 2) return ERROR_PARAM;

    // xStreamBufferSend — thread-safe với 1 writer; nếu multi-writer thêm mutex ngoài
    size_t sent = _txStreamBuf.Send(const_cast<uint8_t*>(data.data()), data.size(),
                                    pdMS_TO_TICKS(10));
    return (sent == data.size()) ? OK : ERROR_HAL;
#else
    return OK;
#endif
}

StatusEnum HW::Peripheral::Uart::Write(const char* str)
{
#ifdef HAL_UART_MODULE_ENABLED
    if(!str) return ERROR_PARAM;
    return Write(std::span<const uint8_t>(
        reinterpret_cast<const uint8_t*>(str), strlen(str)));
#else
    return OK;
#endif
}
/** @brief ham open hardware
 *  @param none
 *  @return trang thai qua trinh open
 * */
StatusEnum HW::Peripheral::Uart::Open(void)
{
#ifdef HAL_UART_MODULE_ENABLED
    StatusEnum ret = OK;

    /// CHECK
    if(!IS_UART_ID(id))
    {
        return ERROR_NOT_EXIST;
    }

    if(isOpen == true)
    {
        return ERROR_EXIST;
    }

    /// PARAM DEFAULT
    __handle = HARDWARE_UART_CONFIG[id].handle;
    txPin    = g_hardware_convert_gpio_to_pin_enum(HARDWARE_UART_CONFIG[id].txPort, HARDWARE_UART_CONFIG[id].txPin);
    rxPin    = g_hardware_convert_gpio_to_pin_enum(HARDWARE_UART_CONFIG[id].rxPort, HARDWARE_UART_CONFIG[id].rxPin);

    if(readBufferSize == 0)
    {
        readBufferSize = HW_UART_READ_BUFFTSIZE;
    }

    if(readTime == 0)
    {
        readTime = HW_UART_READ_TIME;
    }

    if(baudrate == 0)
    {
        baudrate = __handle->Init.BaudRate;
    }

    if(priority == PRIORITY_NONE)
    {
        priority = PRIORITY_LOW;
    }

    /// UART INIT
    ret = (StatusEnum)HAL_UART_DeInit(__handle);
    if(ret != OK)
    {
        return ret;
    }

    __handle->Init.BaudRate                   = baudrate;
    __handle->Init.StopBits                   = HARDWARE_UART_STOPBIT[stopbits];
    __handle->Init.Parity                     = HARDWARE_UART_PARITY[parity];
    __handle->Init.WordLength                 = HARDWARE_UART_WORDLENGTH[wordlength];
    ret = (StatusEnum)HAL_UART_Init(__handle);
    if(ret != OK)
    {
        return ret;
    }

    __handle->RxCpltCallback                  = hardware_u5_peripheral_uart_rx_cplt_callback;
    __handle->TxCpltCallback                  = hardware_u5_peripheral_uart_tx_cplt_callback;
    __handle->ErrorCallback                   = hardware_u5_peripheral_uart_error_callback;

    /// CAP PHAT VUNG NHO
    if(rxBuff == 0)
    {
        rxBuff      = (uint8_t*)g_calloc(readBufferSize);
    }

    if(rxBuffTemp == 0)
    {
        rxBuffTemp  = (uint8_t*)g_calloc(HW_UART_READ_BUFFTEMP_SIZE);
    }

    rxBuffPtrOld= 0;

    /// UART INIT
    /** @brief cap phat dia chi mang va do lon mang cho dma rx uart */
    if(__handle->hdmarx)
    {
        ret = (StatusEnum)HAL_UART_Receive_DMA(__handle, rxBuff, readBufferSize);
        if(ret != OK)
        {
            return ret;
        }
    }
    else
    {
        ret = (StatusEnum)HAL_UART_Receive_IT(__handle, rxBuff, readBufferSize);
        if(ret != OK)
        {
            return ret;
        }
    }

    /// TASK READ
    rxTask.name         = HARDWARE_UART_CONFIG[id].name;
    rxTask.priority     = priority;
    rxTask.size         = (readTaskSize > 0) ? readTaskSize : 4096;
    rxTask.timeSleep    = readTime;
    rxTask.sender       = this;
    rxTask             += hardware_u5_peripheral_uart_rx_task;
    ret = rxTask.Open();
    if(ret != OK)
    {
        return ret;
    }

//    {
//        bool claimed = false;
//        for (size_t i = 0; i < UART_COUNT; ++i) {
//            if (!_txBufPoolUsed[i]) {
//                _txStreamBuf      = &_txBufPool[i];
//                _txBufPoolUsed[i] = true;
//                claimed           = true;
//                break;
//            }
//        }
//        if (!claimed) return ERROR_MEMORY;
//    }

    ret = _txStreamBuf.Open();
    if (ret != OK) {
        return ret;
    }
    if(!_txStreamBuf.IsOpen())
    {
    	LOGS << "Uart id: " << id << " not opened"<<LEND;
    }else
    {

    	//LOGS << "Uart id: " << id << " opened"<<LEND;
    }
    ret = _txTask.Open();
    if(ret != OK)
    {
        LOGS << "Uart id: " << id << " Cannot Open _txTask"<<LEND;
    	return ret;
    }

    ///
    __gp_hardware_peripheral_uart[id] = this;
    isOpen = true;

    return ret;
#else
    return ERROR_HAL;
#endif
}

/** @brief ham close hardware
 *  @param none
 *  @return trang thai qua trinh close
 * */
StatusEnum HW::Peripheral::Uart::Close(void)
{
#ifdef HAL_UART_MODULE_ENABLED
    StatusEnum ret = OK;

    /// CHECK
    if(!IS_UART_ID(id))
    {
        return ERROR_NOT_EXIST;
    }

    /// PARAM DEFAULT
    __handle = HARDWARE_UART_CONFIG[id].handle;
    txPin    = g_hardware_convert_gpio_to_pin_enum(HARDWARE_UART_CONFIG[id].txPort, HARDWARE_UART_CONFIG[id].txPin);
    rxPin    = g_hardware_convert_gpio_to_pin_enum(HARDWARE_UART_CONFIG[id].rxPort, HARDWARE_UART_CONFIG[id].rxPin);

    /// DEINIT UART
    ret = (StatusEnum)HAL_UART_DeInit(__handle);

    // CHECK
    if(isOpen == false || rxBuff == 0 || rxBuffTemp == 0 || readBufferSize == 0 || baudrate == 0)
    {
        return ERROR_NOT_EXIST;
    }

    /// DEINIT
    isOpen                              = false;
    __gp_hardware_peripheral_uart[id]   = 0;

    /// TASK
    rxTask.Close();

    /// TX — ← thay txEvent.Close()
    _txTask.Close();

//    if (_txStreamBuf != nullptr) {
        _txStreamBuf.Close();

//        for (size_t i = 0; i < UART_COUNT; ++i) {
//            if (&_txBufPool[i] == _txStreamBuf) {
//                _txBufPoolUsed[i] = false;
//                break;
//            }
//        }
//        _txStreamBuf = nullptr;
//    }
    // StreamBufferStatic dùng static memory, không cần free thêm

    /// FREE RX buffers
    g_free(rxBuff);
    g_free(rxBuffTemp);

    ///
    rxBuff      = 0;
    rxBuffTemp  = 0;

    return ret;
#else
    return ERROR_HAL;
#endif
}

StatusEnum HW::Peripheral::Uart::Reset(void)
{
#ifdef HAL_UART_MODULE_ENABLED
    StatusEnum ret = OK;
        /// PARAM DEFAULT
    // __handle = HARDWARE_UART_CONFIG[id].handle;

    // txPin    = g_hardware_convert_gpio_to_pin_enum(HARDWARE_UART_CONFIG[id].txPort, HARDWARE_UART_CONFIG[id].txPin);
    // rxPin    = g_hardware_convert_gpio_to_pin_enum(HARDWARE_UART_CONFIG[id].rxPort, HARDWARE_UART_CONFIG[id].rxPin);
    // if(readBufferSize == 0)
    // {
    //     readBufferSize = HW_UART_READ_BUFFTSIZE;
    // }

    // if(readTime == 0)
    // {
    //     readTime = HW_UART_READ_TIME;
    // }

    // if(baudrate == 0)
    // {
    //     baudrate = __handle->Init.BaudRate;
    // }

    // if(priority == PRIORITY_NONE)
    // {
    //     priority = PRIORITY_LOW;
    // }

    // /// UART INIT
    // ret = (StatusEnum)HAL_UART_DeInit(__handle);
    // if(ret != OK)
    // {
    //     return ret;
    // }

    
   
    // __handle->Init.BaudRate                   = baudrate;
    // __handle->Init.StopBits                   = HARDWARE_UART_STOPBIT[stopbits];
    // __handle->Init.Parity                     = HARDWARE_UART_PARITY[parity];
    // __handle->Init.WordLength                 = HARDWARE_UART_WORDLENGTH[wordlength];
    // ret = (StatusEnum)HAL_UART_Init(__handle);
    // if(ret != OK)
    // {
    //     return ret;
    // }
    // __handle->RxCpltCallback = hardware_u5_peripheral_uart_rx_cplt_callback;
    // __handle->TxCpltCallback = hardware_u5_peripheral_uart_tx_cplt_callback;
    // __handle->ErrorCallback  = hardware_u5_peripheral_uart_error_callback;
    if (__handle->hdmarx)
    {
        memset(rxBuff, 0, readBufferSize);
        rxBuffPtrOld = 0;
        HAL_UART_Receive_DMA(__handle, rxBuff, readBufferSize);
    }
    else
    {
        memset(rxBuff, 0, readBufferSize);
        rxBuffPtrOld = 0;
        HAL_UART_Receive_IT(__handle, rxBuff, readBufferSize);
    }   
    LOGS << "Reset UART[" << (int)id << "]" << LEND;
    return ret;

#else
    return ERROR_HAL;
#endif
}

#endif /** STM32U5 */
/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
