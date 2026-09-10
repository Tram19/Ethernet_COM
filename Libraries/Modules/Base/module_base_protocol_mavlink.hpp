 
/** 
  ******************************************************************************
  * @file    module_base_protocol_mavlink.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    May 5, 2023
  * @brief   This file contains all the functions prototypes for the module_base_protocol_mavlink.cpp 
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

#ifndef __MODULES_BASE_MODULE_BASE_PROTOCOL_MAVLINK_HPP__
#define __MODULES_BASE_MODULE_BASE_PROTOCOL_MAVLINK_HPP__

/* Includes ------------------------------------------------------------------*/

#include "module_base_common.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/// @brief class su dung chung cho protocol mavlink
class Module::Base::ProtocolMavlink : public Module::Base::Common {

public:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________

    ProtocolMavlinkTypeEnum     type;
    uint32_t                    systemId;
    uint32_t                    compId;
    uint32_t                    channel;
    uint32_t                    writeBufferSize;


    Type::Node                  sysID;

    /// OPERATOR _______________________________________________________________

    /** @brief operator constructor */
    ProtocolMavlink()
    {
        type                                        = PROTOCOL_MAVLINK_TYPE_MASTER;
        systemId                                    = 0;
        compId                                      = 0;
        channel                                     = 0;
        writeBuff                                   = 0;
        writeBuffPtr                                = 0;
        writeBufferSize                             = 0;

        stateCommon                                 = PROTOCOL_MAVLINK_STATE_IDLE;

        funcWriteCallback                           = NULL;
        funcWriteCallbackArg                        = NULL;

        funcTransmitMessageCallback                 = NULL;
        funcTransmitMessageCallbackArg              = NULL;

        funcTransmitMessageIsNotExistCallback       = NULL;
        funcTransmitMessageIsNotExistCallbackArg    = NULL;

        funcReceivedMessageCallback                 = NULL;
        funcReceivedMessageCallbackArg              = NULL;

        funcReceivedMessageIsNotExistCallback       = NULL;
        funcReceivedMessageIsNotExistCallbackArg    = NULL;

        funcReceivedMessageHeartbeatCallback        = NULL;
        funcReceivedMessageHeartbeatCallbackArg     = NULL;

        funcTransmittedMessageHeartbeatCallback     = NULL;
        funcTransmittedMessageHeartbeatCallbackArg  = NULL;

        /** @brief tx countinuous */
        txMessage.sysid                             = 0;
        txContinuousMessageNode.ptrData             = NULL;
        pTxContinuousMessageNode                    = NULL;

        /** @brief tx once */
        txMessageOnce[0].msgID                      = 0;
        txMessageOnceCount                          = 0;
        txMessageOnceIndex                          = 0;

        /** @brief rx */
        rxMessage.sysid                             = 0;
        rxStatus.msg_received                       = 0;
        rxMessageNode.ptrData                       = 0;

        /** @brief task */
        task.priority                               = PRIORITY_NONE;
    }

    /** @breif operator copy constructor */
    ProtocolMavlink(const ProtocolMavlink &c)
    {
        type                                        = c.type;
        systemId                                    = c.systemId;
        compId                                      = c.compId;
        channel                                     = c.channel;
        writeBuff                                   = c.writeBuff;
        writeBuffPtr                                = c.writeBuffPtr;
        writeBufferSize                             = c.writeBufferSize;

        stateCommon                                 = c.stateCommon;

        funcWriteCallback                           = c.funcWriteCallback;
        funcWriteCallbackArg                        = c.funcWriteCallbackArg;

        funcTransmitMessageCallback                 = c.funcTransmitMessageCallback;
        funcTransmitMessageCallbackArg              = c.funcTransmitMessageCallbackArg;

        funcTransmitMessageIsNotExistCallback       = c.funcTransmitMessageIsNotExistCallback;
        funcTransmitMessageIsNotExistCallbackArg    = c.funcTransmitMessageIsNotExistCallbackArg;

        funcReceivedMessageCallback                 = c.funcReceivedMessageCallback;
        funcReceivedMessageCallbackArg              = c.funcReceivedMessageCallbackArg;

        funcReceivedMessageIsNotExistCallback       = c.funcReceivedMessageIsNotExistCallback;
        funcReceivedMessageIsNotExistCallbackArg    = c.funcReceivedMessageIsNotExistCallbackArg;

        funcReceivedMessageHeartbeatCallback        = c.funcReceivedMessageHeartbeatCallback;
        funcReceivedMessageHeartbeatCallbackArg     = c.funcReceivedMessageHeartbeatCallbackArg;

        funcTransmittedMessageHeartbeatCallback     = c.funcTransmittedMessageHeartbeatCallback;
        funcTransmittedMessageHeartbeatCallbackArg  = c.funcTransmittedMessageHeartbeatCallbackArg;

        /** @brief tx countinuous */
        txMessage.sysid                             = c.txMessage.sysid;
        txContinuousMessageNode.ptrData             = c.txContinuousMessageNode.ptrData;
        pTxContinuousMessageNode                    = c.pTxContinuousMessageNode;

        /** @brief tx once */
        txMessageOnce[0].msgID                      = c.txMessageOnce[0].msgID;
        txMessageOnceCount                          = c.txMessageOnceCount;
        txMessageOnceIndex                          = c.txMessageOnceIndex;

        /** @brief rx */
        rxMessage.sysid                             = c.rxMessage.sysid;
        rxStatus.msg_received                       = c.rxStatus.msg_received;
        rxMessageNode.ptrData                       = c.rxMessageNode.ptrData;

        /** @brief task */
        task                                        = c.task;
    }

    /** @brief operation delete */
    virtual ~ProtocolMavlink()
    {
        type                                        = PROTOCOL_MAVLINK_TYPE_MASTER;
        systemId                                    = 0;
        compId                                      = 0;
        channel                                     = 0;
        writeBuff                                   = 0;
        writeBuffPtr                                = 0;
        writeBufferSize                             = 0;

        stateCommon                                 = PROTOCOL_MAVLINK_STATE_IDLE;

        funcWriteCallback                           = NULL;
        funcWriteCallbackArg                        = NULL;

        funcTransmitMessageCallback                 = NULL;
        funcTransmitMessageCallbackArg              = NULL;

        funcTransmitMessageIsNotExistCallback       = NULL;
        funcTransmitMessageIsNotExistCallbackArg    = NULL;

        funcReceivedMessageCallback                 = NULL;
        funcReceivedMessageCallbackArg              = NULL;

        funcReceivedMessageIsNotExistCallback       = NULL;
        funcReceivedMessageIsNotExistCallbackArg    = NULL;

        funcReceivedMessageHeartbeatCallback        = NULL;
        funcReceivedMessageHeartbeatCallbackArg     = NULL;

        funcTransmittedMessageHeartbeatCallback     = NULL;
        funcTransmittedMessageHeartbeatCallbackArg  = NULL;

        /** @brief tx countinuous */
        txMessage.sysid                             = 0;
        txContinuousMessageNode.ptrData             = NULL;
        pTxContinuousMessageNode                    = NULL;

        /** @brief tx once */
        txMessageOnce[0].msgID                      = 0;
        txMessageOnceCount                          = 0;
        txMessageOnceIndex                          = 0;

        /** @brief rx */
        rxMessage.sysid                             = 0;
        rxStatus.msg_received                       = 0;
        rxMessageNode.ptrData                       = 0;

        /** @brief task */
        task.priority                               = PRIORITY_NONE;
    }

    /** @brief operator assignment */
    ProtocolMavlink &operator = (const ProtocolMavlink &c);

    /** @brief Operation add function write
     *  @param[in] func function address
     *  @return mavlink v2
     * */
    virtual ProtocolMavlink &operator += (FuncProtocolMavlinkWriteCallbackType func){
        funcWriteCallback = func;

        if(funcWriteCallbackArg == 0)
        {
            funcWriteCallbackArg = (ProtocolMavlinkWriteType*)g_calloc(sizeof(ProtocolMavlinkWriteType));
        }

        funcWriteCallbackArg->senderLL = (uint32_t)senderLL;

        return *this;
    }

    /** @brief Operation add function transmitted message
     *  @param[in] func function address
     *  @return mavlink v2
     * */
    virtual ProtocolMavlink &operator += (FuncProtocolMavlinkTransmittedMessageCallbackType func){
        funcTransmitMessageCallback = func;

        if(funcTransmitMessageCallbackArg == 0)
        {
            funcTransmitMessageCallbackArg = (ProtocolMavlinkTransmittedMessageType*)g_calloc(sizeof(ProtocolMavlinkTransmittedMessageType));
        }

        return *this;
    }

    /** @brief Operation add function transmitted message not is exist
     *  @param[in] func function address
     *  @return mavlink v2
     * */
    virtual ProtocolMavlink &operator += (FuncProtocolMavlinkTransmittedMessageIsNotExistCallbackType func){
        funcTransmitMessageIsNotExistCallback = func;

        if(funcTransmitMessageIsNotExistCallbackArg == 0)
        {
            funcTransmitMessageIsNotExistCallbackArg = (ProtocolMavlinkTransmittedMessageIsNotExistType*)g_calloc(sizeof(ProtocolMavlinkTransmittedMessageIsNotExistType));
        }

        funcTransmitMessageIsNotExistCallbackArg->sender = (uint32_t)sender;

        return *this;
    }

    /** @brief Operation add function received message
     *  @param[in] func function address
     *  @return mavlink v2
     * */
    virtual ProtocolMavlink &operator += (FuncProtocolMavlinkReceivedMessageCallbackType func){
        funcReceivedMessageCallback = func;

        if(funcReceivedMessageCallbackArg == 0)
        {
            funcReceivedMessageCallbackArg = (ProtocolMavlinkReceivedMessageType*)g_calloc(sizeof(ProtocolMavlinkReceivedMessageType));
        }

        funcReceivedMessageCallbackArg->sender = (uint32_t)sender;

        return *this;
    }

    /** @brief Operation add function received message not is exist
     *  @param[in] func function address
     *  @return mavlink v2
     * */
    virtual ProtocolMavlink &operator += (FuncProtocolMavlinkReceivedMessageIsNotExistCallbackType func){
        funcReceivedMessageIsNotExistCallback = func;

        if(funcReceivedMessageIsNotExistCallbackArg == 0)
        {
            funcReceivedMessageIsNotExistCallbackArg = (ProtocolMavlinkReceivedMessageIsNotExistType*)g_calloc(sizeof(ProtocolMavlinkReceivedMessageIsNotExistType));
        }

        funcReceivedMessageIsNotExistCallbackArg->sender = (uint32_t)sender;

        return *this;
    }

    /** @brief Operation add function received message heartbeat
     *  @param[in] func function address
     *  @return mavlink v2
     * */
    virtual ProtocolMavlink &operator += (FuncProtocolMavlinkReceivedMessageHeartbeatCallbackType func){
        funcReceivedMessageHeartbeatCallback = func;

        if(funcReceivedMessageHeartbeatCallbackArg == 0)
        {
            funcReceivedMessageHeartbeatCallbackArg = (ProtocolMavlinkReceivedMessageHeartbeatType*)g_calloc(sizeof(ProtocolMavlinkReceivedMessageHeartbeatType));
        }

        funcReceivedMessageHeartbeatCallbackArg->sender = (uint32_t)sender;

        return *this;
    }

    /** @brief Operation add function received message heartbeat
     *  @param[in] func function address
     *  @return mavlink v2
     * */
    virtual ProtocolMavlink &operator += (FuncProtocolMavlinkTransmittedMessageHeartbeatCallbackType func){
        funcTransmittedMessageHeartbeatCallback = func;

        if(funcTransmittedMessageHeartbeatCallbackArg == 0)
        {
            funcTransmittedMessageHeartbeatCallbackArg = (ProtocolMavlinkTransmittedMessageHeartbeatType*)g_calloc(sizeof(ProtocolMavlinkTransmittedMessageHeartbeatType));
        }

        funcTransmittedMessageHeartbeatCallbackArg->sender = (uint32_t)sender;

        return *this;
    }

    /// FUNCTION _______________________________________________________________

    /** @brief ham nhan tung byte trong mavlink
     *  @param[in] c gia tri theo dang byte nhan duoc
     *  @return trang thai qua trinh nhan byte
     * */
    virtual StatusEnum ReceivedByte(uint8_t c){return ERROR_NOT_EXIST;}

    /** @brief ham doc trang thai cua protocol mavlink
     *  @param none
     *  @return trang thai cua qua trinh mavlink
     * */
    virtual ProtocolMavlinkStateEnum GetState(void){return stateCommon;}

    /** @brief ham add transmitted message
     *  @param[in] sender dia chi doi tuong chua ham truyen
     *  @param[in] msgid message id
     *  @param[in] func dia chi ham xu ly truyen
     *  @param[in] time(Us) thoi gian giua cac lan truyen
     *  @return trang thai qua trinh add
     * */
    virtual StatusEnum AddTx(object                                             senderObj,
                             uint32_t                                           msgid,
                             FuncProtocolMavlinkTransmittedMessageCallbackType  func,
                             uint32_t                                           time)
    {
        StatusEnum              ret = OK;
        MessageTransmitType*    msgIns;

        if(IsTransmitContinousMessage(msgid, func) == true)
        {
            return ERROR_EXIST;
        }

        /// calloc
        msgIns = (MessageTransmitType*)g_calloc(sizeof(MessageTransmitType));

        ///
        msgIns->msgID           = msgid;
        msgIns->time            = TIME_US;
        msgIns->func            = (uint32_t)func;
        msgIns->timeSet         = time;
        msgIns->funcArg.sender  = (uint32_t)senderObj;
        //msgIns->funcArg.ptrMsg  = (uint32_t)&txMessage;

        ///
        ret = txContinuousMessageNode.Append(msgIns);

        ///
        if(ret != OK)
        {
            g_free(msgIns);
        }

        return ret;
    }

    /** @brief ham remove transmit countinous message
     *  @param[in] msgid dia chi cua msgid
     *  @return trang thai qua trinh remove msg transmit
     * */
    virtual StatusEnum RemoveTx(uint32_t msgid)
    {
        StatusEnum ret = OK;
        uint32_t index = 0;

        /// Search dia chi cua msg_id
        ret = SearchTransmitContinousMessage(msgid, &index);

        ///
        if(ret == OK)
        {
            ret = txContinuousMessageNode.RemoveAt(index);
        }

        return ret;
    }

    /** @brief ham add received message
     *  @param[in] sender dia chi doi tuong chua ham nhan
     *  @param[in] msgid message id
     *  @param[in] func dia chi ham xu ly nhan
     *  @param[in] time(Us) thoi gian giua cac lan nhan
     *  @return trang thai qua trinh add
     * */
    virtual StatusEnum AddRx(object                                             senderObj,
                             uint32_t                                           msgid,
                             FuncProtocolMavlinkReceivedMessageCallbackType     func)
    {
        StatusEnum              ret = OK;
        MessageReceiverType*    msgIns;

        if(IsReceivedMessage(msgid, func) == true)
        {
            return ERROR_EXIST;
        }

        /// calloc
        msgIns = (MessageReceiverType*)g_calloc(sizeof(MessageReceiverType));

        ///
        msgIns->msgID           = msgid;
        msgIns->time            = TIME_US;
        msgIns->func            = (uint32_t)func;
        msgIns->funcArg.sender  = (uint32_t)senderObj;

        ///
        ret = rxMessageNode.Append(msgIns);

        ///
        if(ret != OK)
        {
            g_free(msgIns);
        }


        return ret;
    }

    /** @brief ham remove receive countinous message
     *  @param[in] msgid dia chi cua msgid
     *  @return trang thai qua trinh remove msg receive
     * */
    virtual StatusEnum RemoveRx(uint32_t msgid)
    {
        StatusEnum ret = OK;
        uint32_t index = 0;

        /// Search dia chi cua msg_id
        ret = SearchReceiveMessage(msgid, &index);

        ///
        if(ret == OK)
        {
            ret = rxMessageNode.RemoveAt(index);
        }

        return ret;
    }

    /** @brief ham add transmitted once message
     *  @param[in] sender dia chi doi tuong chua ham truyen
     *  @param[in] msgid message id
     *  @param[in] func dia chi ham xu ly truyen
     *  @return trang thai qua trinh add
     * */
    virtual StatusEnum AddTxOnce(object                                             senderObj,
                                 uint32_t                                           msgid,
                                 FuncProtocolMavlinkTransmittedMessageCallbackType  func)
    {
        StatusEnum              ret = OK;
        MessageTransmitType*    msgIns;

        /// Kiem tra do lon
        if(txMessageOnceCount >= MAVLINK_TX_MSG_ONESHOOT_NUM)
        {
            txMessageOnceCount = 0;
        }

        /// Lay gia tri vung nho
        msgIns = &txMessageOnce[txMessageOnceCount++];

        ///
        msgIns->msgID           = msgid;
        msgIns->time            = TIME_US;
        msgIns->func            = (uint32_t)func;
        msgIns->funcArg.sender  = (uint32_t)senderObj;

        return ret;
    }

    /** @brief ham add transmitted message
     *  @param[in] sender dia chi doi tuong chua ham truyen
     *  @param[in] msgid message id
     *  @param[in] func dia chi ham xu ly truyen
     *  @param[in] time(Us) thoi gian giua cac lan truyen
     *  @return trang thai qua trinh add
     * */
    virtual StatusEnum AddSystem(uint8_t sysid, uint8_t compid)
    {
        StatusEnum              ret = OK;
        mavlink_system_t*       system;

        if(IsSystem(sysid, compid) == true)
        {
            return ERROR_EXIST;
        }

        /// calloc
        system = (mavlink_system_t*)g_calloc(sizeof(mavlink_system_t));

        ///
        system->sysid               = sysid;
        system->compid              = compid;

        ///
        ret = sysID.Append(system);

        ///
        if(ret != OK)
        {
            g_free(system);
        }

        return ret;
    }

protected:

    /// TYPEDEF ________________________________________________________________
    typedef struct
    {
        uint32_t                                            msgID;
        HW::Peripheral::Time                                time;
        uint32_t                                            func;
        ProtocolMavlinkReceivedMessageType                  funcArg;
    }
    MessageReceiverType;

    typedef struct
    {
        uint32_t                                            msgID;
        HW::Peripheral::Time                                time;
        uint32_t                                            timeSet;
        uint32_t                                            func;
        ProtocolMavlinkTransmittedMessageType               funcArg;
    }
    MessageTransmitType;

    /// PARRAM _________________________________________________________________

    ProtocolMavlinkStateEnum                                    stateCommon;


    FuncProtocolMavlinkWriteCallbackType                        funcWriteCallback;
    ProtocolMavlinkWriteType*                                   funcWriteCallbackArg;

    FuncProtocolMavlinkTransmittedMessageCallbackType           funcTransmitMessageCallback;
    ProtocolMavlinkTransmittedMessageType*                      funcTransmitMessageCallbackArg;

    FuncProtocolMavlinkTransmittedMessageIsNotExistCallbackType funcTransmitMessageIsNotExistCallback;
    ProtocolMavlinkTransmittedMessageIsNotExistType*            funcTransmitMessageIsNotExistCallbackArg;

    FuncProtocolMavlinkReceivedMessageCallbackType              funcReceivedMessageCallback;
    ProtocolMavlinkReceivedMessageType*                         funcReceivedMessageCallbackArg;

    FuncProtocolMavlinkReceivedMessageIsNotExistCallbackType    funcReceivedMessageIsNotExistCallback;
    ProtocolMavlinkReceivedMessageIsNotExistType*               funcReceivedMessageIsNotExistCallbackArg;

    FuncProtocolMavlinkReceivedMessageHeartbeatCallbackType     funcReceivedMessageHeartbeatCallback;
    ProtocolMavlinkReceivedMessageHeartbeatType*                funcReceivedMessageHeartbeatCallbackArg;

    FuncProtocolMavlinkTransmittedMessageHeartbeatCallbackType  funcTransmittedMessageHeartbeatCallback;
    ProtocolMavlinkTransmittedMessageHeartbeatType*             funcTransmittedMessageHeartbeatCallbackArg;

    /** @brief write */
    uint8_t*                                                    writeBuff;
    uint32_t                                                    writeBuffPtr;

    /** @brief tx countinuous */
    mavlink_message_t                                           txMessage;
    Type::Node                                                  txContinuousMessageNode;
    Type::Node*                                                 pTxContinuousMessageNode;

    /** @brief tx once */
    MessageTransmitType                                         txMessageOnce[MAVLINK_TX_MSG_ONESHOOT_NUM];
    uint32_t                                                    txMessageOnceCount;
    uint32_t                                                    txMessageOnceIndex;

    /** @brief rx */
    mavlink_message_t                                           rxMessage;
    mavlink_status_t                                            rxStatus;
    Type::Node                                                  rxMessageNode;

    /** @brief task */
    RTOS::TaskEvent                                             task;

    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________

    /** @brief ham kiem tra msgid da ton tai trong received message chua
     *  @param[in] msgid id cua msg kiem tra
     *  @param[in] func dia chi ham dua vao
     *  @return true da nam trong received message
     *          false chua nam trong received message
     * */
    virtual bool IsReceivedMessage(uint32_t msgid, FuncProtocolMavlinkReceivedMessageCallbackType func)
    {
        Type::Node* ptrnode = &rxMessageNode;

        while(ptrnode != NULL)
        {
            if(ptrnode->ptrData != NULL)
            {
                MessageReceiverType* msgreceived = (MessageReceiverType*)ptrnode->ptrData;

                ///
                if((msgreceived->msgID == msgid) && (msgreceived->func == (uint32_t)func))
                {
                    return true;
                }
            }

            ptrnode = ptrnode->ptrNext;
        }

        return false;
    }

    /** @brief ham kiem tra msgid da ton tai trong transmitted message chua
     *  @param[in] msgid id cua msg kiem tra
     *  @param[in] func dia chi ham dua vao
     *  @return true da nam trong transmitted message
     *          false chua nam trong transmitted message
     * */
    virtual bool IsTransmitContinousMessage(uint32_t msgid, FuncProtocolMavlinkTransmittedMessageCallbackType func)
    {
        Type::Node* ptrnode = &txContinuousMessageNode;

        while(ptrnode != NULL)
        {
            if(ptrnode->ptrData != NULL)
            {
                MessageTransmitType* msgtransmitted = (MessageTransmitType*)ptrnode->ptrData;

                ///
                if((msgtransmitted->msgID == msgid) && (msgtransmitted->func == (uint32_t)func))
                {
                    return true;
                }
            }

            ptrnode = ptrnode->ptrNext;
        }

        return false;
    }

    /** @brief ham tim dia chi msgid trong transmitted message
     *  @param[in] msgid dia chi msgid trong mavlink
     *  @param[out] ptrIndex dia chia bien tra ve index
     *  @return trang thai qua trinh tim kiem
     * */
    virtual StatusEnum SearchReceiveMessage(uint32_t msgid, uint32_t* ptrIndex)
    {
        Type::Node* ptrnode = &rxMessageNode;
        uint32_t index = 0;

        ///
        while(ptrnode != NULL)
        {
            if(ptrnode->ptrData != NULL)
            {
                MessageReceiverType* msgreceiver = (MessageReceiverType*)ptrnode->ptrData;

                ///
                if(msgreceiver->msgID == msgid)
                {
                    *ptrIndex = index;
                    return OK;
                }
            }

            ptrnode = ptrnode->ptrNext;
            index += 1;
        }

        return ERROR_NOT_EXIST;
    }

    /** @brief ham tim dia chi msgid trong transmitted message
     *  @param[in] msgid dia chi msgid trong mavlink
     *  @param[out] ptrIndex dia chia bien tra ve index
     *  @return trang thai qua trinh tim kiem
     * */
    virtual StatusEnum SearchTransmitContinousMessage(uint32_t msgid, uint32_t* ptrIndex)
    {
        Type::Node* ptrnode = &txContinuousMessageNode;
        uint32_t    index = 0;

        ///
        while(ptrnode != NULL)
        {
            if(ptrnode->ptrData != NULL)
            {
                MessageTransmitType* msgtransmit = (MessageTransmitType*)ptrnode->ptrData;

                ///
                if(msgtransmit->msgID == msgid)
                {
                    *ptrIndex = index;
                    return OK;
                }
            }

            ptrnode = ptrnode->ptrNext;
            index += 1;
        }

        return ERROR_NOT_EXIST;
    }


    virtual bool IsSystem(uint8_t sysid, uint8_t compid)
    {
        Type::Node* ptrnode = &sysID;

        ///
        while(ptrnode != NULL)
        {
            if(ptrnode->ptrData != NULL)
            {
                mavlink_system_t* system = (mavlink_system_t*)ptrnode->ptrData;

                if(system->compid == compid && system->sysid == sysid)
                {
                    return true;
                }
            }

            ptrnode = ptrnode->ptrNext;
        }

        return false;
    }
private:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


};

/* Exported functions --------------------------------------------------------*/

#endif /* __MODULES_BASE_MODULE_BASE_PROTOCOL_MAVLINK_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
