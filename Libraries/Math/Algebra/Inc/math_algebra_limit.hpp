 
/** 
  ******************************************************************************
  * @file    math_algebra_limit.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Jun 13, 2023
  * @brief   This file contains all the functions prototypes for the math_algebra_limit.cpp 
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

#ifndef __MATH_ALGEBRA_INC_MATH_ALGEBRA_LIMIT_HPP__
#define __MATH_ALGEBRA_INC_MATH_ALGEBRA_LIMIT_HPP__

/* Includes ------------------------------------------------------------------*/

#include "math_algebra_config.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/// @brief cac thuat toan lien quan den gioi han
template<class T>
class Math::Algebra::Limit
{

public:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________

    /** @brief ham gioi han mot nua vong
     *  @param[in] val gia tri can gioi han
     *  @param[in] threshold nguong dung de gioi han
     *  @return gia tri da gioi han
     * */
    static T HalfRound(T val, T threshold)
    {
        T threshold_per_2 = threshold/2;

        if(val > threshold_per_2)       return (val - threshold);
        else if(val < -threshold_per_2) return (val + threshold);

        return val;
    }

    /** @brief ham gioi han max min
     *  @param[in] val gia tri can gioi han
     *  @param[in] max gia tri lon nhat
     *  @param[in] min gia tri nho nhat
     *  @return gia tri da gioi han
     * */
    static T MaxMin(T val, T max, T min)
    {
        if(val > max)
        {
            return max;
        }
        if(val < min)
        {
            return min;
        }

        return val;
    }

    /** @brief ham gioi han mot vong
     *  @param[in] val gia tri can gioi han
     *  @param[in] threshold nguong can gioi han
     *  @return gia tri da gioi han
     * */
    static T Round(T val, T threshold)
    {
        int soNguyen;
        T offset;

        while(val < 0)
        {
            val += threshold;
        }

        /// cal
        soNguyen = val/threshold;
        offset = val - soNguyen*threshold;

        return offset;
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

#endif /* __MATH_ALGEBRA_INC_MATH_ALGEBRA_LIMIT_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
