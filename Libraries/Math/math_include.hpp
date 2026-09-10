 
/** 
  ******************************************************************************
  * @file    math_include.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Jun 7, 2023
  * @brief   This file contains all the functions prototypes for the math_include.cpp 
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

#ifndef __MATH_MATH_INCLUDE_HPP__
#define __MATH_MATH_INCLUDE_HPP__

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

namespace Math
{
    /// TYPEDEF ________________________________________________________________


    /// CLASS __________________________________________________________________
    namespace Algebra
    {
        template<class T>
        class Sign;

        template<class T>
        class Limit;

        template<class T>
        class Check;

        template<class T>
        class Exp;

    }

    namespace Algorithm
    {
        template<class T>
        class Pid;
    }

    namespace String
    {
        class Convert;
        class Compare;
        class Check;
    }

    namespace Geometry
    {
        template<class T>
        class Trigometry;
    }



}

/* Exported functions --------------------------------------------------------*/

#endif /* __MATH_MATH_INCLUDE_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
