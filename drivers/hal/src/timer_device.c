/************************************************************************************
Copyright (c) 2023-2025 released WuXi Indie Microelectronics Ltd.     All rights reserved.

WuXi Indie Microelectronics Ltd licenses to you the right to use, modify, copy and distribute
Software only when embedded on a WuXi Indie Microelectronics Ltd's microcontroller or digital signal
controller  that is integrated into your product or third party product

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL WUXI INDIE MICROELECTRONICS LTD OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*************************************************************************************/

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <rugbyHP.h>
#include <timer_device.h>
#include "errno.h"
#include <isrfuncs.h>


static HwGpTimer_t hwGpTimer[HW_GP_TIMER_NUM] = {
  {.timeReloadCnt = 0xFFFFFFFFU, .periodCount = 0U, .handler = NULL},
  {.timeReloadCnt = 0xFFFFFFFFU, .periodCount = 0U, .handler = NULL},
};

void Timer0_Handler( void )
{
    if (hwGpTimer[TIMER0].mode == HW_TIMER_ONE_SHOT_MODE){
        TIMER0_SFRS->ENA = 0U;
        NVIC_DisableIRQ(Timer0_IRQn);
    }else{
        TIMER0_SFRS->COUNT = hwGpTimer[TIMER0].timeReloadCnt;
    }
    if (hwGpTimer[TIMER0].handler != NULL){
        hwGpTimer[TIMER0].handler();
    }else{
          
    }
}

void Timer1_Handler( void )
{
    if (hwGpTimer[TIMER1].mode == HW_TIMER_ONE_SHOT_MODE){
        TIMER1_SFRS->ENA = 0U;
        NVIC_DisableIRQ(Timer1_IRQn);
    }else{
        TIMER1_SFRS->COUNT = hwGpTimer[TIMER1].timeReloadCnt;
    }
    if (hwGpTimer[TIMER1].handler != NULL){
        hwGpTimer[TIMER1].handler();
    }else{
      
    }
}




void TIMER_Init(HwGpTimerId_t timerId, HwGpTimeMode_t mode, uint32_t timeoutTicks, TimeoutCallbackFunc_t callback)
{
    hwGpTimer[timerId].timeReloadCnt = 0xFFFFFFFFU - timeoutTicks;
    hwGpTimer[timerId].handler = callback;
    hwGpTimer[timerId].mode = mode;
    hwGpTimer[timerId].periodCount = 0U;
    if (timerId == TIMER0){
        TIMER0_SFRS->COUNT = hwGpTimer[timerId].timeReloadCnt;
        TIMER0_SFRS->ENA = 1U;
        NVIC_EnableIRQ(Timer0_IRQn);
    }else{
        TIMER1_SFRS->COUNT = hwGpTimer[timerId].timeReloadCnt;
        TIMER1_SFRS->ENA = 1U;
        NVIC_EnableIRQ(Timer1_IRQn);
    }
}


void TIMER_Disable(HwGpTimerId_t timerId)
{
    hwGpTimer[timerId].handler = NULL;
    if (timerId == TIMER0){
        TIMER0_SFRS->ENA = 0U;
        NVIC_DisableIRQ(Timer0_IRQn);
    }else{
        TIMER1_SFRS->ENA = 0U;
        NVIC_DisableIRQ(Timer1_IRQn);
    }
}




















