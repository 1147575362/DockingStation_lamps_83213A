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

#include <string.h>
#include <rugbyHP.h>
#include "wdt_device.h"
#include "errno.h"
#include <isrfuncs.h>
/**
 * @brief The instance of WDT device
 */
static wdt_cb_func_t  wdt1Callback = NULL;


void Watchdog_Handler(void)
{
    if (wdt1Callback != NULL){
        wdt1Callback();
    }
}

void WDT1_Enable(WDT1Mode_t mode, WDT1Interval_t interval, wdt_cb_func_t callback)
{
    WDT1_SFRS->CFG.PRESET           = (uint8_t)interval;
    if (mode == WDT1_MODE_RESET){
        WDT1_SFRS->CFG.RSTEN        = 1U;
    }else{
        wdt1Callback = callback;
        NVIC_EnableIRQ(Watchdog_IRQn);
    }
    WDT1_SFRS->CFG.ENA              = 1U;
}

#pragma optimize=none
void WDT1_Clear(void)
{
    __disable_irq();
    WDT1_SFRS->KEY       = 0x3C570001U;
    WDT1_SFRS->KEY       = 0x007F4AD6U; 
    __enable_irq();
}


void WatchdogA_Handler(void)
{

}

void WDTA_Enable(WDTAInterval_t interval)
{
    WDTA_SFRS->CTRL.WDTA_TIMEOUT_SEL = (uint8_t)interval;
    WDTA_SFRS->CLEAR = 0x3c574ad6U;
    WDTA_SFRS->CTRL.UPDATE = 1U;
    WDTA_SFRS->STOP = 0U;/* start wdt*/
}

void WDTA_Disable(void)
{
    WDTA_SFRS->STOP = 0x6da475c3U;/* stop wdt*/
}

void WDTA_Clear(void)
{
    WDTA_SFRS->CLEAR = 0x3c574ad6U;
}






























