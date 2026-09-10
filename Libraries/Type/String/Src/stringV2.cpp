 
/** 
  ******************************************************************************
  * @file    stringV2.cpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Nov 8, 2023
  * @brief   
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2023 Gremsy.  
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

#include "Type/String/Inc/stringV2.hpp"
#include "RTOS/rtos.hpp"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/** @brief ham giai phong vung nho
 * */
void Type::StringV2::Free(void* ptr)
{
    /// kiem tra vung nho da cap phat hay chua?
    if(g_is_var_heap(ptr))
    {
        g_free(ptr);
        array = 0;
    }
}

/** @brief ham cao phat vung nho
 * */
void* Type::StringV2::Malloc(int size)
{
    return g_malloc(size);
}

/* Private functions ---------------------------------------------------------*/

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
