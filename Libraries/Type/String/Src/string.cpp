/** 
  ******************************************************************************
  * @file    String.cpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    16 thg 7, 2021
  * @brief   
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2018 Gremsy.  
  * All rights reserved.
  *
  * The information contained herein is confidential
  * property of Company. The use, copying, transfer or 
  * disclosure of such information is prohibited except
  * by express written agreement with Company.
  *
  ******************************************************************************
*/ 

/* Includes ------------------------------------------------------------------*/
#include "Type/String/Inc/string.hpp"
#include "RTOS/rtos.hpp"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/


/// PRIVATE ____________________________________________________________________
/** @brief ham xoa vung nho cua string
 *  @param none
 *  @return trang thai qua trinh free
 * */
StatusEnum Type::String::Free(void)
{
    if(g_is_var_heap(array))
    {
        g_free(array);
        array = 0;
        length = 0;

        return OK;
    }

    return ERROR_NOT_EXIST;
}

/** @brief ham cap phat vung nho cho string
 *  @param[in] size do lon cua mang string
 *  @return tran thai cap phat vung nho
 * */
StatusEnum Type::String::Malloc(int size)
{
    /// giai phong vung nho neu co
    Free();

    /// cap phat vung nho
    array = (char*)g_malloc(size);

    return OK;
}

/// PUBLIC _____________________________________________________________________

/** phep so sanh */
bool Type::String::operator != (const char* str) const
{
    int lenStr = strlen(str);
    int i;

    if(lenStr != length)
    {
        return true;
    }

    for(i = 0; i < length; i++)
    {
        if(array[i] != str[i])
        {
            return true;
        }
    }

    return false;
}

bool Type::String::operator != (const String &v) const
{
    int i;

    if(v.length != length)
    {
        return true;
    }

    for(i = 0; i < length; i++)
    {
        if(array[i] != v.array[i])
        {
            return true;
        }
    }

    return false;
}

bool Type::String::operator == (const char* str) const
{
    int lenStr = strlen(str);
    int i;

    if(lenStr != length)
    {
        return false;
    }

    for(i = 0; i < length; i++)
    {
        if(array[i] != str[i])
        {
            return false;
        }
    }

    return true;
}

bool Type::String::operator == (const String &v) const
{
    int i;

    if(v.length != length)
    {
        return false;
    }

    for(i = 0; i < length; i++)
    {
        if(array[i] != v.array[i])
        {
            return false;
        }
    }

    return true;
}

/** phep bang */
Type::String &Type::String::operator =(const String &v)
{
    /** SELF ASSIGNMENT CHECK */
    if(this != &v)
    {
        /// cap phat vung nho cho string
        Malloc(v.length);

        /// di chuyen mang
        /// kiem tra mang da duoc khai bao truoc do va trung voi dia chi cap phat hien tai
        if(array != v.array)
        {
            memcpy((uint8_t*)array, (uint8_t*)v.array, v.length);
        }

        /// cap nhat len
        length  = v.length;
    }

    return *this;
}

Type::String &Type::String::operator =(const char* str)
{
    if(str != NULL)
    {
        int len = strlen(str);

        /// cap phat vung nho cho string
        Malloc(len);

        /// di chuyen mang
        memcpy((uint8_t*)array, (uint8_t*)str, len);

        /// cap nhat len
        length  = len;
    }

    return *this;
}

/** phep cong chuoi */
Type::String Type::String::operator +(const char* str)
{
    Type::String strTemp;
    int lenStr = strlen(str);
    int lenAll = lenStr + length;

    /// cap phat vung nho
    strTemp.array = (char*)g_malloc(lenAll);
    strTemp.length = lenAll;

    /// dua gia tri mang vao mang tam
    memcpy((uint8_t*)strTemp.array, (uint8_t*)array, length);
    memcpy((uint8_t*)&strTemp.array[length], (uint8_t*)str, lenStr);

    /// dua theo nguyen ly cua gia tri temp thi khi giai phong vung nho
    /// mang van con ton tai trong khoang thoi gian chua co mang nao duoc malloc tiep theo
    g_free(strTemp.array);

    return strTemp;
}

Type::String Type::String::operator +(const String &v)
{
    Type::String strTemp;
    int lenAll = v.length + length;

    /// cap phat vung nho
    strTemp.array = (char*)g_malloc(lenAll);
    strTemp.length = lenAll;

    /// dua gia tri mang vao mang tam
    memcpy((uint8_t*)strTemp.array, (uint8_t*)array, length);
    memcpy((uint8_t*)&strTemp.array[length], (uint8_t*)v.array, v.length);

    /// dua theo nguyen ly cua gia tri temp thi khi giai phong vung nho
    /// mang van con ton tai trong khoang thoi gian chua co mang nao duoc malloc tiep theo
    g_free(strTemp.array);

    return strTemp;
}

/** phep cong don */
Type::String &Type::String::operator +=(const String &v)
{
    int lenOld = length;
    char* arrayOld = (char*)g_malloc(lenOld);
    int lenAll = lenOld + v.length;

    /// Copy du lieu hien tai ra mang temp
    memcpy(arrayOld, array, lenOld);

    /// cap phat vung nho moi co do dai lon hon do dai mang cu
    Malloc(lenAll);

    /// dua gia tri mang vao
    memcpy(array, arrayOld, lenOld);
    memcpy(&array[lenOld], v.array, v.length);

    /// input len
    length = lenAll;

    /// giai phong vung nho tam
    g_free(arrayOld);

    return *this;

}

Type::String &Type::String::operator +=(const char* str)
{
    int lenOld = length;
    int lenStr = strlen(str);
    char* arrayOld = (char*)g_malloc(lenOld);
    int lenAll = lenOld + lenStr;

    /// Copy du lieu hien tai ra mang temp
    memcpy((uint8_t*)arrayOld, (uint8_t*)array, lenOld);

    /// cap phat vung nho moi co do dai lon hon do dai mang cu
    Malloc(lenAll);

    /// dua gia tri mang vao
    memcpy((uint8_t*)array, (uint8_t*)arrayOld, lenOld);
    memcpy((uint8_t*)&array[lenOld], (uint8_t*)str, lenStr);

    /// input len
    length = lenAll;

    /// giai phong vung nho tam
    g_free(arrayOld);

    return *this;
}

/** @brief ham remove
 *  @param none
 *  @return none
 * */
StatusEnum Type::String::Remove(void)
{
    return Free();
}

/** @brief ham slip cua string
 *  @param[in] c ky tu de slip
 *  @return chuoi duoc slip
 * */
Type::String Type::String::Slip(const char c)
{
    return *this;
}

/** @brief ham so sanh do dai cua chuoi voi do dai co dinh
 *  @param[in] len do dai can kiem tra
 *  @return true giong
 * */
bool Type::String::Compare(const char* str, int len)
{
    int i;

    /// kiem tra
    if(len > length)
    {
        return false;
    }

    ///
    for(i = 0; i < len; i++)
    {
        if(array[i] != str[i])
        {
            return false;
        }
    }

    return true;
}

bool Type::String::CompareFormEnd(const char* str, int len)
{
    int i;

    if(len < length)
    {
        return false;
    }

    ///
    for(i = 1; i < length; i++)
    {
        if(array[length - i] != str[len - i])
        {
            return false;
        }
    }

    return true;
}

/** @brief ham chuyen doi qua buffer
 *  @param[in] buff dia chi buff
 *  @return trang thai qua trinh chuyen
 * */
StatusEnum Type::String::Convert(char* buff)
{
    int i;

    if(buff == 0)   return ERROR_NOT_EXIST;

    for(i = 0; i < length; i++)
    {
        buff[i] = array[i];
    }

    return OK;
}

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/

