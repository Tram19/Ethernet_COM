 
/** 
  ******************************************************************************
  * @file    ConnectionBase.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Apr 1, 2026
  * @brief   This file contains all the functions prototypes for the ConnectionBase.cpp 
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

#ifndef __DRIVER_CONNECTIONBASE_HPP__
#define __DRIVER_CONNECTIONBASE_HPP__

/* Includes ------------------------------------------------------------------*/
#include <span>
#include "gremsy_include.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/
class Connection {
public:

    /**
     * @brief Send raw buffer data to low level port without packing msg
     *
     * @param buff
     * @return true
     * @return false
     */
    virtual bool SinkIt(std::span<const uint8_t> buff) = 0;

    bool should_forward_messages() const
    {
        return _forwarding_option;
    }
protected:

    Connection() = default;
    Connection(bool forwarding_option) : _forwarding_option(forwarding_option){};
    virtual ~Connection() { Close(); }

    /**
     * @brief Open the COM port
     *
     * @return StatusEnum
     */
    virtual StatusEnum Open()  { return OK; };

    /**
     * @brief Close the COM port
     *
     * @return StatusEnum
     */
    virtual StatusEnum Close()  { return OK; };

    /**
     * @brief Read data callback, must be called from the implementation of the port after read data from low level
     *
     * @param data
     */
    virtual void ReadDataCallback(std::span<const uint8_t> data) { (void)data; };

    bool _forwarding_option{true};
};
/* Exported functions --------------------------------------------------------*/

#endif /* __DRIVER_CONNECTIONBASE_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
