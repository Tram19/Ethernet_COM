 
/** 
  ******************************************************************************
  * @file    rtos_cmsis_os_v2_task_message_queue.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Dec 26, 2024
  * @brief   This file contains all the functions prototypes for the rtos_cmsis_os_v2_task_message_queue.cpp 
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

#ifndef __RTOS_CMSIS_OS_V2_INC_RTOS_CMSIS_OS_V2_TASK_MESSAGE_QUEUE_HPP__
#define __RTOS_CMSIS_OS_V2_INC_RTOS_CMSIS_OS_V2_TASK_MESSAGE_QUEUE_HPP__

/* Includes ------------------------------------------------------------------*/
#include "rtos_cmsis_os_v2_config.hpp"
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

template <typename T>
class TaskMessageQueue : public RTOS::Base::Task {

public:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________
    uint32_t capacity;

    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________

    /** @brief ham bat dau hoat dong cua task
     *  @return trang thai mo
     * */
    StatusEnum Open(void)
    {
        StatusEnum ret = OK;

        /// CHECK
        if(name.IsExist() != OK)
        {
            return ERROR_PARAM;
        }

        if(isOpen == true)
        {
            return ERROR_EXIST;
        }
        /// Default
        if(size == 0)                   size        = 128*4;
        if(timeSleep == 0)              timeSleep   = 1000;
        if(priority == PRIORITY_NONE)   priority    = PRIORITY_NORMAL;

        /// Param
        attrInitStruct.name       = name.array;
        attrInitStruct.priority   = (osPriority_t)priority;
        attrInitStruct.stack_size = size;

        /// Tao task
        idTask = osThreadNew(funcTaskCallback, sender, &attrInitStruct);

        // Default
        if(this->capacity == 0) this->capacity = 10;

        //Param
        attrQueueStruct.name = name.array;

        //Tao queue
        idQueue = osMessageQueueNew(this->capacity, sizeof(T), &attrQueueStruct);

        if(idTask == 0 || idQueue == 0)
        {
            return ERROR_HAL;
        }

        isOpen = true;
        return ret;
    }

    /** @brief ham dong hoat dong cua task
     *  @return trang thai dong
     * */
    StatusEnum Close(void)
    {
        StatusEnum ret = OK;

        /// CHECK
        if(name.IsExist() != OK ||  size == 0 || priority == PRIORITY_NONE || funcTaskCallback == 0)
        {
            isOpen = false;
            return ERROR_PARAM;
        }

        if(isOpen == false)
        {
            return ERROR_EXIST;
        }

        /// remove task
        osStatus_t status = osThreadTerminate(idTask);

        if(status != osOK)
        {
            return ERROR_HAL;
        }


        /// delete event
        status = osMessageQueueDelete(idQueue);

        if(status != osOK)
        {
            return ERROR_HAL;
        }

        ///
        isOpen = false;

        return ret;
    }
    /** @brief ham gui du lieu vao queue
     *  @param[in] pointer cua du lieu
     *  @return trang thai set co
     * */
    osStatus_t Put(T* ptr_mes)
    {
        if(isOpen)
        {
            return osMessageQueuePut(idQueue,(void *) ptr_mes, 0U, 0U);
        }
        else
        {
            return osError;
        }
    }

    /** @brief ham doi du lieu vao queue
     *  @param[in] pointer cua du lieu
     *  @param[in] timeout
     *  @return trang thai doi co
     * */
    osStatus_t Get(T* ptr_mes, uint32_t timeout)
    {
        if(isOpen)
        {
            return  osMessageQueueGet(idQueue,(T*) ptr_mes, NULL, timeout);
        }
        else
        {
            return osError;
        }
    }

    /** @brief ham delay trong task
     *  @param[in] time thoi gian ms
     *  @return none
     * */
    void Sleep(uint32_t time)
    {
        osDelay(time);
    }

    /** @brief ham sleep theo thoi gian mac dinh cua task (timeSleep)
     *  @param none
     *  @return none
     * */
    void Sleep(void)
    {
        osDelay(timeSleep);
    }

protected:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________
    osMessageQueueId_t    idQueue;
    osMessageQueueAttr_t  attrQueueStruct;


    osThreadId_t                idTask;
    osThreadAttr_t              attrInitStruct;
    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


private:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


};
#endif /* __RTOS_CMSIS_OS_V2_INC_RTOS_CMSIS_OS_V2_TASK_MESSAGE_QUEUE_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
