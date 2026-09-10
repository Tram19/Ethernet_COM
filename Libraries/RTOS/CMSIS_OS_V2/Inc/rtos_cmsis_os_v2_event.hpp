 
/** 
  ******************************************************************************
  * @file    rtos_cmsis_os_v2_event.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Apr 18, 2023
  * @brief   This file contains all the functions prototypes for the rtos_cmsis_os_v2_event.cpp 
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

#ifndef __RTOS_CMSIS_OS_V2_INC_RTOS_CMSIS_OS_V2_EVENT_HPP__
#define __RTOS_CMSIS_OS_V2_INC_RTOS_CMSIS_OS_V2_EVENT_HPP__

/* Includes ------------------------------------------------------------------*/

#include "rtos_cmsis_os_v2_config.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/// @brief class event
class RTOS::Event : public RTOS::Base::Event {

public:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________


    /// OPERATOR _______________________________________________________________
    Event()
    {
        idEvent = NULL;

        attrInitStruct.attr_bits = 0;
        attrInitStruct.cb_mem    = NULL;
        attrInitStruct.cb_size   = 0;
        attrInitStruct.name      = NULL;
    }
    Event(FlagWaitOptionEnum _option) : Event()
    {
        option = _option;
        name = "Event Default";
    }
    Event(FlagWaitOptionEnum _option, const char _name[]) : Event()
    {
        option = _option;
        name = _name;
    }

    /** @breif operator copy constructor */
    Event(const Event &c)
    {
        idEvent = c.idEvent;

        attrInitStruct.attr_bits = c.attrInitStruct.attr_bits;
        attrInitStruct.cb_mem    = c.attrInitStruct.cb_mem;
        attrInitStruct.cb_size   = c.attrInitStruct.cb_size;
        attrInitStruct.name      = c.attrInitStruct.name;
    }

    /** @brief operation delete */
    virtual ~Event()
    {
        idEvent = NULL;

        attrInitStruct.attr_bits = 0;
        attrInitStruct.cb_mem    = NULL;
        attrInitStruct.cb_size   = 0;
        attrInitStruct.name      = NULL;
    }

    /** @brief operator assignment */
    Event &operator = (const Event &c);
    /// FUNCTION _______________________________________________________________

    /** @brief ham bat dau hoat dong cua task
     *  @return trang thai mo
     * */
    StatusEnum Open(void);

    /** @brief ham dong hoat dong cua task
     *  @return trang thai dong
     * */
    StatusEnum Close(void);

    /** @brief ham set co ngat
     *  @param[in] flag co ngat duoc set
     *  @return trang thai set co
     * */
    uint32_t Set(uint32_t flag);

    /** @brief ham doi co ngat
     *  @param[in] flag co ngat doi
     *  @return trang thai doi co
     * */
    uint32_t Wait(uint32_t flag, uint32_t timeOut);
    /** @brief ham lay gia tri khong
     *  @return trang thai doi co
     * */
    uint32_t Get();

protected:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________
    osEventFlagsAttr_t   attrInitStruct {0};
    osEventFlagsId_t     idEvent {0};

    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


private:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


};

/* Exported functions --------------------------------------------------------*/

#endif /* __RTOS_CMSIS_OS_V2_INC_RTOS_CMSIS_OS_V2_EVENT_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
