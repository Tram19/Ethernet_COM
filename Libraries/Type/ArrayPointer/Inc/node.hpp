 
/** 
  ******************************************************************************
  * @file    node.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Oct 25, 2022
  * @brief   This file contains all the functions prototypes for the node.cpp 
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

#ifndef __TYPE_ELEMENT_NODE_HPP__
#define __TYPE_ELEMENT_NODE_HPP__

/* Includes ------------------------------------------------------------------*/
#include "Type/type_include.hpp"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

class Type::Node {
public:

    /// TYPEDEF


    /// PARAM
    Node* ptrNext;
    void* ptrData;


    /// OPERATION

    /** @brief operation them vao trong node */
    Node* operator += (void* ptrdata);

    /** @brief operator lay gia tri tu index */
    void* operator [] (uint32_t index);
    //void* operator [] (uint32_t index);

    /** @brief operator */


    /// FUNCTION

    /** @brief ham them vao trong node
     * */
    StatusEnum Append(void* ptrdata);

    /** @brief ham insert vao vi tri bat ky
     * */
    StatusEnum Insert(uint32_t index, void* ptrdata);

    /** @brief ham lay gia tri dau tien va xoa
     * */
    void* TakeFist(void);
    void* TakeLast(void);
    void* TakeAt(uint32_t index);

    /** @brif ham xoa gia tri trong node
     * */
    StatusEnum RemoveAt(uint32_t index);
    StatusEnum RemoveFist(void);
    StatusEnum RemoveLast(void);


    /** @brief
     * */

    /// PRIVATE

private:


    /// TYPEDEF


    /// OPERATOR


    /// FUNCTION

    /** @brief ham tim dia chi co vi tri index trong node
     * */
    StatusEnum Search(uint32_t index, uint32_t* pptr);

    /** @brief ham tim dia chi trong tron node
     * */
    StatusEnum SearchEmpty(uint32_t* pptr);

    /** @brief ham tim vi tri cuoi co gia tri cung trong node
     * */
    StatusEnum SearchLast(uint32_t* pptr);


};

/* Exported functions --------------------------------------------------------*/

#endif /* __TYPE_ELEMENT_NODE_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
