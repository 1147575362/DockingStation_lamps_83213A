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
#include <appConfig.h>

#if (LED_NUM <= 8U)

#include <stdint.h>
#include <string.h>
#include <rugbyHP.h>
#include <errno.h>
#include <pwm_device.h>
#include <appConfig.h>
#include <isrfuncs.h>
#include <adc_device.h>

/* 128us for PN voltage measurement*/
#define MAX_MEASURE_TIME_TICKS          (MAIN_CPU_CLOCK * 64U)   /* us  */
#define MAX_DEBUNCE_TIME_TICKS          (MAIN_CPU_CLOCK * 128U) /* us  */

#define MAX_PWM_CHN_DELAY_ON_TIME_TICKS (MAIN_CPU_CLOCK * 4U)   /* us  */

static uint16_t currPWM[24U];
static PwmIsrCallback_t pwmReloadCallback = NULL;

void PWM_Handler(void)
{
    if (pwmReloadCallback != NULL){
        pwmReloadCallback();
    }
}

/**
 * @brief The instance of PMW device
 */
void PWM_Init(PwmPrescaler_t divide, uint16_t period)
{
    IOCTRLA_SFRS->HWMODE = 0xFFFFFFU;
    PWM_SFRS->BASE.PRESCALESEL = (uint8_t)divide;
    PWM_SFRS->BASE.PERIOD = period;
    
    /* set duty cycle to 0*/
    for (uint8_t i = 0U; i < 24U; i++){
        PWM_SFRS->PULSE[i].PFALL = PWM_VALUE_MAX;
        PWM_SFRS->PULSE[i].PRISE = PWM_VALUE_MAX;
    }
    PWM_StartAndUpdate();
}

/**
 * @brief The instance of PMW device
 */
void PWM_SetMatchValue(PwmChannel_t channel, uint16_t matchRisingValue, uint16_t matchFaillValue)
{
    PWM_SFRS->PULSE[channel].PRISE = matchRisingValue;
    PWM_SFRS->PULSE[channel].PFALL = matchFaillValue;
}

/**
 * @brief The instance of PMW device
 */
void PWM_StartAndUpdate(void)
{
    PWM_SFRS->ENAREQ.ENAREQALL = 1U;
    PWM_SFRS->ENAREQ.ENAREQ = 0xFFFFFF;
    PWM_SFRS->UPDATE = 1U;
}



/**
 * @brief The instance of PWM_SetRGBValue
 */
void PWM_SetRGBValue(uint8_t ledNo, uint16_t red, uint16_t green, uint16_t blue)
{
    uint32_t maxDUTY;
    uint16_t r,g,b;
    maxDUTY = (1U << PWM_VALUE_MAX_POS) - MAX_MEASURE_TIME_TICKS - MAX_DEBUNCE_TIME_TICKS - 1U;
    
    r = (uint16_t)((red  *maxDUTY) >> 16);
    g = (uint16_t)((green*maxDUTY) >> 16);
    b = (uint16_t)((blue *maxDUTY) >> 16);
   
    currPWM[PHY_CHANNEL_R(ledNo)] = r;
    currPWM[PHY_CHANNEL_G(ledNo)] = g;
    currPWM[PHY_CHANNEL_B(ledNo)] = b;
    
    PWM_SetMatchValue(PHY_CHANNEL_R(ledNo),MAX_MEASURE_TIME_TICKS, r + MAX_MEASURE_TIME_TICKS);
    PWM_SetMatchValue(PHY_CHANNEL_G(ledNo),(((uint16_t)PWM_VALUE_MAX - g)>>1U), (uint16_t)((PWM_VALUE_MAX + (uint32_t)g) >> 1U));
    PWM_SetMatchValue(PHY_CHANNEL_B(ledNo),((uint16_t)PWM_VALUE_MAX - MAX_DEBUNCE_TIME_TICKS - b), (uint16_t)PWM_VALUE_MAX - MAX_DEBUNCE_TIME_TICKS);
    
    PWM_StartAndUpdate();
}

uint8_t PWM_UpdateFinished(void)
{
    uint8_t result = FALSE;
    uint8_t update = PWM_SFRS->UPDATE;
    if (update == 0U){
        result = TRUE;
    }
    return result;
}


uint8_t PWM_LedIsOn(uint8_t ledNo)
{
    uint8_t result = 1U;
    if ( (currPWM[PHY_CHANNEL_R(ledNo)] <= (PWM_VALUE_MAX/100U)) &&
         (currPWM[PHY_CHANNEL_G(ledNo)] <= (PWM_VALUE_MAX/100U)) &&
         (currPWM[PHY_CHANNEL_B(ledNo)] <= (PWM_VALUE_MAX/100U)) ){
        result = 0U;
    }
    return result;
}

/**
 * @brief The instance of PMW device
 */
void PWM_RegisterPWMReloadIRQ(PwmIsrCallback_t callback)
{
    pwmReloadCallback = callback;
    PWM_SFRS->INTPWM.CLEAR.PERIOD = 1U;
    PWM_SFRS->INTPWM.ENABLE.PERIOD = 1U;
    PWM_SFRS->UPDATE = 1U;
    NVIC_EnableIRQ(PWM_IRQn);
}

/**
 * @brief The instance of PMW device
 */
void PWM_UnRegisterPWMReloadIRQ(void)
{
    pwmReloadCallback = NULL;
    PWM_SFRS->INTPWM.CLEAR.PERIOD = 1U;
    PWM_SFRS->INTPWM.ENABLE.PERIOD   = 0U;
    PWM_SFRS->UPDATE = 1U;
    NVIC_DisableIRQ(PWM_IRQn);
}


/**
 * @brief The instance of PMW device
 */
void PWM_EnableAllChannels(void)
{
    for (uint8_t i = 0U; i < 23U; i++){
        PWM_SFRS->PULSE[i].PFALL = PWM_VALUE_MAX;
        PWM_SFRS->PULSE[i].PRISE = PWM_VALUE_MAX;
    }
    PWM_SFRS->ENAREQ.ENAREQALL = 0x01U;
    PWM_SFRS->UPDATE = 1U;
}

/**
 * @brief The instance of PMW device
 */
void PWM_DisableAllChannels(void)
{
    PWM_SFRS->ENAREQ.CLRREQALL   = 0x1U;
    PWM_SFRS->ENAREQ.FORCEINACTIVE = 1U;
    PWM_SFRS->UPDATE = 1U;
}

/**
 * @brief The instance of PMW device
 */
void PWM_Disable(PwmChannel_t channel)
{
    PWM_SFRS->ENAREQ.ENAREQ &= ~(1UL << channel);
    PWM_SFRS->UPDATE = 1U;
}


/**
 * @brief The instance of PMW device
 */
void PWM_SetPrescaler(PwmPrescaler_t divide)
{
    PWM_SFRS->BASE.PRESCALESEL = (uint8_t)divide;
    PWM_SFRS->UPDATE = 1U;
}

/**
 * @brief The instance of PMW device
 */
void PWM_SetPeriod(uint16_t period)
{
    PWM_SFRS->BASE.PERIOD = period;
    PWM_SFRS->UPDATE = 1U;
}

/**
 * @brief The instance of PMW device
 */
void PWM_SetInvert(PwmChannel_t channel, uint8_t invertEn)
{
    PWM_SFRS->INVERT |= (uint32_t)((uint32_t)invertEn << channel);
    PWM_SFRS->UPDATE = 1U;
}

/**
 * @brief The instance of PMW device
 */
int8_t PWM_SetLedChannelCurrent(void)
{
    /* Enable trim access write enable */
    SYSCTRLA_SFRS->TRIM_ACCESS_KEY.TRIM_ACCESS_KEY = 0x0EU;
    SYSCTRLA_SFRS->BIAS.LEDBIASTRIMEN = 1U;
    SYSCTRLA_SFRS->BIAS.LEDBIASTRIM   = HWCFG_GetLEDBiasTrimValue();
    for (uint8_t i = 0U; i < 24U; i++){
        SYSCTRLA_SFRS->TRIMLED[i].TRIM = HWCFG_GetLEDTrimValue(i);
    }
    SYSCTRLA_SFRS->TRIMVFW = HWCFG_GetOffLEDTrimValue();
    
    return 0;
}

void PWM_TurnOffChannelCurrent(void)
{
    /* Enable trim access write enable */
    SYSCTRLA_SFRS->TRIM_ACCESS_KEY.TRIM_ACCESS_KEY  = 0x0EU;
    for (uint8_t i = 0U; i < 24U; i++){
        SYSCTRLA_SFRS->TRIMLED[i].TRIM = 0U;
    }
    SYSCTRLA_SFRS->TRIMVFW              = 0U;
    /* Disable trim access write enable */
    SYSCTRLA_SFRS->TRIM_ACCESS_KEY.TRIM_ACCESS_KEY = 0x00U; 

}



#endif