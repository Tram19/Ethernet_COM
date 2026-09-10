 
/** 
  ******************************************************************************
  * @file    vector3.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Oct 21, 2023
  * @brief   This file contains all the functions prototypes for the vector3.cpp 
  *          firmware library.
  *
  ******************************************************************************
  * @copyright
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

#ifndef __TYPE_MATRIX_INC_VECTOR3_HPP__
#define __TYPE_MATRIX_INC_VECTOR3_HPP__

/* Includes ------------------------------------------------------------------*/

#include "Type/type_include.hpp"
#include "Type/Matrix/Inc/matrix3.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/** @brief class for vector row 1 col 3
 *  vector fotmat
 *  [x, y, z]
 *           1x3
 * */
template <class T>
class Type::Vector3 {

public:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________

    /// OPERATOR _______________________________________________________________

    /** @brief operator khai bao gia tri gan data bang 0
     * */
    Vector3()
    {
        data[0] = 0; data[1] = 0; data[2] = 0;
    }

    /** @brief operator khai bao gia tri gan data bang x,y,z
     * */
    Vector3(const T x, const T y, const T z)
    {
        data[0] = x; data[1] = y; data[2] = z;
    }

    /** @brief operator khai bao gia tri gan data bang val
         * */
    Vector3(const T val)
    {
        data[0] = val; data[1] = val; data[2] = val;
    }

    /** @brief operator doc gia tri cua phan tu
     * */
    const T &operator [] (const int i) const
    {
        if(i<3) return data[i];
        /** @TODO xu ly khi gia tri lon hon mang*/
        return data[2];
    }

    /** @brief operation gan gia tri cho cac phan tu
     * */
    T &operator [] (const int i)
    {
        if(i<3) return data[i];
        /** @TODO xu ly khi gia tri lon hon mang*/
        return data[2];
    }

    /** @brief phep = 2 vector
     * */
    Vector3 &operator = (const Vector3 v)
    {
        if(&v != this)
        {
            data[0] = v[0];
            data[1] = v[1];
            data[2] = v[2];
        }

        return *this;
    }

    /** @brief phep = 2 vector
     * */
    Vector3 &operator = (const T val)
    {
        data[0] = val;
        data[1] = val;
        data[2] = val;

        return *this;
    }

    /** @brief phep gan vector voi gia tri
     * */
    Vector3 Equal(const T val)
    {
        data[0] = val;
        data[1] = val;
        data[2] = val;

        return *this;
    }

    /** @brief phep cong 2 vector
     * Khong thu hien theo duoc nhu ben duoi vi

     Vector3 &operator + (const Vector3 v)
     {
       Vector3 sub; // gia tri nay se duoc reset khi ra khoi ham

        for(int i = 0; i < 3; i++)
        {
            sub[i] = data[i] - v[i];
        }

        return sub;
      }

     * */
    Vector3 operator + (const Vector3 v) const
    {

        return Vector3(data[0] + v[0], data[1] + v[1], data[2] + v[2]);
    }

    /** @brief phep cong don vector
     * */
    Vector3 &operator += (const Vector3 v)
    {
        data[0] += v[0];
        data[1] += v[1];
        data[2] += v[2];

        return *this;
    }

    /** @brief phep tru 2 vector
     * */
    Vector3 operator - (const Vector3 v) const
    {
        return Vector3(data[0] - v[0], data[1] - v[1], data[2] - v[2]);
    }

    /** @brief phep tru don vector
     * */
    Vector3 &operator -= (const Vector3 v)
    {
        data[0] -= v[0];
        data[1] -= v[1];
        data[2] -= v[2];

        return *this;
    }

    /** @brief cross product
     * */
    Vector3 operator % (const Vector3 v) const
    {
        return Vector3(data[1]*v[2] - data[2]*v[1], data[2]*v[0] - data[0]*v[2], data[0]*v[1] - data[1]*v[0]);
    }
    /** @brief the dot product of two vector
     * */
    T operator * (Vector3 v) const
    {
        return (data[0]* v[0] + data[1]*v[1] + data[2]*v[2]);
    }

    /** @brief phep nhan vector voi hang so
     * */
    Vector3 operator * (const T k) const
    {
        return Vector3(data[0]*k, data[1]*k, data[2]*k);
    }

    /** @brief phep nhan don vector
     * */
    Vector3 &operator *= (const T k)
    {
        data[0] *= k;
        data[1] *= k;
        data[2] *= k;

        return *this;
    }

    /** @brief ham nhan vector voi matrix
     *                                [a1, a2, a3]
     *  [x2, y2, z2] = [x1, y1, z1] * |b1, b2, b3|
     *                                [c1, c2, c3]
     * */
    Vector3 operator * (const Matrix3<T> m) const
    {
        const T x = (*this)*m.col0();
        const T y = (*this)*m.col1();
        const T z = (*this)*m.col2();

        return Vector3(x, y, z);
    }

    /** @brief ham nhan matrix voi vector tan dung vector de luu gia tri khong can sinh them bien
     *                                [a1, a2, a3]
     *  [x1, y1, z1] = [x1, y1, z1] * |b1, b2, b3|
     *                                [c1, c2, c3]
     * */
    Vector3 &operator *= (const Matrix3<T> m)
    {

        const T x = (*this)*m[0];
        const T y = (*this)*m[1];
        const T z = (*this)*m[2];

        data[0] = x;
        data[1] = y;
        data[2] = z;

        return *this;
    }

    /** @brief phep nhan phan tử giữa 2 vector
     * */
    Vector3 MulEle(Vector3 v)
    {
        return Vector3(data[0]*v[0], data[1]*v[1], data[2]*v[2]);
    }

    /** @brief phep nhan don vector
     * */
    Vector3 MulEleEqual(Vector3 v)
    {
        data[0] *= v[0];
        data[1] *= v[1];
        data[2] *= v[2];

        return *this;
    }

    /** @brief phep chia vector voi hang so
     * */
    Vector3 operator / (const T k) const
    {
        return Vector3(data[0]/k, data[1]/k, data[2]/k);
    }

    /** @brief phep chia don vector
     * */
    Vector3 &operator /= (const T k)
    {
        for(int i = 0; i < 3; i++)
        {
            data[i] /= k;
        }

        return *this;
    }

    /** @brief phep chia phan tử giữa 2 vector
     * */
    Vector3 DivEle(Vector3 v)
    {
        return Vector3(data[0]/v[0], data[1]/v[1], data[2]/v[2]);
    }

    /** @brief phep chia don vector
     * */
    Vector3 DivEleEqual(Vector3 v)
    {
        data[0] /= v[0];
        data[1] /= v[1];
        data[2] /= v[2];

        return *this;
    }

    /** @brief doc dia chi cua bien
     * */
    T* GetAddr(void)
    {
        return data;
    }

    /** @brief do lon vector
     * */
    float Length(void) const
    {
        return sqrt(data[0]* data[0] + data[1]*data[1] + data[2]*data[2]);
    }

    /** @brief tinh goc 2 vector
     * */
    T angle(const Vector3 v) const
    {
        const T len = Length() * v.Length();

        if(len < 0)
        {
            return 0;
        }
        else
        {
            const T cosv = ((*this)*v)/len; /// tinh dot of product two vector

            if(absF(cosv) >= 1){
                return 0;
            }

            return acosF(cosv);
        }
    }

    /** @brief tinh toan Cos cho gia tri cua vector (chi su dung cho vector tinh goc rad)
     *  @param none
     *  @return gia tri da tinh Cos
     * */
    Vector3 Cos(void)
    {
        return Vector3(cos(data[0]), cos(data[1]), cos(data[2]));
    }

    /** @brief tinh toan Sin cho gia tri cua vector (chi su dung cho vector tinh goc rad)
     *  @param none
     *  @return gia tri da tinh Sin
     * */
    Vector3 Sin(void)
    {
        return Vector3(sin(data[0]), sin(data[1]), sin(data[2]));
    }


    /// FUNCTION _______________________________________________________________


protected:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


private:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________
    /** @brief element of vector
     *  @dot
        digraph G {
            node [shape=record, fontname=Helvetica, fontsize=10];
            a [ label="data0" URL="\ref B"];
            b [ label="data1" URL="\ref B"];
            c [ label="data2" URL="\ref C"];
            a->b [ arrowhead="open", style="dashed" ];
            b -> c [ arrowhead="open", style="dashed" ];
        }
        @enddot
     * */
    T data[3];


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________


};

/* Exported functions --------------------------------------------------------*/

#endif /* __TYPE_MATRIX_INC_VECTOR3_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
