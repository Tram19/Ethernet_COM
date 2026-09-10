 
/** 
  ******************************************************************************
  * @file    rtos_base_event.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Apr 18, 2023
  * @brief   This file contains all the functions prototypes for the rtos_base_event.cpp 
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

#ifndef __RTOS_BASE_INC_RTOS_BASE_EVENT_HPP__
#define __RTOS_BASE_INC_RTOS_BASE_EVENT_HPP__

/* Includes ------------------------------------------------------------------*/

#include "rtos_base_common.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/// @brief
class RTOS::Base::Event : public RTOS::Base::Common {

public:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________

    Type::String        name;
    FlagWaitOptionEnum  option{FLAGS_WAIT_ANY};

    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________

    /** @brief ham set co ngat
     *  @param[in] flag co ngat duoc set
     *  @return trang thai set co
     * */
    virtual uint32_t Set(uint32_t flag){return 0;}

    /** @brief ham doi co ngat
     *  @param[in] flag co ngat doi
     *  @return trang thai doi co
     * */
    virtual uint32_t Wait(uint32_t flag, uint32_t timeOut){return 0;}


protected:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


private:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


};

/* Exported functions --------------------------------------------------------*/

#endif /* __RTOS_BASE_INC_RTOS_BASE_EVENT_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
