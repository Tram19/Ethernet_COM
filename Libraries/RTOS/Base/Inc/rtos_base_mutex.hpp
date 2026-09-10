/**
 ******************************************************************************
 * @file    freertos_mutex_base.hpp
 * @author  Gremsy Team
 * @version V2.0.0
 * @date    Sep 20, 2024
 * @brief   This file contains all the functions prototypes for the freertos_mutex_base.cpp
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

#ifndef __RTOSV2_FREERTOS_INC_FREERTOS_MUTEX_BASE_HPP__
# define __RTOSV2_FREERTOS_INC_FREERTOS_MUTEX_BASE_HPP__

/* Includes ------------------------------------------------------------------*/
#include "RTOS/rtos_include.hpp"
#include "FreeRTOS.h"
#include "semphr.h"
#include "rtos_base_common.hpp"
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

class RTOS::Base::SemaphoreBase {
public:
    SemaphoreBase() = default;
    ~SemaphoreBase() { Close(); }

    /**
     * @brief Close the mutex
     *
     * @return StatusEnum
     */
    StatusEnum Close()
    {
        if(!IsOpen()) return OK;

        vSemaphoreDelete(handle);
        return OK;
    }

    /**
     * @brief Check if mutex is open
     *
     * @return true
     * @return false
     */
    bool IsOpen() const { return handle != nullptr; }

    /**
     * Acquires the mutex, blocking indefinitely if it is already locked.
     *
     * @return None
     */
    void lock() {
        if (!IsOpen() || !IsSchedulerRunning()) {
            return;
        }

        if (!IsInsideISR()) {
            xSemaphoreTake(handle, portMAX_DELAY);
            return;
        }

        // We are in ISR, call from ISR
        BaseType_t isHigherPriorityTaskWoken = pdFALSE;

        xSemaphoreTakeFromISR(handle, &isHigherPriorityTaskWoken);

        /* If xHigherPriorityTaskWoken is now set to pdTRUE then a context switch should be requested. */
        portYIELD_FROM_ISR(isHigherPriorityTaskWoken);
    }

    /**
     * Acquires the mutex, blocking until either the mutex is available or the timeout has been reached.
     *
     * @param[in] _timeout Timeout in milliseconds
     *
     * @return true if the mutex was successfully acquired, false otherwise
     */
    bool lock(uint32_t timeout) {
        if (!IsOpen()) {
            return false;
        }

        if (!IsSchedulerRunning()) {
            return true;
        }

        if (!IsInsideISR()) {
            return xSemaphoreTake(handle, pdMS_TO_TICKS(timeout));
        }

        // We are in ISR, call from ISR
        BaseType_t isHigherPriorityTaskWoken = pdFALSE;

        bool result = xSemaphoreTakeFromISR(handle, &isHigherPriorityTaskWoken);

        /* If xHigherPriorityTaskWoken is now set to pdTRUE then a context switch should be requested. */
        portYIELD_FROM_ISR(isHigherPriorityTaskWoken);

        return result;
    }

    /**
     * Releases the lock on the mutex if it is currently locked.
     *
     */
    void unlock() {
        if (!IsOpen() || !IsSchedulerRunning()) {
            return;
        }

        if (!IsInsideISR()) {
            xSemaphoreGive(handle);
            return;
        }

        // We are in ISR, call from ISR
        BaseType_t isHigherPriorityTaskWoken = pdFALSE;

        xSemaphoreGiveFromISR(handle, &isHigherPriorityTaskWoken);

        /* If xHigherPriorityTaskWoken is now set to pdTRUE then a context switch should be requested. */
        portYIELD_FROM_ISR(isHigherPriorityTaskWoken);
    }

    /**
     * Attempts to acquire the mutex without blocking.
     *
     * @return true if the mutex was successfully acquired, false otherwise
     */
    bool try_lock() { return lock(0); }

protected:

    SemaphoreHandle_t handle = {0};
};

/* Exported functions --------------------------------------------------------*/



#endif /* __RTOSV2_FREERTOS_INC_FREERTOS_MUTEX_BASE_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
