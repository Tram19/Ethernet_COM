 
/** 
  ******************************************************************************
  * @file    ringbuffer.hpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Dec 25, 2024
  * @brief   This file contains all the functions prototypes for the ringbuffer.cpp 
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

#ifndef __TYPE_RINGBUFFER_INC_RINGBUFFER_HPP__
#define __TYPE_RINGBUFFER_INC_RINGBUFFER_HPP__

#include <iostream>
#include <stdexcept>
#include <stdint.h>

template <typename T, size_t Size>
class RingBuffer {
private:
    T buffer[Size];       // Con trỏ động dùng làm vòng đệm
    uint8_t head;     // Vị trí đầu của buffer
    uint8_t tail;     // Vị trí cuối của buffer
    uint8_t capacity; // Dung lượng của buffer
    bool isFull;     // Cờ để kiểm tra buffer có đầy hay không

public:
    RingBuffer() :  head(0), tail(0), capacity(10), isFull(false) {}
    RingBuffer(const RingBuffer&) = delete;
    RingBuffer& operator=(const RingBuffer&) = delete;
    // Thêm một phần tử vào buffer
    void push(const T& item) {
        memcpy(&buffer[tail], &item, sizeof(T));

        tail = (tail + 1) % capacity;

        if (isFull) {
            head = (head + 1) % capacity;
        }
        isFull = (head == tail);
    }

    T* pop() {
        if (isEmpty()) {
            return nullptr; // Trả về nullptr nếu buffer rỗng
        }

        T* item = &buffer[head]; // Trỏ tới phần tử tại vị trí head
        head = (head + 1) % capacity;
        isFull = false;

        return item;
    }

    // Kiểm tra buffer có rỗng không
    bool isEmpty() const {
        return (!isFull && (head == tail));
    }

    // Kiểm tra buffer có đầy không
    bool isFullBuffer() const {
        return isFull;
    }

    // Lấy kích thước hiện tại của buffer
    size_t size() const {
        if (isFull) {
            return capacity;
        }

        if (tail >= head) {
            return tail - head;
        } else {
            return capacity - head + tail;
        }
    }

    // Lấy dung lượng tối đa của buffer
    size_t maxCapacity() const {
        return capacity;
    }

    // Xóa toàn bộ buffer
    void clear() {
        head = 0;
        tail = 0;
        isFull = false;
    }
};

#endif /* __TYPE_RINGBUFFER_INC_RINGBUFFER_HPP__ */

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
