 
/** 
  ******************************************************************************
  * @file    module_protocol_mavlink.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    May 5, 2023
  * @brief   This file contains all the functions prototypes for the module_protocol_mavlink.cpp 
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

#ifndef __MODULES_PROTOCOL_INC_MODULE_PROTOCOL_MAVLINK_HPP__
#define __MODULES_PROTOCOL_INC_MODULE_PROTOCOL_MAVLINK_HPP__

/* Includes ------------------------------------------------------------------*/

#include "module_protocol_config.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/// @brief class protocol mavlink
class Module::Protocol::Mavlink : public Module::Base::ProtocolMavlink {

public:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________


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

    /** @brief ham nhan tung byte trong mavlink
     *  @param[in] c gia tri theo dang byte nhan duoc
     *  @return trang thai qua trinh nhan byte
     * */
    StatusEnum ReceivedByte(uint8_t c);

    /// PRIVATE ________________________________________________________________

    /** @brief ham xu ly task chay an trong class
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


    /// PARRAM _________________________________________________________________
    HW::Peripheral::Time timeCheckConnection;

    HW::Peripheral::Time txMessageHeartbeatTime;


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________

    /** @breif ham truyen heartbeat trong mavlink
     *  @param none
     *  @return trang thai qua trinh truyen heartbeat
     * */
    StatusEnum TransmittedMessageHeartbeat(void);

    /** @breif ham truyen onece trong mavlink
     *  @param none
     *  @return trang thai qua trinh truyen once
     * */
    StatusEnum TransmittedMessageOnce(void);

    /** @brief ham reset thoi gian truyen du lieu lien tuc
     *  @param none
     *  @return trang thai qua trinh reset time
     * */
    StatusEnum TransmittedMessageContinuousResetTime(void);

    /** @brief ham xu ly qua trinh truyen lien tuc theo tung goi mot
     *  @param none
     *  @return trang thai qua trinh xu ly
     * */
    StatusEnum TransmittedContinousOneByOneProcess(void);

    /** @brief ham xu ly qua trinh truyen lien tuc theo nhieu goi trong mot lan truyen
     *  @param none
     *  @return trang thai qua trinh xu ly
     * */
    StatusEnum TransmittedContinousManyAtOnceProcess(void);

    /** @brief ham xu ly qua trinh truyen mot lan
     *  @param none
     *  @return trang thai qua trinh xu ly
     * */
    StatusEnum TransmittedOnceProcess(void);

    /** @brief ham parse trong mavlink
     *  @param[in] c gia tri nhan duoc theo dang byte
     *  @return trang thai qua trinh parse
     * */
    StatusEnum Parse(uint8_t c);

    /** @breif ham nhan heartbeat trong mavlink
     *  @param none
     *  @return trang thai qua trinh nhan heartbeat
     * */
    StatusEnum ReceivedMessageHeartbeat(void);

    /** @brief ham xu ly qua trinh nhan msg trong mavlink
     *  @param[in] pmsg dia chi msg nhan duoc
     *  @return trang thai qua trinh xu ly nhan msg
     * */
    StatusEnum ReceivedMessageHanlde(mavlink_message_t* pmsg);


};

/* Exported functions --------------------------------------------------------*/

#endif /* __MODULES_PROTOCOL_INC_MODULE_PROTOCOL_MAVLINK_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
