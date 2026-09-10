 
/** 
  ******************************************************************************
  * @file    rtos_cmsis_os_v2_config.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Apr 20, 2023
  * @brief   This file contains all the functions prototypes for the rtos_cmsis_os_v2_config.cpp 
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

#ifndef __RTOS_CMSIS_OS_V2_INC_RTOS_CMSIS_OS_V2_CONFIG_HPP__
#define __RTOS_CMSIS_OS_V2_INC_RTOS_CMSIS_OS_V2_CONFIG_HPP__

/* Includes ------------------------------------------------------------------*/

#include "RTOS/Base/Inc/rtos_base.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

/** @brief ham doc gia tri con lai trong heap
 *  @param none
 *  @return gia tri con lai tron heap
 * */
uint32_t g_heap_size(void);

/** @brief ham cap phat vung nho
 *  @param[in] size do lon can cap phat
 *  @return dia chi cap phat
 * */
void* g_malloc(int size);

/** @brief ham cap phat vung nho trong
 *  @param[in] size do lon can cap phat
 *  @return dia chi cap phat
 * */
void* g_calloc(int size);

/** @brief ham giai phong dia chi vung nho
 *  @param[in] ptr dia chi can giai phong
 *  @return none
 * */
void g_free(void* ptr);

/** @brief ham kiem tra dia chi nam trong heap
 *  @param[in] ptr dia chi can kiem tra
 *  @return true dia chi nam trong heap
 *          false dia chi nam ngoai heap
 * */
bool g_is_var_heap(void* ptr);

/** @brief ham delay ms
 *  @param[in] time thoi gian delay (ms)
 *  @return none
 * */
void g_os_delay(int time);

/** @brief ham cai dat ban dau cho rtos
 *  @param none
 *  @return none
 * */
void g_rtos_init(void);



#endif /* __RTOS_CMSIS_OS_V2_INC_RTOS_CMSIS_OS_V2_CONFIG_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
