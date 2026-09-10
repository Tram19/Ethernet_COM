 
/** 
  ******************************************************************************
  * @file    rtos_base_stream_buffer.cpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Jun 20, 2025
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

#include "RTOS/Base/Inc/rtos_base_stream_buffer.hpp"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
using LG = std::lock_guard<RTOS::StaticMutex>;
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
StatusEnum RTOS::Base::StreamBuffer::Close()
{
    // Cannot remove message queue in ISR
    if (IsInsideISR()) {
        return BUSY;
    }

    if (!IsOpen()) {
        return ERROR_NOT_EXIST;
    }

    vStreamBufferDelete(handle);
    handle = nullptr;

#if USING_LOCK_STREAM_BUFFER
    _mutex.Close();
#endif
    return OK;
}

uint16_t RTOS::Base::StreamBuffer::DoSend(void * data, uint16_t size , uint32_t _timeout)
{
    if (!IsOpen()) {
        return false;
    }

    if (!IsInsideISR()) {
#if USING_LOCK_STREAM_BUFFER
        LG lock(_mutex);
#endif
        return xStreamBufferSend(handle, data, size, _timeout);
    }

    BaseType_t xHigherPriorityTaskWoken = pdFALSE; /* Initialised to pdFALSE. */

    uint16_t ret = xStreamBufferSendFromISR(handle, data, size, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

    return ret;
}

uint16_t RTOS::Base::StreamBuffer::DoReceive(void * data, uint16_t size , uint32_t _timeout)
{
    if (!IsOpen()) {
        return false;
    }

    if (!IsInsideISR()) {

        return xStreamBufferReceive(handle, data, size, _timeout);

    }

    BaseType_t xHigherPriorityTaskWoken = pdFALSE; /* Initialised to pdFALSE. */

    uint16_t ret =
        xStreamBufferReceiveFromISR(handle, data, size, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

    return ret;
}

uint16_t RTOS::Base::StreamBuffer::Send (const char* data, uint32_t timeout)
{
    return DoSend((void *) data, strlen(data), timeout);
}

uint16_t RTOS::Base::StreamBuffer::Send (char * data, uint16_t size, uint32_t timeout)
{
    return DoSend((void *) data, size, timeout);
}

uint16_t RTOS::Base::StreamBuffer::Send (uint8_t * data, uint16_t size, uint32_t timeout)
{
    return DoSend((void *) data, size, timeout);
}

uint16_t RTOS::Base::StreamBuffer::Send (void * data, uint16_t size, uint32_t timeout)
{
    return DoSend(data, size, timeout);
}

uint16_t RTOS::Base::StreamBuffer::Receive(void * data, uint16_t size, uint32_t timeout)
{
    return DoReceive(data, size, timeout);
}
/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
