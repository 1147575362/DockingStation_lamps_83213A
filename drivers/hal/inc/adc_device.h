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

#ifndef ADC_DEVICE_H__
#define ADC_DEVICE_H__

#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <rugbyHP.h>
#include <pdsTask.h>

#define INVALID_ADC_CODE         (0x7FFFU)

typedef enum {
  ADC_REF_VBG = 0U,     /* VBG = 1.22V  */
  ADC_REF_VBG_2TIMES,   /* 2* VBG       */
  ADC_REF_VDD_3V3,
}AdcReference_t;

typedef enum {
  ADC_TRIGGER_SOURCE_SOFT_INPUT  = 0x01U,
  ADC_TRIGGER_SOURCE_PWM_RISING  = 0x02U,
  ADC_TRIGGER_SOURCE_PWM_FALLING = 0x04U,
  ADC_TRIGGER_SOURCE_PWM_PERIOD  = 0x08U,
}AdcTriggerSource_t;

typedef enum {
  ADC_AVG_SEL_04_SAMPLES = 0U,
  ADC_AVG_SEL_08_SAMPLES,
  ADC_AVG_SEL_16_SAMPLES,
  ADC_AVG_SEL_32_SAMPLES
}AdcAvgSel_t;


typedef enum {
  ADC_AFE_GAIN_14_OF_32 = 0U,
  ADC_AFE_GAIN_22_OF_32,
  ADC_AFE_GAIN_31_OF_32,
}AdcAfeGain_t;

typedef enum {
  ADC_AFE_INPUT_INP_EXT_INN_EXT = 0U,
  ADC_AFE_INPUT_INP_BUF_INN_EXT,
  ADC_AFE_INPUT_INP_EXT_INN_BUF,
  ADC_AFE_INPUT_INP_BUF_INN_BUF,
}AdcAfeInputMode_t;


typedef enum {
  ADC_VBAT_GAIN_1_OF_14 = 0U,
  ADC_VBAT_GAIN_1_OF_28,
}AdcVbatGain_t;

typedef enum {
  ADC_CH_SEL_REF_REF= 0U,
  ADC_CH_SEL_REF_GND,
  ADC_CH_SEL_GND_REF,
  ADC_CH_SEL_GPIO6P_GPIO7N_DIFF,
  ADC_CH_SEL_PTAT_DIFF,
  ADC_CH_SEL_1V5_GND,
  ADC_CH_SEL_3V3_GND,
  ADC_CH_SEL_5V0_GND,   /*  1/4  gain  */
  ADC_CH_SEL_VBAT_GND,  /*  1/14 gain  */
  ADC_CH_SEL_GPIO6_GND,
  ADC_CH_SEL_GPIO7_GND,
  ADC_CH_SEL_GPIO8_GND,
  ADC_CH_SEL_GPIO1_GND,
  ADC_CH_SEL_GPIO2_GND,
  ADC_CH_SEL_GPIO3_GND,
  ADC_CH_SEL_GPIO4_GND,
  ADC_CH_SEL_GPIO5_GND,
  ADC_CH_SEL_GPIO9_GND,
  ADC_CH_SEL_VBUCK_GND,
  
  ADC_CH_SEL_VBUCK_LED0,
  ADC_CH_SEL_VBUCK_LED1,
  ADC_CH_SEL_VBUCK_LED2,
  ADC_CH_SEL_VBUCK_LED3,
  
  ADC_CH_SEL_VBUCK_LED4,
  ADC_CH_SEL_VBUCK_LED5,
  ADC_CH_SEL_VBUCK_LED6,
  ADC_CH_SEL_VBUCK_LED7,
  
  ADC_CH_SEL_VBUCK_LED8,
  ADC_CH_SEL_VBUCK_LED9,
  ADC_CH_SEL_VBUCK_LED10,
  ADC_CH_SEL_VBUCK_LED11,
  
  ADC_CH_SEL_VBUCK_LED12,
  ADC_CH_SEL_VBUCK_LED13,
  ADC_CH_SEL_VBUCK_LED14,
  ADC_CH_SEL_VBUCK_LED15,
  
  ADC_CH_SEL_VBUCK_LED16,
  ADC_CH_SEL_VBUCK_LED17,
  ADC_CH_SEL_VBUCK_LED18,
  ADC_CH_SEL_VBUCK_LED19,
  
  ADC_CH_SEL_VBUCK_LED20,
  ADC_CH_SEL_VBUCK_LED21,
  ADC_CH_SEL_VBUCK_LED22,
  ADC_CH_SEL_VBUCK_LED23,
  
}AdcChannelSelect_t;


typedef enum {
  ADC_LED_GAIN_2_OF_5 = 0U,
  ADC_LED_GAIN_1_OF_5,
}AdcLedGain_t;

typedef enum {
  PTAT_BUCK = 0U,
  PTAT_LED,
}PtatSelect_t;


typedef enum {
  ADC_SQ_CH1 = 0U,
  ADC_SQ_CH1_CH2,
  ADC_SQ_CH1_CH2_CH3,
  ADC_SQ_CH1_CH2_CH3_CH4,
}AdcSequence_t;


typedef enum {
  ADC_MEASURE_ITEM_NONE = 0U,
  ADC_MEASURE_ITEM_VBG,
  ADC_MEASURE_ITEM_BUCK,
  ADC_MEASURE_ITEM_VBAT,                
  ADC_MEASURE_ITEM_VDD1V5,
  ADC_MEASURE_ITEM_GPIO,
  ADC_MEASURE_ITEM_VTEMP,
  ADC_MEASURE_ITEM_VPTATBUCK_VBAT_VBUCK_VPTATLED,        
  ADC_MEASURE_ITEM_VLED_VBAT_VBUCK_VPTATLED,
  ADC_MEASURE_ITEM_LED,                   
}AdcMeasureItem_t;

typedef struct{
  AdcMeasureItem_t item;
  uint8_t channel;
}ADCMeasureParam_t;


typedef void (*adc_cb_func_t)(ADCMeasureParam_t item, uint16_t *const result);

void ADC_GeneralInit(void);
void ADC_Init(AdcMeasureItem_t item, uint8_t channel, LedType_t type, AdcTriggerSource_t triggerSource);
void ADC_UnInit(AdcMeasureItem_t item);
void ADC_RegisterIRQ(adc_cb_func_t callback);
void ADC_UnregisterIRQ(void);
void ADC_Start(void);


#endif /* __ADC_DEVICE_H__ */