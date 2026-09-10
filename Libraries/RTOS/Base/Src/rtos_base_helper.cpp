 
/** 
  ******************************************************************************
  * @file    rtos_base_helper.cpp
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

#include "RTOS/rtos_include.hpp"
#include "FreeRTOS.h"
#include "task.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/**
 * @brief Check if we are inside an ISR
 *
 * @return true
 * @return false
 */
bool RTOS::IsInsideISR() { return xPortIsInsideInterrupt(); }

bool RTOS::IsSchedulerRunning() { return xTaskGetSchedulerState() == taskSCHEDULER_RUNNING; }

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
