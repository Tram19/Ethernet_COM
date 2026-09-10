//
///**
//  ******************************************************************************
//  * @file    rtos_cmsis_os_v2_task_message_queue.cpp
//  * @author  Gremsy Team
//  * @version V2.0.0
//  * @date    Dec 26, 2024
//  * @brief
//  *
//  ******************************************************************************
//  * @Copyright
//  * COPYRIGHT NOTICE: (c) 2024 Gremsy.
//  * All rights reserved.
//  *
//  * The information contained herein is confidential
//  * property of Company. The use, copying, transfer or
//  * disclosure of such information is prohibited except
//  * by express written agreement with Company.
//  *
//  ******************************************************************************
//*/
//
///* Includes ------------------------------------------------------------------*/
//
//#include "RTOS/CMSIS_OS_V2/Inc/rtos_cmsis_os_v2_task_message_queue.hpp"
//#if (GREMSY_FREERTOS_USE != 0)
///* Private typedef -----------------------------------------------------------*/
//
///* Private define ------------------------------------------------------------*/
//
///* Private macro -------------------------------------------------------------*/
//
///* Private variables ---------------------------------------------------------*/
//
///* Private function prototypes -----------------------------------------------*/
//
///* Private functions ---------------------------------------------------------*/
//
//template <typename T>
//void TaskMessageQueue<T>::Sleep(uint32_t time)
//{
//    osDelay(time);
//}
//
///** @brief ham sleep theo thoi gian mac dinh cua task (timeSleep)
// *  @param none
// *  @return none
// * */
//template <typename T>
//void TaskMessageQueue<T>::Sleep(void)
//{
//    osDelay(timeSleep);
//}
//
//
//template <typename T>
//osStatus_t TaskMessageQueue<T>::Put(T* ptr_mes)
//{
//    if(isOpen)
//    {
//        return osMessageQueuePut(idQueue, ptr_mes, 0U, 0U);
//    }
//    else
//    {
//        return osError;
//    }
//}
//
//template <typename T>
//osStatus_t TaskMessageQueue<T>::Get(T* ptr_mes, uint32_t timeout)
//{
//    if(isOpen)
//    {
//        return  osMessageQueueGet(idQueue, ptr_mes, NULL, timeout);
//    }
//    else
//    {
//        return osError;
//    }
//}
//
////template <typename T>
////StatusEnum TaskMessageQueue<T>::Open(void)
////{
////    StatusEnum ret = OK;
////
////    /// CHECK
////    if(name.IsExist() != OK)
////    {
////        return ERROR_PARAM;
////    }
////
////    if(isOpen == true)
////    {
////        return ERROR_EXIST;
////    }
////    /// Default
////    if(size == 0)                   size        = 128*4;
////    if(timeSleep == 0)              timeSleep   = 1000;
////    if(priority == PRIORITY_NONE)   priority    = PRIORITY_NORMAL;
////
////    /// Param
////    attrInitStruct.name       = name.array;
////    attrInitStruct.priority   = (osPriority_t)priority;
////    attrInitStruct.stack_size = size;
////
////    /// Tao task
////    idTask = osThreadNew(funcTaskCallback, sender, &attrInitStruct);
////
////    // Default
////    if(this->capacity == 0) this->capacity = 10;
////
////    //Param
////    attrQueueStruct.name = name.array;
////
////    //Tao queue
////    idQueue = osMessageQueueNew(this->capacity, sizeof(uint32_t), &attrQueueStruct);
////
////    if(idTask == 0 || idQueue == 0)
////    {
////        return ERROR_HAL;
////    }
////
////    isOpen = true;
////    return ret;
////}
////
////template <typename T>
////StatusEnum TaskMessageQueue<T>::Close(void)
////{
////    StatusEnum ret = OK;
////
////    /// CHECK
////    if(name.IsExist() != OK ||  size == 0 || priority == PRIORITY_NONE || funcTaskCallback == 0)
////    {
////        isOpen = false;
////        return ERROR_PARAM;
////    }
////
////    if(isOpen == false)
////    {
////        return ERROR_EXIST;
////    }
////
////    /// remove task
////    osStatus_t status = osThreadTerminate(idTask);
////
////    if(status != osOK)
////    {
////        return ERROR_HAL;
////    }
////
////
////    /// delete event
////    status = osMessageQueueDelete(idQueue);
////
////    if(status != osOK)
////    {
////        return ERROR_HAL;
////    }
////
////    ///
////    isOpen = false;
////
////    return ret;
////}
//#endif /** GREMSY_FREERTOS_USE */
///************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
