 
/** 
  ******************************************************************************
  * @file    module_protocol_jr.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    May 22, 2023
  * @brief   This file contains all the functions prototypes for the module_protocol_jr.cpp 
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

#ifndef __MODULES_PROTOCOL_INC_MODULE_PROTOCOL_JR_HPP__
#define __MODULES_PROTOCOL_INC_MODULE_PROTOCOL_JR_HPP__

/* Includes ------------------------------------------------------------------*/

#include "module_protocol_config.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

#define MODULE_PROTOCOL_REMOTE_JR_UART_BAUDRATE                   115200
#define MODULE_PROTOCOL_REMOTE_JR_UART_PARITY                     PARITY_NONE
#define MODULE_PROTOCOL_REMOTE_JR_UART_STOPBIT                    STOPBITS_1
#define MODULE_PROTOCOL_REMOTE_JR_UART_WORDLENGTH                 WORDLENGTH_8B
#define MODULE_PROTOCOL_REMOTE_JR_UART_RX_LEVEL                   TTL_LEVEL_INV_DISABLE
#define MODULE_PROTOCOL_REMOTE_JR_UART_TX_LEVEL                   TTL_LEVEL_INV_DISABLE

/* Exported class ------------------------------------------------------------*/

/// @brief Class jr protocol
class Module::Protocol::Jr : public Module::Base::ProtocolRemote {

public:

    /// TYPEDEF ________________________________________________________________

    typedef enum
    {
        PROTOCOL_JR_TYPE_AUTO,
        PROTOCOL_JR_TYPE_11BITS,
        PROTOCOL_JR_TYPE_10BITS
    }
    ProtocolJrTypeEnum;

    /// PARRAM _________________________________________________________________

    ProtocolJrTypeEnum  type;

    /// OPERATOR _______________________________________________________________

    /** @brief operator constructor */
    Jr()
    {
        type                    = PROTOCOL_JR_TYPE_11BITS;

        task.priority           = PRIORITY_NONE;

        decodeState             = PROTOCOL_JR_DECODE_STATE_IDLE;

        timeCheck               = 0;

        timeLostConnection      = 0;

        txBuff                  = 0;
        txBuffCount             = 0;

        rxBuff                  = 0;
        rxBuffCount             = 0;

        syncError               = 0;
        syncCorrect             = 0;
    }

    /** @breif operator copy constructor */
    Jr(const Jr &c)
    {
        type                    = c.type;

        task                    = c.task;

        decodeState             = c.decodeState;

        timeCheck               = c.timeCheck;

        timeLostConnection      = c.timeLostConnection;

        txBuff                  = c.txBuff;
        txBuffCount             = c.txBuffCount;

        rxBuff                  = c.rxBuff;
        rxBuffCount             = c.rxBuffCount;

        syncError               = c.syncError;
        syncCorrect             = c.syncCorrect;
    }

    /** @brief operation delete */
    virtual ~Jr()
    {
        type                    = PROTOCOL_JR_TYPE_11BITS;

        task.priority           = PRIORITY_NONE;

        decodeState             = PROTOCOL_JR_DECODE_STATE_IDLE;

        timeCheck               = 0;

        timeLostConnection      = 0;

        txBuff                  = 0;
        txBuffCount             = 0;

        rxBuff                  = 0;
        rxBuffCount             = 0;

        syncError               = 0;
        syncCorrect             = 0;
    }

    /** @brief operator assignment */
    Jr &operator = (const Jr &c);


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

    /** @brief ham truyen gia tri doc duoc vao protocol remote sbus
     *  @param[in] c ky tu doc duoc
     *  @return trang thai qua trinh doc
     * */
    StatusEnum ReceivedByte(uint8_t c);

    /// PRIVATE ________________________________________________________________

    /** @brief ham xu ly task
     *  @param none
     *  @return none
     * */
    void __TaskProcess(void);

protected:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


private:

    /// TYPEDEF ________________________________________________________________

    /** @brief trang thai qua trinh decode JR
     * */
    typedef enum
    {
        PROTOCOL_JR_DECODE_STATE_IDLE,
        PROTOCOL_JR_DECODE_STATE_11BIT_SYNC,
        PROTOCOL_JR_DECODE_STATE_10BIT_SYNC,
        PROTOCOL_JR_DECODE_STATE_11BIT_DECODE,
        PROTOCOL_JR_DECODE_STATE_10BIT_DECODE,
        PROTOCOL_JR_DECODE_STATE_DONE,
        PROTOCOL_JR_DECODE_STATE_ERROR,
    }
    ProtocolJrDecodeStateEnum;

    /// PARRAM _________________________________________________________________
    RTOS::Task                  task;

    ProtocolJrDecodeStateEnum   decodeState;

    HW::Peripheral::Time        timeCheck;

    uint32_t                    timeLostConnection;

    uint8_t*                    txBuff;
    uint8_t                     txBuffCount;

    uint8_t*                    rxBuff;
    uint8_t                     rxBuffCount;

    uint8_t                     syncError;
    uint8_t                     syncCorrect;

//    uint32_t                testCount;

    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________

    /** @brief ham giai nen gia tri
     *  @param none
     *  @return trang thai qua trinh giai nen
     * */
    StatusEnum Decode11BitPacket(uint8_t c);

    /** @brief ham giai nen gia tri
     *  @param none
     *  @return trang thai qua trinh giai nen
     * */
    StatusEnum Decode10BitPacket(uint8_t c);

    /** @brief ham nen gia tri
     *  @param none
     *  @return trang thai qua trinh nen
     * */
    StatusEnum Encode11BitPacket(void);

    /** @brief ham nen gia tri
     *  @param none
     *  @return trang thai qua trinh nen
     * */
    StatusEnum Encode10BitPacket(void);

};

/* Exported functions --------------------------------------------------------*/

#endif /* __MODULES_PROTOCOL_INC_MODULE_PROTOCOL_JR_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
