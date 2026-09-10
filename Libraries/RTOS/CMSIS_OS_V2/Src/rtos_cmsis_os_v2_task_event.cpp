 
/** 
  ******************************************************************************
  * @file    rtos_cmsis_os_v2_task_event.cpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Apr 12, 2023
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

#include "RTOS/CMSIS_OS_V2/Inc/rtos_cmsis_os_v2_task_event.hpp"

#if (GREMSY_FREERTOS_USE != 0)
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/** @brief ham doc so task hoat dong trong rtos
 * */
uint32_t RTOS::TaskEvent::GetTaskCount(void)
{
    return osThreadGetCount();
}


/** @brief ham delay trong task
 *  @param[in] time thoi gian ms
 *  @return none
 * */
void RTOS::TaskEvent::Sleep(uint32_t time)
{
    osDelay(time);
}

/** @brief ham sleep theo thoi gian mac dinh cua task (timeSleep)
 *  @param none
 *  @return none
 * */
void RTOS::TaskEvent::Sleep(void)
{
    osDelay(timeSleep);
}

/** @brief ham set co ngat
 *  @param[in] flag co ngat duoc set
 *  @return trang thai set co
 * */
uint32_t RTOS::TaskEvent::Set(uint32_t flag)
{
    return osEventFlagsSet(idEvent, flag);
}

/** @brief ham doi co ngat
 *  @param[in] flag co ngat doi
 *  @return trang thai doi co
 * */
uint32_t RTOS::TaskEvent::Wait(uint32_t flag, uint32_t timeOut)
{
    return osEventFlagsWait(idEvent, flag, option, timeOut);
}

/** @brief ham bat dau hoat dong cua task
 *  @return trang thai mo
 * */
StatusEnum RTOS::TaskEvent::Open(void)
{
    if(name.IsExist() != OK || funcTaskCallback == 0)
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
    if(idTask == 0)
    {
        idTask = osThreadNew(funcTaskCallback, sender, &attrInitStruct);
    }
    else
    {
        osThreadResume(idTask);
    }

    /// Tao event
    attrEventInitStruct.name = name.array;
    idEvent = osEventFlagsNew(&attrEventInitStruct);

    if(idEvent == 0 || idTask == 0)
    {
        return ERROR_HAL;
    }

    ///
    isOpen = true;

    return OK;
}

/** @brief ham dong hoat dong cua task
 *  @return trang thai dong
 * */
StatusEnum RTOS::TaskEvent::Close(void)
{
    osStatus_t status;

    if(name.IsExist() != OK ||  size == 0 || priority == PRIORITY_NONE || funcTaskCallback == 0 || idTask == 0)
    {
        isOpen = false;
        return ERROR_PARAM;
    }

    if(isOpen == false)
    {
        return ERROR_NOT_EXIST;
    }

    /// Supend task
    status = osThreadSuspend(idTask);

    if(status != osOK)
    {
        return ERROR_HAL;
    }

    /// delete event
    status = osEventFlagsDelete(idEvent);

    if(status != osOK)
    {
        return ERROR_HAL;
    }

    /// PARAM
    name.Remove();
    isOpen = false;

    return OK;
}

#endif /** GREMSY_FREERTOS_USE */
/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
