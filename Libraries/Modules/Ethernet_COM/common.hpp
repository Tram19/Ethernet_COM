/**
  ******************************************************************************
  * @file           : common.h
  * @brief          : Common project data
  * @author         : MicroTechnics (microtechnics.ru)
  ******************************************************************************
  */

#ifndef ETH_COMMON_H
#define ETH_COMMON_H

#include "main.h"
#include "gremsy.hpp"
#include "gremsy_include.hpp"

/* Includes ------------------------------------------------------------------*/



/* Declarations and definitions ----------------------------------------------*/

#define MAC_ADDRESS_BYTES_NUM                                   6
#define IP_ADDRESS_BYTES_NUM                                    4

//Host TO Network Short, Cuyển đổi một số 16-bit (kiểu short) từ chuẩn của vi điều khiển (Host) sang chuẩn của mạng (Network). Hàm này được dùng khi bạn đóng gói dữ liệu để gửi đi.
#define htons(val)                                              ((val << 8) & 0xFF00) | ((val >> 8) & 0xFF)
#define htonl(val)                                              ((val << 8) & 0xFF0000) | ((val >> 8) & 0xFF00) | ((val << 24) & 0xFF000000) | ((val >> 24) & 0xFF)

// (Network TO Host Short): Chuyển đổi một số 16-bit (kiểu short) từ chuẩn của mạng (Network) về lại chuẩn của vi điều khiển (Host). Hàm này được dùng khi bạn nhận dữ liệu từ mạng và cần giải mã.
#define ntohs(val)                                              htons(val)
#define ntohl(val)                                              htonl(val)



extern uint8_t ipAddr[IP_ADDRESS_BYTES_NUM];
extern  uint8_t macAddr[MAC_ADDRESS_BYTES_NUM];



/* Functions -----------------------------------------------------------------*/



#endif // #ifndef ETH_COMMON_H
