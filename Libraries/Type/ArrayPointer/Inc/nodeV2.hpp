 
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

#ifndef __TYPE_ELEMENT_NODE_V2_HPP__
#define __TYPE_ELEMENT_NODE_V2_HPP__

/* Includes ------------------------------------------------------------------*/
#include "Type/type_include.hpp"
#include "FreeRTOS.h"

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

template <class T>
class Type::NodeV2 {
public:

    /// TYPEDEF


    /// PARAM
    NodeV2* ptrNext;
    T* ptrData;

    /// OPERATION

    /** @brief operation them vao trong node */
    NodeV2* operator += (T* ptrdata);

    /** @brief operator lay gia tri tu index */
    T* operator [] (uint32_t index)
    {
        uint32_t    pptr       = (uint32_t)this;
        StatusEnum  ret        = Search(index, &pptr);
        NodeV2*     ptrnode    = (NodeV2*)pptr;


        if(ret == OK)
        {
            return ptrnode->ptrData;
        }

        return NULL;
    }
    //void* operator [] (uint32_t index);

    /** @brief operator */


    /// FUNCTION

    /** @brief ham them vao trong node
     * */
    StatusEnum Append(T* ptrdata)
    {
        uint32_t    pptr       = (uint32_t)this;
        StatusEnum  ret        = SearchEmpty(&pptr);
        NodeV2*       ptrnode    = (NodeV2*)pptr;

        if(ret == OK)
        {
            ptrnode->ptrData = ptrdata;
            ptrnode->ptrNext = (NodeV2*)pvPortMalloc(sizeof(NodeV2));
            ptrnode->ptrNext->ptrData = 0;
            ptrnode->ptrNext->ptrNext = 0;
        }

        return ret;
    }
    /** @brief ham insert vao vi tri bat ky
     * */
    StatusEnum Insert(uint32_t index, T* ptrdata)
    {
        uint32_t pptr = (uint32_t)this;
        StatusEnum ret = OK;

        /// start node
        if(index == 0)
        {
            if(ptrNext != 0)
            {
                NodeV2* ptrnext1 = ptrNext;
                void* ptrdata1 = ptrData;

                /// INSERT
                ptrNext = (NodeV2*)pvPortMalloc(sizeof(NodeV2));
                ptrData = ptrdata;

                ptrNext->ptrNext = ptrnext1;
                ptrNext->ptrData = ptrdata1;
            }
            else
            {
                /// INSERT
                ptrNext = (NodeV2*)pvPortMalloc(sizeof(NodeV2));
                ptrData = ptrdata;
                ptrNext->ptrData = 0;
                ptrNext->ptrNext = 0;
            }
        }
        else
        {
            ret = Search(index - 1, &pptr);

            if(ret == OK)
            {
                NodeV2* ptrnode = (NodeV2*)pptr;

                if(ptrnode->ptrNext != 0)   /// vi tri cua index
                {
                    /// Save dia chi truoc
                    NodeV2* ptrnext = ptrnode->ptrNext;

                    /// INSERT
                    ptrnode->ptrNext = (NodeV2*)pvPortMalloc(sizeof(NodeV2));

                    ptrnode->ptrNext->ptrNext = ptrnext;
                    ptrnode->ptrNext->ptrData = ptrdata;

                }
                /// end node
                else
                {
                    /// INSERT
                    ptrnode->ptrNext = (NodeV2*)pvPortMalloc(sizeof(NodeV2));
                    ptrnode->ptrData = ptrdata;
                    ptrnode->ptrNext->ptrData = 0;
                    ptrnode->ptrNext->ptrNext = 0;
                }
            }
        }

        return ret;
    }

    /** @brief ham lay gia tri dau tien va xoa
     * */
    T* TakeFist(void)
    {
        void* ptrret = ptrData;

        /// remove gia tri dau tien
        RemoveFist();

        return ptrret;
    }

    T* TakeLast(void)
    {
        uint32_t    pptr           = (uint32_t)this;
        StatusEnum  ret            = SearchLast(&pptr);
        void*       ptrnextdata    = NULL;

        if(ret == OK)
        {
            NodeV2* ptrnode = (NodeV2*)pptr;
            ptrnextdata   = ptrnode->ptrData;

            /// REMOVE
            ptrnode->ptrData = 0;
            g_free(ptrnode->ptrNext);
            ptrnode->ptrNext = 0;
        }

        return ptrnextdata;
    }

    T* TakeAt(uint32_t index)
    {
        uint32_t    pptr = (uint32_t)this;
        StatusEnum  ret  = Search(index, &pptr);
        void*       ptrdata = NULL;


        if(ret == OK)
        {
            NodeV2* ptrnode = (NodeV2*)pptr;

            /// GETDATA
            ptrdata = ptrnode->ptrData;

            if(ptrnode->ptrNext->ptrNext != 0)
            {
                /// save dia chi truoc
                NodeV2* ptrnextnext = ptrnode->ptrNext->ptrNext;
                void* ptrnextdata = ptrnode->ptrNext->ptrData;

                /// REMOVE dia chi hien tai
                ptrnode->ptrData = 0;
                g_free(ptrnode->ptrNext);
                ptrnode->ptrNext = 0;

                /// Add lai dia chi noi tiep
                ptrnode->ptrData = ptrnextdata;
                ptrnode->ptrNext = ptrnextnext;
            }
            else
            {
                /// REMOVE dia chi hien tai
                ptrnode->ptrData = 0;
                g_free(ptrnode->ptrNext);
                ptrnode->ptrNext = 0;
            }
        }

        return ptrdata;
    }

    /** @brif ham xoa gia tri trong node
     * */
    StatusEnum RemoveAt(uint32_t index)
    {
        uint32_t    pptr = (uint32_t)this;
        StatusEnum  ret  = Search(index, &pptr);

        if(ret == OK)
        {
            NodeV2* ptrnode = (NodeV2*)pptr;

            if(ptrnode->ptrNext->ptrNext != 0)
            {
                /// save dia chi truoc
                NodeV2* ptrnextnext = ptrnode->ptrNext->ptrNext;
                void* ptrnextdata = ptrnode->ptrNext->ptrData;

                /// REMOVE dia chi hien tai
                ptrnode->ptrData = 0;
                g_free(ptrnode->ptrNext);
                ptrnode->ptrNext = 0;

                /// Add lai dia chi noi tiep
                ptrnode->ptrData = ptrnextdata;
                ptrnode->ptrNext = ptrnextnext;
            }
            else
            {
                /// REMOVE dia chi hien tai
                ptrnode->ptrData = 0;
                g_free(ptrnode->ptrNext);
                ptrnode->ptrNext = 0;
            }
        }

        return ret;
    }

    StatusEnum RemoveFist(void)
    {
        if(ptrNext != 0)
        {
            /// save dia chi truoc
            NodeV2* ptrnextnext = ptrNext->ptrNext;
            void* ptrnextdata = ptrNext->ptrData;

            /// REMOVE
            ptrData = 0;
            g_free(ptrNext);
            ptrNext = 0;

            ///
            ptrNext = ptrnextnext;
            ptrData = ptrnextdata;

            return OK;
        }
        else
        {
            return ERROR_NOT_EXIST;
        }
    }

    StatusEnum RemoveLast(void)
    {
        uint32_t    pptr       = (uint32_t)this;
        StatusEnum  ret        = SearchLast(&pptr);


        if(ret == OK)
        {
            NodeV2* ptrnode = (NodeV2*)pptr;

            /// REMOVE
            ptrnode->ptrData = 0;
            g_free(ptrnode->ptrNext);
            ptrnode->ptrNext = 0;
        }

        return ret;
    }


    /** @brief
     * */

    /// PRIVATE

private:


    /// TYPEDEF


    /// OPERATOR


    /// FUNCTION

    /** @brief ham tim dia chi co vi tri index trong node
     * */
    StatusEnum Search(uint32_t index, uint32_t* pptr)
    {
        NodeV2* ptrnode = (NodeV2*)*pptr;

        if(index > 0)
        {
            if(ptrnode->ptrNext == 0)
            {
                return ERROR_NOT_EXIST;
            }
            else
            {
                index = index - 1;
                *pptr = (uint32_t)ptrnode->ptrNext;
                return Search(index, pptr);
            }
        }
        else if(index == 0)
        {
            if(ptrnode->ptrNext == 0)
            {
                return ERROR_NOT_EXIST;
            }
            else
            {
                return OK;
            }
        }


        return ERROR_NOT_EXIST;
    }

    /** @brief ham tim dia chi trong tron node
     * */
    StatusEnum SearchEmpty(uint32_t* pptr)
    {
        NodeV2* ptrnode = (NodeV2*)*pptr;

        if(ptrnode->ptrNext == 0)
        {
            return OK;
        }
        else
        {
            *pptr = (uint32_t)ptrnode->ptrNext;
            return SearchEmpty(pptr);
        }
    }

    /** @brief ham tim vi tri cuoi co gia tri cung trong node
     * */
    StatusEnum SearchLast(uint32_t* pptr)
    {
        NodeV2* ptrnode = (NodeV2*)*pptr;

        if(ptrnode->ptrNext == 0)
        {
            return ERROR_NOT_EXIST;
        }
        else if(ptrnode->ptrNext->ptrNext == 0)
        {
            return OK;
        }
        else
        {
            *pptr = (uint32_t)ptrnode->ptrNext;
            return SearchLast(pptr);
        }

        return OK;
    }


};

/* Exported functions --------------------------------------------------------*/

#endif /* __TYPE_ELEMENT_NODE_V2_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
