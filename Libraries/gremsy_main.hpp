 
/** 
  ******************************************************************************
  * @file    gremsy_main.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Apr 11, 2023
  * @brief   This file contains all the functions prototypes for the gremsy_main.cpp 
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

#ifndef __GREMSY_MAIN_HPP__
#define __GREMSY_MAIN_HPP__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/**
 * @brief ham chay trong main chinh trong core
 * @param none
 * @return none
 * @note:
 *      khong co freertos se cho chay cuoi cai dat
 *      co freertos se chay trong qua trinh cai dat freertos
 * */
void gremsy_main(void);

#ifdef __cplusplus
}
#endif

#endif /* __GREMSY_MAIN_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
