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
#include <hwProtection.h>
#include <appConfig.h>
#include <isrfuncs.h>

static hwProtIsrCallback_t uvCallback = NULL;
static hwProtIsrCallback_t ovCallback = NULL;


void OVTEMP_Handler(void)
{
  
}


void VBAT_Handler(void)
{
    if (PMUA_SFRS->PMUIRQ.STATUS.UV == 1U){
        if (uvCallback != NULL){
            uvCallback();
        }
        PMUA_SFRS->PMUIRQ.CLEAR.UV =1U;
    }
    if (PMUA_SFRS->PMUIRQ.STATUS.OV == 1U){
        if (ovCallback != NULL){
            ovCallback();
        }
        PMUA_SFRS->PMUIRQ.CLEAR.OV =1U;
    }
    NVIC_DisableIRQ(VBAT_IRQn);
}



void HW_PROT_SetOverVoltage(OverVoltThres_t threshold,OverVoltHysteresis_t hys, VoltDebouceTime_t triggerTime, VoltDebouceTime_t releaseTime)
{
    PMUA_SFRS->VBATTRIM.OVLEVEL = (uint8_t)threshold;
    PMUA_SFRS->VBATTRIM.OVHYS   = (uint8_t)hys;
    
    PMUA_SFRS->VBATDBNC.OVSTRB0SEL = 1U; /*  16KHz source  */
    PMUA_SFRS->VBATDBNC.OVSTRB1SEL = 1U; /*  16KHz source  */
    
    PMUA_SFRS->VBATDBNCTHRES.OVTHRES1 = (uint8_t)triggerTime;/* value*62.5us when 16Khz*/
    PMUA_SFRS->VBATDBNCTHRES.OVTHRES0 = (uint8_t)releaseTime;/* value*62.5us when 16Khz*/
    
    PMUA_SFRS->VBATCTRL.BAT_OV_EN = 1U;
    
}

void HW_PROT_RegisterOverVoltageIRQ(hwProtIsrCallback_t callback)
{
    ovCallback = callback;
    PMUA_SFRS->PMUIRQ.CLEAR.OV  = 1U;
    PMUA_SFRS->PMUIRQ.ENABLE.OV = 1U;
    NVIC_EnableIRQ(VBAT_IRQn);
}

void HW_PROT_UnRegisterOverVoltageIRQ(void)
{
    if (uvCallback == NULL){
        NVIC_DisableIRQ(VBAT_IRQn);
    }
    PMUA_SFRS->PMUIRQ.ENABLE.OV = 0U;
    PMUA_SFRS->PMUIRQ.CLEAR.OV  = 1U;
    ovCallback = NULL;
}


void HW_PROT_SetUnderVoltage(UnderVoltThres_t threshold,UnderVoltHysteresis_t hys, VoltDebouceTime_t triggerTime, VoltDebouceTime_t releaseTime)
{
    PMUA_SFRS->VBATTRIM.UVLEVEL = (uint8_t)threshold;
    PMUA_SFRS->VBATTRIM.UVHYS   = (uint8_t)hys;
    
    PMUA_SFRS->VBATDBNC.UVSTRB0SEL = 1U; /*  16KHz source  */
    PMUA_SFRS->VBATDBNC.UVSTRB1SEL = 1U; /*  16KHz source  */
    
    PMUA_SFRS->VBATDBNCTHRES.UVTHRES1 = (uint8_t)triggerTime; /* value*62.5us when 16Khz*/
    PMUA_SFRS->VBATDBNCTHRES.UVTHRES0 = (uint8_t)releaseTime; /* value*62.5us when 16Khz*/
    
    PMUA_SFRS->VBATCTRL.BAT_UV_EN = 1U;
}

void HW_PROT_RegisterUnderVoltageIRQ(hwProtIsrCallback_t callback)
{
    uvCallback = callback;
    PMUA_SFRS->PMUIRQ.CLEAR.UV  = 1U;
    PMUA_SFRS->PMUIRQ.ENABLE.UV = 1U;
    NVIC_EnableIRQ(VBAT_IRQn);
}

void HW_PROT_UnRegisterUnderVoltageIRQ(void)
{
    if (ovCallback == NULL){
        NVIC_DisableIRQ(VBAT_IRQn);
    }
    PMUA_SFRS->PMUIRQ.ENABLE.UV = 0U;
    PMUA_SFRS->PMUIRQ.CLEAR.UV  = 1U;
    uvCallback = NULL;
}


uint8_t HW_PROT_GetBattVoltState(void)
{
    uint8_t result = (uint8_t)PMUA_SFRS->VBATCTRL.BATT_STATE;
    return result;
}

uint8_t HW_PROT_ChipIsOverHeat(void)
{
    uint8_t result = (uint8_t)CRGA_SFRS->RESETCTRL.OVTEMPFLAG;
    return result;
}




