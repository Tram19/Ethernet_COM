 
/** 
  ******************************************************************************
  * @file    rtos_cmsis_os_v2_config.cpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Apr 20, 2023
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

#include "RTOS/CMSIS_OS_V2/Inc/rtos_cmsis_os_v2_config.hpp"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

uint32_t __gp_heap_startadd;
uint32_t __gp_heap_endadd;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/* Public functions ----------------------------------------------------------*/

/** @brief ham cai dat cho heap
 *  @param none
 *  @return none
 * */
void g_heap_init(void)
{
    uint8_t* ptr = (uint8_t*)g_malloc(10);

    ///
    __gp_heap_startadd = (uint32_t)ptr;
    __gp_heap_endadd   = __gp_heap_startadd + xPortGetFreeHeapSize();

    ///
    g_free(ptr);
}

/** @brief ham doc gia tri con lai trong heap
 *  @param none
 *  @return gia tri con lai tron heap
 * */
uint32_t g_heap_size(void)
{
    return xPortGetFreeHeapSize();
}

/** @brief ham cap phat vung nho
 *  @param[in] size do lon can cap phat
 *  @return dia chi cap phat
 * */
void* g_malloc(int size)
{
    return pvPortMalloc(size);
}

/** @brief ham cap phat vung nho trong
 *  @param[in] size do lon can cap phat
 *  @return dia chi cap phat
 * */
void* g_calloc(int size)
{
    uint8_t* buff = (uint8_t*)pvPortMalloc(size);
    int i;

    for(i = 0; i < size; i++)
    {
        buff[i] = 0;
    }

    return buff;
}

/** @brief ham giai phong dia chi vung nho
 *  @param[in] ptr dia chi can giai phong
 *  @return none
 * */
void g_free(void* ptr)
{
    vPortFree(ptr);
}

/** @brief ham kiem tra dia chi nam trong heap
 *  @param[in] ptr dia chi can kiem tra
 *  @return true dia chi nam trong heap
 *          false dia chi nam ngoai heap
 * */
bool g_is_var_heap(void* ptr)
{
    uint32_t ptrAdd = (uint32_t)ptr;

    if(ptrAdd > __gp_heap_startadd && ptrAdd < __gp_heap_endadd)
    {
        return true;
    }

    return false;
}

/** @brief ham delay ms
 *  @param[in] time thoi gian delay (ms)
 *  @return none
 * */
void g_os_delay(int time)
{
    osDelay(time);
}


/** @brief ham cai dat ban dau cho rtos
 *  @param none
 *  @return none
 * */
void g_rtos_init(void)
{
    /// cai dat lay gia tri trong heap
    g_heap_init();
}

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
