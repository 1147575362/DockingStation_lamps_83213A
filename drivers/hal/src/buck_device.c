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
#include <buck_device.h>
#include <clock_device.h>
#include "errno.h"
#include <isrfuncs.h>
#include <buckOperation.h>

static BuckIsrCallback_t buckIsrCallback = NULL;

/* freqiency: KHz vBuckVolt: 4400- 5400mV*/
void BUCK_Init(BuckOutputVoltage_t enumBuckVolt)
{
    /* Comment the BUCKOPT_SetBuckFreq function for mass production */
    uint16_t version = HWCFG_GetFlashMapVersion();
    if ( (version < 0xA0U) || (version == 0xFFFFU) ){
        BUCKOPT_Init();
        BUCKOPT_SetBuckFreq((uint8_t)BUCK_FREQ_400KHz);
    }
    
    BUCK_INF_SFRS->BUCK_CFG2.SET_VOUT_PD = 1U;
    
    BUCK_INF_SFRS->BUCK_CFG0.VDAC_TRIM          = (uint8_t)enumBuckVolt;

    
    BUCK_INF_SFRS->BUCK_CFG0.RAMP_UPDATE = 0x1U; 
    while(BUCK_INF_SFRS->BUCK_CFG0.RAMP_UPDATE == 1U){}  
    
    BUCK_INF_SFRS->BUCK_CFG6.SSC_ENA      = 1U;
    BUCK_INF_SFRS->BUCK_CFG6.SEL_SSC_STEP = (uint8_t)BUCK_SSC_STEP_8STEPS_1_PEROIDS;
    BUCK_INF_SFRS->BUCK_CFG2.BUCK_ENA     = 1U;                           /* enable buck */
	
    BUCK_INF_SFRS->BUCK_CFG2.SET_VOUT_PD = 0U;
}

void BUCK_RegisterEmergencyIRQ(BuckIsrCallback_t callback)
{
    buckIsrCallback = callback;
    BUCK_INF_SFRS->BUCK_CTRL_INT.ENABLE.INT_VOUT_OC_ENA         = 1U;
    BUCK_INF_SFRS->BUCK_CTRL_INT.ENABLE.INT_VOUT_SHORT_ENA      = 1U;
    BUCK_INF_SFRS->BUCK_CTRL_INT.ENABLE.INT_VIN_UV_ENA          = 1U;
    BUCK_INF_SFRS->BUCK_CTRL_INT.ENABLE.INT_START_ERROR_ENA     = 1U;
    NVIC_EnableIRQ(BUCK_IRQn);
}


void BUCK_Handler(void)
{
    uint8_t status = BUCK_INF_SFRS->BUCK_CTRL_INT.STATUS.BYTE;
    if (buckIsrCallback != NULL){
        buckIsrCallback(status);
    }
    BUCK_INF_SFRS->BUCK_CTRL_INT.CLEAR.BYTE = 0xFFU;
}


void BUCK_DisableBuck(void)
{
    BUCK_INF_SFRS->BUCK_CFG2.SET_VOUT_PD = 1U;
    BUCK_INF_SFRS->BUCK_CFG2.BUCK_ENA = 0U;             /* disable buck */
}

void BUCK_EnableBuck(void)
{
    BUCK_INF_SFRS->BUCK_CFG2.SET_VOUT_PD = 1U;
    BUCK_INF_SFRS->BUCK_CFG2.BUCK_ENA = 1U;             /* enable buck */
    BUCK_INF_SFRS->BUCK_CFG2.SET_VOUT_PD = 0U;
}
