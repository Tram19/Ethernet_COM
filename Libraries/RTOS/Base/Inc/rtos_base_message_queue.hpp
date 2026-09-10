 
/** 
  ******************************************************************************
  * @file    rtos_base_message_queue.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Dec 26, 2024
  * @brief   This file contains all the functions prototypes for the rtos_base_message_queue.cpp 
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

#ifndef __RTOS_BASE_INC_RTOS_BASE_MESSAGE_QUEUE_HPP__
#define __RTOS_BASE_INC_RTOS_BASE_MESSAGE_QUEUE_HPP__

/* Includes ------------------------------------------------------------------*/

#include "rtos_base_common.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/


class RTOS::Base::MessageQueue : public RTOS::Base::Common {

public:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________
    MessageQueue() = default;
    Type::String        name;


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________

protected:

    /// TYPEDEF ________________________________________________________________
    uint32_t            capacity{0};

    /// PARRAM _________________________________________________________________


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


private:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


};
#endif /* __RTOS_BASE_INC_RTOS_BASE_MESSAGE_QUEUE_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
