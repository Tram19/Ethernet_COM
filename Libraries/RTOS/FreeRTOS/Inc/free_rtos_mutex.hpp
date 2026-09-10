 
/** 
  ******************************************************************************
  * @file    free_rtos_mutex.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Apr 3, 2025
  * @brief   This file contains all the functions prototypes for the free_rtos_mutex.cpp 
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

#ifndef __RTOSV2_FREERTOSV2_INC_FREE_RTOS_MUTEX_HPP__
#define __RTOSV2_FREERTOSV2_INC_FREE_RTOS_MUTEX_HPP__

/* Includes ------------------------------------------------------------------*/
#include "RTOS/rtos_include.hpp"
#include "RTOS/Base/Inc/rtos_base_mutex.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/
/// @brief
class RTOS::StaticMutex : public RTOS::Base::SemaphoreBase {

public:
    StaticMutex() = default;
    ~StaticMutex() = default;
    StatusEnum Open();

private:
    StaticSemaphore_t xMutexBuffer = {0};
};


/**
 * @brief Mutex class created with dynamic memory allocation.
 * The mutex is compatible with std::lock_guard and std::unique_lock
 *
 */
class RTOS::DynamicMutex : public RTOS::Base::SemaphoreBase {
public:

    /**
     * @brief Opens the mutex
     *
     * @return StatusEnum
     */
    StatusEnum Open();
};

/* Exported functions --------------------------------------------------------*/

#endif /* __RTOSV2_FREERTOSV2_INC_FREE_RTOS_MUTEX_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
