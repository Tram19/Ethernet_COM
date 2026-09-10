 
/** 
  ******************************************************************************
  * @file    free_rtos_stream_buffer.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Jun 20, 2025
  * @brief   This file contains all the functions prototypes for the free_rtos_stream_buffer.cpp 
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

#ifndef __RTOS_FREERTOS_INC_FREE_RTOS_STREAM_BUFFER_HPP__
#define __RTOS_FREERTOS_INC_FREE_RTOS_STREAM_BUFFER_HPP__

/* Includes ------------------------------------------------------------------*/
#include "RTOS/rtos_include.hpp"
#include "RTOS/Base/Inc/rtos_base.hpp"
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

 template <size_t S>
 class RTOS::StreamBufferStatic : public RTOS::Base::StreamBuffer
 {
 public:
     StatusEnum Open() {
         if (IsOpen()) {
             return ERROR_EXIST;
         }
#if USING_LOCK_STREAM_BUFFER
        if (_mutex.Open()    	!= OK) {return ERROR_HAL; }
#endif
         handle = xStreamBufferCreateStatic(S, 1, buffer, &msgBuffer);

         if (handle) {
             return OK;
         }

         return ERROR_HAL;
     }
     uint16_t Receive(void * data, uint32_t timeout = OsMaxDELAY)
     {
         return RTOS::Base::StreamBuffer::Receive(data, S, timeout);
     }
 private:

     static constexpr size_t GetBufferSize() { return S + 1; };
     StaticStreamBuffer_t msgBuffer;
     uint8_t buffer[GetBufferSize()];
 };

 class RTOS::StreamBufferDynamic : public RTOS::Base::StreamBuffer
 {
 public:

     StreamBufferDynamic(size_t _queueSize) : queueSize(_queueSize){

     };
     StreamBufferDynamic(const StreamBufferDynamic& other)
         : queueSize(other.queueSize)
     {
         handle = nullptr;
     }
     StatusEnum Open() {
         if (IsOpen()) {
             return ERROR_EXIST;
         }
#if USING_LOCK_STREAM_BUFFER
        if (_mutex.Open()    	!= OK) {return ERROR_HAL; }
#endif
         handle = xStreamBufferCreate(GetBufferSize(), 1);

         if (handle) {
             return OK;
         }

         return ERROR_HAL;
     }
     uint16_t Receive(void * data)
     {
         return RTOS::Base::StreamBuffer::Receive(data, GetBufferSize());
     }
 private:
     StreamBufferDynamic() = delete;
     constexpr size_t GetBufferSize() { return queueSize  + 1; };

     size_t queueSize;
 };
/* Exported functions --------------------------------------------------------*/

#endif /* __RTOS_FREERTOS_INC_FREE_RTOS_STREAM_BUFFER_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
