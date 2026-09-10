 
/** 
  ******************************************************************************
  * @file    module_protocol_sbus.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    May 17, 2023
  * @brief   This file contains all the functions prototypes for the module_protocol_sbus.cpp 
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

#ifndef __MODULES_PROTOCOL_INC_MODULE_PROTOCOL_SBUS_HPP__
#define __MODULES_PROTOCOL_INC_MODULE_PROTOCOL_SBUS_HPP__

/* Includes ------------------------------------------------------------------*/

#include "module_protocol_config.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

#define MODULE_PROTOCOL_REMOTE_SBUS_UART_BAUDRATE                100000
#define MODULE_PROTOCOL_REMOTE_SBUS_UART_PARITY                  PARITY_EVEN
#define MODULE_PROTOCOL_REMOTE_SBUS_UART_STOPBIT                 STOPBITS_2
#define MODULE_PROTOCOL_REMOTE_SBUS_UART_WORDLENGTH              WORDLENGTH_9B
#define MODULE_PROTOCOL_REMOTE_SBUS_UART_RX_LEVEL                TTL_LEVEL_INV_ENABLE
#define MODULE_PROTOCOL_REMOTE_SBUS_UART_TX_LEVEL                TTL_LEVEL_INV_ENABLE

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/// @brief Class sbus protocol
class Module::Protocol::Sbus : public Module::Base::ProtocolRemote {

public:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________


    /// OPERATOR _______________________________________________________________

    /** @brief operator constructor */
    Sbus()
    {
        task.priority           = PRIORITY_NONE;

        decodeState             = PROTOCOL_SBUS_DECODE_STATE_IDLE;

        timeCheck               = 0;

        timeLostConnection      = 0;

        txBuff                  = 0;
        txBuffCount             = 0;

        rxBuff                  = 0;
        rxBuffCount             = 0;
    }

    /** @breif operator copy constructor */
    Sbus(const Sbus &c)
    {
        task                    = c.task;

        decodeState             = c.decodeState;

        timeCheck               = c.timeCheck;

        timeLostConnection      = c.timeLostConnection;

        txBuff                  = c.txBuff;
        txBuffCount             = c.txBuffCount;

        rxBuff                  = c.rxBuff;
        rxBuffCount             = c.rxBuffCount;
    }

    /** @brief operation delete */
    virtual ~Sbus()
    {
        task.priority           = PRIORITY_NONE;

        decodeState             = PROTOCOL_SBUS_DECODE_STATE_IDLE;

        timeCheck               = 0;

        timeLostConnection      = 0;

        txBuff                  = 0;
        txBuffCount             = 0;

        rxBuff                  = 0;
        rxBuffCount             = 0;
    }

    /** @brief operator assignment */
    Sbus &operator = (const Sbus &c);

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

    /** @brief trang thai qua trinh decode sbus
     * */
    typedef enum
    {
        PROTOCOL_SBUS_DECODE_STATE_IDLE,
        PROTOCOL_SBUS_DECODE_STATE_DETECT_TIMING,
        PROTOCOL_SBUS_DECODE_STATE_START,
        PROTOCOL_SBUS_DECODE_STATE_DATA,
        PROTOCOL_SBUS_DECODE_STATE_DONE,
        PROTOCOL_SBUS_DECODE_STATE_ERROR,
    }
    SbusDecodeStateEnum;

    /// PARRAM _________________________________________________________________
    RTOS::Task              task;

    SbusDecodeStateEnum     decodeState;

    HW::Peripheral::Time    timeCheck;

    uint32_t                timeLostConnection;

    uint8_t*                txBuff;
    uint8_t                 txBuffCount;

    uint8_t*                rxBuff;
    uint8_t                 rxBuffCount;

    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________

    /** @brief ham giai nen gia tri
     *  @param none
     *  @return trang thai qua trinh giai nen
     * */
    StatusEnum DecodePacket(void);

    /** @brief ham nen gia tri
     *  @param none
     *  @return trang thai qua trinh nen
     * */
    StatusEnum EncodePacket(void);

};

/* Exported functions --------------------------------------------------------*/

#endif /* __MODULES_PROTOCOL_INC_MODULE_PROTOCOL_SBUS_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
