 
/** 
  ******************************************************************************
  * @file    free_rtos_mutex.cpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Apr 3, 2025
  * @brief   
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2025 Gremsy.  
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

#include <RTOS/FreeRTOS/Inc/free_rtos_mutex.hpp>

/* Private typedef -----------------------------------------------------------*/
using namespace  RTOS;

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/**
 * @brief Opens the mutex
 *
 * @return StatusEnum
 */
StatusEnum StaticMutex::Open() {
    if (IsOpen()) {
        return ERROR_EXIST;
    }

    handle = xSemaphoreCreateMutexStatic(&xMutexBuffer);

    if (handle) {
        return OK;
    }

    return ERROR_HAL;
}

/**
 * @brief Opens the mutex
 *
 * @return StatusEnum
 */
StatusEnum DynamicMutex::Open() {
    if (IsOpen()) {
        return ERROR_EXIST;
    }

    handle = xSemaphoreCreateMutex();

    if (handle) {
        return OK;
    }

    return ERROR_HAL;
}

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
