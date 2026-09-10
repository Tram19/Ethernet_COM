 
/** 
  ******************************************************************************
  * @file    rtos_cmsis_os_v2_event.cpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Apr 18, 2023
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

#include "RTOS/CMSIS_OS_V2/Inc/rtos_cmsis_os_v2_event.hpp"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/** @brief ham set co ngat
 *  @param[in] flag co ngat duoc set
 *  @return trang thai set co
 * */
uint32_t RTOS::Event::Set(uint32_t flag)
{
    if(isOpen)
        return osEventFlagsSet(idEvent, flag);
    else
        return 0;
}

/** @brief ham doi co ngat
 *  @param[in] flag co ngat doi
 *  @return trang thai doi co
 * */
uint32_t RTOS::Event::Wait(uint32_t flag, uint32_t timeOut)
{
    if(isOpen)
        return osEventFlagsWait(idEvent, flag, option, timeOut);
    else
        return 0;
}
uint32_t RTOS::Event::Get()
{
    if(isOpen)
        return osEventFlagsGet(idEvent);
    else
        return 0;
}
/** @brief ham bat dau hoat dong cua task
 *  @return trang thai mo
 * */
StatusEnum RTOS::Event::Open(void)
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

    /// Tao event
    attrInitStruct.name = name.array;
    idEvent = osEventFlagsNew(&attrInitStruct);

    if(idEvent == 0)
    {
        return ERROR_HAL;
    }

    /// Param
    isOpen = true;

    return ret;
}

/** @brief ham dong hoat dong cua task
 *  @return trang thai dong
 * */
StatusEnum RTOS::Event::Close(void)
{
    StatusEnum ret = OK;

    /// CHECK
    if(name.IsExist() != OK)
    {
        return ERROR_PARAM;
    }

    if(isOpen == false)
    {
        return ERROR_EXIST;
    }

    /// delete event
    uint32_t status = osEventFlagsDelete(idEvent);

    if(status != osOK)
    {
        return ERROR_HAL;
    }

    ///
    isOpen = false;

    return ret;
}

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
