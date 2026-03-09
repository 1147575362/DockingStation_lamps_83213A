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

#if (LED_NUM > 32U && LED_NUM <= 40U)

#include <stdint.h>
#include <string.h>
#include <rugbyHP.h>
#include <errno.h>
#include <pwm_device.h>
#include <isrfuncs.h>
#include <adc_device.h>
#include <gpio_device.h>

#define PHY_LED_NUM     40U
#define TIME_DIV_BASE   (PHY_LED_NUM/8U)

#define MAX_PWM_DUTY                    (PWM_VALUE_MAX - MAX_MEASURE_TIME_TICKS - MAX_DEBUNCE_TIME_TICKS - 1U)

#define ADC_MEAS_TYPE_LEDPN_VBAT_VBUCK_PTATLED          (0U)
#define ADC_MEAS_TYPE_PTATBUCK_VBAT_VBUCK_PTATLED       (1U)


static const uint8_t ledNumTable[]={
  /* PWM21 - PWM23 */
  LED35,        /* LED0 */
  LED36,        /* LED1 */      
  LED37,        /* LED2 */
  LED38,        /* LED3 */
  LED39,        /* LED4 */
  /* PWM18 - PWM20 */
  LED30,
  LED31,
  LED32,
  LED33,
  LED34,
  /* PWM15 - PWM17 */
  LED25,
  LED26,
  LED27,
  LED28,
  LED29,
  /* PWM12 - PWM14 */
  LED20,
  LED21,
  LED22,
  LED23,
  LED24,
  /* PWM9 - PWM11  */
  LED15,
  LED16,
  LED17,
  LED18,
  LED19,
  /* PWM6 - PWM8   */
  LED10,
  LED11,
  LED12,
  LED13,
  LED14,
  /* PWM3 - PWM5   */
  LED5,
  LED6,
  LED7,
  LED8,
  LED9,
  /* PWM0 - PWM2   */
  LED0,
  LED1,
  LED2,
  LED3,
  LED4,
};

uint8_t PWM_GetPhyLedIndex(uint8_t no)
{
    return ledNumTable[no];
}

static volatile uint16_t currPWMMatchValue[PHY_LED_NUM][3U];
static volatile uint8_t ledIsActive[PHY_LED_NUM];
static volatile uint16_t vLedPNCode[PHY_LED_NUM][3];
static volatile uint16_t vBuckCode;
static volatile uint16_t vBattCode;
static volatile uint16_t vTempCode;
static volatile uint8_t currLedNo  = 0U;
static volatile uint8_t ledConvertType = ADC_MEAS_TYPE_PTATBUCK_VBAT_VBUCK_PTATLED;


void ADC_Handler(void)
{
    static uint32_t LedPowerSwitch = 0U;
    static uint8_t ledChannel   = 3U;
    static uint8_t ledNo        = 38U;
    static uint8_t scanLedNo    = 3U;
    static uint8_t lastLedChannel;
    static uint8_t lastLedNo;
    uint16_t adcCode;
    static uint8_t ledPhyChannel = 0U;
    
    if (LedPowerSwitch == 0U){
        LedPowerSwitch = 1U;
        GPIO_SetGPIOs((1U << GPIO_PORT_1) | (1U << GPIO_PORT_2) | (1U << GPIO_PORT_3) | (1U << GPIO_PORT_6)| (1U << GPIO_PORT_7), (1U << GPIO_PORT_7));
        scanLedNo = 4U;
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*0U].PRISE  = currPWMMatchValue[0U*TIME_DIV_BASE + 0U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*0U].PRISE  = currPWMMatchValue[0U*TIME_DIV_BASE + 0U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*0U].PRISE  = currPWMMatchValue[0U*TIME_DIV_BASE + 0U][PHY_CHANNEL_B];
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*1U].PFALL  = currPWMMatchValue[1U*TIME_DIV_BASE + 0U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*1U].PFALL  = currPWMMatchValue[1U*TIME_DIV_BASE + 0U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*1U].PFALL  = currPWMMatchValue[1U*TIME_DIV_BASE + 0U][PHY_CHANNEL_B];
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*2U].PRISE  = currPWMMatchValue[2U*TIME_DIV_BASE + 0U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*2U].PRISE  = currPWMMatchValue[2U*TIME_DIV_BASE + 0U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*2U].PRISE  = currPWMMatchValue[2U*TIME_DIV_BASE + 0U][PHY_CHANNEL_B];
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*3U].PFALL  = currPWMMatchValue[3U*TIME_DIV_BASE + 0U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*3U].PFALL  = currPWMMatchValue[3U*TIME_DIV_BASE + 0U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*3U].PFALL  = currPWMMatchValue[3U*TIME_DIV_BASE + 0U][PHY_CHANNEL_B];
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*4U].PRISE  = currPWMMatchValue[4U*TIME_DIV_BASE + 0U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*4U].PRISE  = currPWMMatchValue[4U*TIME_DIV_BASE + 0U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*4U].PRISE  = currPWMMatchValue[4U*TIME_DIV_BASE + 0U][PHY_CHANNEL_B];

        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*5U].PFALL  = currPWMMatchValue[5U*TIME_DIV_BASE + 0U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*5U].PFALL  = currPWMMatchValue[5U*TIME_DIV_BASE + 0U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*5U].PFALL  = currPWMMatchValue[5U*TIME_DIV_BASE + 0U][PHY_CHANNEL_B];

        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*6U].PRISE  = currPWMMatchValue[6U*TIME_DIV_BASE + 0U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*6U].PRISE  = currPWMMatchValue[6U*TIME_DIV_BASE + 0U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*6U].PRISE  = currPWMMatchValue[6U*TIME_DIV_BASE + 0U][PHY_CHANNEL_B];

        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*7U].PFALL  = currPWMMatchValue[7U*TIME_DIV_BASE + 0U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*7U].PFALL  = currPWMMatchValue[7U*TIME_DIV_BASE + 0U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*7U].PFALL  = currPWMMatchValue[7U*TIME_DIV_BASE + 0U][PHY_CHANNEL_B];
    }else if (LedPowerSwitch == 1U){
        LedPowerSwitch = 2U;
        GPIO_SetGPIOs((1U << GPIO_PORT_1) | (1U << GPIO_PORT_2) | (1U << GPIO_PORT_3) | (1U << GPIO_PORT_6)| (1U << GPIO_PORT_7), (1U << GPIO_PORT_2)); 
        scanLedNo = 0U;
        /*   Next scan Channel */
        ledChannel++;
        if (ledChannel >= 3U){
            ledChannel = 0U;
            ledNo += TIME_DIV_BASE;
            if (ledNo >= PHY_LED_NUM) {
                ledNo = 0U;
            }
        }
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*0U].PRISE  = currPWMMatchValue[0U*TIME_DIV_BASE + 1U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*0U].PRISE  = currPWMMatchValue[0U*TIME_DIV_BASE + 1U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*0U].PRISE  = currPWMMatchValue[0U*TIME_DIV_BASE + 1U][PHY_CHANNEL_B];
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*1U].PFALL  = currPWMMatchValue[1U*TIME_DIV_BASE + 1U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*1U].PFALL  = currPWMMatchValue[1U*TIME_DIV_BASE + 1U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*1U].PFALL  = currPWMMatchValue[1U*TIME_DIV_BASE + 1U][PHY_CHANNEL_B];
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*2U].PRISE  = currPWMMatchValue[2U*TIME_DIV_BASE + 1U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*2U].PRISE  = currPWMMatchValue[2U*TIME_DIV_BASE + 1U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*2U].PRISE  = currPWMMatchValue[2U*TIME_DIV_BASE + 1U][PHY_CHANNEL_B];
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*3U].PFALL  = currPWMMatchValue[3U*TIME_DIV_BASE + 1U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*3U].PFALL  = currPWMMatchValue[3U*TIME_DIV_BASE + 1U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*3U].PFALL  = currPWMMatchValue[3U*TIME_DIV_BASE + 1U][PHY_CHANNEL_B];
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*4U].PRISE  = currPWMMatchValue[4U*TIME_DIV_BASE + 1U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*4U].PRISE  = currPWMMatchValue[4U*TIME_DIV_BASE + 1U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*4U].PRISE  = currPWMMatchValue[4U*TIME_DIV_BASE + 1U][PHY_CHANNEL_B];

        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*5U].PFALL  = currPWMMatchValue[5U*TIME_DIV_BASE + 1U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*5U].PFALL  = currPWMMatchValue[5U*TIME_DIV_BASE + 1U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*5U].PFALL  = currPWMMatchValue[5U*TIME_DIV_BASE + 1U][PHY_CHANNEL_B];

        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*6U].PRISE  = currPWMMatchValue[6U*TIME_DIV_BASE + 1U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*6U].PRISE  = currPWMMatchValue[6U*TIME_DIV_BASE + 1U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*6U].PRISE  = currPWMMatchValue[6U*TIME_DIV_BASE + 1U][PHY_CHANNEL_B];

        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*7U].PFALL  = currPWMMatchValue[7U*TIME_DIV_BASE + 1U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*7U].PFALL  = currPWMMatchValue[7U*TIME_DIV_BASE + 1U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*7U].PFALL  = currPWMMatchValue[7U*TIME_DIV_BASE + 1U][PHY_CHANNEL_B];
    }else if (LedPowerSwitch == 2U){
        LedPowerSwitch = 3U;
        GPIO_SetGPIOs((1U << GPIO_PORT_1) | (1U << GPIO_PORT_2) | (1U << GPIO_PORT_3) | (1U << GPIO_PORT_6)| (1U << GPIO_PORT_7), (1U << GPIO_PORT_1)); 
        scanLedNo = 1U;
          
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*0U].PRISE  = currPWMMatchValue[0U*TIME_DIV_BASE + 2U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*0U].PRISE  = currPWMMatchValue[0U*TIME_DIV_BASE + 2U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*0U].PRISE  = currPWMMatchValue[0U*TIME_DIV_BASE + 2U][PHY_CHANNEL_B];
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*1U].PFALL  = currPWMMatchValue[1U*TIME_DIV_BASE + 2U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*1U].PFALL  = currPWMMatchValue[1U*TIME_DIV_BASE + 2U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*1U].PFALL  = currPWMMatchValue[1U*TIME_DIV_BASE + 2U][PHY_CHANNEL_B];
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*2U].PRISE  = currPWMMatchValue[2U*TIME_DIV_BASE + 2U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*2U].PRISE  = currPWMMatchValue[2U*TIME_DIV_BASE + 2U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*2U].PRISE  = currPWMMatchValue[2U*TIME_DIV_BASE + 2U][PHY_CHANNEL_B];
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*3U].PFALL  = currPWMMatchValue[3U*TIME_DIV_BASE + 2U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*3U].PFALL  = currPWMMatchValue[3U*TIME_DIV_BASE + 2U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*3U].PFALL  = currPWMMatchValue[3U*TIME_DIV_BASE + 2U][PHY_CHANNEL_B];
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*4U].PRISE  = currPWMMatchValue[4U*TIME_DIV_BASE + 2U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*4U].PRISE  = currPWMMatchValue[4U*TIME_DIV_BASE + 2U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*4U].PRISE  = currPWMMatchValue[4U*TIME_DIV_BASE + 2U][PHY_CHANNEL_B];

        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*5U].PFALL  = currPWMMatchValue[5U*TIME_DIV_BASE + 2U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*5U].PFALL  = currPWMMatchValue[5U*TIME_DIV_BASE + 2U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*5U].PFALL  = currPWMMatchValue[5U*TIME_DIV_BASE + 2U][PHY_CHANNEL_B];

        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*6U].PRISE  = currPWMMatchValue[6U*TIME_DIV_BASE + 2U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*6U].PRISE  = currPWMMatchValue[6U*TIME_DIV_BASE + 2U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*6U].PRISE  = currPWMMatchValue[6U*TIME_DIV_BASE + 2U][PHY_CHANNEL_B];

        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*7U].PFALL  = currPWMMatchValue[7U*TIME_DIV_BASE + 2U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*7U].PFALL  = currPWMMatchValue[7U*TIME_DIV_BASE + 2U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*7U].PFALL  = currPWMMatchValue[7U*TIME_DIV_BASE + 2U][PHY_CHANNEL_B];
    }else if (LedPowerSwitch == 3U){
        LedPowerSwitch = 4U;
        GPIO_SetGPIOs((1U << GPIO_PORT_1) | (1U << GPIO_PORT_2) | (1U << GPIO_PORT_3) | (1U << GPIO_PORT_6)| (1U << GPIO_PORT_7), (1U << GPIO_PORT_3)); 
        scanLedNo = 2U;
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*0U].PRISE  = currPWMMatchValue[0U*TIME_DIV_BASE + 3U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*0U].PRISE  = currPWMMatchValue[0U*TIME_DIV_BASE + 3U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*0U].PRISE  = currPWMMatchValue[0U*TIME_DIV_BASE + 3U][PHY_CHANNEL_B];
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*1U].PFALL  = currPWMMatchValue[1U*TIME_DIV_BASE + 3U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*1U].PFALL  = currPWMMatchValue[1U*TIME_DIV_BASE + 3U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*1U].PFALL  = currPWMMatchValue[1U*TIME_DIV_BASE + 3U][PHY_CHANNEL_B];
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*2U].PRISE  = currPWMMatchValue[2U*TIME_DIV_BASE + 3U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*2U].PRISE  = currPWMMatchValue[2U*TIME_DIV_BASE + 3U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*2U].PRISE  = currPWMMatchValue[2U*TIME_DIV_BASE + 3U][PHY_CHANNEL_B];
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*3U].PFALL  = currPWMMatchValue[3U*TIME_DIV_BASE + 3U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*3U].PFALL  = currPWMMatchValue[3U*TIME_DIV_BASE + 3U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*3U].PFALL  = currPWMMatchValue[3U*TIME_DIV_BASE + 3U][PHY_CHANNEL_B];
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*4U].PRISE  = currPWMMatchValue[4U*TIME_DIV_BASE + 3U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*4U].PRISE  = currPWMMatchValue[4U*TIME_DIV_BASE + 3U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*4U].PRISE  = currPWMMatchValue[4U*TIME_DIV_BASE + 3U][PHY_CHANNEL_B];

        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*5U].PFALL  = currPWMMatchValue[5U*TIME_DIV_BASE + 3U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*5U].PFALL  = currPWMMatchValue[5U*TIME_DIV_BASE + 3U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*5U].PFALL  = currPWMMatchValue[5U*TIME_DIV_BASE + 3U][PHY_CHANNEL_B];

        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*6U].PRISE  = currPWMMatchValue[6U*TIME_DIV_BASE + 3U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*6U].PRISE  = currPWMMatchValue[6U*TIME_DIV_BASE + 3U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*6U].PRISE  = currPWMMatchValue[6U*TIME_DIV_BASE + 3U][PHY_CHANNEL_B];

        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*7U].PFALL  = currPWMMatchValue[7U*TIME_DIV_BASE + 3U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*7U].PFALL  = currPWMMatchValue[7U*TIME_DIV_BASE + 3U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*7U].PFALL  = currPWMMatchValue[7U*TIME_DIV_BASE + 3U][PHY_CHANNEL_B];
    }else{
        LedPowerSwitch = 0U;
        GPIO_SetGPIOs((1U << GPIO_PORT_1) | (1U << GPIO_PORT_2) | (1U << GPIO_PORT_3) | (1U << GPIO_PORT_6)| (1U << GPIO_PORT_7), (1U << GPIO_PORT_6)); 
        scanLedNo = 3U;
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*0U].PRISE  = currPWMMatchValue[0U*TIME_DIV_BASE + 4U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*0U].PRISE  = currPWMMatchValue[0U*TIME_DIV_BASE + 4U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*0U].PRISE  = currPWMMatchValue[0U*TIME_DIV_BASE + 4U][PHY_CHANNEL_B];
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*1U].PFALL  = currPWMMatchValue[1U*TIME_DIV_BASE + 4U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*1U].PFALL  = currPWMMatchValue[1U*TIME_DIV_BASE + 4U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*1U].PFALL  = currPWMMatchValue[1U*TIME_DIV_BASE + 4U][PHY_CHANNEL_B];
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*2U].PRISE  = currPWMMatchValue[2U*TIME_DIV_BASE + 4U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*2U].PRISE  = currPWMMatchValue[2U*TIME_DIV_BASE + 4U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*2U].PRISE  = currPWMMatchValue[2U*TIME_DIV_BASE + 4U][PHY_CHANNEL_B];
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*3U].PFALL  = currPWMMatchValue[3U*TIME_DIV_BASE + 4U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*3U].PFALL  = currPWMMatchValue[3U*TIME_DIV_BASE + 4U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*3U].PFALL  = currPWMMatchValue[3U*TIME_DIV_BASE + 4U][PHY_CHANNEL_B];
        
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*4U].PRISE  = currPWMMatchValue[4U*TIME_DIV_BASE + 4U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*4U].PRISE  = currPWMMatchValue[4U*TIME_DIV_BASE + 4U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*4U].PRISE  = currPWMMatchValue[4U*TIME_DIV_BASE + 4U][PHY_CHANNEL_B];

        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*5U].PFALL  = currPWMMatchValue[5U*TIME_DIV_BASE + 4U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*5U].PFALL  = currPWMMatchValue[5U*TIME_DIV_BASE + 4U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*5U].PFALL  = currPWMMatchValue[5U*TIME_DIV_BASE + 4U][PHY_CHANNEL_B];

        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*6U].PRISE  = currPWMMatchValue[6U*TIME_DIV_BASE + 4U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*6U].PRISE  = currPWMMatchValue[6U*TIME_DIV_BASE + 4U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*6U].PRISE  = currPWMMatchValue[6U*TIME_DIV_BASE + 4U][PHY_CHANNEL_B];

        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_R+ 3U*7U].PFALL  = currPWMMatchValue[7U*TIME_DIV_BASE + 4U][PHY_CHANNEL_R];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_G+ 3U*7U].PFALL  = currPWMMatchValue[7U*TIME_DIV_BASE + 4U][PHY_CHANNEL_G];
        PWM_SFRS->PULSE[(uint8_t)PHY_CHANNEL_B+ 3U*7U].PFALL  = currPWMMatchValue[7U*TIME_DIV_BASE + 4U][PHY_CHANNEL_B];
    }

    PWM_SFRS->UPDATE = 1U;
    
    if(ledConvertType == ADC_MEAS_TYPE_LEDPN_VBAT_VBUCK_PTATLED){
        adcCode = SAR_CTRL_SFRS->ADC_DATA0.DATA0;
        if((adcCode&0x800U) == 0x800U){
          adcCode = 0;
        }        
        vLedPNCode[lastLedNo][lastLedChannel]   = adcCode;
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
    uint8_t runTimeLedNo = ledNo+scanLedNo;
    if (runTimeLedNo < PHY_LED_NUM){
        if (ledIsActive[runTimeLedNo] == TRUE){
            ledPhyChannel = ((runTimeLedNo)/TIME_DIV_BASE) * 3U +  ledChannel;
            SAR_CTRL_SFRS->SAR_CHAN_CFG.CH1_SEL = (uint8_t)ADC_CH_SEL_VBUCK_LED0 + ledPhyChannel;
            ledConvertType = ADC_MEAS_TYPE_LEDPN_VBAT_VBUCK_PTATLED;
            
            lastLedChannel = ledChannel;
            lastLedNo = runTimeLedNo;
        }else{
            SAR_CTRL_SFRS->SAR_CHAN_CFG.CH1_SEL = (uint8_t)ADC_CH_SEL_PTAT_DIFF;
            ledConvertType = ADC_MEAS_TYPE_PTATBUCK_VBAT_VBUCK_PTATLED;
        }
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
void PWM_SetRGBValue(uint8_t ledNum, uint16_t red, uint16_t green, uint16_t blue)
{
    uint8_t no;
    uint16_t r,g,b;
    if (ledNum < PHY_LED_NUM){
        uint8_t ledNo = ledNumTable[ledNum];
        r = (uint16_t)((red  *MAX_PWM_DUTY) >> 16);
        g = (uint16_t)((green*MAX_PWM_DUTY) >> 16);
        b = (uint16_t)((blue *MAX_PWM_DUTY) >> 16);
        
        if (red >= (1U*65535U/100U) || green >= (1U*65535U/100U) || blue >= (1U*65535U*95U/100U) ||(red > 0U && green > 0U && blue > 0U)){ /* do PN detect when intensity R => 1% or G => 1% or B => 95% */
           /* It is recommended to set blue intensity value larger than 95% , or the red and green PN voltage measurement will lead to blinking while only the blue LED is on. */
            ledIsActive[ledNo] = TRUE;
        }else{
            ledIsActive[ledNo] = FALSE;
            vLedPNCode[ledNo][0] = INVALID_ADC_CODE;
            vLedPNCode[ledNo][1] = INVALID_ADC_CODE;
            vLedPNCode[ledNo][2] = INVALID_ADC_CODE;
        }
        
        no = ledNo/5U;
        if ((no - ((no>>1U)*2U) ) == 0U){
            currPWMMatchValue[ledNo][PHY_CHANNEL_R] = (uint16_t)PWM_VALUE_MAX - MAX_DEBUNCE_TIME_TICKS - r;
            currPWMMatchValue[ledNo][PHY_CHANNEL_G] = (uint16_t)PWM_VALUE_MAX - MAX_DEBUNCE_TIME_TICKS - g;
            currPWMMatchValue[ledNo][PHY_CHANNEL_B] = (uint16_t)PWM_VALUE_MAX - MAX_DEBUNCE_TIME_TICKS - b;
        }else{
            currPWMMatchValue[ledNo][PHY_CHANNEL_R] = r + MAX_MEASURE_TIME_TICKS;
            currPWMMatchValue[ledNo][PHY_CHANNEL_G] = g + MAX_MEASURE_TIME_TICKS;
            currPWMMatchValue[ledNo][PHY_CHANNEL_B] = b + MAX_MEASURE_TIME_TICKS;
        }
    }
}

void PWM_GetStaticPNVolt(uint8_t ledNo, uint16_t *pnRCode,uint16_t *pnGCode,uint16_t *pnBCode)
{
    if (ledNo < LED_NUM){
        uint32_t uLedCode[3];
        uint8_t lastCHSEQ;
        uint8_t lastCH2;
        uint8_t led_no = ledNumTable[ledNo];
        ADC_UnregisterIRQ();
        lastCHSEQ   = SAR_CTRL_SFRS->SAR_CHAN_CFG.CHAN_SEQ_NUM;
        lastCH2     = SAR_CTRL_SFRS->SAR_CHAN_CFG.CH1_SEL;
        
        uint8_t groupNo       = led_no/TIME_DIV_BASE;
        uint8_t fragmentIndex = led_no - (groupNo*(uint8_t)TIME_DIV_BASE);
        
        if (fragmentIndex == 0U){
            GPIO_SetGPIOs((1U << GPIO_PORT_1) | (1U << GPIO_PORT_2) | (1U << GPIO_PORT_3) | (1U << GPIO_PORT_6)| (1U << GPIO_PORT_7), (1U << GPIO_PORT_2));
        }else if (fragmentIndex == 1U){
            GPIO_SetGPIOs((1U << GPIO_PORT_1) | (1U << GPIO_PORT_2) | (1U << GPIO_PORT_3) | (1U << GPIO_PORT_6)| (1U << GPIO_PORT_7), (1U << GPIO_PORT_1));
        }else if (fragmentIndex == 2U){
            GPIO_SetGPIOs((1U << GPIO_PORT_1) | (1U << GPIO_PORT_2) | (1U << GPIO_PORT_3) | (1U << GPIO_PORT_6)| (1U << GPIO_PORT_7), (1U << GPIO_PORT_3));
        }else if (fragmentIndex == 3U){
            GPIO_SetGPIOs((1U << GPIO_PORT_1) | (1U << GPIO_PORT_2) | (1U << GPIO_PORT_3) | (1U << GPIO_PORT_6)| (1U << GPIO_PORT_7), (1U << GPIO_PORT_6));
        }else{
            GPIO_SetGPIOs((1U << GPIO_PORT_1) | (1U << GPIO_PORT_2) | (1U << GPIO_PORT_3) | (1U << GPIO_PORT_6)| (1U << GPIO_PORT_7), (1U << GPIO_PORT_7));
        }
        SAR_CTRL_SFRS->SAR_CTRL.SAR_ENA_REQ = 0U;
        PWM_Reset();
        while(PWM_UpdateFinished() == TRUE){}
        SAR_CTRL_SFRS->SAR_CTRL.TRIGSEL = (uint8_t)ADC_TRIGGER_SOURCE_SOFT_INPUT;
        SAR_CTRL_SFRS->SAR_CTRL.SAR_ENA_REQ = 1U;
        SAR_CTRL_SFRS->SAR_CHAN_CFG.CHAN_SEQ_NUM  = (uint8_t)ADC_SQ_CH1;
        for (uint8_t channel = 0U; channel < 3U; channel++){
            uLedCode[channel] = 0U;
            SAR_CTRL_SFRS->SAR_CHAN_CFG.CH1_SEL = (uint8_t)ADC_CH_SEL_VBUCK_LED0 + groupNo * 3U + channel;
            for (uint16_t i = 0U; i < 16U; i++){
                SAR_CTRL_SFRS->SAR_INT.CLEAR.INT_SAR_DONE_CLR = 1U;
                SAR_CTRL_SFRS->SAR_INT.ENABLE.INT_SAR_DONE_ENA = 1U;
                SAR_CTRL_SFRS->SAR_CTRL.CONVERT = 1U;
                while(SAR_CTRL_SFRS->SAR_INT.STATUS.INT_SAR_DONE_STS == 0U){}
                uLedCode[channel] += SAR_CTRL_SFRS->ADC_DATA0.DATA0;
            }
            uLedCode[channel] = (uLedCode[channel] >> 3);
        }
        *pnRCode = (uint16_t)uLedCode[PHY_CHANNEL_R];
        *pnGCode = (uint16_t)uLedCode[PHY_CHANNEL_G];
        *pnBCode = (uint16_t)uLedCode[PHY_CHANNEL_B];
        
        
        SAR_CTRL_SFRS->SAR_CTRL.TRIGSEL =  (uint8_t)ADC_TRIGGER_SOURCE_PWM_PERIOD;
        SAR_CTRL_SFRS->SAR_CHAN_CFG.CHAN_SEQ_NUM = lastCHSEQ;
        SAR_CTRL_SFRS->SAR_CHAN_CFG.CH1_SEL = lastCH2;
        ADC_RegisterIRQ(NULL);
    }
  
}


/**
 * @brief The instance of PMW device
 */
void PWM_Init(PwmPrescaler_t divide, uint16_t period)
{
    for (uint8_t i = 0; i < PHY_LED_NUM; i++){
        vLedPNCode[i][0] = INVALID_ADC_CODE;
        vLedPNCode[i][1] = INVALID_ADC_CODE;
        vLedPNCode[i][2] = INVALID_ADC_CODE;
        ledIsActive[i] = FALSE;
    }
    vBuckCode = INVALID_ADC_CODE;
    vBattCode = INVALID_ADC_CODE;
    vTempCode = INVALID_ADC_CODE;
  
    IOCTRLA_SFRS->HWMODE = 0xFFFFFFU;
    IOCTRLA_SFRS->LEDPIN.VFW_ENA = 1U;        /* Enable 2mA led current source */
    IOCTRLA_SFRS->LEDPIN.SENSE_ENA = 1U;      /* LED Forward Voltage Sense Enable. Set to enable LED forward voltage sense module.*/
    PWM_SFRS->BASE.PERIOD = PWM_VALUE_MAX;
    PWM_SFRS->ENAREQ.ENAREQALL = 1U;
    /* set duty cycle to 0*/
    PWM_Reset();
    
    SAR_CTRL_SFRS->SAR_CHAN_CFG.CH1_SEL = (uint8_t)ADC_CH_SEL_PTAT_DIFF;
    SAR_CTRL_SFRS->SAR_CHAN_CFG.CH2_SEL = (uint8_t)ADC_CH_SEL_VBAT_GND;
    SAR_CTRL_SFRS->SAR_CHAN_CFG.CH3_SEL = (uint8_t)ADC_CH_SEL_VBUCK_GND;
    SAR_CTRL_SFRS->SAR_CHAN_CFG.CH4_SEL = (uint8_t)ADC_CH_SEL_PTAT_DIFF;
    SAR_CTRL_SFRS->SAR_CHAN_CFG.CHAN_SEQ_NUM = (uint8_t)ADC_SQ_CH1_CH2_CH3_CH4;
    ledConvertType = ADC_MEAS_TYPE_PTATBUCK_VBAT_VBUCK_PTATLED;
    
    SAR_CTRL_SFRS->SAR_INT.CLEAR.INT_SAR_DONE_CLR  = 1U;
    SAR_CTRL_SFRS->SAR_CTRL.CONVERT = 1U;
}

void PWM_Reset(void)
{
    uint8_t no;
    for (uint8_t i = 0U; i < PHY_LED_NUM; i++){
        no = i/5U;
        if( (no - ((no>>1U)*2U)) ==  0U ){
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



uint16_t ADC_GetLedPNCode(uint8_t ledNum, uint16_t *pnCodeR,uint16_t *pnCodeG,uint16_t *pnCodeB)
{   
    if (ledNum < PHY_LED_NUM){
      uint8_t ledNo = ledNumTable[ledNum];
__atomic_enter()
        *pnCodeR = vLedPNCode[ledNo][PHY_CHANNEL_R];
        *pnCodeG = vLedPNCode[ledNo][PHY_CHANNEL_G];
        *pnCodeB = vLedPNCode[ledNo][PHY_CHANNEL_B];
__atomic_exit()
        return 0U;
   }else{
        return 0xFFU;
   }
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


uint8_t PWM_LedIsOn(uint8_t ledNum)
{
    if (ledNum < PHY_LED_NUM){
        return ledIsActive[ledNumTable[ledNum]];
    }else{
        return 0U;
    }
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