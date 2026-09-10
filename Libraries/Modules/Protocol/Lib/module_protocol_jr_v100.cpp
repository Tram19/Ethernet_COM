 
/** 
  ******************************************************************************
  * @file    module_protocol_jr.cpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    May 22, 2023
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
#include "Modules/Protocol/Inc/module_protocol_jr.hpp"

#ifdef GREMSY_MODULE_PROTOCOL_JR_V100
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

#define TAG         "REMOTE_JR"

#define MODULE_PROTOCOL_REMOTE_JR_RX_TASK_SLEEP_TIME             1000    //ms
#define MODULE_PROTOCOL_REMOTE_JR_TX_TASK_SLEEP_TIME             20    //ms

#define MODULE_PROTOCOL_REMOTE_JR_TIME_LOSTCONNECTION            2000    //ms

#define MODULE_PROTOCOL_REMOTE_JR_TIME_LOSTCONNECTION_COUNT      (MODULE_PROTOCOL_REMOTE_JR_TIME_LOSTCONNECTION/task.timeSleep) /// count

/// JRBit
#define MODULE_PROTOCOL_REMOTE_JR_SYNC_TIME_MAX                 30000     /// Us
#define MODULE_PROTOCOL_REMOTE_JR_SYNC_TIME_MIN                 10000     /// Us

#define MOUDLE_PROTOCOL_REMOTE_JR_BUFFSIZE                      16
#define MOUDLE_PROTOCOL_REMOTE_JR_CHANNEL_NUM                   8

#define MODULE_PROTOCOL_REMOTE_JR11Bit_MASK_ID                  0x78    //0b0111 1000
#define MODULE_PROTOCOL_REMOTE_JR11Bit_RSH_ID                   0x03    //        xxx
#define MODULE_PROTOCOL_REMOTE_JR11Bit_MASK_VALUE_HIGH          0x07    //0b0000 0111
#define MODULE_PROTOCOL_REMOTE_JR11Bit_MASK_VALUE_LOW           0xFF    //0b1111 1111


#define MODULE_PROTOCOL_REMOTE_JR11Bit_VALUE_MAX                1706
#define MODULE_PROTOCOL_REMOTE_JR11Bit_VALUE_MID                1024
#define MODULE_PROTOCOL_REMOTE_JR11Bit_VALUE_MIN                342
#define MODULE_PROTOCOL_REMOTE_JR11Bit_VALUE_RANGE              (MODULE_PROTOCOL_REMOTE_JR11Bit_VALUE_MID - MODULE_PROTOCOL_REMOTE_JR11Bit_VALUE_MIN)

#define MODULE_PROTOCOL_REMOTE_JR10Bit_MASK_ID                  0x3C    //0b0011 1100
#define MODULE_PROTOCOL_REMOTE_JR10Bit_RSH_ID                   0x02    //         xx
#define MODULE_PROTOCOL_REMOTE_JR10Bit_MASK_VALUE_HIGH          0x03    //0b0000 0011
#define MODULE_PROTOCOL_REMOTE_JR10Bit_MASK_VALUE_LOW           0xFF    //0b1111 1111

#define MODULE_PROTOCOL_REMOTE_JR10Bit_VALUE_MAX                853
#define MODULE_PROTOCOL_REMOTE_JR10Bit_VALUE_MID                512
#define MODULE_PROTOCOL_REMOTE_JR10Bit_VALUE_MIN                171
#define MODULE_PROTOCOL_REMOTE_JR10Bit_VALUE_RANGE              (MODULE_PROTOCOL_REMOTE_JR10Bit_VALUE_MID - MODULE_PROTOCOL_REMOTE_JR10Bit_VALUE_MIN)

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/** @brief ham chay task xu ly
 * */
static void module_protocol_jr_task(void* arg)
{
    Module::Protocol::Jr* pjr = (Module::Protocol::Jr*)arg;

    pjr->__TaskProcess();
}

/** @brief ham xu ly task
 *  @param none
 *  @return trang thai qua trinh xu ly
 * */
void Module::Protocol::Jr::__TaskProcess(void)
{

    while(1)
    {
        /// THong  bao nhan
        if((mode & PROTOCOL_REMOTE_MODE_RX_ONLY) == PROTOCOL_REMOTE_MODE_RX_ONLY)
        {
            if(rxStateCommon == PROTOCOL_REMOTE_STATE_IDLE)
            {

            }
            else if(rxStateCommon == PROTOCOL_REMOTE_STATE_INIT)
            {
                timeLostConnection = 0;
                rxStateCommon = PROTOCOL_REMOTE_STATE_UNPLUG;
                decodeState = PROTOCOL_JR_DECODE_STATE_ERROR;
            }
            else if(rxStateCommon == PROTOCOL_REMOTE_STATE_PLUG)
            {
                rxStateCommon = PROTOCOL_REMOTE_STATE_RUN;
            }
            else if(rxStateCommon == PROTOCOL_REMOTE_STATE_RUN)
            {
                if(timeLostConnection++ >= MODULE_PROTOCOL_REMOTE_JR_TIME_LOSTCONNECTION_COUNT)
                {
                    rxStateCommon = PROTOCOL_REMOTE_STATE_UNPLUG;
                    decodeState = PROTOCOL_JR_DECODE_STATE_ERROR;
                }
            }
            else if(rxStateCommon == PROTOCOL_REMOTE_STATE_UNPLUG)
            {
                if(timeLostConnection < MODULE_PROTOCOL_REMOTE_JR_TIME_LOSTCONNECTION_COUNT)
                {
                    rxStateCommon = PROTOCOL_REMOTE_STATE_PLUG;
                }
            }
        }
        else
        {
            rxStateCommon = PROTOCOL_REMOTE_STATE_ERROR;
        }

        ///
        if((mode & PROTOCOL_REMOTE_MODE_TX_ONLY) == PROTOCOL_REMOTE_MODE_TX_ONLY)
        {
            if(txStateCommon == PROTOCOL_REMOTE_STATE_IDLE)
            {
                txStateCommon = PROTOCOL_REMOTE_STATE_INIT;
            }
            else if(txStateCommon == PROTOCOL_REMOTE_STATE_INIT)
            {
                txStateCommon = PROTOCOL_REMOTE_STATE_PLUG;
            }
            else if(txStateCommon == PROTOCOL_REMOTE_STATE_PLUG)
            {
                txStateCommon = PROTOCOL_REMOTE_STATE_RUN;
            }
            else if(txStateCommon == PROTOCOL_REMOTE_STATE_RUN)
            {
                if(type == PROTOCOL_JR_TYPE_AUTO || type == PROTOCOL_JR_TYPE_11BITS)
                {
                    Encode11BitPacket();
                }
                else
                {
                    Encode10BitPacket();
                }
            }
            else if(txStateCommon == PROTOCOL_REMOTE_STATE_UNPLUG)
            {
                txStateCommon = PROTOCOL_REMOTE_STATE_INIT;
            }

        }
        else
        {
            txStateCommon = PROTOCOL_REMOTE_STATE_ERROR;
        }

        task.Sleep();
    }
}

/** @brief ham giai nen gia tri
 *  @param none
 *  @return trang thai qua trinh giai nen
 * */
StatusEnum Module::Protocol::Jr::Decode10BitPacket(uint8_t c)
{
    uint32_t time = timeCheck>>TIME_LOOP;

    /// kiem tra thoi gian
    if(time < MODULE_PROTOCOL_REMOTE_JR_SYNC_TIME_MAX &&
       time > MODULE_PROTOCOL_REMOTE_JR_SYNC_TIME_MIN)
    {
        StatusEnum ret = OK;

//        ///
//        if(testCount++ >= 50)
//        {
//            testCount = 0;
//            LOGTAGI<<" b0: "<<rxBuff[0]<<", b1: "<<rxBuff[1]<<"\n";
//        }

        /// kiem tra dieu kien vao decode
        if(rxBuffCount == (MOUDLE_PROTOCOL_REMOTE_JR_BUFFSIZE - 1))
        {
            uint8_t i;
            uint8_t ID;

            for(i = 2; i < MOUDLE_PROTOCOL_REMOTE_JR_BUFFSIZE; i += 2)
            {
                ID = ((rxBuff[i]) & MODULE_PROTOCOL_REMOTE_JR10Bit_MASK_ID) >> MODULE_PROTOCOL_REMOTE_JR10Bit_RSH_ID;

                if(ID < MOUDLE_PROTOCOL_REMOTE_JR_CHANNEL_NUM)
                {
                    if(rxChannelCount < ID)
                    {
                        rxChannelCount = ID;
                    }

                    ///
                    rxChannels[ID] = (int16_t)(((int16_t)rxBuff[i] & MODULE_PROTOCOL_REMOTE_JR10Bit_MASK_VALUE_HIGH)<<8) | ((int16_t)(rxBuff[i+1] & MODULE_PROTOCOL_REMOTE_JR10Bit_MASK_VALUE_LOW));

                    /// reset time bao lost connection
                    timeLostConnection  = 0;
                }
            }
        }
        else
        {
            ret = ERROR_HAL;
        }

        /// reset contro payload
        rxBuffCount         = 0;
        rxBuff[rxBuffCount] = c;

        return ret;
    }
    else
    {
        /// cong don con tro
        rxBuffCount = rxBuffCount + 1;

        /// kiem tra trong chan cho mang nhan
        if(rxBuffCount < MOUDLE_PROTOCOL_REMOTE_JR_BUFFSIZE)
        {
            /// luu du lieu vao payload de decode
            rxBuff[rxBuffCount] = c;
        }

        return BUSY;
    }
}

/** @brief ham giai nen gia tri
 * */
StatusEnum Module::Protocol::Jr::Decode11BitPacket(uint8_t c)
{
    uint32_t time = timeCheck>>TIME_LOOP;

    /// kiem tra thoi gian
    if(time < MODULE_PROTOCOL_REMOTE_JR_SYNC_TIME_MAX &&
       time > MODULE_PROTOCOL_REMOTE_JR_SYNC_TIME_MIN)
    {
//        ///
//        if(testCount++ >= 50)
//        {
//            testCount = 0;
//            LOGTAGI<<" b0: "<<rxBuff[0]<<", b1: "<<rxBuff[1]<<"\n";
//        }

        /// kiem tra dieu kien vao decode
        if(rxBuffCount == (MOUDLE_PROTOCOL_REMOTE_JR_BUFFSIZE - 1))
        {
            uint8_t i;
            uint8_t ID;

            for(i = 2; i < MOUDLE_PROTOCOL_REMOTE_JR_BUFFSIZE; i += 2)
            {
                ID = ((rxBuff[i]) & MODULE_PROTOCOL_REMOTE_JR11Bit_MASK_ID) >> MODULE_PROTOCOL_REMOTE_JR11Bit_RSH_ID;

                if(ID < MOUDLE_PROTOCOL_REMOTE_JR_CHANNEL_NUM)
                {
                    if(rxChannelCount < ID)
                    {
                        rxChannelCount = ID;
                    }

                    ///
                    rxChannels[ID] = (int16_t)(((int16_t)rxBuff[i] & MODULE_PROTOCOL_REMOTE_JR11Bit_MASK_VALUE_HIGH)<<8) | ((int16_t)(rxBuff[i+1] & MODULE_PROTOCOL_REMOTE_JR11Bit_MASK_VALUE_LOW));

                    /// reset time bao lost connection
                    timeLostConnection  = 0;
                }
            }
        }

        /// reset contro payload
        rxBuffCount         = 0;
        rxBuff[rxBuffCount] = c;

        return OK;
    }
    else
    {
        /// cong don con tro
        rxBuffCount = rxBuffCount + 1;

        /// kiem tra trong chan cho mang nhan
        if(rxBuffCount < MOUDLE_PROTOCOL_REMOTE_JR_BUFFSIZE)
        {
            /// luu du lieu vao payload de decode
            rxBuff[rxBuffCount] = c;
        }

        return BUSY;
    }
}

/** @brief ham nen gia tri
 *  @param none
 *  @return trang thai qua trinh nen
 * */
StatusEnum Module::Protocol::Jr::Encode11BitPacket(void)
{
    StatusEnum ret = OK;
    int i,n;

    ///
    txBuff[0] = 0x03;
    txBuff[1] = 0xAA;

    n = 0;

    /// clear buffer
    for(i = 2; i < MOUDLE_PROTOCOL_REMOTE_JR_BUFFSIZE; i += 2)
    {
        txBuff[i]   = (uint8_t)((n<<MODULE_PROTOCOL_REMOTE_JR11Bit_RSH_ID) & MODULE_PROTOCOL_REMOTE_JR11Bit_MASK_ID)|((txChannels[n]>>8)&MODULE_PROTOCOL_REMOTE_JR11Bit_MASK_VALUE_HIGH);
        txBuff[i+1] = (uint8_t)txChannels[n];
        n = n + 1;
    }

    /// write sbus
    if(funcWriteCallback)
    {
        funcWriteCallbackArg->sender    = (uint32_t)sender;
        funcWriteCallbackArg->buffsize  = MOUDLE_PROTOCOL_REMOTE_JR_BUFFSIZE;
        ret = funcWriteCallback(funcWriteCallbackArg, txBuff);
    }

    return ret;
}

/** @brief ham nen gia tri
 *  @param none
 *  @return trang thai qua trinh nen
 * */
StatusEnum Module::Protocol::Jr::Encode10BitPacket(void)
{
    StatusEnum ret = OK;
    int i,n;

    ///
    txBuff[0] = 0x03;
    txBuff[1] = 0xAA;

    n = 0;

    /// clear buffer
    for(i = 2; i < MOUDLE_PROTOCOL_REMOTE_JR_BUFFSIZE; i += 2)
    {
        txBuff[i]   = (uint8_t)((n<<MODULE_PROTOCOL_REMOTE_JR10Bit_RSH_ID) & MODULE_PROTOCOL_REMOTE_JR10Bit_MASK_ID)|((txChannels[n]>>8)&MODULE_PROTOCOL_REMOTE_JR10Bit_MASK_VALUE_HIGH);
        txBuff[i+1] = (uint8_t)txChannels[n];
        n = n + 1;
    }

    /// write sbus
    if(funcWriteCallback)
    {
        funcWriteCallbackArg->sender    = (uint32_t)sender;
        funcWriteCallbackArg->buffsize  = MOUDLE_PROTOCOL_REMOTE_JR_BUFFSIZE;
        ret = funcWriteCallback(funcWriteCallbackArg, txBuff);
    }

    return ret;
}

/* Public functions ----------------------------------------------------------*/

/** @brief ham truyen gia tri doc duoc vao protocol remote sbus
 *  @param[in] c ky tu doc duoc
 *  @return trang thai qua trinh doc
 * */
StatusEnum Module::Protocol::Jr::ReceivedByte(uint8_t c)
{
    ///
    if(decodeState == PROTOCOL_JR_DECODE_STATE_IDLE)
    {
        /// reset time
        timeCheck = 0;
        rxChannelCount = 0;

        /// kiem tra timing
        if(type == PROTOCOL_JR_TYPE_11BITS || type == PROTOCOL_JR_TYPE_AUTO)
        {
            decodeState = PROTOCOL_JR_DECODE_STATE_11BIT_SYNC;
        }
        else
        {
            decodeState = PROTOCOL_JR_DECODE_STATE_10BIT_SYNC;
        }
    }
    else if(decodeState == PROTOCOL_JR_DECODE_STATE_11BIT_SYNC)
    {
        StatusEnum state = Decode11BitPacket(c);

        if(state == OK)
        {
            if(rxChannelCount >= 6)
            {
                if(syncCorrect++ > 5)
                {
                    rxChannelValueMin   = MODULE_PROTOCOL_REMOTE_JR11Bit_VALUE_MIN;
                    rxChannelValueMid   = MODULE_PROTOCOL_REMOTE_JR11Bit_VALUE_MID;
                    rxChannelValueMax   = MODULE_PROTOCOL_REMOTE_JR11Bit_VALUE_MAX;
                    rxChannelValueRange = MODULE_PROTOCOL_REMOTE_JR11Bit_VALUE_RANGE;
                    decodeState = PROTOCOL_JR_DECODE_STATE_11BIT_DECODE;
                }
            }
            else
            {
                if(syncError++ > 20)
                {
                    syncError = 0;
                    syncCorrect = 0;
                    if(type == PROTOCOL_JR_TYPE_AUTO)
                    {
                        decodeState = PROTOCOL_JR_DECODE_STATE_10BIT_SYNC;
                    }
                    else
                    {
                        decodeState = PROTOCOL_JR_DECODE_STATE_ERROR;
                    }
                }
            }

            rxChannelCount = 0;
        }
        else if(state == ERROR_HAL)
        {
            if(syncError++ > 20)
            {
                syncError = 0;
                syncCorrect = 0;
                if(type == PROTOCOL_JR_TYPE_AUTO)
                {
                    decodeState = PROTOCOL_JR_DECODE_STATE_10BIT_SYNC;
                }
                else
                {
                    decodeState = PROTOCOL_JR_DECODE_STATE_ERROR;
                }
            }
        }
    }
    else if(decodeState == PROTOCOL_JR_DECODE_STATE_10BIT_SYNC)
    {
        StatusEnum state = Decode10BitPacket(c);

        if(state == OK)
        {
            if(rxChannelCount >= 6)
            {
                if(syncCorrect++ > 5)
                {
                    rxChannelValueMin   = MODULE_PROTOCOL_REMOTE_JR10Bit_VALUE_MIN;
                    rxChannelValueMid   = MODULE_PROTOCOL_REMOTE_JR10Bit_VALUE_MID;
                    rxChannelValueMax   = MODULE_PROTOCOL_REMOTE_JR10Bit_VALUE_MAX;
                    rxChannelValueRange = MODULE_PROTOCOL_REMOTE_JR10Bit_VALUE_RANGE;
                    decodeState = PROTOCOL_JR_DECODE_STATE_10BIT_DECODE;
                }
            }
            else
            {
                if(syncError++ > 20)
                {
                    syncError = 0;
                    decodeState = PROTOCOL_JR_DECODE_STATE_ERROR;
                }
            }

            rxChannelCount = 0;
        }
        else if(state == ERROR_HAL)
        {
            if(syncError++ > 20)
            {
                syncError = 0;
                decodeState = PROTOCOL_JR_DECODE_STATE_ERROR;
            }
        }
    }
    else if(decodeState == PROTOCOL_JR_DECODE_STATE_10BIT_DECODE)
    {
        if(Decode10BitPacket(c) == OK)
        {
            /** goi ham tra ve gia tri cua channel
             * */
            if(funcDataReadyCallback)
            {
                funcDataReadyCallbackArg->sender            = (uint32_t)sender;
                funcDataReadyCallbackArg->channelCount      = rxChannelCount;
                funcDataReadyCallbackArg->channelValueMax   = rxChannelValueMax;
                funcDataReadyCallbackArg->channelValueMin   = rxChannelValueMin;
                funcDataReadyCallbackArg->channelValueMid   = rxChannelValueMid;
                funcDataReadyCallbackArg->channelValueRange = rxChannelValueRange;
                return funcDataReadyCallback(funcDataReadyCallbackArg, rxChannels);
            }
        }
        else
        {

        }
    }

    else if(decodeState == PROTOCOL_JR_DECODE_STATE_11BIT_DECODE)
    {
        if(Decode11BitPacket(c) == OK)
        {
            /** goi ham tra ve gia tri cua channel
             * */
            if(funcDataReadyCallback)
            {
                funcDataReadyCallbackArg->sender            = (uint32_t)sender;
                funcDataReadyCallbackArg->channelCount      = rxChannelCount;
                funcDataReadyCallbackArg->channelValueMax   = rxChannelValueMax;
                funcDataReadyCallbackArg->channelValueMin   = rxChannelValueMin;
                funcDataReadyCallbackArg->channelValueMid   = rxChannelValueMid;
                funcDataReadyCallbackArg->channelValueRange = rxChannelValueRange;
                return funcDataReadyCallback(funcDataReadyCallbackArg, rxChannels);
            }
        }
        else
        {

        }
    }

    else
    {
        ///
        decodeState = PROTOCOL_JR_DECODE_STATE_IDLE;
    }

    return OK;
}

/** @brief ham open hardware
 *  @param none
 *  @return trang thai qua trinh open
 * */
StatusEnum Module::Protocol::Jr::Open(void)
{
    StatusEnum ret = OK;

    /// CHECK
    if(name.IsExist() == ERROR_NOT_EXIST || mode == PROTOCOL_REMOTE_MODE_NONE)
    {
        LOGTAGE<<name<<" mode or name is not set!\n";
        return ERROR_NOT_EXIST;
    }

    if(isOpen == true)
    {
        LOGTAGE<<name<<" has been open!\n";
        return ERROR_EXIST;
    }

    /// DEFAULT
    if(priority == PRIORITY_NONE)
    {
        priority = PRIORITY_NORMAL;
    }

    /// PARAM
    if((mode & PROTOCOL_REMOTE_MODE_RX_ONLY) == PROTOCOL_REMOTE_MODE_RX_ONLY)
    {
        if(rxChannels == 0) rxChannels = (uint16_t*)g_calloc(sizeof(uint16_t)*MOUDLE_PROTOCOL_REMOTE_JR_CHANNEL_NUM);
        if(rxBuff == 0)     rxBuff     = (uint8_t*)g_calloc(MOUDLE_PROTOCOL_REMOTE_JR_BUFFSIZE);

        /// dinh thoi hoat dong jr
        task.timeSleep = MODULE_PROTOCOL_REMOTE_JR_RX_TASK_SLEEP_TIME;
    }

    if((mode & PROTOCOL_REMOTE_MODE_TX_ONLY) == PROTOCOL_REMOTE_MODE_TX_ONLY)
    {
        if(txChannels == 0) txChannels = (uint16_t*)g_calloc(sizeof(uint16_t)*MOUDLE_PROTOCOL_REMOTE_JR_CHANNEL_NUM);
        if(txBuff == 0)     txBuff     = (uint8_t*)g_calloc(MOUDLE_PROTOCOL_REMOTE_JR_BUFFSIZE);

        /// dinh thoi hoat dong jr
        task.timeSleep = MODULE_PROTOCOL_REMOTE_JR_TX_TASK_SLEEP_TIME;
    }

    ///
    task.name           = name;
    task.sender         = this;
    task.priority       = priority;
    task               += module_protocol_jr_task;
    task.Open();

    ///
    timeCheck           = TIME_US;

    ///
    rxStateCommon       = PROTOCOL_REMOTE_STATE_INIT;
    txStateCommon       = PROTOCOL_REMOTE_STATE_INIT;

    isOpen              = true;

    ///
    LOGTAGI<<name<<" JR open done!\n";

    return ret;
}

/** @brief ham close hardware
 *  @param none
 *  @return trang thai qua trinh close
 * */
StatusEnum Module::Protocol::Jr::Close(void)
{
    StatusEnum ret = OK;

    ///
    if(name.IsExist() == ERROR_NOT_EXIST || mode == PROTOCOL_REMOTE_MODE_NONE)
    {
        LOGTAGE<<name<<" mode or name is not set!\n";
        return ERROR_NOT_EXIST;
    }

    if(isOpen == true)
    {
        LOGTAGE<<name<<" has been close!\n";
        return ERROR_EXIST;
    }

    ///
    task.Close();

    ///
    if(rxChannels != 0) g_free(rxChannels);
    if(rxBuff != 0)     g_free(rxBuff);

    if(txChannels != 0) g_free(txChannels);
    if(txBuff != 0)     g_free(txBuff);

    rxChannels  = 0;
    rxBuff      = 0;

    txChannels  = 0;
    txBuff      = 0;

    ///
    isOpen = false;

    ///
    LOGTAGI<<name<<" JR close done!\n";

    return ret;
}

#endif  /// GREMSY_MODULE_PROTOCOL_JR_V100
/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
