 
/** 
  ******************************************************************************
  * @file    math_string_convert.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Jun 19, 2023
  * @brief   This file contains all the functions prototypes for the math_string_convert.cpp 
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

#ifndef __MATH_STRING_INC_MATH_STRING_CONVERT_HPP__
#define __MATH_STRING_INC_MATH_STRING_CONVERT_HPP__

/* Includes ------------------------------------------------------------------*/

#include "math_string_config.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/// @brief class dung trong chuyen doi cac dang chuoi
class Math::String::Convert {

public:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________

    /** @brief ham chuyen doi so thanh chuoi
     *  @param[in] buff dia chi mang sau khi truyen doi
     *  @param[in] num so can chuyen doi
     *  @return do dai cua mang chuyen doi
     * */
    int IntToString(char* buff, int num)
    {
        while(num != 0)
        {

        }

        return 0;
    }

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

#endif /* __MATH_STRING_INC_MATH_STRING_CONVERT_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
