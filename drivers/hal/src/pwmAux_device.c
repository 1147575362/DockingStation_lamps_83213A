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

#include <isrfuncs.h>
#include <pwmAux_device.h>
#include <gpio_device.h>

static PwmAuxIsrCallback_t pwmAuxReloadCallback[2] = {NULL,NULL};

void PWMAUX_Init(PWMAUX_Port_t port,PWMAUX_ClockSource_t source,PWMAUX_Prescaler_t devider,uint16_t peroid)
{
    /* IO configuration */
    IOCTRLA_SFRS->GPIO[(uint8_t)port].HWMODE   = (uint8_t)GPIO_MUX_PWM;
    IOCTRLA_SFRS->GPIO[(uint8_t)port].PULLMODE = (uint8_t)GPIO_PULL_NONE;
    GPIO_SFRS->GPIO_CFG[(uint8_t)port].DIR     = (uint8_t)GPIO_DIR_OUTPUT;
    
    
    PWM_AUX_SFRS->PWM_AUX[(uint8_t)source].BASE.PRESCALESEL = (uint8_t)devider;
    PWM_AUX_SFRS->PWM_AUX[(uint8_t)source].BASE.PERIOD = peroid;
    if (port <= PWMAUX_PORT_5){
        PWM_AUX_SFRS->ENAREQ.ENAREQ |= 1U << (uint8_t)port;                     /*  CHN:0-4   */
        PWM_AUX_SFRS->BASE_SEL |= (uint8_t)source << (uint8_t)port;
    }else{
        PWM_AUX_SFRS->ENAREQ.ENAREQ |= 1U << ((uint8_t)port - (uint8_t)PWMAUX_PORT_5);    /*  CHN:1-4   */
        PWM_AUX_SFRS->BASE_SEL |= (uint8_t)source << ((uint8_t)port - (uint8_t)PWMAUX_PORT_5);
    }
}


void PWMAUX_RegisterReloadIRQ(PWMAUX_ClockSource_t source, PwmAuxIsrCallback_t callback)
{
    PWM_AUX_SFRS->INTPERIOD.CLEAR.PERIOD  = 1U << (uint8_t)source;
    PWM_AUX_SFRS->INTPERIOD.ENABLE.PERIOD |= 1U << (uint8_t)source;
    pwmAuxReloadCallback[(uint8_t)source] = callback;
    NVIC_EnableIRQ(PWMAUX_IRQn);
}

void PWMAUX_UnRegisterReloadIRQ(PWMAUX_ClockSource_t source)
{
    PWM_AUX_SFRS->INTPERIOD.CLEAR.PERIOD  = 1U << (uint8_t)source;
    PWM_AUX_SFRS->INTPERIOD.ENABLE.PERIOD &= ~(1U << (uint8_t)source);
    pwmAuxReloadCallback[(uint8_t)source] = NULL;
    NVIC_DisableIRQ(PWMAUX_IRQn);
}


void PWMAUX_SetMatchValue(PwmAuxChannel_t channel, uint16_t matchRisingValue, uint16_t matchFaillValue)
{
    PWM_AUX_SFRS->PULSE[(uint8_t)channel].WORD = (uint32_t)matchFaillValue + ((uint32_t)matchRisingValue << 16);
    PWM_AUX_SFRS->UPDATE = 0x03U;
}


void PWMAUX_Handler(void)
{
    uint8_t status = PWM_AUX_SFRS->INTPERIOD.STATUS.PERIOD;
    if (pwmAuxReloadCallback[(uint8_t)PWMAUX_BASE_0]!= NULL){
        if ((status & (1U << (uint8_t)PWMAUX_BASE_0)) == (1U << (uint8_t)PWMAUX_BASE_0)){
            PWM_AUX_SFRS->INTPERIOD.CLEAR.PERIOD = (1U << (uint8_t)PWMAUX_BASE_0);
            pwmAuxReloadCallback[(uint8_t)PWMAUX_BASE_0]();
        }
    }
    if (pwmAuxReloadCallback[(uint8_t)PWMAUX_BASE_1]!= NULL){
        if ((status & (1U << (uint8_t)PWMAUX_BASE_1)) == (1U << (uint8_t)PWMAUX_BASE_1)){
            PWM_AUX_SFRS->INTPERIOD.CLEAR.PERIOD = (1U << (uint8_t)PWMAUX_BASE_1);
            pwmAuxReloadCallback[(uint8_t)PWMAUX_BASE_1]();
        }
    }
}