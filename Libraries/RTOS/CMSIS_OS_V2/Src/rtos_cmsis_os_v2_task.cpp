 
/** 
  ******************************************************************************
  * @file    rtos_cmsis_os_v2_task.cpp
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

#include "RTOS/CMSIS_OS_V2/Inc/rtos_cmsis_os_v2_task.hpp"
#include "gremsy_main.hpp"
#include "gremsy.hpp"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/* Public functions ----------------------------------------------------------*/

/** @brief ham doc so task hoat dong trong rtos
 * */
uint32_t RTOS::Task::GetTaskCount(void)
{
    return osThreadGetCount();
}


/** @brief ham delay trong task
 *  @param[in] time thoi gian ms
 *  @return none
 * */
void RTOS::Task::Sleep(uint32_t time)
{
    osDelay(time);
}

/** @brief ham sleep theo thoi gian mac dinh cua task (timeSleep)
 *  @param none
 *  @return none
 * */
void RTOS::Task::Sleep(void)
{
    osDelay(timeSleep);
}

/** @brief ham sleep theo thoi gian mac dinh cua task (timeSleep)
 *  @param none
 *  @return none
 * */
void RTOS::Task::SleepUntil(void)
{
    vTaskDelayUntil(&lastWakeUpTime, incrementTick);
}
/** @brief ham bat dau hoat dong cua task
 *  @return trang thai mo
 * */
StatusEnum RTOS::Task::Open(void)
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

    incrementTick                               = timeSleep;
    lastWakeUpTime                              = 0;
    /// Param
    attrInitStruct.name       = name.array;
    attrInitStruct.priority   = (osPriority_t)priority;
    attrInitStruct.stack_size = size;

    /// Init task
    idTask = osThreadNew(funcTaskCallback, sender, &attrInitStruct);
    if(idTask == 0)
    {
        LOGE<<"Task loop khong cai dat duocádasdasd!\n\r";
        return ERROR_HAL;
    }

    ///
    isOpen          = true;

    return OK;
}

/** @brief ham dong hoat dong cua task
 *  @return trang thai dong
 * */
StatusEnum RTOS::Task::Close(void)
{
    if(name.IsExist() != OK ||  size == 0 || priority == PRIORITY_NONE || funcTaskCallback == 0)
    {
        isOpen = false;
        return ERROR_PARAM;
    }

    if(isOpen == false)
    {
        return ERROR_NOT_EXIST;
    }

    osStatus_t status = osThreadTerminate(idTask);

    if(status != osOK)
    {
        return ERROR_HAL;
    }

    /// PARAM
    isOpen = false;

    return OK;
}

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
