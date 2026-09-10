 
/** 
  ******************************************************************************
  * @file    type_include.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Apr 12, 2023
  * @brief   This file contains all the functions prototypes for the type_include.cpp 
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

#ifndef __TYPE_TYPE_INCLUDE_HPP__
#define __TYPE_TYPE_INCLUDE_HPP__

/* Includes ------------------------------------------------------------------*/

#include "gremsy_include.hpp"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/// @brief namespace chua cac kieu bien cua gremsy
namespace Type
{
    class String;
    class StringV2;

    class Node;

    template <class T>
    class NodeV2;

    template <class T>
    class Vector3;

    template <class T>
    class Matrix3;
}

/* Exported functions --------------------------------------------------------*/

#endif /* __TYPE_TYPE_INCLUDE_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
