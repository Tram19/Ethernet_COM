 
/** 
  ******************************************************************************
  * @file    hardware_u5_peripheral_time.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Nov 25, 2024
  * @brief   This file contains all the functions prototypes for the hardware_u5_peripheral_time.cpp 
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

#ifndef __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_TIME_HPP__
#define __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_TIME_HPP__

/* Includes ------------------------------------------------------------------*/
#include <Hardware/Stm32U5/Inc/hardware_u5_peripheral_config.hpp>

#if defined STM32U5 || defined STM32H5
/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/// @brief
class HW::Peripheral::Time : public HW::Base::Time {

public:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________
    TimeOptionEnum  type;
    int             time;

    /// OPERATOR _______________________________________________________________

    /** @brief operator add phan tu trong time
     *  @param[in] t time type
     *  @return tra ve time type
     * */
    Time &operator =(const Time      &t)
    {
        type = t.type;
        time = t.time;
        tick = GetTick();
        return *this;
    }

    /** @brief operator add phan tu trong time
     *  @param[in] t thoi gian set vao time
     *  @return tra ve time type
     * */
    Time &operator =(const int       t);

    /** @brief operator add phan tu trong time
     *  @param[in] t kieu thoi gian cua time
     *  @return tra ve time type
     * */
    Time &operator =(TimeOptionEnum  t)
    {
        type = t;
        return *this;
    }

    /** @brief phep tinh so sanh bang
     *  @param[in] t thoi gian muon so sanh
     *  @return true/false
     * */
    bool operator ==(const int t);

    /** @brief phep tinh so sanh lon hon
     *  @param[in] t thoi gian muon so sanh
     *  @return true/false
     * */
    bool operator >(const int t);

    /** @brief phep tinh so sanh be hon
     *  @param[in] t thoi gian muon so sanh
     *  @return true/false
     * */
    bool operator <(const int t);

    /** @brief phep tinh so sanh lon hon hoac bang
     *  @param[in] t thoi gian muon so sanh
     *  @return true/false
     * */
    bool operator >=(const int t);

    /** @brief phep tinh so sanh be hon hoac bang
     *  @param[in] t thoi gian muon so sanh
     *  @return true/false
     * */
    bool operator <=(const int t);

    /** @brief operation hoat dong mong muon cua time nhu:
     *         TIME_GET: doc tai thoi diem dat lenh,
     *         TIME_LOOP: doc tai vi tri dat lenh va lap lai
     *  @param[in] t kieu hoat dong
     *  @return thoi gian doc duoc
     * */
    uint32_t operator >>(TimeProcessEnum t);

    /// FUNCTION _______________________________________________________________

    /** @brief ham doc gia tri tick cua time tinh toan trong periperal
     *  @param none
     *  @return gia tri tick hoat dong tai thoi diem doc
     * */
    static uint32_t GetTick(void);

    /** @brief ham doc thoi gian tu khi he thong khoi dong theo us
     *  @param none
     *  @return thoi gian tu khi he thong khoi dong
     * */
    static uint32_t GetSystemTimeUs(void);

    /** @brief ham doc thoi gian tu khi he thong khoi dong theo us
     *  @param none
     *  @return thoi gian tu khi he thong khoi dong
     * */
    static uint32_t GetSystemTimeMs(void);

    /** @brief ham doc thoi gian tu khi he thong khoi dong theo us
     *  @param none
     *  @return thoi gian tu khi he thong khoi dong
     * */
    static uint32_t GetSystemTimeS(void);

    /** @brief ham cai dat ngoai vi cho time
     *  @param none
     *  @return trang thai qua trinh cai dat
     * */
    static StatusEnum Init(void);

protected:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________
    uint32_t tick;

    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________



private:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


};

/* Exported functions --------------------------------------------------------*/

#endif /** STM32U5xx  */

#endif /* __HARDWARE_STM32U5_INC_HARDWARE_U5_PERIPHERAL_TIME_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
