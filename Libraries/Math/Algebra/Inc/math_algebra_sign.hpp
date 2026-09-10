 
/** 
  ******************************************************************************
  * @file    math_algebra_sign.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Jun 13, 2023
  * @brief   This file contains all the functions prototypes for the math_algebra_sign.cpp 
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

#ifndef __MATH_ALGEBRA_INC_MATH_ALGEBRA_SIGN_HPP__
#define __MATH_ALGEBRA_INC_MATH_ALGEBRA_SIGN_HPP__

/* Includes ------------------------------------------------------------------*/

#include "math_algebra_config.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/// @brief
template<class T>
class Math::Algebra::Sign {

public:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________

    /** @breif tri tuyet doi gia tri
     *  @param[in] val gia tri can tri tuyet doi
     *  @return gia tri da tri tuyet doi
     * */
    static T Abs(T val)
    {
        if(val < 0) return -val;
        return val;
    }

    /** @brief ham tim dau cua gia tri
     *  @parma[in] val gia tri tim dau
     *  @return 1: dau duong
     *         -1: dau am
     * */
    static T Is(T val)
    {
        if(val < 0) return (T)-1;

        return (T)1;
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

#endif /* __MATH_ALGEBRA_INC_MATH_ALGEBRA_SIGN_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
