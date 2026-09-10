/** 
  ******************************************************************************
  * @file    rtos_include.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Apr 12, 2023
  * @brief   This file contains all the functions prototypes for the rtos_include.cpp 
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

#ifndef __RTOS_RTOS_INCLUDE_HPP__
#define __RTOS_RTOS_INCLUDE_HPP__

/* Includes ------------------------------------------------------------------*/

#include "inplace_function.h"
#include "Type/type.hpp"

#ifdef __cplusplus
extern "C" {
#endif
extern uint32_t SystemCoreClock;
#ifdef __cplusplus
}
#endif

/* Exported macro ------------------------------------------------------------*/

#define OsMaxDELAY portMAX_DELAY
#define USING_LOCK_STREAM_BUFFER 1
/* Exported types ------------------------------------------------------------*/

namespace RTOS
{
    /// TYPEDEF
    /// TASK
    /** @brief dinh nghia ham chay trong task
     *  @note ham tra ve cua rtos task callback hoi dac biet de tranh sinh ra cac
     *        ham con se su dung ham nguyen mau cua freertos
     * */
    typedef void (*FuncRtosTaskCallbackType)(void* arg);

    using FreeRTOSCallbackService = void (*)(void *); // tim diem khac biet

    constexpr uint16_t TASK_FUNC_SIZE = 64;
    using TaskFunc = stdext::inplace_function<void(void), TASK_FUNC_SIZE>;
    /// BASE
    namespace Base
    {
        class Common;
        class Task;
        class Event;
        class MessageQueue;
        class SemaphoreBase;
        class StreamBuffer;
    }

    ///
    class Task;
    class TaskEvent;
    class Event;
    class StaticMutex;
    class DynamicMutex;
    template <size_t S>
    class TaskStatic;

    template <typename T, size_t Capacity>
    class MessageQueue;

    template <typename T, size_t Capacity>
    class MessageQueueStatic;

    template <size_t S>
    class StreamBufferStatic;

    class StreamBufferDynamic;

    template <typename Task>
    void FreeTaskServiceCallback(void *arg) {
        Task &task = *static_cast<Task *>(arg);
        task();
    }

    bool IsInsideISR();
    bool IsSchedulerRunning();

}

/* Exported functions --------------------------------------------------------*/

#endif /* __RTOS_RTOS_INCLUDE_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
