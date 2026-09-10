
/**
  ******************************************************************************
  * @file    gremsy_component_hall_v1.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Apr 11, 2023
  * @brief   This file contains all the functions prototypes for the gremsy_component_hall_v1.cpp
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

#ifndef __GREMSY_COMPONENT_HALL_V1_HPP__
#define __GREMSY_COMPONENT_HALL_V1_HPP__

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#if (GREMSY_FREERTOS_USE == 1)      /// FREERTOS
#include "FreeRTOS.h"
#include "task.h"

#define GREMSY_HEAP_ENABLE     1
#elif (GREMSY_FREERTOS_USE == 2)    /// CMSIS_V1 & CMSIS_V2
#include "cmsis_os.h"
#define GREMSY_HEAP_ENABLE     1
#else
#error Thu vien gremsy phai su dung RTOS
#endif

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/


/* Exported macro ------------------------------------------------------------*/


/// LOGGER _____________________________________________________________________


/// COMMANDLINE ________________________________________________________________

/* Exported class ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/


#endif /* __GREMSY_COMPONENT_HALL_V1_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
