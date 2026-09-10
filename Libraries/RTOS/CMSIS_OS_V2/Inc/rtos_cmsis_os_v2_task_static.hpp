 
/** 
  ******************************************************************************
  * @file    rtos_cmsis_os_v2_task_static.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Mar 3, 2025
  * @brief   This file contains all the functions prototypes for the rtos_cmsis_os_v2_task_static.cpp 
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

#ifndef __RTOS_CMSIS_OS_V2_INC_RTOS_CMSIS_OS_V2_TASK_STATIC_HPP__
#define __RTOS_CMSIS_OS_V2_INC_RTOS_CMSIS_OS_V2_TASK_STATIC_HPP__

/* Includes ------------------------------------------------------------------*/
#include "RTOS/rtos_include.hpp"
#include "rtos_cmsis_os_v2_config.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
using namespace RTOS;

/* Exported class ------------------------------------------------------------*/
template <size_t S>
class RTOS::TaskStatic : public RTOS::Base::Task {
public:
    TaskStatic(PriorityEnum prio ,TaskFunc &&_func, const char _name[])
    {
        priority = prio;
        func = std::move(_func);
        name = _name;
    }

    TaskStatic(PriorityEnum prio ,TaskFunc &&_func, const char _name[] , uint32_t  _options)
    {
        priority = prio;
        func = std::move(_func);
        name = _name;
        options = _options;
    }

    StatusEnum Open()
    {
        if(name.IsExist() != OK || func == 0)
        {
            return ERROR_PARAM;
        }

        if(isOpen == true)
        {
            return ERROR_EXIST;
        }
        size        = S;
        if(priority == PRIORITY_NONE)   priority    = PRIORITY_NORMAL;
        /// Param
        attrInitStruct.name       = name.array;
        attrInitStruct.priority   = (osPriority_t)priority;
        attrInitStruct.stack_size = size;
        attrInitStruct.stack_mem  = stack;
        attrInitStruct.cb_size    = sizeof(tcb);
        attrInitStruct.cb_mem     = &tcb;

        idTask = osThreadNew(FreeTaskServiceCallback<TaskStatic>, this, &attrInitStruct);
        if(idTask)
        {
            isOpen = true;
        }
        return OK;
    }

    StatusEnum Close()
    {
        if(name.IsExist() != OK ||  size == 0 || priority == PRIORITY_NONE || func == 0)
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

    uint32_t Set(uint32_t flags)
    {
        if(isOpen == false)
        {
            return 0;
        }
        return osThreadFlagsSet(idTask, flags);
    }

    uint32_t Get()
    {
        if(isOpen == false)
        {
            return 0;
        }
        return osThreadFlagsGet();
    }
    uint32_t Clear(uint32_t flags)
    {
        if(isOpen == false)
        {
            return 0;
        }
        return osThreadFlagsClear(flags);
    }
    uint32_t Wait(uint32_t flags, uint32_t timeout)
    {
        if(isOpen == false)
        {
            return 0;
        }
        return osThreadFlagsWait(flags, options, timeout);
    }

    uint32_t Wait(uint32_t flags, uint32_t options , uint32_t timeout)
    {
        if(isOpen == false)
        {
            return 0;
        }
        return osThreadFlagsWait(flags, options, timeout);
    }

    /**
     * @brief Get the available stack space (High Water Mark)
     * @return uint32_t Remaining stack space in bytes. Returns 0 if task is not open.
     */
    uint32_t GetStackSpace() const
    {
        if(isOpen == false || idTask == nullptr)
        {
            return 0;
        }
        // Trả về số byte stack chưa từng được sử dụng tới (càng gần 0 càng nguy hiểm)
        return osThreadGetStackSpace(idTask); 
    }

    /**
     * @brief Get the total configured stack size
     * @return uint32_t Total stack size in bytes
     */
    uint32_t GetTotalStackSize() const
    {
        return S; 
    }

    /**
     * @brief Get stack usage percentage
     * @return uint8_t Percentage of stack used (0 - 100)
     */
    uint8_t GetStackUsage() const
    {
        if(isOpen == false || idTask == nullptr || S == 0)
        {
            return 0;
        }
        uint32_t free_space = osThreadGetStackSpace(idTask);
        uint32_t used_space = S - free_space;
        
        return (uint8_t)((used_space * 100) / S);
    }
    
    void NotifyFromISR(BaseType_t *pxHigherPriorityTaskWoken) {
            if (idTask != nullptr) {
                vTaskNotifyGiveFromISR((TaskHandle_t)idTask, pxHigherPriorityTaskWoken);
            }
    }
    /**
     * @brief Operator() to execute client callback
     *
     */
    void execute() const { func(); }
    void operator()() const { execute(); }
private:

    static constexpr bool StackSizeRequired() { return S % sizeof(StackType_t) == 0; }

    /**
     * @brief Get the Stack Size in StackType_t
     *
     * @return constexpr uint32_t
     */
    static constexpr uint32_t GetStackSize() {
        static_assert(StackSizeRequired(), "Stack size must be multiple of StackType_t");
        return S / sizeof(StackType_t);
    }
    osThreadId_t                idTask{0};
    osThreadAttr_t              attrInitStruct{0};
    TaskFunc                    func{0};
    uint32_t                    options{FLAGS_WAIT_ANY};
    StackType_t  stack[GetStackSize()] {0};  // Task stack
    StaticTask_t tcb{0};                    // Task control block
};
/* Exported functions --------------------------------------------------------*/

#endif /* __RTOS_CMSIS_OS_V2_INC_RTOS_CMSIS_OS_V2_TASK_STATIC_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
