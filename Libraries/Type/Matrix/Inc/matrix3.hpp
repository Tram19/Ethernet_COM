 
/** 
  ******************************************************************************
  * @file    matrix3.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Oct 22, 2023
  * @brief   This file contains all the functions prototypes for the matrix3.cpp 
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

#ifndef __TYPE_MATRIX_INC_MATRIX3_HPP__
#define __TYPE_MATRIX_INC_MATRIX3_HPP__

/* Includes ------------------------------------------------------------------*/
#include "Type/Matrix/Inc/vector3.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/** @brief class for matrix 3x3
 * [a.x, a.y, a.z] vector a
 * |b.x, b.y, b.z| vector b
 * [c.x, c.y, c.z] vector c
 *                3x3
 * */
template <class T>
class Type::Matrix3 {

public:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________



    /// OPERATOR _______________________________________________________________

    /** @brief operator khai bao gia tri gan data bang 0
     * */
    Matrix3()
    {
        v[0].Equal(0);
        v[1].Equal(0);
        v[2].Equal(0);
    }

    /** @brief operator khai bao gia tri gan data bang x,y,z
     * */
    Matrix3(const Vector3<T> v0, const Vector3<T> v1, const Vector3<T> v2)
    {
        v[0] = v0;
        v[1] = v1;
        v[2] = v2;
    }

    /** @brief operator khai bao gia tri gan data bang val
         * */
    Matrix3(const T val)
    {
        v[0].Equal(val);
        v[1].Equal(val);
        v[2].Equal(val);
    }

    /** @brief operator doc gia tri cua phan tu
     * */
    const Vector3<T> &operator [] (const int i) const
    {
        if(i<3) return v[i];
        return v[2];
    }

    /** @brief operation gan gia tri cho cac phan tu
     * */
    Vector3<T> &operator [] (const int i)
    {
        if(i<3) return v[i];
        /** @TODO xu ly khi gia tri lon hon mang*/
        return v[2];
    }

    /** @brief phep = 2 matrix
     * */
    Matrix3 &operator = (const Matrix3 m)
    {
        v[0] = m[0];
        v[1] = m[1];
        v[2] = m[2];

        return *this;
    }

    /** @brief phep gan matrix voi gia tri
     * */
    Matrix3 Equal(const T val)
    {
        v[0].Equal(val);
        v[1].Equal(val);
        v[2].Equal(val);

        return *this;
    }

    /** @brief phep dua matrix ve matrix don vi
     * */
    Matrix3 Identity()
    {
        v[0][0] = 1; v[0][1] = 0; v[0][2] = 0;
        v[1][0] = 0; v[1][1] = 1; v[1][2] = 0;
        v[2][0] = 0; v[2][1] = 0; v[2][2] = 1;

        return *this;
    }

    /** @brief phep cong 2 matrix
     * */
    Matrix3 operator + (const Matrix3 m)
    {
        return Matrix3(v[0] + m[0], v[1] + m[1], v[2] + m[2]);
    }

    /** @brief phep cong don matrix
     * */
    Matrix3 &operator += (const Matrix3 m)
    {
        v[0] += m[0];
        v[1] += m[1];
        v[2] += m[2];

        return *this;
    }

    /** @brief phep tru 2 matrix
     * */
    Matrix3 operator - (const Matrix3 m)
    {
        return Matrix3(v[0] - m[0], v[1] - m[1], v[2] - m[2]);
    }

    /** @brief phep tru don matrix
     * */
    Matrix3 &operator -= (const Matrix3 m)
    {
        v[0] -= m[0];
        v[1] -= m[1];
        v[2] -= m[2];

        return *this;
    }

    /** @brief phep nhan vector voi hang so
     * */
    Matrix3 operator * (const T k){
        return Matrix3(v[0]*k, v[1]*k, v[2]*k);
    }

    /** @brief phep nhan don vector
     * */
    Matrix3 &operator *= (const T k)
    {
        v[0] *= k;
        v[1] *= k;
        v[2] *= k;

        return *this;
    }

    /** @brief phep chia matrix voi hang so
     * */
    Matrix3 operator / (const T k){
        return Matrix3(v[0]/k, v[1]/k, v[2]/k);
    }

    /** @brief phep chia don matrix
     * */
    Matrix3 &operator /= (const T k)
    {
        v[0] /= k;
        v[1] /= k;
        v[2] /= k;

        return *this;
    }

    /** @brief phep nhan voi vector
     *  [x]   [a1 a2 a3]   [x]
     *  |y| = |b1 b2 b3| * |y|
     *  [z]   [c1 c2 c3]   [z]
     * */
    Vector3<T> operator * (Vector3<T> vt){
        return Vector3<T>(v[0]*vt, v[1]*vt, v[2]*vt);
    }

    /** @brief phep nhan voi matrix
     * */
    Matrix3 operator * (Matrix3 m){
        return Matrix3(Vector3<T>(v[0][0]*m[0][0] + v[0][1]*m[1][0] + v[0][2]*m[2][0],
                                  v[0][0]*m[0][1] + v[0][1]*m[1][1] + v[0][2]*m[2][1],
                                  v[0][0]*m[0][2] + v[0][1]*m[1][2] + v[0][2]*m[2][2]),

                       Vector3<T>(v[1][0]*m[0][0] + v[1][1]*m[1][0] + v[1][2]*m[2][0],
                                  v[1][0]*m[0][1] + v[1][1]*m[1][1] + v[1][2]*m[2][1],
                                  v[1][0]*m[0][2] + v[1][1]*m[1][2] + v[1][2]*m[2][2]),

                       Vector3<T>(v[2][0]*m[0][0] + v[2][1]*m[1][0] + v[2][2]*m[2][0],
                                  v[2][0]*m[0][1] + v[2][1]*m[1][1] + v[2][2]*m[2][1],
                                  v[2][0]*m[0][2] + v[2][1]*m[1][2] + v[2][2]*m[2][2])
                );
    }

    /** @brief phep nhan don 2 matrix
     * */
    Matrix3 &operator *= (Matrix3 m){
        /// row 0
        const T x0 = v[0][0]*m[0][0] + v[0][1]*m[1][0] + v[0][2]*m[2][0];
        const T y0 = v[0][0]*m[0][1] + v[0][1]*m[1][1] + v[0][2]*m[2][1];
        const T z0 = v[0][0]*m[0][2] + v[0][1]*m[1][2] + v[0][2]*m[2][2];

        /// row 1
        const T x1 = v[1][0]*m[0][0] + v[1][1]*m[1][0] + v[1][2]*m[2][0];
        const T y1 = v[1][0]*m[0][1] + v[1][1]*m[1][1] + v[1][2]*m[2][1];
        const T z1 = v[1][0]*m[0][2] + v[1][1]*m[1][2] + v[1][2]*m[2][2];

        /// row 2
        const T x2 = v[2][0]*m[0][0] + v[2][1]*m[1][0] + v[2][2]*m[2][0];
        const T y2 = v[2][0]*m[0][1] + v[2][1]*m[1][1] + v[2][2]*m[2][1];
        const T z2 = v[2][0]*m[0][2] + v[2][1]*m[1][2] + v[2][2]*m[2][2];

        /// gan vao matrix
        v[0][0] = x0; v[0][1] = y0; v[0][2] = z0;
        v[1][0] = x1; v[1][1] = y1; v[1][2] = z1;
        v[2][0] = x2; v[2][1] = y2; v[2][2] = z2;

        return *this;
    }

    /** @brief ham tra ve coloum0 cua matrix
     * */
    Vector3<T> col0(void) const
    {
        return Vector3<T>(v[0][0], v[1][0], v[2][0]);
    }

    /** @brief ham tra ve coloum1 cua matrix
     * */
    Vector3<T> col1(void) const
    {
        return Vector3<T>(v[0][1], v[1][1], v[2][1]);
    }

    /** @brief ham tra ve coloum2 cua matrix
     * */
    Vector3<T> col2(void) const
    {
        return Vector3<T>(v[0][2], v[1][2], v[2][2]);
    }

    /** @brief matrix rotary quanh truc z
     * */
    Matrix3 RotaryZ(float angle)
    {
        (*this) *= DefaultRotaryZ(angle);
        return *this;
    }

    /** @brief matrix rotary quanh truc x
     * */
    Matrix3 RotaryX(float angle)
    {
        (*this) *= DefaultRotaryX(angle);
        return *this;
    }

    /** @brief matrix rotary quanh truc y
     * */
    Matrix3 RotaryY(float angle)
    {
        (*this) *= DefaultRotaryY(angle);
        return *this;
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

    /** @brief vector of matrix
     * */
    Vector3<T> v[3];

    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________

    /** @brief matrix rotary z
     * */
    Matrix3 DefaultRotaryZ(float angle)
    {
        T cs = cosf(angle);
        T sn = sinf(angle);

        if(abs(cs) < 0.000001f) cs = 0;
        if(abs(sn) < 0.000001f) sn = 0;

        return Matrix3(Vector3<T>(cs, -sn, 0),
                       Vector3<T>(sn,  cs, 0),
                       Vector3<T>(0,    0, 1));
    }

    /** @brief matrix rotary x
     * */
    Matrix3 DefaultRotaryX(float angle)
    {
        T cs = cosf(angle);
        T sn = sinf(angle);

        if(abs(cs) < 0.000001f) cs = 0;
        if(abs(sn) < 0.000001f) sn = 0;

        return Matrix3(Vector3<T>(1,  0,   0),
                       Vector3<T>(0, cs, -sn),
                       Vector3<T>(0, sn,  cs));
    }

    /** @brief matrix rotary x
     * */
    Matrix3 DefaultRotaryY(float angle)
    {
        T cs = cosf(angle);
        T sn = sinf(angle);

        if(abs(cs) < 0.000001f) cs = 0;
        if(abs(sn) < 0.000001f) sn = 0;

        return Matrix3(Vector3<T>( cs, 0, sn),
                       Vector3<T>(  0, 1,  0),
                       Vector3<T>(-sn, 0, cs));
    }

};

/* Exported functions --------------------------------------------------------*/

#endif /* __TYPE_MATRIX_INC_MATRIX3_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
