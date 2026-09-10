 
/** 
  ******************************************************************************
  * @file    rtos_cmsis_os_v2_task_event.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Apr 12, 2023
  * @brief   This file contains all the functions prototypes for the rtos_cmsis_os_v2_task_event.cpp 
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

#ifndef __RTOS_CMSIS_OS_V2_INC_RTOS_CMSIS_OS_V2_TASK_EVENT_HPP__
#define __RTOS_CMSIS_OS_V2_INC_RTOS_CMSIS_OS_V2_TASK_EVENT_HPP__

/* Includes ------------------------------------------------------------------*/

#include "rtos_cmsis_os_v2_config.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/// @brief
class RTOS::TaskEvent : public RTOS::Base::Task {

public:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________
    FlagWaitOptionEnum  option;

    /// OPERATOR _______________________________________________________________

    /** @brief operator constructor */
    TaskEvent()
    {
        option = FLAGS_WAIT_ANY;
        idTask = NULL;

        attrInitStruct.attr_bits    = 0;
        attrInitStruct.cb_mem       = 0;
        attrInitStruct.cb_size      = 0;
        attrInitStruct.name         = 0;
        attrInitStruct.priority     = osPriorityNone;
        attrInitStruct.reserved     = 0;
        attrInitStruct.stack_mem    = 0;
        attrInitStruct.stack_size   = 0;
        attrInitStruct.tz_module    = 0;

        attrEventInitStruct.attr_bits   = 0;
        attrEventInitStruct.cb_mem      = 0;
        attrEventInitStruct.cb_size     = 0;
        attrEventInitStruct.name        = 0;

        idEvent = NULL;
    }

    /** @breif operator copy constructor */
    TaskEvent(const TaskEvent &c)
    {
        option = c.option;
        idTask = c.idTask;

        attrInitStruct.attr_bits    = c.attrInitStruct.attr_bits;
        attrInitStruct.cb_mem       = c.attrInitStruct.cb_mem;
        attrInitStruct.cb_size      = c.attrInitStruct.cb_size;
        attrInitStruct.name         = c.attrInitStruct.name;
        attrInitStruct.priority     = c.attrInitStruct.priority;
        attrInitStruct.reserved     = c.attrInitStruct.reserved;
        attrInitStruct.stack_mem    = c.attrInitStruct.stack_mem;
        attrInitStruct.stack_size   = c.attrInitStruct.stack_size;
        attrInitStruct.tz_module    = c.attrInitStruct.tz_module;

        attrEventInitStruct.attr_bits   = c.attrEventInitStruct.attr_bits;
        attrEventInitStruct.cb_mem      = c.attrEventInitStruct.cb_mem;
        attrEventInitStruct.cb_size     = c.attrEventInitStruct.cb_size;
        attrEventInitStruct.name        = c.attrEventInitStruct.name;

        idEvent = c.idEvent;
    }

    /** @brief operation delete */
    virtual ~TaskEvent()
    {
        option = FLAGS_WAIT_ANY;
        idTask = NULL;

        attrInitStruct.attr_bits    = 0;
        attrInitStruct.cb_mem       = 0;
        attrInitStruct.cb_size      = 0;
        attrInitStruct.name         = 0;
        attrInitStruct.priority     = osPriorityNone;
        attrInitStruct.reserved     = 0;
        attrInitStruct.stack_mem    = 0;
        attrInitStruct.stack_size   = 0;
        attrInitStruct.tz_module    = 0;

        attrEventInitStruct.attr_bits   = 0;
        attrEventInitStruct.cb_mem      = 0;
        attrEventInitStruct.cb_size     = 0;
        attrEventInitStruct.name        = 0;

        idEvent = NULL;
    }

    /** @brief operator assignment */
    TaskEvent &operator = (const TaskEvent &c);

    /// FUNCTION _______________________________________________________________

    /** @brief ham doc so task hoat dong trong rtos
     *  @param none
     *  @return so phan tu chay trong task
     * */
    static uint32_t GetTaskCount(void);

    /** @brief ham bat dau hoat dong cua task
     *  @return trang thai mo
     * */
    StatusEnum Open(void);

    /** @brief ham dong hoat dong cua task
     *  @return trang thai dong
     * */
    StatusEnum Close(void);

    /** @brief ham delay trong task
     *  @param[in] time thoi gian ms
     *  @return none
     * */
    void Sleep(uint32_t time);

    /** @brief ham sleep theo thoi gian mac dinh cua task (timeSleep)
     *  @param none
     *  @return none
     * */
    void Sleep(void);

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


private:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________
    osThreadId_t                idTask{0};
    osThreadAttr_t              attrInitStruct;

    osEventFlagsAttr_t          attrEventInitStruct{0};
    osEventFlagsId_t            idEvent;

    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


};

/* Exported functions --------------------------------------------------------*/

#endif /* __RTOS_CMSIS_OS_V2_INC_RTOS_CMSIS_OS_V2_TASK_EVENT_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
