 
/** 
  ******************************************************************************
  * @file    hardware_u5_peripheral_time.cpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Nov 25, 2024
  * @brief   
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2024 Gremsy.  
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

#include "Hardware/Stm32U5/Inc/hardware_u5_peripheral_time.hpp"

/* Private typedef -----------------------------------------------------------*/
#if defined STM32U5 || defined STM32H5
/* Private typedef -----------------------------------------------------------*/

///
typedef struct
{
    uint32_t isrCount;
}
HarwarePeripheralTimePrivateType;

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

///
const uint32_t HARDWARE_PERIPHERAL_TIME_TYPE_PRESCALE[TIME_TYPE_COUNT] = {1000, 1, 1000000, 1};

/// khai bao gia tri chay mac dinh cua time
HarwarePeripheralTimePrivateType    __gp_hardware_peripheral_time;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

void hardware_h7_peripheral_time_period_elapsed_callback(struct __TIM_HandleTypeDef *htim)
{
    __gp_hardware_peripheral_time.isrCount++;
}

/* Public functions ----------------------------------------------------------*/

/** @brief ham set gia tri thoi gian cho time
 * */
HW::Peripheral::Time &HW::Peripheral::Time::operator =(const int t)
{
    tick = GetTick() + t;

    return *this;
}

/** @brief phep tinh so sanh bang
 *  @param[in] t thoi gian muon so sanh
 *  @return true/false
 * */
bool HW::Peripheral::Time::operator ==(const int t){
    uint32_t delta = GetTick() - tick;
    uint32_t stime = delta/HARDWARE_PERIPHERAL_TIME_TYPE_PRESCALE[type];

    if(stime == (uint32_t)t)
    {
        time = stime;
        return true;
    }

    return false;
}

/** @brief phep tinh so sanh lon hon
 *  @param[in] t thoi gian muon so sanh
 *  @return true/false
 * */
bool HW::Peripheral::Time::operator >(const int t)
{
    uint32_t delta = GetTick() - tick;
    uint32_t stime = delta/HARDWARE_PERIPHERAL_TIME_TYPE_PRESCALE[type];

    if(stime > (uint32_t)t)
    {
        time = stime;
        return true;
    }

    return false;
}

/** @brief phep tinh so sanh be hon
 *  @param[in] t thoi gian muon so sanh
 *  @return true/false
 * */
bool HW::Peripheral::Time::operator <(const int t)
{
    uint32_t delta = GetTick() - tick;
    uint32_t stime = delta/HARDWARE_PERIPHERAL_TIME_TYPE_PRESCALE[type];

    if(stime < (uint32_t)t)
    {
        time = stime;
        return true;
    }

    return false;
}

/** @brief phep tinh so sanh lon hon hoac bang
 *  @param[in] t thoi gian muon so sanh
 *  @return true/false
 * */
bool HW::Peripheral::Time::operator >=(const int t)
{
    uint32_t delta = GetTick() - tick;
    uint32_t stime = delta/HARDWARE_PERIPHERAL_TIME_TYPE_PRESCALE[type];

    if(stime >= (uint32_t)t)
    {
        time = stime;
        return true;
    }

    return false;
}

/** @brief phep tinh so sanh be hon hoac bang
 *  @param[in] t thoi gian muon so sanh
 *  @return true/false
 * */
bool HW::Peripheral::Time::operator <=(const int t)
{
    uint32_t delta = GetTick() - tick;
    uint32_t stime = delta/HARDWARE_PERIPHERAL_TIME_TYPE_PRESCALE[type];

    if(stime <= (uint32_t)t)
    {
        time = stime;
        return true;
    }

    return false;
}

/** @brief operation hoat dong mong muon cua time nhu:
 *         TIME_GET: doc tai thoi diem dat lenh,
 *         TIME_LOOP: doc tai vi tri dat lenh va lap lai
 *  @param[in] t kieu hoat dong
 *  @return thoi gian doc duoc
 * */
uint32_t HW::Peripheral::Time::operator >>(TimeProcessEnum t)
{
    uint32_t delta = GetTick() - tick;
    uint32_t stime = delta/HARDWARE_PERIPHERAL_TIME_TYPE_PRESCALE[type];

    if(t == TIME_LOOP)
    {
        tick = GetTick();
    }

    ///
    time = stime;

    return stime;
}

/** @brief ham doc gia tri tick cua time tinh toan trong periperal
 *  @param none
 *  @return gia tri tick hoat dong tai thoi diem doc
 * */
uint32_t HW::Peripheral::Time::GetTick(void)
{
#ifdef PERIPHERAL_TIME
    if (PERIPHERAL_TIME == nullptr || PERIPHERAL_TIME->Instance == nullptr)
    {
        return 0;
    }
    uint32_t tick = PERIPHERAL_TIME->Instance->CNT;
    return tick;
#else
    return 0;
#endif
}

/** @brief ham doc thoi gian tu khi he thong khoi dong theo us
 *  @param none
 *  @return thoi gian tu khi he thong khoi dong
 * */
uint32_t HW::Peripheral::Time::GetSystemTimeUs(void)
{
    return GetTick();
}

/** @brief ham doc thoi gian tu khi he thong khoi dong theo ms
 *  @param none
 *  @return thoi gian tu khi he thong khoi dong
 * */
uint32_t HW::Peripheral::Time::GetSystemTimeMs(void)
{
    return GetTick()/1000;
}

/** @brief ham doc thoi gian tu khi he thong khoi dong theo us
 *  @param none
 *  @return thoi gian tu khi he thong khoi dong
 * */
uint32_t HW::Peripheral::Time::GetSystemTimeS(void)
{
    return GetTick()/1000000;
}

/** @brief ham cai dat ngoai vi cho time
 *  @param none
 *  @return trang thai qua trinh cai dat
 * */
StatusEnum HW::Peripheral::Time::Init(void)
{

#ifdef PERIPHERAL_TIME
    StatusEnum ret;
    /// khoi dong chay time system of gremsy
    ret = (StatusEnum)HAL_TIM_Base_Start_IT(PERIPHERAL_TIME);

    /// cai dat ngat tang thoi gian
    PERIPHERAL_TIME->PeriodElapsedCallback = hardware_h7_peripheral_time_period_elapsed_callback;

    return ret;
#endif

    return ERROR_HAL;
}
#endif
/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
