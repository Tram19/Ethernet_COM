 
/** 
  ******************************************************************************
  * @file    node.cpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Oct 25, 2022
  * @brief   
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2022 Gremsy.  
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

#include "Type/ArrayPointer/Inc/node.hpp"
#include "RTOS/rtos.hpp"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/


/// OPERATOR ___________________________________________________________________
/** @brief operator lay gia tri tu index */
void* Type::Node::operator [] (uint32_t index)
{
    uint32_t    pptr       = (uint32_t)this;
    StatusEnum  ret        = Search(index, &pptr);
    Node*       ptrnode    = (Node*)pptr;


    if(ret == OK)
    {
        return ptrnode->ptrData;
    }

    return NULL;
}

/// FUNCTION ___________________________________________________________________
/** @brief ham tim dia chi trong
 * */
StatusEnum Type::Node::Search(uint32_t index, uint32_t* pptr)
{
    Node* ptrnode = (Node*)*pptr;

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

/** @brief ham tim vi tri trong trong node
 * */
StatusEnum Type::Node::SearchEmpty(uint32_t* pptr)
{
    Node* ptrnode = (Node*)*pptr;

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
StatusEnum Type::Node::SearchLast(uint32_t* pptr)
{
    Node* ptrnode = (Node*)*pptr;

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

/** @brif ham xoa gia tri trong node
 * */
StatusEnum Type::Node::RemoveFist(void)
{
    if(ptrNext != 0)
    {
        /// save dia chi truoc
        Node* ptrnextnext = ptrNext->ptrNext;
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

StatusEnum Type::Node::RemoveLast(void)
{
    uint32_t    pptr       = (uint32_t)this;
    StatusEnum  ret        = SearchLast(&pptr);


    if(ret == OK)
    {
        Node* ptrnode = (Node*)pptr;

        /// REMOVE
        ptrnode->ptrData = 0;
        g_free(ptrnode->ptrNext);
        ptrnode->ptrNext = 0;
    }

    return ret;
}

StatusEnum Type::Node::RemoveAt(uint32_t index)
{
    uint32_t    pptr = (uint32_t)this;
    StatusEnum  ret  = Search(index, &pptr);

    if(ret == OK)
    {
        Node* ptrnode = (Node*)pptr;

        if(ptrnode->ptrNext->ptrNext != 0)
        {
            /// save dia chi truoc
            Node* ptrnextnext = ptrnode->ptrNext->ptrNext;
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

/** @brief ham lay gia tri dau tien va xoa
 * */
void* Type::Node::TakeFist(void)
{
    void* ptrret = ptrData;

    /// remove gia tri dau tien
    RemoveFist();

    return ptrret;
}

void* Type::Node::TakeLast(void)
{
    uint32_t    pptr           = (uint32_t)this;
    StatusEnum  ret            = SearchLast(&pptr);
    void*       ptrnextdata    = NULL;

    if(ret == OK)
    {
        Node* ptrnode = (Node*)pptr;
        ptrnextdata   = ptrnode->ptrData;

        /// REMOVE
        ptrnode->ptrData = 0;
        g_free(ptrnode->ptrNext);
        ptrnode->ptrNext = 0;
    }

    return ptrnextdata;
}

void* Type::Node::TakeAt(uint32_t index)
{
    uint32_t    pptr = (uint32_t)this;
    StatusEnum  ret  = Search(index, &pptr);
    void*       ptrdata = NULL;


    if(ret == OK)
    {
        Node* ptrnode = (Node*)pptr;

        /// GETDATA
        ptrdata = ptrnode->ptrData;

        if(ptrnode->ptrNext->ptrNext != 0)
        {
            /// save dia chi truoc
            Node* ptrnextnext = ptrnode->ptrNext->ptrNext;
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

/** @brief ham them vao trong node
 * */
StatusEnum Type::Node::Append(void* ptrdata)
{
    uint32_t    pptr       = (uint32_t)this;
    StatusEnum  ret        = SearchEmpty(&pptr);
    Node*       ptrnode    = (Node*)pptr;

    if(ret == OK)
    {
        ptrnode->ptrData = ptrdata;
        ptrnode->ptrNext = (Node*)g_calloc(sizeof(Node));
    }

    return ret;
}

/** @brief ham insert vao vi tri bat ky
 * */
StatusEnum Type::Node::Insert(uint32_t index, void* ptrdata)
{
    uint32_t pptr = (uint32_t)this;
    StatusEnum ret = OK;

    /// start node
    if(index == 0)
    {
        if(ptrNext != 0)
        {
            Node* ptrnext1 = ptrNext;
            void* ptrdata1 = ptrData;

            /// INSERT
            ptrNext = (Node*)g_calloc(sizeof(Node));
            ptrData = ptrdata;

            ptrNext->ptrNext = ptrnext1;
            ptrNext->ptrData = ptrdata1;
        }
        else
        {
            /// INSERT
            ptrNext = (Node*)g_calloc(sizeof(Node));
            ptrData = ptrdata;
        }
    }
    else
    {
        ret = Search(index - 1, &pptr);

        if(ret == OK)
        {
            Node* ptrnode = (Node*)pptr;

            if(ptrnode->ptrNext != 0)   /// vi tri cua index
            {
                /// Save dia chi truoc
                Node* ptrnext = ptrnode->ptrNext;

                /// INSERT
                ptrnode->ptrNext = (Node*)g_calloc(sizeof(Node));

                ptrnode->ptrNext->ptrNext = ptrnext;
                ptrnode->ptrNext->ptrData = ptrdata;

            }
            /// end node
            else
            {
                /// INSERT
                ptrnode->ptrNext = (Node*)g_calloc(sizeof(Node));
                ptrnode->ptrData = ptrdata;
            }
        }
    }

    return ret;
}

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
