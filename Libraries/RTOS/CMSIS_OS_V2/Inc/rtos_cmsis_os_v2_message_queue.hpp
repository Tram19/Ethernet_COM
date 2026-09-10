 
/** 
  ******************************************************************************
  * @file    rtos_cmsis_os_v2_message_queue.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Dec 26, 2024
  * @brief   This file contains all the functions prototypes for the rtos_cmsis_os_v2_message_queue.cpp 
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

#ifndef __RTOS_CMSIS_OS_V2_INC_RTOS_CMSIS_OS_V2_MESSAGE_QUEUE_HPP__
#define __RTOS_CMSIS_OS_V2_INC_RTOS_CMSIS_OS_V2_MESSAGE_QUEUE_HPP__

/* Includes ------------------------------------------------------------------*/
#include "rtos_cmsis_os_v2_config.hpp"
#include "queue.h"
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/
/// @brief class MessageQueue

template <typename T, size_t Capacity>
class RTOS::MessageQueue : public RTOS::Base::MessageQueue {

public:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________
    MessageQueue() = default;
    MessageQueue(const char _name[])
    {
        name = _name;
    }


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________

    /** @brief ham bat dau hoat dong cua task
     *  @return trang thai mo
     * */
    StatusEnum Open(void)
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

        this->capacity = Capacity;

        attrQueueStruct.name = name.array;

        idQueue = osMessageQueueNew(this->capacity, sizeof(T), &attrQueueStruct);

        if(idQueue == 0)
        {
            return ERROR_HAL;
        }

        isOpen = true;
        return ret;
    }

    /** @brief ham dong hoat dong cua task
     *  @return trang thai dong
     * */
    StatusEnum Close(void)
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
        uint32_t status = osMessageQueueDelete(idQueue);

        if(status != osOK)
        {
            return ERROR_HAL;
        }

        ///
        isOpen = false;

        return ret;
    }

    /** @brief ham gui du lieu vao queue
     *  @param[in] pointer cua du lieu
     *  @return trang thai set co
     * */
    osStatus_t Put(T* ptr_mes)
    {
        if(isOpen)
        {
            return osMessageQueuePut(idQueue, (void *)ptr_mes, 0U, 0U);
        }
        else
        {
            return osError;
        }
    }

    /** @brief ham gui du lieu vao queue //chi sai khi capacity = 1
     *  @param[in] pointer cua du lieu
     *  @return trang thai set co
     * */
    osStatus_t PutOver(T* ptr_mes)
    {
        if(isOpen)
        {
             if(this->capacity != 1) return osError;

             QueueHandle_t hQueue = (QueueHandle_t)idQueue;
             osStatus_t stat;
             BaseType_t yield;

             stat = osOK;

             if (RTOS::IsInsideISR()) {
               if ((hQueue == NULL) || (ptr_mes == NULL)) {
                 stat = osErrorParameter;
               }
               else {
                 yield = pdFALSE;

                 if (xQueueOverwriteFromISR (hQueue, (void *)ptr_mes, &yield) != pdTRUE) {
                   stat = osErrorResource;
                 } else {
                   portYIELD_FROM_ISR (yield);
                 }
               }
             }
             else {
               if ((hQueue == NULL) || (ptr_mes == NULL)) {
                 stat = osErrorParameter;
               }
               else
               {
                 if (xQueueOverwrite (hQueue, ptr_mes) != pdPASS)
                 {
                     stat = osErrorResource;
                 }
               }
             }

             return (stat);
        }
        else
        {
            return osError;
        }
    }


    /** @brief ham doi du lieu vao queue
     *  @param[in] pointer cua du lieu
     *  @param[in] timeout
     *  @return trang thai doi co
     * */
    osStatus_t Get(T* ptr_mes, uint32_t timeout)
    {
        if(isOpen)
        {
            return  osMessageQueueGet(idQueue, (T*)ptr_mes, NULL, timeout);
        }
        else
        {
            return osError;
        }
    }

    template<typename... Args>
    osStatus_t Emplace(Args&&... args)
    {
        if (!isOpen) return osError;
        T item(std::forward<Args>(args)...);
        return osMessageQueuePut(idQueue, (void*)&item, 0U, 0U);
    }

    uint32_t GetCount()
    {
        if(isOpen)
        {
            return  osMessageQueueGetCount(idQueue);
        }
        else
        {
            return osError;
        }
    }
    osStatus_t Reset()
    {
        if(isOpen)
        {
            return  osMessageQueueReset(idQueue);
        }
        else
        {
            return osError;
        }
    }
protected:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________
    osMessageQueueId_t    idQueue {0};
    osMessageQueueAttr_t  attrQueueStruct {0};
    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


private:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


};
/* Exported functions --------------------------------------------------------*/
/// @brief Static MessageQueue class (Zero-Heap memory footprint)
template <typename T, size_t Capacity>
class RTOS::MessageQueueStatic : public RTOS::Base::MessageQueue {

public:

    /// PARAM _________________________________________________________________
	MessageQueueStatic() = default;
	MessageQueueStatic(const char _name[])
    {
        name = _name;
    }

    /// FUNCTION _______________________________________________________________

    /** * @brief  Open and initialize the message queue using static memory.
     * @return Status of the operation
     */
    StatusEnum Open(void)
    {
        StatusEnum ret = OK;

        if(name.IsExist() != OK)
        {
            return ERROR_PARAM;
        }

        if(isOpen == true)
        {
            return ERROR_EXIST;
        }

        this->capacity = Capacity;

        // 1. Assign queue name
        attrQueueStruct.name = name.array;

        // 2. Map the static memory blocks to the OS Attributes (CRITICAL FOR ZERO-HEAP)
        attrQueueStruct.cb_mem  = &_queue_cb;
        attrQueueStruct.cb_size = sizeof(_queue_cb);
        attrQueueStruct.mq_mem  = _queue_buffer;
        attrQueueStruct.mq_size = sizeof(_queue_buffer);

        // 3. Create the queue. Since we provided memory, it will NOT use pvPortMalloc.
        idQueue = osMessageQueueNew(this->capacity, sizeof(T), &attrQueueStruct);

        if(idQueue == nullptr)
        {
            return ERROR_HAL;
        }

        isOpen = true;
        return ret;
    }

    /** * @brief  Close and delete the message queue
     * @return Status of the operation
     */
    StatusEnum Close(void)
    {
        StatusEnum ret = OK;

        if(name.IsExist() != OK) return ERROR_PARAM;
        if(isOpen == false)      return ERROR_EXIST;

        uint32_t status = osMessageQueueDelete(idQueue);

        if(status != osOK) return ERROR_HAL;

        isOpen = false;
        return ret;
    }

    /** * @brief  Put data into the queue
     * @param[in] ptr_mes Pointer to the data
     * @return OS status
     */
    osStatus_t Put(T* ptr_mes)
    {
        if(isOpen) return osMessageQueuePut(idQueue, (const void *)ptr_mes, 0U, 0U);
        return osError;
    }

    /** * @brief  Overwrite data in the queue (Valid only if Capacity == 1)
     * @param[in] ptr_mes Pointer to the data
     * @return OS status
     */
    osStatus_t PutOver(T* ptr_mes)
    {
        if(!isOpen) return osError;
        if(this->capacity != 1) return osError;

        QueueHandle_t hQueue = (QueueHandle_t)idQueue;
        osStatus_t stat = osOK;

        if (RTOS::IsInsideISR()) {
            if ((hQueue == NULL) || (ptr_mes == NULL)) {
                stat = osErrorParameter;
            } else {
                BaseType_t yield = pdFALSE;
                if (xQueueOverwriteFromISR(hQueue, (const void *)ptr_mes, &yield) != pdTRUE) {
                    stat = osErrorResource;
                } else {
                    portYIELD_FROM_ISR(yield);
                }
            }
        } else {
            if ((hQueue == NULL) || (ptr_mes == NULL)) {
                stat = osErrorParameter;
            } else {
                if (xQueueOverwrite(hQueue, ptr_mes) != pdPASS) {
                    stat = osErrorResource;
                }
            }
        }
        return stat;
    }

    /** * @brief  Get data from the queue
     * @param[out] ptr_mes Pointer to receive the data
     * @param[in] timeout Wait timeout
     * @return OS status
     */
    osStatus_t Get(T* ptr_mes, uint32_t timeout)
    {
        if(isOpen) return osMessageQueueGet(idQueue, (void*)ptr_mes, NULL, timeout);
        return osError;
    }

    /** * @brief  Construct and insert data directly into the queue
     */
    template<typename... Args>
    osStatus_t Emplace(Args&&... args)
    {
        if (!isOpen) return osError;
        T item(std::forward<Args>(args)...);
        return osMessageQueuePut(idQueue, (const void*)&item, 0U, 0U);
    }

    /** * @brief  Get the number of items in the queue
     */
    uint32_t GetCount()
    {
        if(isOpen) return osMessageQueueGetCount(idQueue);
        return 0; // Better return 0 than osError (which is a negative enum or 0x80)
    }

    /** * @brief  Reset the queue to empty state
     */
    osStatus_t Reset()
    {
        if(isOpen) return osMessageQueueReset(idQueue);
        return osError;
    }

protected:

    /// PARAM _________________________________________________________________
    osMessageQueueId_t    idQueue {nullptr};
    osMessageQueueAttr_t  attrQueueStruct {0};

private:

    /// STATIC MEMORY BUFFERS (ZERO-HEAP ARCHITECTURE) ________________________
    
    // Aligned to 4 bytes as strictly required by FreeRTOS/CMSIS-OS2
    alignas(4) StaticQueue_t 	_queue_cb;
    alignas(4) uint8_t          _queue_buffer[Capacity * sizeof(T)];

};
#endif /* __RTOS_CMSIS_OS_V2_INC_RTOS_CMSIS_OS_V2_MESSAGE_QUEUE_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
