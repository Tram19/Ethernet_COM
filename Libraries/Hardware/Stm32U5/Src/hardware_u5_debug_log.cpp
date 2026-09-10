 
/** 
  ******************************************************************************
  * @file    hardware_u5_debug_log.cpp
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

#include "Hardware/Stm32U5/Inc/hardware_u5_debug_log.hpp"

#if defined STM32U5 || defined STM32H5

/* Private typedef -----------------------------------------------------------*/

#define HW_DEBUG_LOG_TX_BUFF_SIZE       256
#define HW_DEBUG_LOG_TX_BUFF_TEMP_SIZE  HW_DEBUG_LOG_TX_BUFF_SIZE

typedef struct
{
    RTOS::TaskEvent task;

    uint8_t         txBuff[HW_DEBUG_LOG_TX_BUFF_SIZE];
    uint8_t         txBuffTemp[HW_DEBUG_LOG_TX_BUFF_TEMP_SIZE];
    uint8_t         txBuffCount;
    uint8_t         txBuffCountOld;
}
LogPrivateType;

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

LogPrivateType* __gp_log_private;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

#ifdef DEBUG_UART
/** @brief function callback when UART transmit complete
 *  @param[in] huart variable address of UART Handler
 *  @return none
 * */
static void hardware_h7_debug_log_uart_tx_cplt_callback(struct __UART_HandleTypeDef *huart)
{
    __gp_log_private->task.Set(0x01);
}

/** @brief task process receiver
 *  @param[in] arg variable address of object
 *  @return none
 * */
static void hardware_h7_debug_log_uart_tx_task(void* arg)
{
    LogPrivateType* plog = __gp_log_private;

    while(1)
    {
        uint8_t n = plog->txBuffCount - plog->txBuffCountOld;

        if(n > 0)   /// chi su dung duoc voi size bang do lon cua kieu bien vd u8 -> buffsize = 256
        {
            StatusEnum ret = OK;
            uint8_t i,j;

            /// @brief su dung mang temp de tranh TH nhieu tag goi loc mang txBuff
            ///        se bi chep de truyen sai thong tin
            for(i = 0; i < n; i++)
            {
                j = plog->txBuffCountOld + i;           /// chi su dung duoc voi size bang do lon cua kieu bien vd u8 -> buffsize = 256
                plog->txBuffTemp[i] = plog->txBuff[j];
            }

            ret = (StatusEnum)HAL_UART_Transmit_DMA(DEBUG_UART, plog->txBuffTemp, n);

            if(ret == OK)
            {
                if(plog->task.Wait(0x01, 1000) == 0x01)
                {
                    plog->txBuffCountOld = plog->txBuffCount;
                }
            }
            else
            {
                plog->task.Sleep();
            }
        }
        else
        {
            plog->task.Sleep();
        }
    }
}
#endif

/* Public functions ----------------------------------------------------------*/

/** @brief ham set truyen bang dma
 *  @param none
 *  @return none
 * */
void HW::Debug::Log::SetTransmitBuffer()
{
#ifdef DEBUG_UART
    isTransmitBuffer = true;
#endif
}

/** @brief ham truyen gia tri trong mang
 *  @param[in] buff     dia chi mang truyen
 *  @param[in] size     do lon mang truyen
 *  @return trang thai qua trinh truyen
 * */
StatusEnum HW::Debug::Log::Write(uint8_t* buff, int size)
{
#ifdef DEBUG_UART
    StatusEnum ret = OK;

    if(isTransmitBuffer == false){
        ret = (StatusEnum)HAL_UART_Transmit(DEBUG_UART, buff, size, size*100);
    }
    else {
        /// dua vao mang truyen
        if(size > 0)
        {
            LogPrivateType* plog = __gp_log_private;

            for(int i = 0; i < size; i++)
            {
                if(plog->txBuffCount >= HW_DEBUG_LOG_TX_BUFF_SIZE)  plog->txBuffCount = 0;
                plog->txBuff[plog->txBuffCount++] = buff[i];
            }
        }
    }

    return ret;
#else
    return ERROR_HAL;
#endif
}

/** @brief ham truyen gia tri trong mang
 *  @param[in] str    dia chi chuoi can truyen
 *  @return trang thai qua trinh truyen
 * */
StatusEnum HW::Debug::Log::Write(const char* str)
{
    return Write((uint8_t*)str, strlen(str));
}

/** @brief ham truyen thong tin cua debug
 *  @param none
 *  @return none
 * */
void HW::Debug::Log::Info(void)
{
    Write("--------------------------------------\n");
    Write("Company: Gremsy\n");
    Write("Author:  Firmware Team\n");
    Write("Version: 1.0.0\n");
    Write("Platform: Stm32H7xx\n");
    Write("--------------------------------------\n");
}

/** @brief ham cai dat ban dau cho log
 * */
StatusEnum HW::Debug::Log::Init(void)
{
#ifdef DEBUG_UART
    StatusEnum ret = OK;

    if(DEBUG_UART->hdmatx)
    {
        /// Cap phat vung nho
        LogPrivateType* plog;

        __gp_log_private = (LogPrivateType*)g_calloc(sizeof(LogPrivateType));
        plog = __gp_log_private;

        /// TASK EVENT
        plog->task.name     = "LOG";
        plog->task.option   = FLAGS_WAIT_ANY;
        plog->task.size     = 1028;
        plog->task.priority = PRIORITY_LOW;
        plog->task.timeSleep= 100;
        plog->task         += hardware_h7_debug_log_uart_tx_task;
        plog->task.Open();

        /// EVENT
        DEBUG_UART->TxCpltCallback = hardware_h7_debug_log_uart_tx_cplt_callback;
    }
    else
    {
        HAL_UART_Transmit(DEBUG_UART, (uint8_t*)"Log khong ho tro truyen uart qua dma! Can cai dat tx dma de su dung.\n", 68, 10000);
    }

    return ret;
#endif
    return ERROR_NOT_EXIST;
}

#endif /** STM32U5 */
/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
