 
/** 
  ******************************************************************************
  * @file    hardware_u5_debug_command_line.cpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Apr 12, 2023
  * @brief   
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2023 Gremsy.  
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

#include "Hardware/Stm32U5/Inc/hardware_u5_debug_command_line.hpp"

#if defined STM32U5 || defined STM32H5
/* Private typedef -----------------------------------------------------------*/

#define DEBUG_COMMANDLINE_RXBUFFER_SIZE         256
#define DEBUG_COMMANDLINE_CMDBUFFER_SIZE        256
#define DEBUG_COMMANDLINE_CMDBUFFERTEMP_SIZE    256

/// dinh nghia struct command line
typedef struct
{
//    Type::Element   ele;
//    Type::Element   eleGet;
    RTOS::Task      task;

    /// DATA
    Type::Node      node;

    /// RX
    uint8_t         rxBuff[DEBUG_COMMANDLINE_RXBUFFER_SIZE];
    int32_t         rxBuffOld;


    /// cmd buff
    uint8_t         cmdBuff[DEBUG_COMMANDLINE_CMDBUFFER_SIZE];
    int32_t         cmdBuffIndex;
    int32_t         cmdLen;
    uint8_t         cmdBuffTemp[DEBUG_COMMANDLINE_CMDBUFFERTEMP_SIZE];
}
CommandLinePrivateType;

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/** bien cai dat tinh trong qua trinh doc command */
static CommandLinePrivateType*    __gp_cmdline;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/** @brief ham doc du lieu da nhan duoc
 *  @param none
 *  @return so du lieu nhan duoc
 * */
static int32_t hardware_u5_debug_command_line_get_data_count_current(void)
{
#ifdef DEBUG_UART
    return DEBUG_COMMANDLINE_RXBUFFER_SIZE - __HAL_DMA_GET_COUNTER(DEBUG_UART->hdmarx);
#else
    return 0;
#endif
}

/** @brief ham chay task an cua cmd line
 *  @param
 *  @return
 * */
static void hardware_u5_debug_command_line_task(void* arg)
{
    CommandLinePrivateType* pcmd = (CommandLinePrivateType*)arg;

    while(1)
    {
        HW::Debug::CommandLine::Process();
        pcmd->task.Sleep();
    }
}

/** @brief function callback when UART error occurred
 *  @param[in] huart variable address of UART handler
 *  @return none
 * */
void hardware_u5_debug_command_line_error_callback(struct __UART_HandleTypeDef *huart)
{
#ifdef DEBUG_UART
    if(DEBUG_UART->ErrorCode == HAL_UART_ERROR_ORE)
    {
        /** @brief cap phat dia chi mang va do lon mang cho dma rx uart */
        if(DEBUG_UART->hdmarx)
        {
            if(DEBUG_UART->hdmarx->Init.Mode != DMA_LINKEDLIST_CIRCULAR)
            {
                HAL_UART_Receive_DMA(DEBUG_UART, __gp_cmdline->rxBuff, DEBUG_COMMANDLINE_RXBUFFER_SIZE);
            }
        }
        else
        {
            HAL_UART_Receive_IT(DEBUG_UART, __gp_cmdline->rxBuff, DEBUG_COMMANDLINE_RXBUFFER_SIZE);
        }
    }
#endif
}

/** @brief function callback when UART transmit complete
 *  @param[in] huart variable address of UART Handler
 *  @return none
 * */
void hardware_u5_debug_command_line_rx_cplt_callback(struct __UART_HandleTypeDef *huart)
{
#ifdef DEBUG_UART
    /** @brief cap phat dia chi mang va do lon mang cho dma rx uart */
    if(DEBUG_UART->hdmarx)
    {
        if(DEBUG_UART->hdmarx->Init.Mode != DMA_LINKEDLIST_CIRCULAR)
        {
            HAL_UART_Receive_DMA(DEBUG_UART, __gp_cmdline->rxBuff, DEBUG_COMMANDLINE_RXBUFFER_SIZE);
        }
    }
    else
    {
        HAL_UART_Receive_IT(DEBUG_UART, __gp_cmdline->rxBuff, DEBUG_COMMANDLINE_RXBUFFER_SIZE);
    }
#endif
}

/* Public functions ----------------------------------------------------------*/

/** @brief ham tim duong dan
 *  @param[in] dir  dia chi duong dan
 *  @return element trong node
 * */
HW::Debug::CommandLine::CommandLineElementType* HW::Debug::CommandLine::Search(Type::Node* pnode, const char* dir, int dirLen)
{
    if(pnode != 0)
    {
        CommandLineElementType*     pelement    = (CommandLineElementType*)pnode->ptrData;

        if(pelement != 0)
        {
            if(strncmp(dir, pelement->dir, dirLen) == 0)
            {
                return pelement;
            }
            else
            {
                return Search(pnode->ptrNext, dir, dirLen);
            }
        }
        else
        {
            return Search(pnode->ptrNext, dir, dirLen);
        }
    }

    return 0;
}

/** @brief ham add command cho command line
 *  @param[in] dir duong dan lenh
 *  @param[in] func ham nhan lenh
 *  @param[in] funcArg dia chi bien su dung trong ham
 *  @return trang thai cai dat
 * */
StatusEnum HW::Debug::CommandLine::Add(const char* Dir, FuncCommandLineNoReturnCallbackType func, void* funcArg)
{
#ifdef DEBUG_UART

    if(DEBUG_UART->hdmarx != 0)
    {
        CommandLineElementType*     pcmd     = (CommandLineElementType*)g_calloc(sizeof(CommandLineElementType));
        CommandLineNoReturnType*    pcmdRet  = (CommandLineNoReturnType*)g_calloc(sizeof(CommandLineNoReturnType));

        pcmd->type      = COMMAND_LINE_FUNCTION_NO_RETURN;
        pcmd->func      = (void*)func;

        pcmdRet->sender = (uint32_t)funcArg;
        pcmd->funcArg   = pcmdRet;

        pcmd->dir       = Dir;
        pcmd->dirLen    = strlen(Dir);

        if(__gp_cmdline->node.Append(pcmd) == ERROR_EXIST)
        {
            g_free(pcmd);
            g_free(pcmdRet);
        }

        return OK;
    }

#endif

    return ERROR_NOT_EXIST;
}

/** @brief ham add command cho command line
 *  @param[in] dir duong dan lenh
 *  @param[in] func ham nhan lenh
 *  @param[in] funcArg dia chi bien su dung trong ham
 *  @return trang thai cai dat
 * */
StatusEnum HW::Debug::CommandLine::Add(const char* Dir, FuncCommandLineReturnNumberCallbackType func, void* funcArg)
{
#ifdef DEBUG_UART

    if(DEBUG_UART->hdmarx != 0)
    {
        CommandLineElementType*         pcmd     = (CommandLineElementType*)g_calloc(sizeof(CommandLineElementType));
        CommandLineReturnNumberType*    pcmdRet  = (CommandLineReturnNumberType*)g_calloc(sizeof(CommandLineReturnNumberType));

        pcmd->type      = COMMAND_LINE_FUNCTION_RETURN_NUMBER;
        pcmd->func      = (void*)func;

        pcmdRet->sender = (uint32_t)funcArg;
        pcmd->funcArg   = pcmdRet;

        pcmd->dir       = Dir;
        pcmd->dirLen    = strlen(Dir);

        if(__gp_cmdline->node.Append(pcmd) == ERROR_EXIST)
        {
            g_free(pcmd);
            g_free(pcmdRet);
        }

        return OK;
    }

#endif

    return ERROR_NOT_EXIST;
}

/** @brief ham add command cho command line
 *  @param[in] dir duong dan lenh
 *  @param[in] func ham nhan lenh
 *  @param[in] funcArg dia chi bien su dung trong ham
 *  @return trang thai cai dat
 * */
StatusEnum HW::Debug::CommandLine::Add(const char* Dir, FuncCommandLineReturnStringCallbackType func, void* funcArg)
{
#ifdef DEBUG_UART

    if(DEBUG_UART->hdmarx != 0)
    {
        CommandLineElementType*         pcmd     = (CommandLineElementType*)g_calloc(sizeof(CommandLineElementType));
        CommandLineReturnStringType*    pcmdRet  = (CommandLineReturnStringType*)g_calloc(sizeof(CommandLineReturnStringType));

        pcmd->type      = COMMAND_LINE_FUNCTION_RETURN_STRING;
        pcmd->func      = (void*)func;

        pcmdRet->sender = (uint32_t)funcArg;
        pcmd->funcArg   = pcmdRet;

        pcmd->dir       = Dir;
        pcmd->dirLen    = strlen(Dir);

        if(__gp_cmdline->node.Append(pcmd) == ERROR_EXIST)
        {
            g_free(pcmd);
            g_free(pcmdRet);
        }

        return OK;
    }

#endif

    return ERROR_NOT_EXIST;
}

/** @brief ham xu lu data nhan duoc tu uart va tao lenh
 *  @param none
 *  @return none
 * */
void HW::Debug::CommandLine::Process(void)
{
    CommandLinePrivateType* pcmd = __gp_cmdline;
    int32_t n     = hardware_u5_debug_command_line_get_data_count_current();
    int32_t delta = n - pcmd->rxBuffOld;
    int32_t i,j;

    if(delta < 0)
    {
        delta += DEBUG_COMMANDLINE_RXBUFFER_SIZE;
    }

    if(delta > 0)
    {
        for(i = 0 ; i < delta; i++)
        {
            /// di toi vi tri trong buffer dma
            j = pcmd->rxBuffOld + i;

            /// quay dau khi tran mang
            if(j >= DEBUG_COMMANDLINE_RXBUFFER_SIZE)    j -= DEBUG_COMMANDLINE_RXBUFFER_SIZE;

            /// kiem tra gia tri dac biet
            /// gia tri ket thuc lenh
            if(pcmd->rxBuff[j] == '\n' || pcmd->rxBuff[j] == '\r')
            {
                /// chot ky tu cuoi
                pcmd->cmdBuff[pcmd->cmdBuffIndex] = 0;

                if(pcmd->cmdLen == 0)
                {
                    pcmd->cmdLen = pcmd->cmdBuffIndex;
                }

                ///
                if(pcmd->cmdLen != 0)
                {
                    CommandLineElementType* pelement = Search(&pcmd->node, (const char*)pcmd->cmdBuff, pcmd->cmdLen);

                    if(pelement != 0)
                    {
                        if(pelement->type == COMMAND_LINE_FUNCTION_NO_RETURN)
                        {
                            if(pelement->func != 0)
                            {
                                FuncCommandLineNoReturnCallbackType func = (FuncCommandLineNoReturnCallbackType)pelement->func;

                                func((CommandLineNoReturnType*)pelement->funcArg);
                            }
                        }
                        else if(pelement->type == COMMAND_LINE_FUNCTION_RETURN_NUMBER)
                        {
                            if(pelement->func != 0)
                            {
                                FuncCommandLineReturnNumberCallbackType func = (FuncCommandLineReturnNumberCallbackType)pelement->func;
                                CommandLineReturnNumberType* funcarg = (CommandLineReturnNumberType*)pelement->funcArg;
                                int num = 0;

                                /// dua vao mang tam truoc khi cho dua tra ve ham callback
                                /// vi khi mang quay dau gia tri tra ve se loi
                                int len = pcmd->cmdBuffIndex - pcmd->cmdLen - 1;

                                if(len < 0)
                                {
                                    len += DEBUG_COMMANDLINE_RXBUFFER_SIZE;
                                }

                                for(int n = 0; n < len; n++)
                                {
                                    /// di toi vi tri trong buffer dma
                                    int m = pcmd->cmdLen + 1 + n;

                                    /// quay dau khi tran mang
                                    if(m >= DEBUG_COMMANDLINE_RXBUFFER_SIZE)    m -= DEBUG_COMMANDLINE_RXBUFFER_SIZE;
                                    pcmd->cmdBuffTemp[n] = pcmd->cmdBuff[m];
                                }

                                pcmd->cmdBuffTemp[len] = 0;
                                num = atoi((const char*)pcmd->cmdBuffTemp);

                                funcarg->number = num;
                                func(funcarg);
                            }
                        }
                        else if(pelement->type == COMMAND_LINE_FUNCTION_RETURN_STRING)
                        {
                            if(pelement->func != 0)
                            {
                                FuncCommandLineReturnStringCallbackType func    = (FuncCommandLineReturnStringCallbackType)pelement->func;
                                CommandLineReturnStringType*            funcarg = (CommandLineReturnStringType*)pelement->funcArg;

                                /// dua vao mang tam truoc khi cho dua tra ve ham callback
                                /// vi khi mang quay dau gia tri tra ve se loi
                                int len = pcmd->cmdBuffIndex - pcmd->cmdLen - 1;

                                if(len < 0)
                                {
                                    len += DEBUG_COMMANDLINE_RXBUFFER_SIZE;
                                }

                                for(int n = 0; n < len; n++)
                                {
                                    /// di toi vi tri trong buffer dma
                                    int m = pcmd->cmdLen + 1 + n;

                                    /// quay dau khi tran mang
                                    if(m >= DEBUG_COMMANDLINE_RXBUFFER_SIZE)    m -= DEBUG_COMMANDLINE_RXBUFFER_SIZE;
                                    pcmd->cmdBuffTemp[n] = pcmd->cmdBuff[m];
                                }

                                pcmd->cmdBuffTemp[len] = 0;
                                funcarg->len = len;

                                func(funcarg, (const char*)pcmd->cmdBuffTemp);
                            }
                        }
                    }
                    else
                    {
                        /// TODO Them ham tra ve khi khong dung command
                    }
                }

                /// reset
                pcmd->cmdBuffIndex = 0;
                pcmd->cmdLen       = 0;
            }
            else if(pcmd->rxBuff[j] == ':')
            {
                pcmd->cmdLen = pcmd->cmdBuffIndex;
            }
            else
            {
                if(pcmd->cmdBuffIndex >= DEBUG_COMMANDLINE_CMDBUFFER_SIZE)
                {
                    pcmd->cmdBuffIndex = 0;
                }

                pcmd->cmdBuff[pcmd->cmdBuffIndex++] = pcmd->rxBuff[j];
            }

        }

        pcmd->rxBuffOld = n;
    }
}

/** @brief ham cai dat chung cho command line
 *  @param none
 *  @return none
 *  @note ham cai dat toan cuc cho command line vi day la su dung chung DEBUG_UART
 * */
StatusEnum HW::Debug::CommandLine::Init(void)
{
#ifdef DEBUG_UART
    /// cap phat vung nho
    __gp_cmdline                = (CommandLinePrivateType*)g_calloc(sizeof(CommandLinePrivateType));
    __gp_cmdline->cmdBuffIndex  = 0;

    /// tao task hoat dong
    __gp_cmdline->task.name         = "CMDLINE";
    __gp_cmdline->task.size         = 2048;
    __gp_cmdline->task.sender       = __gp_cmdline;
    __gp_cmdline->task.timeSleep    = 1000;
    __gp_cmdline->task             += hardware_u5_debug_command_line_task;
    __gp_cmdline->task.Open();

    /// RX
    DEBUG_UART->ErrorCallback  = hardware_u5_debug_command_line_error_callback;
    DEBUG_UART->RxCpltCallback = hardware_u5_debug_command_line_rx_cplt_callback;

    /// cai dat dma rx
    if(DEBUG_UART->hdmarx)
    {
        return (StatusEnum)HAL_UART_Receive_DMA(DEBUG_UART, __gp_cmdline->rxBuff, DEBUG_COMMANDLINE_RXBUFFER_SIZE);
    }
    else
    {
        return (StatusEnum)HAL_UART_Receive_IT(DEBUG_UART, __gp_cmdline->rxBuff, DEBUG_COMMANDLINE_RXBUFFER_SIZE);
    }

    return ERROR_NOT_EXIST;
#endif

    return ERROR_HAL;
}

#endif /** STM32U5 */
/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
