 
/** 
  ******************************************************************************
  * @file    hardware_base_peripheral_pwm_out.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Apr 19, 2023
  * @brief   This file contains all the functions prototypes for the hardware_base_peripheral_pwm_out.cpp 
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

#ifndef __HARDWARE_BASE_INC_HARDWARE_BASE_PERIPHERAL_PWM_OUT_HPP__
#define __HARDWARE_BASE_INC_HARDWARE_BASE_PERIPHERAL_PWM_OUT_HPP__

/* Includes ------------------------------------------------------------------*/

#include "hardware_base_common.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ----------------------
 *
 * --------------------------------------*/

/// @brief dinh nghia class chung cho cac pwm out
class HW::Base::PwmOut : public HW::Base::Common {

public:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________
    TimeIdEnum      id;
    uint32_t        prescaler;
    uint32_t        frequency;

    /// OPERATOR _______________________________________________________________

    /// FUNCTION _______________________________________________________________

    /** @brief ham thay doi prescale cua pwm
     *  @param[in] num_prescaler bo chia can dua vao
     *  @return none
     * */
    virtual void SetPrescaler(uint16_t num_prescaler){};

    /** @brief ham set duty pwm cho channel
     *  @param[in] duty do rong xung cua channel
     *  @return none
     * */
    virtual void Set1(int duty){};
    virtual void Set2(int duty){};
    virtual void Set3(int duty){};
    virtual void Set4(int duty){};

    /** @brief ham set duty pwm cho channel
     *  @param[in] percent phan tram do rong xung cua channel
     *  @return none
     * */
    virtual void Set1(float percent){};
    virtual void Set2(float percent){};
    virtual void Set3(float percent){};
    virtual void Set4(float percent){};

    /** @brief ham set tan so cho pwm
     *  @param[in] freq tan so thay doi
     *  @return none
     * */
    virtual uint32_t SetFreq(uint32_t freq){return 0;};

    /** @brief ham doc gia tri duty max cua pwm
     *  @param none
     *  @return gia tri duty max
     * */
    virtual uint32_t GetDutyMax(void){return dutyMax;}

    /** @brief ham doc dia chi thanh ghi cua pwm out
     *  @param none
     *  @return dia chi thanh ghi
     * */
    virtual volatile void* GetRegisterChannel1(void){return 0;}
    virtual volatile void* GetRegisterChannel2(void){return 0;}
    virtual volatile void* GetRegisterChannel3(void){return 0;}
    virtual volatile void* GetRegisterChannel4(void){return 0;}

protected:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________
    int     dutyMax;

    uint8_t isCh1;
    uint8_t isCh2;
    uint8_t isCh3;
    uint8_t isCh4;

    float ch1Precent;
    float ch2Precent;
    float ch3Precent;
    float ch4Precent;

    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


private:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


};

/* Exported functions --------------------------------------------------------*/

#endif /* __HARDWARE_BASE_INC_HARDWARE_BASE_PERIPHERAL_PWM_OUT_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
