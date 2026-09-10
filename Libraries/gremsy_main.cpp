 
/** 
  ******************************************************************************
  * @file    gremsy_main.cpp
  * @author  Gremsy Team
  * @version V2.0.0
  * @date    Apr 11, 2023
  * @brief   
  *
  ******************************************************************************
  * @Copyright
  * COPYRIGHT NOTICE: (c) 2023 Gremsy.  
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
#include "gremsy_main.hpp"
#include "gremsy.hpp"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

RTOS::Task __gp_task_loop;

/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/

/** @breif ham chay task main loop trong ung dung
 * */
void gremsy_main_loop(void* arg)
{

    while(1)
    {
        #ifdef __PROGRAM_HPP__
        loop();
        #endif
    }
}

/**
 * @brief ham chay trong main chinh trong core
 * @param none
 * @return none
 * @note:
 *      khong co freertos se cho chay cuoi cai dat
 *      co freertos se chay trong qua trinh cai dat freertos
 * */
void gremsy_main(void)
{
     StatusEnum state = OK;

    // /// cai dat ban dau
     g_hardware_init();
     g_rtos_init();

    // /***/
     state = HW::Debug::Log::Init();

    // /***/
     state = HW::Debug::CommandLine::Init();

    /** Debug log */
    LOGCLEAR();

    if(state == ERROR_NOT_EXIST)
    {
        LOGINS<<"Warning: Command line khong su dung duoc. Muon chay duoc command line xin enable uart rx dma mode circular\n\r";
    }

    /***/
    if(HW::Peripheral::Time::Init() != OK)
    {
        LOGINS<<"Warning: Peripheral time khong su dung duoc. Muon chay duoc peripheral time xin cap cho PERIPHERAL_TIME gia tri time handle\n\r";
    }

    LOGINS<<"\n\n";

    ///
    LOGINS<<"-------------------------------\n\r";
    LOGINS<<" Company: GREMSY \n\r";
    LOGINS<<" Author: Gremsy Firmware Team \n\r";
    LOGINS<<" Version: "<<(const char*)versionTotal<<"\n\r";
    LOGINS<<" Date: "<<__DATE__<<"\n\r";
    LOGINS<<"\n\r";

    /// cai dat ham setup
    #ifdef __PROGRAM_HPP__
    setup();
    #else
    #error file program.hpp không tin thấy file
    #endif

//     /// cai dat ham loop hoac chay loop
    __gp_task_loop.name     = "LOOP";
    __gp_task_loop.sender   = NULL;
    __gp_task_loop.size		= 4096;
    __gp_task_loop         += gremsy_main_loop;


    if(__gp_task_loop.Open() != OK)
    {
        /// bao loi cai dat task loop
        LOGE<<"Task loop khong cai dat duoc!\n\r";

        while(1)
        {

        }
    }
    LOGI<<"Heap size: "<<(int)g_heap_size()<<"\n\r";
    LOGINS.SetTransmitBuffer();
 }

/************************ (C) COPYRIGHT GREMSY *****END OF FILE****************/
