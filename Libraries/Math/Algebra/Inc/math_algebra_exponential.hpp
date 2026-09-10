 
/** 
  ******************************************************************************
  * @file    math_algebra_exponential.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Jun 27, 2023
  * @brief   This file contains all the functions prototypes for the math_algebra_exponential.cpp 
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

#ifndef __MATH_ALGEBRA_INC_MATH_ALGEBRA_EXPONENTIAL_HPP__
#define __MATH_ALGEBRA_INC_MATH_ALGEBRA_EXPONENTIAL_HPP__

/* Includes ------------------------------------------------------------------*/

#include "math_algebra_config.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/// @brief
template<class T>
/// @brief class dung cho cac ham ly thua va can bac
class Math::Algebra::Exp {

public:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________

    /** @brief ham co so mu cua mot so
     *  @param base so bi mu
     *  @param exp so mu
     *  @return so da duoc tinh co so mu
     * */
    static T Pow(T base, int exp)
    {
        T ret = base;

        if(exp == 0) return 1;

        for(int i = 0; i < exp; i++)
        {
            ret *= base;
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

#endif /* __MATH_ALGEBRA_INC_MATH_ALGEBRA_EXPONENTIAL_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
