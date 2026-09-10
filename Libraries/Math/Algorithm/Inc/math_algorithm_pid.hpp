 
/** 
  ******************************************************************************
  * @file    math_algorithm_pid.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Jun 13, 2023
  * @brief   This file contains all the functions prototypes for the math_algorithm_pid.cpp 
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

#ifndef __MATH_ALGORITHM_INC_MATH_ALGORITHM_PID_HPP__
#define __MATH_ALGORITHM_INC_MATH_ALGORITHM_PID_HPP__

/* Includes ------------------------------------------------------------------*/

#include "math_algorithm_config.hpp"
#include "Math/Algebra/Inc/math_algebra_limit.hpp"
#include "Math/Algebra/Inc/math_algebra_sign.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/// @brief
template <class T>
class Math::Algorithm::Pid {

public:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________
    T kp,kd,ki;             /// he so pid

    T setpoint;             /// gia tri mong muon
    T*pcurrent;             /// dia chi gia tri hien tai
    T current;

    float inFilter;         /// gia tri tu [0:1]

    T out;                  /// gia tri dau ra
    T outMax;               /// gia tri lon nhat cua dau ra
    T outMin;               /// gia tri nho nhat cua dau ra
    float outFilter;        /// gia tri tu [0:1] theo dang phan tran do tin tuong vao gia tri cu

    T errorMin;             /// gia tri error nho nhat
    T error;                ///
    T errorOld;             ///
    T errorDelta;
    T errorSum;


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________

    /** @brief ham set gia tri cho pid
     *  @param[in] sp gia tri setpoint
     *  @return none
     * */
    void Set(T sp, T* pcur)
    {
        setpoint    = sp;
        pcurrent    = pcur;
        current     = *pcur;
        errorSum    = 0;
        out         = 0;
        errorDelta  = 0;
        errorOld    = 0;
        error       = 0;
    }

    /** @brief ham xu ly pid
     *  @param none
     *  @return true da ve dung setpoint theo chuan 5%
     *          false chua ve duoc setpoint
     * */
    bool Process(void)
    {
        bool ret = false;
        T outTemp;

        /// filter input
        current = current*inFilter + (*pcurrent)*(1.0f - inFilter);

        /// error calculator
        error       = setpoint - current;
        errorDelta  = error - errorOld;
        errorOld    = error;

        /// cho phep hoat dong error sum
        if(ki > 0.001f) errorSum += error;
        else            errorSum  = 0;

        ///
        outTemp = kp*error + kd*errorDelta + ki*errorSum/1000.0f;

        /// filter output
        out = out*outFilter + outTemp*(1.0f - outFilter);

        /// gioi han
        out = Math::Algebra::Limit<T>::MaxMin(out, outMax, outMin);

        /// kiem tra gia tri error
        if(Math::Algebra::Sign<T>::Abs(error) < errorMin)
        {
            ret = true;
        }

        return ret;
    }

protected:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


private:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


};

/* Exported functions --------------------------------------------------------*/

#endif /* __MATH_ALGORITHM_INC_MATH_ALGORITHM_PID_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
