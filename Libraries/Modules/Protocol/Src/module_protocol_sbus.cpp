 
/** 
  ******************************************************************************
  * @file    module_protocol_sbus.cpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    May 17, 2023
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
#include "Modules/Protocol/Inc/module_protocol_sbus.hpp"

#ifdef GREMSY_DEV_MODULE_PROTOCOL_SBUS
/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/

#define TAG                                                 "PROTOCOL_REMOTE_SBUS"

#define MOUDLE_PROTOCOL_REMOTE_SBUS_BUFFSIZE                25

#define MOUDLE_PROTOCOL_REMOTE_SBUS_CHANNEL_NUM             22

#define MODULE_PROTOCOL_REMOTE_SBUS_FRAME_TIME_MAX          50      /// ms
#define MODULE_PROTOCOL_REMOTE_SBUS_FRAME_TIME_MIN          7       /// ms  thoi gian nay phai lon hon 1 lan xu ly packet

#define MODULE_PROTOCOL_REMOTE_SBUS_CHANNEL_BYTE            16

#define MODULE_PROTOCOL_REMOTE_SBUS_START_BYTE              0
#define MODULE_PROTOCOL_REMOTE_SBUS_START_MASK              0x0F        /// SBUS 2.0

#define MODULE_PROTOCOL_REMOTE_SBUS_STOP_BYTE               24
#define MODULE_PROTOCOL_REMOTE_SBUS_STOP_MASK               0x00        /// SBUS 1.0

#define MODULE_PROTOCOL_REMOTE_SBUS_STATUS_BYTE             23

#define MODULE_PROTOCOL_REMOTE_SBUS_STATUS_FAILSAFE_MASK    0x04
#define MODULE_PROTOCOL_REMOTE_SBUS_STATUS_FRAMELOST_MASK   0x08

#define MODULE_PROTOCOL_REMOTE_SBUS_CHANNEL17_MASK          0x01
#define MODULE_PROTOCOL_REMOTE_SBUS_CHANNEL18_MASK          0x02

#define MODULE_PROTOCOL_REMOTE_SBUS_VALUE_MAX               1697
#define MODULE_PROTOCOL_REMOTE_SBUS_VALUE_MID               1025
#define MODULE_PROTOCOL_REMOTE_SBUS_VALUE_MIN               353
#define MODULE_PROTOCOL_REMOTE_SBUS_VALUE_RANGE             (MODULE_PROTOCOL_REMOTE_SBUS_VALUE_MID - MODULE_PROTOCOL_REMOTE_SBUS_VALUE_MIN)

#define MODULE_PROTOCOL_REMOTE_SBUS_TX_TASK_SLEEP_TIME           20    //ms
#define MODULE_PROTOCOL_REMOTE_SBUS_RX_TASK_SLEEP_TIME           1000    //ms

#define MODULE_PROTOCOL_REMOTE_SBUS_TIME_LOSTCONNECTION          2000    //ms

#define MODULE_PROTOCOL_REMOTE_SBUS_TIME_LOSTCONNECTION_COUNT    (MODULE_PROTOCOL_REMOTE_SBUS_TIME_LOSTCONNECTION/task.timeSleep) /// count


/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/*
 * S.bus decoder matrix.
 *
 * Each channel value can come from up to 3 input bytes. Each row in the
 * matrix describes up to three bytes, and each entry gives:
 *
 * - byte offset in the data portion of the frame
 * - right shift applied to the data byte
 * - mask for the data byte
 * - left shift applied to the result into the channel value
 */
typedef struct{
    uint8_t byte;
    uint8_t rshift;
    uint8_t mask;
    uint8_t lshift;
}
ModuleProtocolRemoteSbusBitPickType;

const ModuleProtocolRemoteSbusBitPickType MODULE_PROTOCOL_REMOTE_SBUS_PACKET[16][3] = {
    /*  0 */ { { 0, 0, 0xff, 0}, { 1, 0, 0x07, 8}, { 0, 0, 0x00,  0} },
    /*  1 */ { { 1, 3, 0x1f, 0}, { 2, 0, 0x3f, 5}, { 0, 0, 0x00,  0} },
    /*  2 */ { { 2, 6, 0x03, 0}, { 3, 0, 0xff, 2}, { 4, 0, 0x01, 10} },
    /*  3 */ { { 4, 1, 0x7f, 0}, { 5, 0, 0x0f, 7}, { 0, 0, 0x00,  0} },
    /*  4 */ { { 5, 4, 0x0f, 0}, { 6, 0, 0x7f, 4}, { 0, 0, 0x00,  0} },
    /*  5 */ { { 6, 7, 0x01, 0}, { 7, 0, 0xff, 1}, { 8, 0, 0x03,  9} },
    /*  6 */ { { 8, 2, 0x3f, 0}, { 9, 0, 0x1f, 6}, { 0, 0, 0x00,  0} },
    /*  7 */ { { 9, 5, 0x07, 0}, {10, 0, 0xff, 3}, { 0, 0, 0x00,  0} },
    /*  8 */ { {11, 0, 0xff, 0}, {12, 0, 0x07, 8}, { 0, 0, 0x00,  0} },
    /*  9 */ { {12, 3, 0x1f, 0}, {13, 0, 0x3f, 5}, { 0, 0, 0x00,  0} },
    /* 10 */ { {13, 6, 0x03, 0}, {14, 0, 0xff, 2}, {15, 0, 0x01, 10} },
    /* 11 */ { {15, 1, 0x7f, 0}, {16, 0, 0x0f, 7}, { 0, 0, 0x00,  0} },
    /* 12 */ { {16, 4, 0x0f, 0}, {17, 0, 0x7f, 4}, { 0, 0, 0x00,  0} },
    /* 13 */ { {17, 7, 0x01, 0}, {18, 0, 0xff, 1}, {19, 0, 0x03,  9} },
    /* 14 */ { {19, 2, 0x3f, 0}, {20, 0, 0x1f, 6}, { 0, 0, 0x00,  0} },
    /* 15 */ { {20, 5, 0x07, 0}, {21, 0, 0xff, 3}, { 0, 0, 0x00,  0} }
};



/* Private function prototypes -----------------------------------------------*/


/* Private functions ---------------------------------------------------------*/
/** @brief ham chay task xu ly
 * */
static void module_protocol_remote_sbus_task(void* arg)
{
    Module::Protocol::Sbus* psbus = (Module::Protocol::Sbus*)arg;

    psbus->__TaskProcess();
}

/** @brief ham xu ly task
 *  @param none
 *  @return trang thai qua trinh xu ly
 * */
void Module::Protocol::Sbus::__TaskProcess(void)
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
                decodeState = PROTOCOL_SBUS_DECODE_STATE_ERROR;
            }
            else if(rxStateCommon == PROTOCOL_REMOTE_STATE_PLUG)
            {
                rxStateCommon = PROTOCOL_REMOTE_STATE_RUN;
            }
            else if(rxStateCommon == PROTOCOL_REMOTE_STATE_RUN)
            {
                if(timeLostConnection++ >= MODULE_PROTOCOL_REMOTE_SBUS_TIME_LOSTCONNECTION_COUNT)
                {
                    rxStateCommon = PROTOCOL_REMOTE_STATE_UNPLUG;
                    decodeState = PROTOCOL_SBUS_DECODE_STATE_ERROR;
                }
            }
            else if(rxStateCommon == PROTOCOL_REMOTE_STATE_UNPLUG)
            {
                if(timeLostConnection < MODULE_PROTOCOL_REMOTE_SBUS_TIME_LOSTCONNECTION_COUNT)
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
                EncodePacket();
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

/* Public functions ----------------------------------------------------------*/
/** @brief ham giai nen gia tri
 *  @param none
 *  @return trang thai qua trinh giai nen
 * */
StatusEnum Module::Protocol::Sbus::DecodePacket(void)
{
    int chancount = MODULE_PROTOCOL_REMOTE_SBUS_CHANNEL_BYTE;

//    /// check frame boundary markets to avoid out-of-sync cases
//    if(buff[MODULE_PROTOCOL_REMOTE_SBUS_START_BYTE] != MODULE_PROTOCOL_REMOTE_SBUS_START_MASK)
//    {
//        return ERR;
//    }

    // check the last byte
    switch (rxBuff[MODULE_PROTOCOL_REMOTE_SBUS_STOP_BYTE])
    {
        case 0x00:
            /* this is S.BUS 1 */
            break;
        case 0x03:
            /* S.BUS 2 SLOT0: RX battery and external voltage */
            break;
        case 0x83:
            /* S.BUS 2 SLOT1 */
            break;
        case 0x43:
        case 0xC3:
        case 0x23:
        case 0xA3:
        case 0x63:
        case 0xE3:
            break;
        default:
            /* we expect one of the bits above, but there are some we don't know yet */
            return ERROR_HAL;
            break;
    }

    /** kiem tra failsafe active */
    if((rxBuff[MODULE_PROTOCOL_REMOTE_SBUS_STATUS_BYTE] & MODULE_PROTOCOL_REMOTE_SBUS_STATUS_FAILSAFE_MASK) != 0)
    {
        isFailsafe = true;
    }
    else
    {
        isFailsafe = false;
    }

    /** kiem tra frame lost*/
    if((rxBuff[MODULE_PROTOCOL_REMOTE_SBUS_STATUS_BYTE] & MODULE_PROTOCOL_REMOTE_SBUS_STATUS_FRAMELOST_MASK) != 0)
    {
        isFramelost = true;
    }
    else
    {
        isFramelost = false;
    }

    /* Use the decoder matrix to extract channel data*/
    for(int i = 0; i < chancount; i++)
    {
        uint16_t value = 0;
        ModuleProtocolRemoteSbusBitPickType* decode;

        for(int pick = 0; pick < 3; pick++)
        {
           decode = (ModuleProtocolRemoteSbusBitPickType*)&MODULE_PROTOCOL_REMOTE_SBUS_PACKET[i][pick];

           if(decode->mask != 0)
           {
               unsigned piece = rxBuff[1 + decode->byte];
               piece >>= decode->rshift;
               piece &= decode->mask;
               piece <<= decode->lshift;

               value |= piece;
           }
        }

        /* get value */
        rxChannels[i] = value;
    }

    /** decode status & 2 channel 17, 18 */
    if(chancount == 16)
    {
        ///
        rxChannelCount    = 18;

        /* channel 17 (index 16) */
        rxChannels[16]    = (rxBuff[MODULE_PROTOCOL_REMOTE_SBUS_STATUS_BYTE] & MODULE_PROTOCOL_REMOTE_SBUS_CHANNEL17_MASK) ? MODULE_PROTOCOL_REMOTE_SBUS_VALUE_MAX:MODULE_PROTOCOL_REMOTE_SBUS_VALUE_MID;

        /* channel 18 (index 17) */
        rxChannels[17]    = (rxBuff[MODULE_PROTOCOL_REMOTE_SBUS_STATUS_BYTE] & MODULE_PROTOCOL_REMOTE_SBUS_CHANNEL18_MASK) ? MODULE_PROTOCOL_REMOTE_SBUS_VALUE_MAX:MODULE_PROTOCOL_REMOTE_SBUS_VALUE_MID;
    }
    else
    {
        ///
        rxChannelCount    = chancount;

        /* channel 17 (index 16) */
        rxChannels[16]    = MODULE_PROTOCOL_REMOTE_SBUS_VALUE_MID;

        /* channel 18 (index 17) */
        rxChannels[17]    = MODULE_PROTOCOL_REMOTE_SBUS_VALUE_MID;
    }

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

    return OK;
}

/** @brief ham nen gia tri
 *  @param none
 *  @return trang thai qua trinh nen
 * */
StatusEnum Module::Protocol::Sbus::EncodePacket(void)
{
    StatusEnum ret = OK;
    int i;

    /// clear buffer
    for(i = 0; i < MOUDLE_PROTOCOL_REMOTE_SBUS_BUFFSIZE; i++)
    {
        txBuff[i] = 0;
    }

    /// startbyte
    txBuff[0] = MODULE_PROTOCOL_REMOTE_SBUS_START_MASK;

    /// decode
    for(i = 0; i < MOUDLE_PROTOCOL_REMOTE_SBUS_BUFFSIZE; i++)
    {
        ModuleProtocolRemoteSbusBitPickType* decode;

        for(int pick = 0; pick < 3; pick++)
        {
           decode = (ModuleProtocolRemoteSbusBitPickType*)&MODULE_PROTOCOL_REMOTE_SBUS_PACKET[i][pick];

           if(decode->mask != 0)
           {

               uint16_t value = txChannels[i];

               value >>= decode->lshift;
               value &= decode->mask;
               value <<= decode->rshift;

               txBuff[1 + decode->byte] |= value;
           }
        }
    }

    /// write sbus
    if(funcWriteCallback)
    {
        funcWriteCallbackArg->sender    = (uint32_t)sender;
        funcWriteCallbackArg->buffsize  = MOUDLE_PROTOCOL_REMOTE_SBUS_BUFFSIZE;
        ret = funcWriteCallback(funcWriteCallbackArg, txBuff);
    }

    return ret;
}

/* Public functions ----------------------------------------------------------*/

/** @brief ham truyen gia tri doc duoc vao protocol remote sbus
 *  @param[in] c ky tu doc duoc
 *  @return trang thai qua trinh doc
 * */
StatusEnum Module::Protocol::Sbus::ReceivedByte(uint8_t c)
{
    if((mode & PROTOCOL_REMOTE_MODE_RX_ONLY) == PROTOCOL_REMOTE_MODE_RX_ONLY)
    {
        ///
        if(decodeState == PROTOCOL_SBUS_DECODE_STATE_IDLE)
        {
            /// reset time
            timeCheck = 0;

            /// kiem tra timing
            decodeState = PROTOCOL_SBUS_DECODE_STATE_START;
        }
        else if(decodeState == PROTOCOL_SBUS_DECODE_STATE_START)
        {
            if(c == MODULE_PROTOCOL_REMOTE_SBUS_START_MASK)
            {
                uint32_t time = timeCheck>>TIME_GET;

                /// kiem tra thoi gian doc cua 2 startbyte
                if((time >= MODULE_PROTOCOL_REMOTE_SBUS_FRAME_TIME_MIN) &&
                   (time < MODULE_PROTOCOL_REMOTE_SBUS_FRAME_TIME_MAX))
                {
                    /// reset gia tri
                    /// lay du lieu
                    rxBuffCount             = 0;
                    rxBuff[rxBuffCount++]   = c;

                    decodeState = PROTOCOL_SBUS_DECODE_STATE_DATA;
                }
                else
                {
                    /// reset time
                    timeCheck = 0;
                }
            }
        }
        else if(decodeState == PROTOCOL_SBUS_DECODE_STATE_DATA)
        {
            /// lay du lieu
            rxBuff[rxBuffCount++] = c;

            /// kiem tra da du du lieu decode
            if(rxBuffCount >= MOUDLE_PROTOCOL_REMOTE_SBUS_BUFFSIZE)
            {
                if(DecodePacket() == OK)
                {
                    /// reset time detected frame
                    timeCheck           = 0;

                    /// reset time bao lost connection
                    timeLostConnection  = 0;
                }

                decodeState = PROTOCOL_SBUS_DECODE_STATE_START;
            }
        }
        else
        {
            ///
            decodeState = PROTOCOL_SBUS_DECODE_STATE_IDLE;
        }

        return OK;
    }

    return ERROR_NOT_EXIST;
}

/** @brief ham open hardware
 *  @param none
 *  @return trang thai qua trinh open
 * */
StatusEnum Module::Protocol::Sbus::Open(void)
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
        if(rxChannels == 0) rxChannels = (uint16_t*)g_calloc(sizeof(uint16_t)*MOUDLE_PROTOCOL_REMOTE_SBUS_CHANNEL_NUM);
        if(rxBuff == 0)     rxBuff     = (uint8_t*)g_calloc(MOUDLE_PROTOCOL_REMOTE_SBUS_BUFFSIZE);

        rxChannelValueMin       = MODULE_PROTOCOL_REMOTE_SBUS_VALUE_MIN;
        rxChannelValueMid       = MODULE_PROTOCOL_REMOTE_SBUS_VALUE_MID;
        rxChannelValueMax       = MODULE_PROTOCOL_REMOTE_SBUS_VALUE_MAX;
        rxChannelValueRange     = MODULE_PROTOCOL_REMOTE_SBUS_VALUE_RANGE;

        /// dinh thoi hoat dong sbus
        task.timeSleep          = MODULE_PROTOCOL_REMOTE_SBUS_RX_TASK_SLEEP_TIME;
    }

    if((mode & PROTOCOL_REMOTE_MODE_TX_ONLY) == PROTOCOL_REMOTE_MODE_TX_ONLY)
    {
        if(txChannels == 0) txChannels = (uint16_t*)g_calloc(sizeof(uint16_t)*MOUDLE_PROTOCOL_REMOTE_SBUS_CHANNEL_NUM);
        if(txBuff == 0)     txBuff     = (uint8_t*)g_calloc(MOUDLE_PROTOCOL_REMOTE_SBUS_BUFFSIZE);

        txChannelValueMin       = MODULE_PROTOCOL_REMOTE_SBUS_VALUE_MIN;
        txChannelValueMid       = MODULE_PROTOCOL_REMOTE_SBUS_VALUE_MID;
        txChannelValueMax       = MODULE_PROTOCOL_REMOTE_SBUS_VALUE_MAX;
        txChannelValueRange     = MODULE_PROTOCOL_REMOTE_SBUS_VALUE_RANGE;

        /// dinh thoi hoat dong sbus
        task.timeSleep          = MODULE_PROTOCOL_REMOTE_SBUS_TX_TASK_SLEEP_TIME;
    }

    ///
    task.name           = name;
    task.sender         = this;
    task.priority       = priority;
    task               += module_protocol_remote_sbus_task;
    task.Open();

    ///
    timeCheck           = TIME_MS;

    ///
    rxStateCommon       = PROTOCOL_REMOTE_STATE_INIT;
    txStateCommon       = PROTOCOL_REMOTE_STATE_INIT;

    isOpen              = true;

    ///
    LOGTAGI<<name<<" Sbus open done!\n";

    return ret;
}

/** @brief ham close hardware
 *  @param none
 *  @return trang thai qua trinh close
 * */
StatusEnum Module::Protocol::Sbus::Close(void)
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
    LOGTAGI<<name<<" Sbus close done!\n";

    return ret;
}

#endif /// GREMSY_DEV_MODULE_PROTOCOL_SBUS
/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
