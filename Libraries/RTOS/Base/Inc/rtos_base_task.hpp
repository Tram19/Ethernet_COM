 
/** 
  ******************************************************************************
  * @file    rtos_base_task.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Apr 12, 2023
  * @brief   This file contains all the functions prototypes for the rtos_base_task.cpp 
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

#ifndef __RTOS_BASE_INC_RTOS_BASE_TASK_HPP__
#define __RTOS_BASE_INC_RTOS_BASE_TASK_HPP__

/* Includes ------------------------------------------------------------------*/

#include "RTOS/rtos_include.hpp"
#include "FreeRTOS.h"
#include "rtos_base_common.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/// @brief class su dung chung cho cac task trong cac thu vien rtos
class RTOS::Base::Task : public RTOS::Base::Common {

public:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________
    Type::String    name{""};
    PriorityEnum    priority;       /// do u tien cua task su dung chung voi FreeRtos
    uint32_t        size;           /// do lon cua ham chay
    object          sender;         /// bien tra ve dia cShi bien/ham cho ham tra ve cua task
    uint32_t        timeSleep;      /// thoi gian sleep mac dinh cua task

    /// OPERATOR _______________________________________________________________

    /** @brief operator constructor */
    Task()
    {
        name                = NULL;
        priority            = PRIORITY_NONE;
        size                = 0;
        sender              = 0;
        timeSleep           = 100;

        funcTaskCallback    = NULL;
    }

    /** @breif operator copy constructor */
    Task(const Task &c)
    {
        name                = c.name;
        priority            = c.priority;
        size                = c.size;
        sender              = c.sender;
        timeSleep           = c.timeSleep;

        funcTaskCallback    = c.funcTaskCallback;
    }

    /** @brief operation delete */
    virtual ~Task()
    {
        name                = NULL;
        priority            = PRIORITY_NONE;
        size                = 0;
        sender              = 0;
        timeSleep           = 100;

        funcTaskCallback    = NULL;
    }

    /** @brief operator assignment */
    Task &operator = (const Task &c);

    /** @brief add ham tra ve cua task */
    virtual Task &operator += (const FuncRtosTaskCallbackType func){
        funcTaskCallback = func;
        return *this;
    };

    /// FUNCTION _______________________________________________________________

    /** @brief ham delay trong task
     *  @param[in] time thoi gian ms
     *  @return none
     * */
    virtual void Sleep(uint32_t time){}

    /** @brief ham sleep theo thoi gian mac dinh cua task (timeSleep)
     *  @param none
     *  @return none
     * */
    virtual void Sleep(void){}


protected:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________

    FuncRtosTaskCallbackType    funcTaskCallback = nullptr;
    TickType_t 					incrementTick;
    TickType_t 					lastWakeUpTime;
    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


private:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


};

/* Exported functions --------------------------------------------------------*/

#endif /* __RTOS_BASE_INC_RTOS_BASE_TASK_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
