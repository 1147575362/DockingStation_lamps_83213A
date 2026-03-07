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

#include <stddef.h>
#include "errno.h"
#include <rugbyHP.h>
#include "systick_sfrs.h"
#include "systick_device.h"
#include "clock_device.h"
#include <isrfuncs.h>

static sysTick_cb_func_t sysTickCallback = NULL;

void SysTick_Handler(void)
{
    if (sysTickCallback != NULL){
        sysTickCallback();
    }
}


void SysTick_Init(uint32_t ticks, sysTick_cb_func_t callback)
{
    SysTick->VAL = 0U;
    SysTick->LOAD = ticks;
    sysTickCallback = callback;
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
    SysTick->CTRL |= (SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk);
    NVIC_EnableIRQ(SysTick_IRQn);
}


uint32_t SysTick_GetCount(void)
{
    uint32_t count = SysTick->VAL;
    return count;
}

uint32_t SysTick_GetLoad(void)
{
    uint32_t count = SysTick->LOAD;
    return count;
}


