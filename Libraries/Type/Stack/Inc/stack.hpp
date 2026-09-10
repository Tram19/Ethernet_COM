 
/** 
  ******************************************************************************
  * @file    stack.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Dec 27, 2024
  * @brief   This file contains all the functions prototypes for the stack.cpp 
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

#ifndef __TYPE_STACK_INC_STACK_HPP__
#define __TYPE_STACK_INC_STACK_HPP__

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/

/* Exported class ------------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

template <typename T, size_t Size>
class Stack {
private:
    T arr[Size];
    int top;
    int capacity;

public:
    // Constructor: Khởi tạo stack
    Stack() {
        capacity = Size;
        top = -1; // Stack rỗng ban đầu
    }

    // Destructor: Giải phóng bộ nhớ
    ~Stack() = default;

    // Hàm thêm phần tử vào stack
    bool push(T* value) {
        if (isFull()) {
            return false;
        }
        memcpy(&arr[++top],value,sizeof(T));
        return true;
    }

    bool pop() {
        if (isEmpty()) {
            return false;
        }
        --top;
        return true;
    }

    // Hàm trả về phần tử trên cùng của stack
    T* peek() {
        if (isEmpty()) {
            return nullptr; // Giá trị mặc định khi stack rỗng
        }
        return &arr[top];
    }

    // Kiểm tra stack có rỗng không
    bool isEmpty() {
        return top == -1;
    }

    // Kiểm tra stack có đầy không
    bool isFull() {
        return top == capacity - 1;
    }

};
#endif /* __TYPE_STACK_INC_STACK_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
