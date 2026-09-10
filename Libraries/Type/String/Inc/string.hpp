/** 
  ******************************************************************************
  * @file    String.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    16 thg 7, 2021
  * @brief   This file contains all the functions prototypes for the | 
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

#ifndef __TYPE_STRING_STRING_HPP_
#define __TYPE_STRING_STRING_HPP_

/* Includes ------------------------------------------------------------------*/

#include "Type/type_include.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

class Type::String
{
public:

    /// TYPEDEF

    /// PARAM
    char*   array;
    int     length;

    /// OPERATION

    /** cai dat ban dau */
    constexpr String()
            :array(0), length(0){}

    String(String &c)
    {
        array = c.array;
        length = c.length;
    }

    constexpr String(const char* str)
        :array((char*)str), length(strlen(str)){}

    constexpr String(const char* str, int len)
        :array((char*)str), length(len){}

    ~String()
    {
        array = 0;
        length = 0;
    }
    /** phep so sanh */
    bool operator != (const char* str) const;
    bool operator != (const String &v) const;

    bool operator == (const char* str) const;
    bool operator == (const String &v) const;

    /** phep bang */
    String &operator =(const String &v);
    String &operator =(const char* str);

    /** phep cong chuoi */
    String operator +(const char* str);
    String operator +(const String &v);

    /** phep cong don */
    String &operator +=(const String &v);
    String &operator +=(const char* str);

    /// FUNCTION

    /** @brief ham kiem tra chuoi co ton tai
     * */
    StatusEnum IsExist(void)
    {
        if(length != 0 && array != 0)
        {
            return OK;
        }

        return ERROR_NOT_EXIST;
    }

    /** @brief ham remove
     *  @param none
     *  @return none
     * */
    StatusEnum Remove(void);

    /** @brief ham slip cua string
     *  @param[in] c ky tu de slip
     *  @return chuoi duoc slip
     * */
    String Slip(const char c);

    /** @brief ham so sanh do dai cua chuoi voi do dai co dinh
     *  @param[in] len do dai can kiem tra
     *  @return true giong
     * */
    bool Compare(const char* str, int len);
    bool CompareFormEnd(const char* str, int len);

    /** @brief ham chuyen doi qua buffer
     *  @param[in] buff dia chi buff
     *  @return trang thai qua trinh chuyen
     * */
    StatusEnum Convert(char* buff);

    /// PRIVATE


private:

    /// TYPEDEF

    /// PARAM

    /// FUNCTION

    /** @brief ham cap phat vung nho cho string
     *  @param[in] size do lon cua mang string
     *  @return tran thai cap phat vung nho
     * */
    StatusEnum Malloc(int size);

    /** @brief ham xoa vung nho cua string
     *  @param none
     *  @return trang thai qua trinh free
     * */
    StatusEnum Free(void);

};

/* Exported functions --------------------------------------------------------*/

#endif /* __TYPE_STRING_STRING_HPP_ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
