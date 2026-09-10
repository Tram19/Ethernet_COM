 
/** 
  ******************************************************************************
  * @file    gremsy_version.cpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    May 5, 2023
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

#include "gremsy_version.hpp"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/** @brief bien luu gia tri version */
const char versionTotal[] =
{
    VERSION_MAJOR_INIT,
    '.',
    VERSION_MINOR_INIT,
    '.',
    VERSION_PATCH_INIT,
    '.',
    BUILD_YEAR_CH0, BUILD_YEAR_CH1, BUILD_YEAR_CH2, BUILD_YEAR_CH3,
    BUILD_MONTH_CH0, BUILD_MONTH_CH1,
    BUILD_DAY_CH0, BUILD_DAY_CH1,
    BUILD_HOUR_CH0, BUILD_HOUR_CH1,
    BUILD_MIN_CH0, BUILD_MIN_CH1,
    BUILD_SEC_CH0, BUILD_SEC_CH1,
    '\0'
};


/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
