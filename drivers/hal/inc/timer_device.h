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
#ifndef TIMER_DEVICE_H__
#define TIMER_DEVICE_H__

#include <stdint.h>
#include <timer0_sfr.h>
#include <timer1_sfr.h>

/* Timer 2 have been used by lin stack ,please don't use !!!   */
#define HW_GP_TIMER_NUM  2U
typedef enum{
  TIMER0 = 0U,
  TIMER1,
}HwGpTimerId_t;


typedef enum GlobalTimerMode_t{
    HW_TIMER_ONE_SHOT_MODE = 0U,
    HW_TIMER_PERIODIC_MODE,
} HwGpTimeMode_t;

typedef void (*TimeoutCallbackFunc_t)(void);

typedef struct{
    /* Timer parameters */
    uint32_t timeReloadCnt;
    uint32_t periodCount;
    HwGpTimeMode_t mode;
    TimeoutCallbackFunc_t handler;
} HwGpTimer_t;


void TIMER_Init(HwGpTimerId_t timerId, HwGpTimeMode_t mode, uint32_t timeoutTicks,TimeoutCallbackFunc_t callback);
void TIMER_Disable(HwGpTimerId_t timerId);


#endif /* __TIMER_DEVICE_H__ */