 
/** 
  ******************************************************************************
  * @file    module_protocol_mavlink.cpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    May 5, 2023
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
#include "Modules/Protocol/Inc/module_protocol_mavlink.hpp"

#ifdef GREMSY_DEV_MODULE_PROTOCOL_MAVLINK

// Shut up warnings about missing declarations; TODO: should be fixed on
// mavlink/pymavlink project for when MAVLINK_SEPARATE_HELPERS is defined
#ifdef MAVLINK_SEPARATE_HELPERS
#include <ThirdParty/MavlinkV2/mavlink_helpers.h>
#endif

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

#define TAG     "PROTOCOL_MAVLINK_V2"

/* Private macro -------------------------------------------------------------*/

static uint8_t MAVLINK_CHANNEL_COUNT = 0;

//static uint32_t __gp_module_protocol_mavlink_channels_isexist[MAVLINK_COMM_NUM_BUFFERS];

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/** @brief function task process
 *  @param[in] arg argument of class
 *  @return none
 * */
static void module_protocol_mavlink_task_process(void* arg)
{
    Module::Protocol::Mavlink* pmav = (Module::Protocol::Mavlink*)arg;

    pmav->__TaskProcess();
}

/** @brief ham xu ly task chay an trong class
 *  @param none
 *  @return none
 * */
void Module::Protocol::Mavlink::__TaskProcess(void)
{

    while(1)
    {
        if(stateCommon == PROTOCOL_MAVLINK_STATE_IDLE)
        {
            if(type == PROTOCOL_MAVLINK_TYPE_MASTER)
            {
                txMessageHeartbeatTime  = 1000000; /// cho phep khi chuyen trang thai se truyen heartbeat lien
                stateCommon = PROTOCOL_MAVLINK_STATE_TRANSMIT_HEARTBEAT;
            }
            else
            {
                stateCommon = PROTOCOL_MAVLINK_STATE_LOSTCONNECTION;
            }

            task.Sleep(100);
        }
        else if(stateCommon == PROTOCOL_MAVLINK_STATE_INIT)
        {
            /// reset cac bien vao qua trinh truyen
            writeBuffPtr            = 0;
            txMessageHeartbeatTime  = 1000000; /// cho phep khi chuyen trang thai se truyen heartbeat lien
            TransmittedMessageContinuousResetTime();

            stateCommon = PROTOCOL_MAVLINK_STATE_TRANSMIT;
            task.Sleep(10);
        }
        else if(stateCommon == PROTOCOL_MAVLINK_STATE_TRANSMIT_HEARTBEAT)
        {
            /// Ham truyen heartbeat
            TransmittedMessageHeartbeat();

            /// Check transmit data
            if(writeBuffPtr > 0)
            {
                /// UART transmit data
                if(funcWriteCallback)
                {
                    funcWriteCallbackArg->buffsize   = writeBuffPtr;
                    funcWriteCallback(funcWriteCallbackArg, writeBuff);
                }

                /// reset pointer buffer
                writeBuffPtr = 0;
            }

            /// Kiem tra heartbeat tu doi tuong tra ve de truyen trang thai truyen
            /// lien tuc cac msg da mac dinh san
            /// Vi khoang thoi gian doi nen thoi gian truyen heartbeat se cham hon bt (da kiem tra 0.7Hz)
            if(task.Wait(0x01, 500) == 0x01)
            {
                /// reset cac bien vao qua trinh truyen
                writeBuffPtr            = 0;
                TransmittedMessageContinuousResetTime();

                ///
                stateCommon = PROTOCOL_MAVLINK_STATE_TRANSMIT;
            }
        }
        else if(stateCommon == PROTOCOL_MAVLINK_STATE_TRANSMIT)
        {
            /// Ham truyen heartbeat
            TransmittedMessageHeartbeat();

            /// Function transmit once call
            TransmittedMessageOnce();

            /// Function transmit continuous call
            TransmittedContinousManyAtOnceProcess();

            /// Check transmit data
            if(writeBuffPtr > 0)
            {
                /// UART transmit data
                if(funcWriteCallback)
                {
                    funcWriteCallbackArg->buffsize   = writeBuffPtr;
                    funcWriteCallback(funcWriteCallbackArg, writeBuff);
                }

                /// reset pointer buffer
                writeBuffPtr = 0;
            }
            else
            {

            }

            /// Time check lost connection
            if(timeCheckConnection > 3000)
            {
                LOGTAGW<<name<<" Lost connection!\n";
                stateCommon = PROTOCOL_MAVLINK_STATE_LOSTCONNECTION;
            }

            /// GAP Time Between 2 msg
            task.Sleep(1);
        }
        else if(stateCommon == PROTOCOL_MAVLINK_STATE_LOSTCONNECTION)
        {
            if(type == PROTOCOL_MAVLINK_TYPE_MASTER)
            {
                stateCommon = PROTOCOL_MAVLINK_STATE_TRANSMIT_HEARTBEAT;
            }
            else
            {
                if(task.Wait(0x01, 3000) == 0x01)
                {
                    stateCommon = PROTOCOL_MAVLINK_STATE_RECONNECTION;
                }
            }
        }
        else if(stateCommon == PROTOCOL_MAVLINK_STATE_RECONNECTION)
        {
            if(task.Wait(0x01, 3000) == 0x01)
            {
                LOGTAGI<<name<<" Reconnection!\n";
                stateCommon = PROTOCOL_MAVLINK_STATE_INIT;
            }
            else
            {
                stateCommon = PROTOCOL_MAVLINK_STATE_LOSTCONNECTION;
            }
        }
        else if(stateCommon == PROTOCOL_MAVLINK_STATE_DONE)
        {
            /// delay
            task.Sleep(3000);
        }
        else if(stateCommon == PROTOCOL_MAVLINK_STATE_ERROR)
        {
            /// delay
            task.Sleep(3000);
        }
        else
        {
            /// delay
            task.Sleep(3000);
        }
    }
}

/** @brief ham parse trong mavlink
 *  @param[in] c gia tri nhan duoc theo dang byte
 *  @return trang thai qua trinh parse
 * */
StatusEnum Module::Protocol::Mavlink::Parse(uint8_t c)
{
    if(mavlink_parse_char(channel, c, &rxMessage, &rxStatus))
    {
        /// xu ly msg heartbeat
        if(rxMessage.msgid == MAVLINK_MSG_ID_HEARTBEAT)
        {
            ReceivedMessageHeartbeat();
        }
        /// xu ly cac msg khac cai dat trong mavlink
        else
        {
            StatusEnum ret = ReceivedMessageHanlde(&rxMessage);

            if(ret == ERROR_NOT_EXIST)
            {
                if(funcReceivedMessageIsNotExistCallback)
                {
                    funcReceivedMessageIsNotExistCallbackArg->sender = (uint32_t)sender;
                    funcReceivedMessageIsNotExistCallback(funcReceivedMessageIsNotExistCallbackArg, &rxMessage);
                }
            }

            return ret;
        }
    }

    return BUSY;
}

/** @brief ham xu ly qua trinh nhan msg trong mavlink
 *  @param[in] pmsg dia chi msg nhan duoc
 *  @return trang thai qua trinh xu ly nhan msg
 * */
StatusEnum Module::Protocol::Mavlink::ReceivedMessageHanlde(mavlink_message_t* pmsg)
{
    StatusEnum ret      = ERROR_NOT_EXIST;
    Type::Node* ptrnode = &rxMessageNode;

    ///
    while(ptrnode != NULL)
    {
        if(ptrnode->ptrData != NULL)
        {
            MessageReceiverType* msgreceiver = (MessageReceiverType*)ptrnode->ptrData;

            ///
            if(msgreceiver->msgID == pmsg->msgid)
            {
                if(msgreceiver->func)
                {
                    FuncProtocolMavlinkReceivedMessageCallbackType func = (FuncProtocolMavlinkReceivedMessageCallbackType)msgreceiver->func;
                    /// Tinh toan thoi gian
                    msgreceiver->funcArg.time = msgreceiver->time>>TIME_LOOP;

                    /// call function
                    ret = func(&msgreceiver->funcArg, pmsg);
                }
                return ret;
            }
        }

        ptrnode = ptrnode->ptrNext;
    }

    return ret;
}

/** @brief ham reset thoi gian truyen du lieu lien tuc
 *  @param none
 *  @return trang thai qua trinh reset time
 * */
StatusEnum Module::Protocol::Mavlink::TransmittedMessageContinuousResetTime(void)
{
    StatusEnum ret = ERROR_NOT_EXIST;
    Type::Node* ptrnode = &txContinuousMessageNode;

    ///
    while(ptrnode != NULL)
    {
        if(ptrnode->ptrData != NULL)
        {
            MessageTransmitType* msgtransmit = (MessageTransmitType*)ptrnode->ptrData;

            ///
            msgtransmit->time = 0;

            ///
            ret = OK;
        }

        ptrnode = ptrnode->ptrNext;
    }

    return ret;
}

/** @breif ham truyen onece trong mavlink
 *  @param none
 *  @return trang thai qua trinh truyen once
 * */
StatusEnum Module::Protocol::Mavlink::TransmittedMessageOnce(void)
{
    StatusEnum ret = OK;
    int delta = txMessageOnceCount - txMessageOnceIndex;

    ///
    if(delta != 0)
    {
        MessageTransmitType* msgtransmit;

        /// Kiem tra index
        if(txMessageOnceIndex >= MAVLINK_TX_MSG_ONESHOOT_NUM)
        {
            txMessageOnceIndex = 0;
        }

        /// Lay dia chi
        msgtransmit = &txMessageOnce[txMessageOnceIndex];

        /// kiem tra ham cai dat
        if(msgtransmit->func)
        {
            FuncProtocolMavlinkTransmittedMessageCallbackType func = (FuncProtocolMavlinkTransmittedMessageCallbackType)msgtransmit->func;
            ///
            ret = func(&msgtransmit->funcArg, &txMessage);

            ///
            if(ret == OK)
            {
                uint32_t len = writeBuffPtr + msgtransmit->funcArg.msgBuffLen;

                ///
                if(len < writeBufferSize)
                {
                    memcpy(&writeBuff[writeBuffPtr],
                           &txMessage.magic,
                           msgtransmit->funcArg.msgBuffLen);

                    writeBuffPtr = len;
                }

                /// di chuyen den dia chi tiep theo
                txMessageOnceIndex += 1;
            }
            else if(ret == BUSY)
            {
                uint32_t len = writeBuffPtr + msgtransmit->funcArg.msgBuffLen;

                if(len < writeBufferSize)
                {
                    memcpy(&writeBuff[writeBuffPtr],
                           &txMessage.magic,
                           msgtransmit->funcArg.msgBuffLen);

                    writeBuffPtr = len;
                }
            }
        }
    }

    return ret;
}

/** @brief ham xu ly qua trinh truyen lien tuc theo tung goi mot
 *  @param none
 *  @return trang thai qua trinh xu ly
 * */
StatusEnum Module::Protocol::Mavlink::TransmittedContinousOneByOneProcess(void)
{
    StatusEnum ret = ERROR_NOT_EXIST;

    return ret;
}

/** @brief ham xu ly qua trinh truyen lien tuc theo nhieu goi trong mot lan truyen
 *  @param none
 *  @return trang thai qua trinh xu ly
 * */
StatusEnum Module::Protocol::Mavlink::TransmittedContinousManyAtOnceProcess(void)
{
    StatusEnum ret = ERROR_NOT_EXIST;

    /// cai dat lai diem bat dau
    pTxContinuousMessageNode = &txContinuousMessageNode;

    ///
    while(pTxContinuousMessageNode != NULL)
    {
        if(pTxContinuousMessageNode->ptrData != NULL)
        {
            MessageTransmitType* msgtransmit = (MessageTransmitType*)pTxContinuousMessageNode->ptrData;

            if(msgtransmit->func)
            {
                if(msgtransmit->time >= msgtransmit->timeSet)
                {
                    FuncProtocolMavlinkTransmittedMessageCallbackType func = (FuncProtocolMavlinkTransmittedMessageCallbackType)msgtransmit->func;

                    ///
                    msgtransmit->time = 0;

                    ///
                    if(func(&msgtransmit->funcArg, &txMessage) == OK)
                    {
                        uint32_t len = writeBuffPtr + msgtransmit->funcArg.msgBuffLen;

                        if(len < writeBufferSize)
                        {
                            memcpy(&writeBuff[writeBuffPtr],
                                   &txMessage.magic,
                                   msgtransmit->funcArg.msgBuffLen);

                            writeBuffPtr = len;
                            ret = OK;
                        }
                    }
                }
            }
        }

        pTxContinuousMessageNode = pTxContinuousMessageNode->ptrNext;
    }

    return ret;
}

/** @breif ham truyen heartbeat trong mavlink
 *  @param none
 *  @return trang thai qua trinh truyen heartbeat
 * */
StatusEnum Module::Protocol::Mavlink::TransmittedMessageHeartbeat(void)
{
    /// kiem tra thoi gian truyen cua heartbeat theo us
    if(txMessageHeartbeatTime >= 1000000)
    {
        Type::Node* ptrnode = &sysID;

        ///
        while(ptrnode != NULL)
        {
            if(ptrnode->ptrData != NULL)
            {
                ///
                mavlink_system_t* system = (mavlink_system_t*)ptrnode->ptrData;
                StatusEnum          ret = OK;
                mavlink_heartbeat_t param = {0};
                uint8_t             sysid = system->sysid;
                uint8_t             compid = system->compid;

                ///
                txMessageHeartbeatTime = 0;

                /// goi ham xu ly truoc truyen heartbeat
                if(funcTransmittedMessageHeartbeatCallback)
                {
                    funcTransmittedMessageHeartbeatCallbackArg->sender     = (uint32_t)sender;
                    funcTransmittedMessageHeartbeatCallbackArg->compId = system->compid;
                    funcTransmittedMessageHeartbeatCallbackArg->systemId = system->sysid;
                    ///
                    ret = funcTransmittedMessageHeartbeatCallback(funcTransmittedMessageHeartbeatCallbackArg, &param);

                    /// lay gia tri system id va compid
                    sysid = funcTransmittedMessageHeartbeatCallbackArg->systemId;
                    compid = funcTransmittedMessageHeartbeatCallbackArg->compId;
                }

                ///
                if(ret == OK)
                {
                    uint32_t len = mavlink_msg_heartbeat_encode_chan(sysid,
                                                                     compid,
                                                                     channel,
                                                                     &txMessage,
                                                                     &param);

                    if((writeBuffPtr + len) < writeBufferSize)
                    {
                        memcpy(&writeBuff[writeBuffPtr],
                               &txMessage.magic,
                               len);

                        writeBuffPtr += len;
                    }
                }

            }

            ptrnode = ptrnode->ptrNext;
        }

    }

    return BUSY;
}

/** @breif ham nhan heartbeat trong mavlink
 *  @param none
 *  @return trang thai qua trinh nhan heartbeat
 * */
StatusEnum Module::Protocol::Mavlink::ReceivedMessageHeartbeat(void)
{
    StatusEnum ret = OK;
    mavlink_heartbeat_t param = {0};

    /// decode msg
    mavlink_msg_heartbeat_decode(&rxMessage, &param);

    ///
    timeCheckConnection = 0;

    /// function callback
    if(funcReceivedMessageHeartbeatCallback)
    {
        funcReceivedMessageHeartbeatCallbackArg->sender          = (uint32_t)sender;
        funcReceivedMessageHeartbeatCallbackArg->systemIdSender  = rxMessage.sysid;
        funcReceivedMessageHeartbeatCallbackArg->compIdSender    = rxMessage.compid;
        ret = funcReceivedMessageHeartbeatCallback(funcReceivedMessageHeartbeatCallbackArg, &param);
    }

    /// bao da co heartbeat
    if(ret == OK)
    {
        if(stateCommon == PROTOCOL_MAVLINK_STATE_LOSTCONNECTION ||
           stateCommon == PROTOCOL_MAVLINK_STATE_RECONNECTION ||
           stateCommon == PROTOCOL_MAVLINK_STATE_TRANSMIT_HEARTBEAT)
        {
            task.Set(0x01);
        }
    }

    return ret;
}

/* Public functions ----------------------------------------------------------*/

/** @brief ham nhan tung byte trong mavlink
 *  @param[in] c gia tri theo dang byte nhan duoc
 *  @return trang thai qua trinh nhan byte
 * */
StatusEnum Module::Protocol::Mavlink::ReceivedByte(uint8_t c)
{
    return Parse(c);
}

/** @brief ham open hardware
 *  @param none
 *  @return trang thai qua trinh open
 * */
StatusEnum Module::Protocol::Mavlink::Open(void)
{
    StatusEnum ret = OK;

    /// CHECK
    if(name.IsExist() == ERROR_NOT_EXIST)
    {
        LOGTAGE<<"Name not setting!\n";
        return ERROR_NOT_EXIST;
    }

    if(sysID.ptrData == NULL)
    {
        LOGTAGE<<"SystemID, CompID not setting!\n";
        return ERROR_NOT_EXIST;
    }

    if(MAVLINK_CHANNEL_COUNT >= MAVLINK_COMM_NUM_BUFFERS || channel > MAVLINK_COMM_NUM_BUFFERS)
    {
        LOGTAGE<<name<<" channel out of range!\n";
        return ERROR_HAL;
    }

    if(isOpen == true)
    {
        LOGTAGE<<name<<" opened!\n";
        return ERROR_EXIST;
    }

    /// PARAM DEFAULT
    if(writeBufferSize == 0)
    {
        writeBufferSize = 256;
    }

    if(priority == 0)
    {
        priority = PRIORITY_NORMAL;
    }

    /// Cap Phat Vung Nho
    writeBuff       = (uint8_t*)g_calloc(writeBufferSize);
    writeBuffPtr    = 0;

    /// Task
    task.name       = name;
    task.option     = FLAGS_WAIT_ANY;
    task.priority   = priority;
    task.size       = 4096;
    task.sender     = this;
    task.timeSleep  = 100;
    task           += module_protocol_mavlink_task_process;
    task.Open();

    /// Param
    timeCheckConnection     = TIME_MS;
    txMessageHeartbeatTime  = TIME_US;

    txMessageOnceIndex      = 0;
    txMessageOnceCount      = 0;

    /// Debug end
    LOGTAGI<<name<<" Channel: "<<(int)channel<<", WriteBuffSize: "<<(int)writeBufferSize<<" open done!\nn";

    return ret;
}

/** @brief ham close hardware
 *  @param none
 *  @return trang thai qua trinh close
 * */
StatusEnum Module::Protocol::Mavlink::Close(void)
{
    StatusEnum ret = OK;

    /// CHECK
    if(name.IsExist() == ERROR_NOT_EXIST)
    {
        LOGTAGE<<"SystemID, CompID or Name not setting!\n";
        return ERROR_NOT_EXIST;
    }
 //TODO: remove system
    if(MAVLINK_CHANNEL_COUNT >= MAVLINK_COMM_NUM_BUFFERS || channel > MAVLINK_COMM_NUM_BUFFERS)
    {
        LOGTAGE<<name<<" channel out of range!\n";
        return ERROR_HAL;
    }

    if(isOpen == false)
    {
        LOGTAGE<<name<<" closed!\n";
        return ERROR_EXIST;
    }

    /// TASK
    task.Close();

    /// FREE
    g_free(writeBuff);

    ///
    isOpen = false;

    ///
    LOGTAGI<<name<<" Channel: "<<(int)channel<<", WriteBuffSize: "<<(int)writeBufferSize<<" close done!\n";


    return ret;
}

#endif /// GREMSY_MODULE_PROTOCOL_MAVLINK_V100
/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
