 
/** 
  ******************************************************************************
  * @file    rtos_base_stream_buffer.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Jun 20, 2025
  * @brief   This file contains all the functions prototypes for the rtos_base_stream_buffer.cpp 
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

#ifndef __RTOS_BASE_INC_RTOS_BASE_STREAM_BUFFER_HPP__
#define __RTOS_BASE_INC_RTOS_BASE_STREAM_BUFFER_HPP__

/* Includes ------------------------------------------------------------------*/
#include <mutex>
#include <span>
#include "RTOS/rtos_include.hpp"
#include "FreeRTOS.h"
#include "stream_buffer.h"
#include "rtos_base_common.hpp"

#include "RTOS/FreeRTOS/Inc/free_rtos_mutex.hpp"
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/**
 * @brief A C++ wrapper for the FreeRTOS stream buffer functionality.
 *
 * This class provides an object-oriented interface to create and manage
 * FreeRTOS stream buffers, simplifying their use in C++ applications.
 */

class RTOS::Base::StreamBuffer : public RTOS::Base::Common{

public:

    /// TYPEDEF ________________________________________________________________
    StreamBuffer() = default;
    ~StreamBuffer() {Close();};
    /// PARRAM _________________________________________________________________

    /**
     * Closes the message queue.
     *
     * @return StatusEnum indicating the result of the close operation.
     *
     */
    StatusEnum Close();


    /**
     * @brief Check if message queue is open
     *
     * @return true
     * @return false
     */
    bool IsOpen(void) const { return handle != nullptr; }


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________

    uint16_t Send (const char* data, uint32_t timeout = 1000);
    uint16_t Send (char * data, uint16_t size, uint32_t timeout = 1000);
    uint16_t Send (uint8_t * data, uint16_t size, uint32_t timeout = 1000);
    uint16_t Send (void * data, uint16_t size, uint32_t timeout = 1000);
    uint16_t Receive(void * data, uint16_t size, uint32_t timeout = OsMaxDELAY);
//    virtual uint16_t Receive(void * data);
    StreamBufferHandle_t handle = nullptr;
protected:

    /// TYPEDEF ________________________________________________________________
#if USING_LOCK_STREAM_BUFFER
    mutable RTOS::StaticMutex _mutex;
#endif
    /// PARRAM _________________________________________________________________


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________

    /**
     * Sends data to the message queue.
     *
     * @param _data The data to be sent.
     * @param _timeout The time to wait for the send operation to complete. Not used if inside ISR
     *
     * @return True if the data was sent successfully, false otherwise.
     */
    uint16_t DoSend(void * data, uint16_t size, uint32_t timeout);

    /**
     * @brief Receives data from the message queue.
     *
     * @param _data The data buffer to store the received data.
     * @param _timeout The time to wait for the receive operation to complete. Not used if inside ISR
     * @return true
     * @return false
     */
    uint16_t DoReceive(void * data, uint16_t size, uint32_t timeout);

private:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


};
#endif /* __RTOS_BASE_INC_RTOS_BASE_STREAM_BUFFER_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
