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

#if (LED_NUM > 8U)

#include <stdint.h>
#include <string.h>
#include <rugbyHP.h>
#include <errno.h>
#include <pwm_device.h>
#include <isrfuncs.h>
#include <adc_device.h>
#include <gpio_device.h>

/* 128us for PN voltage measurement*/
#define MAX_MEASURE_TIME_TICKS                          (MAIN_CPU_CLOCK * 250U)   /* us  */
#define MAX_DEBUNCE_TIME_TICKS                          (MAIN_CPU_CLOCK * 32U) /* us  */

#define MAX_PWM_DUTY                                    (PWM_VALUE_MAX - MAX_MEASURE_TIME_TICKS - MAX_DEBUNCE_TIME_TICKS - 1U)

#define ADC_MEAS_TYPE_LEDPN_VBAT_VBUCK_PTATLED          (0U)
#define ADC_MEAS_TYPE_PTATBUCK_VBAT_VBUCK_PTATLED       (1U)

static volatile uint16_t currPWMMatchValue[LED_NUM][3U];
static uint32_t LedPowerSwitch = 0U;

static volatile uint8_t ledIsActive[LED_NUM];
static volatile uint16_t vLedPNCode[LED_NUM][3];
static volatile uint16_t vBuckCode;
static volatile uint16_t vBattCode;
static volatile uint16_t vTempCode;
static volatile uint8_t currLedNo  = 0U;
static volatile uint8_t ledConvertType = ADC_MEAS_TYPE_PTATBUCK_VBAT_VBUCK_PTATLED;


void ADC_Handler(void)
{
    uint16_t adcCode;
    static uint8_t ledChannel = 0U;
    static uint8_t ledNo = 0U;
    static uint8_t ledPhyChannel = 0U;
    
    if (ledNo == 0U && ledChannel == 0U){
        ledNo = ledNo;
    }
    
#if (LED_NUM < 9U) || (LED_NUM > 16U)
#error LED_NUM must be in the range [9,16]
#else
    if (LedPowerSwitch == 0U){
        LedPowerSwitch = 1U;
  #if LED_BOARD_TYPE == BOARD_TYPE_DP
        GPIO_SetGPIOs((1U << GPIO_PORT_2) | (1U << GPIO_PORT_3), (1U << GPIO_PORT_3)); /* 1,3,5,7,9,11,13,15*/
  #elif LED_BOARD_TYPE == BOARD_TYPE_SY
        GPIO_SetGPIOs((1U << GPIO_PORT_2) | (1U << GPIO_PORT_3), (1U << GPIO_PORT_2)); /* 1,3,5,7,9,11,13,15*/
  #elif LED_BOARD_TYPE == BOARD_TYPE_S2
        GPIO_SetGPIOs((1U << GPIO_PORT_2) | (1U << GPIO_PORT_3), (1U << GPIO_PORT_3)); /* 1,3,5,7,9,11,13,15*/
  #else
        #error invalid LED_BOARD_TYPE definition!
  #endif
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*0U].PRISE  = currPWMMatchValue[0][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*0U].PRISE  = currPWMMatchValue[0][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*0U].PRISE  = currPWMMatchValue[0][PHY_CHANNEL_B];
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*1U].PFALL  = currPWMMatchValue[2][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*1U].PFALL  = currPWMMatchValue[2][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*1U].PFALL  = currPWMMatchValue[2][PHY_CHANNEL_B];
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*2U].PRISE  = currPWMMatchValue[4][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*2U].PRISE  = currPWMMatchValue[4][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*2U].PRISE  = currPWMMatchValue[4][PHY_CHANNEL_B];
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*3U].PFALL  = currPWMMatchValue[6][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*3U].PFALL  = currPWMMatchValue[6][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*3U].PFALL  = currPWMMatchValue[6][PHY_CHANNEL_B];
        
  #if LED_NUM >= 9U
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*4U].PRISE  = currPWMMatchValue[8][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*4U].PRISE  = currPWMMatchValue[8][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*4U].PRISE  = currPWMMatchValue[8][PHY_CHANNEL_B];
  #endif
  #if LED_NUM >= 11U
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*5U].PFALL  = currPWMMatchValue[10][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*5U].PFALL  = currPWMMatchValue[10][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*5U].PFALL  = currPWMMatchValue[10][PHY_CHANNEL_B];
  #endif
  #if LED_NUM >= 13U
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*6U].PRISE  = currPWMMatchValue[12][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*6U].PRISE  = currPWMMatchValue[12][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*6U].PRISE  = currPWMMatchValue[12][PHY_CHANNEL_B];
  #endif
  #if LED_NUM >= 15U
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*7U].PFALL  = currPWMMatchValue[14][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*7U].PFALL  = currPWMMatchValue[14][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*7U].PFALL  = currPWMMatchValue[14][PHY_CHANNEL_B];
  #endif
    }else{
        LedPowerSwitch = 0U;
  #if LED_BOARD_TYPE == BOARD_TYPE_DP
        GPIO_SetGPIOs((1U << GPIO_PORT_2) | (1U << GPIO_PORT_3), (1U << GPIO_PORT_2));  /* 0,2,4,6,8,10,12,14,16 */
  #elif LED_BOARD_TYPE == BOARD_TYPE_SY
        GPIO_SetGPIOs((1U << GPIO_PORT_2) | (1U << GPIO_PORT_3), (1U << GPIO_PORT_3));  /* 0,2,4,6,8,10,12,14,16 */
  #elif LED_BOARD_TYPE == BOARD_TYPE_S2
        GPIO_SetGPIOs((1U << GPIO_PORT_2) | (1U << GPIO_PORT_3), (1U << GPIO_PORT_2));  /* 0,2,4,6,8,10,12,14,16 */
  #else
        #error invalid LED_BOARD_TYPE definition!
  #endif
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*0U].PRISE  = currPWMMatchValue[1][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*0U].PRISE  = currPWMMatchValue[1][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*0U].PRISE  = currPWMMatchValue[1][PHY_CHANNEL_B];
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*1U].PFALL  = currPWMMatchValue[3][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*1U].PFALL  = currPWMMatchValue[3][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*1U].PFALL  = currPWMMatchValue[3][PHY_CHANNEL_B];
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*2U].PRISE  = currPWMMatchValue[5][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*2U].PRISE  = currPWMMatchValue[5][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*2U].PRISE  = currPWMMatchValue[5][PHY_CHANNEL_B];
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*3U].PFALL  = currPWMMatchValue[7][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*3U].PFALL  = currPWMMatchValue[7][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*3U].PFALL  = currPWMMatchValue[7][PHY_CHANNEL_B];
        
  #if LED_NUM >= 9U
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*4U].PRISE  = currPWMMatchValue[9][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*4U].PRISE  = currPWMMatchValue[9][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*4U].PRISE  = currPWMMatchValue[9][PHY_CHANNEL_B];
  #endif
  #if LED_NUM >= 11U
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*5U].PFALL  = currPWMMatchValue[11][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*5U].PFALL  = currPWMMatchValue[11][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*5U].PFALL  = currPWMMatchValue[11][PHY_CHANNEL_B];
  #endif
  #if LED_NUM >= 13U
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*6U].PRISE  = currPWMMatchValue[13][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*6U].PRISE  = currPWMMatchValue[13][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*6U].PRISE  = currPWMMatchValue[13][PHY_CHANNEL_B];
  #endif
  #if LED_NUM >= 15U
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*7U].PFALL  = currPWMMatchValue[15][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*7U].PFALL  = currPWMMatchValue[15][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*7U].PFALL  = currPWMMatchValue[15][PHY_CHANNEL_B];
  #endif
    }
#endif
    PWM_SFRS->UPDATE = 1U;
    
    if(ledConvertType == ADC_MEAS_TYPE_LEDPN_VBAT_VBUCK_PTATLED){
        adcCode = SAR_CTRL_SFRS->ADC_DATA0.DATA0;
        if((adcCode&0x800U) == 0x800U){
          adcCode = 0;
        }        
        vLedPNCode[ledNo][ledChannel]   = adcCode;
    }
    adcCode = SAR_CTRL_SFRS->ADC_DATA0.DATA1; vBattCode = adcCode;
    adcCode = SAR_CTRL_SFRS->ADC_DATA1.DATA2; vBuckCode = adcCode;
    adcCode = SAR_CTRL_SFRS->ADC_DATA1.DATA3; vTempCode = adcCode;
    
    if((vBattCode&0x800U) == 0x800U){
      vBattCode = 0;
    }
    if((vBuckCode&0x800U) == 0x800U){
      vBuckCode = 0;
    } 
    if((vTempCode&0x800U) == 0x800U){
      vTempCode = 0;
    }
    if (++ledNo >= LED_NUM){
        ledNo = 0U;
        if (++ledChannel >= 3U){
            ledChannel = 0U;
        }
    }
    
    ledPhyChannel = (ledNo >> 1) * 3U +  ledChannel;
    
    if (ledIsActive[ledNo] == TRUE){
        SAR_CTRL_SFRS->SAR_CHAN_CFG.CH1_SEL = (uint8_t)ADC_CH_SEL_VBUCK_LED0 + ledPhyChannel;
        ledConvertType = ADC_MEAS_TYPE_LEDPN_VBAT_VBUCK_PTATLED;
    }else{
        SAR_CTRL_SFRS->SAR_CHAN_CFG.CH1_SEL = (uint8_t)ADC_CH_SEL_PTAT_DIFF;
        ledConvertType = ADC_MEAS_TYPE_PTATBUCK_VBAT_VBUCK_PTATLED;
    }
    SAR_CTRL_SFRS->SAR_INT.CLEAR.INT_SAR_DONE_CLR  = 1U;
    SAR_CTRL_SFRS->SAR_CTRL.CONVERT = 1U;
}

void PWM_Handler(void)
{

}

/**
 * @brief The instance of PWM_SetRGBValue
 */
void PWM_SetRGBValue(uint8_t ledNo, uint16_t red, uint16_t green, uint16_t blue)
{
    uint8_t no;
    uint16_t r,g,b;
    r = (uint16_t)((red  *MAX_PWM_DUTY) >> 16);
    g = (uint16_t)((green*MAX_PWM_DUTY) >> 16);
    b = (uint16_t)((blue *MAX_PWM_DUTY) >> 16);
    
    if (red >= (65535U/256U) || green >= (65535U/256U) || blue >= (65535U/256U)){
        ledIsActive[ledNo] = TRUE;
    }else{
        ledIsActive[ledNo] = FALSE;
        vLedPNCode[ledNo][0] = INVALID_ADC_CODE;
        vLedPNCode[ledNo][1] = INVALID_ADC_CODE;
        vLedPNCode[ledNo][2] = INVALID_ADC_CODE;
    }
    no = ledNo >> 1;
    if ((no - ((no >> 1)*2U) ) == 0U){
        currPWMMatchValue[ledNo][PHY_CHANNEL_R] = (uint16_t)PWM_VALUE_MAX - MAX_DEBUNCE_TIME_TICKS - r;
        currPWMMatchValue[ledNo][PHY_CHANNEL_G] = (uint16_t)PWM_VALUE_MAX - MAX_DEBUNCE_TIME_TICKS - g;
        currPWMMatchValue[ledNo][PHY_CHANNEL_B] = (uint16_t)PWM_VALUE_MAX - MAX_DEBUNCE_TIME_TICKS - b;
    }else{
        currPWMMatchValue[ledNo][PHY_CHANNEL_R] = r + MAX_MEASURE_TIME_TICKS;
        currPWMMatchValue[ledNo][PHY_CHANNEL_G] = g + MAX_MEASURE_TIME_TICKS;
        currPWMMatchValue[ledNo][PHY_CHANNEL_B] = b + MAX_MEASURE_TIME_TICKS;
    }
}


/**
 * @brief The instance of PMW device
 */
void PWM_Init(PwmPrescaler_t divide, uint16_t period)
{
   for (uint8_t i = 0; i < LED_NUM; i++){
        vLedPNCode[i][0] = INVALID_ADC_CODE;
        vLedPNCode[i][1] = INVALID_ADC_CODE;
        vLedPNCode[i][2] = INVALID_ADC_CODE;
        ledIsActive[i] = FALSE;
    }
    vBuckCode = INVALID_ADC_CODE;
    vBattCode = INVALID_ADC_CODE;
    vTempCode = INVALID_ADC_CODE;
  
    IOCTRLA_SFRS->HWMODE                = 0xFFFFFFU;
    IOCTRLA_SFRS->LEDPIN.VFW_ENA        = 1U;                   /* Enable 2mA led current source */
    IOCTRLA_SFRS->LEDPIN.SENSE_ENA      = 1U;                   /* LED Forward Voltage Sense Enable. Set to enable LED forward voltage sense module.*/
    PWM_SFRS->BASE.PERIOD               = PWM_VALUE_MAX;
    /* set duty cycle to 0*/
    PWM_Reset();
    
    SAR_CTRL_SFRS->SAR_CHAN_CFG.CH1_SEL = (uint8_t)ADC_CH_SEL_PTAT_DIFF;
    SAR_CTRL_SFRS->SAR_CHAN_CFG.CH2_SEL = (uint8_t)ADC_CH_SEL_VBAT_GND;
    SAR_CTRL_SFRS->SAR_CHAN_CFG.CH3_SEL = (uint8_t)ADC_CH_SEL_VBUCK_GND;
    SAR_CTRL_SFRS->SAR_CHAN_CFG.CH4_SEL = (uint8_t)ADC_CH_SEL_PTAT_DIFF;
    SAR_CTRL_SFRS->SAR_CHAN_CFG.CHAN_SEQ_NUM = (uint8_t)ADC_SQ_CH1_CH2_CH3_CH4;
    ledConvertType = ADC_MEAS_TYPE_PTATBUCK_VBAT_VBUCK_PTATLED;
    
    SAR_CTRL_SFRS->SAR_INT.CLEAR.INT_SAR_DONE_CLR  = 1U;
    SAR_CTRL_SFRS->SAR_CTRL.CONVERT                = 1U;
    
    PWM_SFRS->ENAREQ.ENAREQALL          = 1U;
}

void PWM_Reset(void)
{
    uint8_t no;
    for (uint8_t i = 0U; i < LED_NUM; i++){
        no = i >> 1U;
        if( (no - ((no >> 1U)*2U)) ==  0U ){
            for (uint8_t j  = 0U; j < 3U; j ++){
                PWM_SFRS->PULSE[no*3U + j].PFALL = (uint16_t)(PWM_VALUE_MAX - MAX_DEBUNCE_TIME_TICKS);
                PWM_SFRS->PULSE[no*3U + j].PRISE = (uint16_t)(PWM_VALUE_MAX - MAX_DEBUNCE_TIME_TICKS); 
                currPWMMatchValue[i][j]          = (uint16_t)(PWM_VALUE_MAX - MAX_DEBUNCE_TIME_TICKS);
            }
        }else{
            for (uint8_t j  = 0U; j < 3U; j ++){
                PWM_SFRS->PULSE[no*3U + j].PFALL = MAX_MEASURE_TIME_TICKS;
                PWM_SFRS->PULSE[no*3U + j].PRISE = MAX_MEASURE_TIME_TICKS;
                currPWMMatchValue[i][j]          = MAX_MEASURE_TIME_TICKS;
            }
        }
        ledIsActive[i] = FALSE;
    }
    PWM_SFRS->UPDATE = 1U;
}



uint16_t ADC_GetLedPNCode(uint8_t ledNo, uint16_t *pnCodeR,uint16_t *pnCodeG,uint16_t *pnCodeB)
{   
__atomic_enter()
    *pnCodeR = vLedPNCode[ledNo][PHY_CHANNEL_R];
    *pnCodeG = vLedPNCode[ledNo][PHY_CHANNEL_G];
    *pnCodeB = vLedPNCode[ledNo][PHY_CHANNEL_B];
__atomic_exit()
    return 0U;
}

uint16_t ADC_GetVBuckCode(void)
{   
    uint16_t code = vBuckCode;
    return code;
}

uint16_t ADC_GetVBattCode(void)
{   
    uint16_t code = vBattCode;
    return code;
}

uint16_t ADC_GetVTempCode(void)
{   
    uint16_t code = vTempCode;
    return code;
}


uint8_t PWM_UpdateFinished(void)
{
    uint8_t result = TRUE;
    uint8_t update = PWM_SFRS->UPDATE;
    if (update == 0U){
        result = FALSE;
    }
    return result;
}


uint8_t PWM_LedIsOn(uint8_t ledNo)
{
    return ledIsActive[ledNo];
}


/**
 * @brief The instance of PMW device
 */
void PWM_RegisterPWMReloadIRQ(PwmIsrCallback_t callback)
{
    PWM_SFRS->INTPWM.CLEAR.PERIOD = 1U;
    PWM_SFRS->INTPWM.IRQ.PERIOD = 1U;
    PWM_SFRS->UPDATE = 1U;
    NVIC_EnableIRQ(PWM_IRQn);
}

/**
 * @brief The instance of PMW device
 */
void PWM_UnRegisterPWMReloadIRQ(void)
{
    PWM_SFRS->INTPWM.CLEAR.PERIOD  = 1U;
    PWM_SFRS->INTPWM.ENABLE.PERIOD = 0U;
    NVIC_DisableIRQ(PWM_IRQn);
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