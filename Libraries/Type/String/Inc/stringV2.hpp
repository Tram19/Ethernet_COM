 
/** 
  ******************************************************************************
  * @file    stringV2.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Nov 8, 2023
  * @brief   This file contains all the functions prototypes for the stringV2.cpp 
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

#ifndef __TYPE_STRING_INC_STRINGV2_HPP__
#define __TYPE_STRING_INC_STRINGV2_HPP__

/* Includes ------------------------------------------------------------------*/

#include "Type/type_include.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/// @brief dinh nghia class for string
class Type::StringV2 {

public:

    /// TYPEDEF ________________________________________________________________


    /// PARRAM _________________________________________________________________

    int     length;
    char*   array;

    /// OPERATOR _______________________________________________________________

    /** @brief khai bao gia tri null cho string
     * */
    StringV2()
    {
        length = 0;
        array = NULL;
    }

    /** @brief khai bao gia tri const char cho string
     * */
    StringV2(const char* str)
    {
        length = strlen(str);
        array = (char*)str;
    }

    /** @brief khai bao gia tri char, length cho str
     * */
    StringV2(char* str, int size)
    {
        length = size;
        array  = str;
    }

//    /***/
//    StringV2(StringV2 &c)
//    {
//        length = c.length;
//        array = c.array;
//    }

    ~StringV2()
    {
        array = 0;
        length = 0;
    }

    /** @brief doc gia tri cua phan tu
     * */
    char  operator [] (const int i) const
    {
        if(i < length)
        {
            return array[i];
        }

        return 0;
    }

    /** @brief ghi gia tri vao dia chi
     * */
    char &operator [] (int i)
    {
        if(i < length)
        {
            return array[i];
        }

        return array[0];
    }

    /** @brief phep bang hai chuoi
     * */
    StringV2 &operator = (const StringV2 str){

        if(&str != this)
        {
            /// giai phong vung nho neu co
            Free(array);

            /// gan gia tri
            length = str.length;
            array  = str.array;
        }

        return *this;
    }

    /** @brief phep bang const char*
     * */
    StringV2 &operator = (const char* buff)
    {
        /// giai phong vung nho neu co
        Free(array);

        /// gan gia tri
        length = strlen(buff);
        array  = (char*)buff;

        return *this;
    }

    /** @brief phep cong string
     * */
    StringV2 operator + (const StringV2 str){
        int   size  = length + str.length;
        char* buff  = (char*)Malloc(size);

        /// lay gia tri tu mang noi tai
        for(int i = 0; i < length; i++)
        {
            buff[i] = array[i];
        }

        /// lay gia tri tu mang cong vao
        for(int i = 0; i < str.length; i++)
        {
            buff[length + i] = str.array[i];
        }

        return StringV2(buff, size);
    }

    /** @brief phep cong const char
     * */
    StringV2 operator + (const char* str){
        int   str_len   = strlen(str);
        int   size      = length + str_len;
        char* buff      = (char*)Malloc(size);

        /// lay gia tri tu mang noi tai
        for(int i = 0; i < length; i++)
        {
            buff[i] = array[i];
        }

        /// lay gia tri tu mang cong vao
        for(int i = 0; i < str_len; i++)
        {
            buff[length + i] = str[i];
        }

        return StringV2(buff, size);
    }

    /** @brief phep cong don 2 string
     * */
    StringV2 &operator += (StringV2 str){
        int   size  = length + str.length;
        char* buff  = (char*)Malloc(size);

        /// lay gia tri tu mang noi tai
        for(int i = 0; i < length; i++)
        {
            buff[i] = array[i];
        }

        /// lay gia tri tu mang cong vao
        for(int i = 0; i < str.length; i++)
        {
            buff[length + i] = str.array[i];
        }

        /// giai phong vung nho
        Free(array);

        /// cap phat vung nho
        array = buff;
        length = size;

        return *this;
    }

    /** @brief phep cong don 2 string
     * */
    StringV2 &operator += (const char* str){
        int   str_len   = strlen(str);
        int   size      = length + str_len;
        char* buff      = (char*)Malloc(size);

        /// lay gia tri tu mang noi tai
        for(int i = 0; i < length; i++)
        {
            buff[i] = array[i];
        }

        /// lay gia tri tu mang cong vao
        for(int i = 0; i < str_len; i++)
        {
            buff[length + i] = str[i];
        }

        /// giai phong vung nho
        Free(array);

        /// cap phat vung nho
        array = buff;
        length = size;

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


    /// OPERATOR _______________________________________________________________


    /// FUNCTION _______________________________________________________________
    /** @brief ham giai phong vung nho
     * */
    void Free(void* ptr);

    /** @brief ham cao phat vung nho
     * */
    void* Malloc(int size);

};


/* Exported functions --------------------------------------------------------*/

#endif /* __TYPE_STRING_INC_STRINGV2_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
