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

#include <adc_device.h>
#include <isrfuncs.h>
static ADCMeasureParam_t adcMeasParamm = {ADC_MEASURE_ITEM_NONE, 0};
static adc_cb_func_t adcCallback = NULL;
static uint16_t adcResult[4];


void ADC_Handler(void)
{
    if (adcCallback !=NULL){
        adcResult[0] = SAR_CTRL_SFRS->ADC_DATA0.DATA0;
        adcResult[1] = SAR_CTRL_SFRS->ADC_DATA0.DATA1;
        adcResult[2] = SAR_CTRL_SFRS->ADC_DATA1.DATA2;
        adcResult[3] = SAR_CTRL_SFRS->ADC_DATA1.DATA3;

        if((adcResult[0]&0x800U) == 0x800U){
          adcResult[0] = 0;
        }
        if((adcResult[1]&0x800U) == 0x800U){
          adcResult[1] = 0;
        } 
        if((adcResult[2]&0x800U) == 0x800U){
          adcResult[2] = 0;
        }
        if((adcResult[3]&0x800U) == 0x800U){
          adcResult[3] = 0;
        }        
        
        adcCallback(adcMeasParamm, adcResult);
    }
    

    SAR_CTRL_SFRS->SAR_INT.CLEAR.INT_SAR_DONE_CLR = 1U;
    SAR_CTRL_SFRS->SAR_CTRL.SAR_ENA_REQ = 0U;
}

void ADC_GeneralInit(void)
{
#if SYS_MAIN_CLOCK_DIV == CLOCK_DIV_1
    SAR_CTRL_SFRS->SAR_CFG2.SAR_CLK_DIV         = 3U;
#else
    #error MAIN_CPU_CLOCK MUST BE at 16MHz system colck!
#endif 
  
    SAR_CTRL_SFRS->SAR_CTRL.TRIGSEL             = (uint8_t)ADC_TRIGGER_SOURCE_SOFT_INPUT;
    SAR_CTRL_SFRS->SAR_CFG2.TRIG_DLY            = 4U;         /* 0.25us/count @4MHz*/
    SAR_CTRL_SFRS->SAR_CFG2.TRIG_DLY_TS         = 32U;

    SAR_CTRL_SFRS->SAR_CFG0.SAR_INPUT_MODE_CH1  = (uint8_t)ADC_AFE_INPUT_INP_EXT_INN_EXT;     /* LED PN/PTAT of Buck          */
    SAR_CTRL_SFRS->SAR_CFG0.SAR_INPUT_MODE_CH2  = (uint8_t)ADC_AFE_INPUT_INP_BUF_INN_EXT;     /* VBAT                         */
    SAR_CTRL_SFRS->SAR_CFG0.SAR_INPUT_MODE_CH3  = (uint8_t)ADC_AFE_INPUT_INP_BUF_INN_EXT;     /* VBUCK                        */
    SAR_CTRL_SFRS->SAR_CFG0.SAR_INPUT_MODE_CH4  = (uint8_t)ADC_AFE_INPUT_INP_BUF_INN_BUF;     /* PTAT of LED                  */
    
    SAR_CTRL_SFRS->SAR_CFG0.ADC_VREF_SEL_CH1    = (uint8_t)ADC_REF_VBG_2TIMES;
    SAR_CTRL_SFRS->SAR_CFG0.ADC_VREF_SEL_CH2    = (uint8_t)ADC_REF_VBG_2TIMES;
    SAR_CTRL_SFRS->SAR_CFG0.ADC_VREF_SEL_CH3    = (uint8_t)ADC_REF_VBG_2TIMES;
    SAR_CTRL_SFRS->SAR_CFG0.ADC_VREF_SEL_CH4    = (uint8_t)ADC_REF_VBG;
    
    SAR_CTRL_SFRS->SAR_CFG0.SAR_INPUT_GAIN_CH1  = (uint8_t)ADC_AFE_GAIN_31_OF_32;
    SAR_CTRL_SFRS->SAR_CFG0.SAR_INPUT_GAIN_CH2  = (uint8_t)ADC_AFE_GAIN_31_OF_32;
    SAR_CTRL_SFRS->SAR_CFG0.SAR_INPUT_GAIN_CH3  = (uint8_t)ADC_AFE_GAIN_31_OF_32;
    SAR_CTRL_SFRS->SAR_CFG0.SAR_INPUT_GAIN_CH4  = (uint8_t)ADC_AFE_GAIN_31_OF_32;
    
    SAR_CTRL_SFRS->SAR_CFG1.SAMPCYC_CH1         = 4U;/* 0.25us/count @4MHz*/
    SAR_CTRL_SFRS->SAR_CFG1.SAMPCYC_CH2         = 4U;/* 0.25us/count @4MHz*/
    SAR_CTRL_SFRS->SAR_CFG1.SAMPCYC_CH3         = 4U;/* 0.25us/count @4MHz*/
    SAR_CTRL_SFRS->SAR_CFG1.SAMPCYC_CH4         = 4U;/* 0.25us/count @4MHz*/
    
    SAR_CTRL_SFRS->SAR_CFG2.TS_SEL_CH4          = (uint8_t)PTAT_LED;
    IOCTRLA_SFRS->LEDPIN.GAIN_SEL               = (uint8_t)ADC_LED_GAIN_2_OF_5;
    SAR_CTRL_SFRS->SAR_CHAN_CFG.TEST_BATTERY_GAIN_CHOOSE = (uint8_t)ADC_VBAT_GAIN_1_OF_14;
    
    SAR_CTRL_SFRS->OT_CTRL_CFG.ADC_SAMP_OT_ENA = 1U;
    SAR_CTRL_SFRS->OT_CTRL_CFG.EN_OVERTEMP = 1U;
    
    SAR_CTRL_SFRS->SAR_CTRL.SAR_AFE_EN          = 1U;
    SAR_CTRL_SFRS->SAR_CTRL.SAR_PREAMP_EN       = 1U;
    SAR_CTRL_SFRS->SAR_CTRL.SAR_ENA_REQ         = 1U;       /* SAR ADC Enable. Set to enable the SAR analog & digital part */
    

}


void ADC_Init(AdcMeasureItem_t item, uint8_t channel, LedType_t type, AdcTriggerSource_t triggerSource)
{
    adcMeasParamm.item       = item;
    adcMeasParamm.channel    = channel;
    
    switch(item){
    case ADC_MEASURE_ITEM_VPTATBUCK_VBAT_VBUCK_VPTATLED:
      SAR_CTRL_SFRS->SAR_CHAN_CFG.TEST_BATTERY_GAIN_CHOOSE      = (uint8_t)ADC_VBAT_GAIN_1_OF_14;
      SAR_CTRL_SFRS->SAR_CHAN_CFG.CH1_SEL       = (uint8_t)ADC_CH_SEL_PTAT_DIFF;
      SAR_CTRL_SFRS->SAR_CHAN_CFG.CH2_SEL       = (uint8_t)ADC_CH_SEL_VBAT_GND;
      SAR_CTRL_SFRS->SAR_CHAN_CFG.CH3_SEL       = (uint8_t)ADC_CH_SEL_VBUCK_GND;
      SAR_CTRL_SFRS->SAR_CHAN_CFG.CH4_SEL       = (uint8_t)ADC_CH_SEL_PTAT_DIFF;
      SAR_CTRL_SFRS->SAR_CHAN_CFG.CHAN_SEQ_NUM  = (uint8_t)ADC_SQ_CH1_CH2_CH3_CH4;
      SAR_CTRL_SFRS->SAR_CTRL.TRIGSEL           = (uint8_t)triggerSource;
      break;

    case ADC_MEASURE_ITEM_VLED_VBAT_VBUCK_VPTATLED:      /*LED: 2/5x, BUCK: 1/4x, VBAT:1/14x*/
      IOCTRLA_SFRS->LEDPIN.VFW_ENA      = 1U;           /* Enable 2mA led current source */
      IOCTRLA_SFRS->LEDPIN.SENSE_ENA    = 1U;           /* LED Forward Voltage Sense Enable. Set to enable LED forward voltage sense module.*/
      
      if (type == LED_RED){
          SAR_CTRL_SFRS->SAR_CHAN_CFG.CH1_SEL   = (uint8_t)ADC_CH_SEL_VBUCK_LED0 + (uint8_t)PHY_CHANNEL_R(channel);/*  measure red PN volt */
      }else if (type == LED_GREEN){
          SAR_CTRL_SFRS->SAR_CHAN_CFG.CH1_SEL   = (uint8_t)ADC_CH_SEL_VBUCK_LED0 + (uint8_t)PHY_CHANNEL_G(channel);/*  measure green PN volt */
      }else{
          SAR_CTRL_SFRS->SAR_CHAN_CFG.CH1_SEL   = (uint8_t)ADC_CH_SEL_VBUCK_LED0 + (uint8_t)PHY_CHANNEL_B(channel);/*  measure blue PN volt */
      }
      SAR_CTRL_SFRS->SAR_CHAN_CFG.CH2_SEL       = (uint8_t)ADC_CH_SEL_VBAT_GND;
      SAR_CTRL_SFRS->SAR_CHAN_CFG.CH3_SEL       = (uint8_t)ADC_CH_SEL_VBUCK_GND;
      SAR_CTRL_SFRS->SAR_CHAN_CFG.CH4_SEL       = (uint8_t)ADC_CH_SEL_PTAT_DIFF;
      SAR_CTRL_SFRS->SAR_CHAN_CFG.CHAN_SEQ_NUM  = (uint8_t)ADC_SQ_CH1_CH2_CH3_CH4;
      SAR_CTRL_SFRS->SAR_CTRL.TRIGSEL           = (uint8_t)triggerSource;
      break;
      
    case ADC_MEASURE_ITEM_VBAT:/* 1/14x  */
      SAR_CTRL_SFRS->SAR_CFG0.SAR_INPUT_MODE_CH1= (uint8_t)ADC_AFE_INPUT_INP_BUF_INN_EXT;   
      SAR_CTRL_SFRS->SAR_CFG0.ADC_VREF_SEL_CH1  = (uint8_t)ADC_REF_VBG_2TIMES;
      SAR_CTRL_SFRS->SAR_CFG0.SAR_INPUT_GAIN_CH1= (uint8_t)ADC_AFE_GAIN_31_OF_32;
      SAR_CTRL_SFRS->SAR_CFG1.SAMPCYC_CH1       = 5U;/* 0.25us/count @4MHz*/
      
      SAR_CTRL_SFRS->SAR_CHAN_CFG.TEST_BATTERY_GAIN_CHOOSE  = (uint8_t)ADC_VBAT_GAIN_1_OF_14;
      
      SAR_CTRL_SFRS->SAR_CHAN_CFG.CH1_SEL       = (uint8_t)ADC_CH_SEL_VBAT_GND;
      SAR_CTRL_SFRS->SAR_CHAN_CFG.CHAN_SEQ_NUM  = (uint8_t)ADC_SQ_CH1;
      SAR_CTRL_SFRS->SAR_CTRL.TRIGSEL           = (uint8_t)triggerSource;
      break;
    case ADC_MEASURE_ITEM_LED:
      IOCTRLA_SFRS->LEDPIN.VFW_ENA      = 1U;           /* Enable 2mA led current source */
      IOCTRLA_SFRS->LEDPIN.SENSE_ENA    = 1U;           /* LED Forward Voltage Sense Enable. Set to enable LED forward voltage sense module.*/
      
      SAR_CTRL_SFRS->SAR_CFG0.SAR_INPUT_MODE_CH1= (uint8_t)ADC_AFE_INPUT_INP_EXT_INN_EXT;    
      SAR_CTRL_SFRS->SAR_CFG0.ADC_VREF_SEL_CH1  = (uint8_t)ADC_REF_VBG_2TIMES;
      SAR_CTRL_SFRS->SAR_CFG0.SAR_INPUT_GAIN_CH1= (uint8_t)ADC_AFE_GAIN_31_OF_32;
      SAR_CTRL_SFRS->SAR_CFG1.SAMPCYC_CH1       = 5U;/* 0.25us/count @4MHz*/
      
      IOCTRLA_SFRS->LEDPIN.GAIN_SEL             = (uint8_t)ADC_LED_GAIN_2_OF_5;
      if (type == LED_RED){
          SAR_CTRL_SFRS->SAR_CHAN_CFG.CH1_SEL   = (uint8_t)ADC_CH_SEL_VBUCK_LED0 + (uint8_t)PHY_CHANNEL_R(channel);/*  measure red PN volt */
      }else if (type == LED_GREEN){
          SAR_CTRL_SFRS->SAR_CHAN_CFG.CH1_SEL   = (uint8_t)ADC_CH_SEL_VBUCK_LED0 + (uint8_t)PHY_CHANNEL_G(channel);/*  measure green PN volt */
      }else{
          SAR_CTRL_SFRS->SAR_CHAN_CFG.CH1_SEL   = (uint8_t)ADC_CH_SEL_VBUCK_LED0 + (uint8_t)PHY_CHANNEL_B(channel);/*  measure blue PN volt */
      }
      SAR_CTRL_SFRS->SAR_CHAN_CFG.CHAN_SEQ_NUM  = (uint8_t)ADC_SQ_CH1;
      SAR_CTRL_SFRS->SAR_CTRL.TRIGSEL           = (uint8_t)triggerSource;
      break;
    case ADC_MEASURE_ITEM_VBG:  /*1x */
      SAR_CTRL_SFRS->SAR_CFG0.SAR_INPUT_MODE_CH1= (uint8_t)ADC_AFE_INPUT_INP_BUF_INN_EXT;     
      SAR_CTRL_SFRS->SAR_CFG0.ADC_VREF_SEL_CH1  = (uint8_t)ADC_REF_VBG;
      SAR_CTRL_SFRS->SAR_CFG0.SAR_INPUT_GAIN_CH1= (uint8_t)ADC_AFE_GAIN_31_OF_32;
      SAR_CTRL_SFRS->SAR_CFG1.SAMPCYC_CH1       = 5U;/* 0.25us/count @4MHz*/
      
      SAR_CTRL_SFRS->SAR_CHAN_CFG.CH1_SEL       = (uint8_t)ADC_CH_SEL_REF_GND;
      SAR_CTRL_SFRS->SAR_CHAN_CFG.CHAN_SEQ_NUM  = (uint8_t)ADC_SQ_CH1;
      SAR_CTRL_SFRS->SAR_CTRL.TRIGSEL           = (uint8_t)triggerSource;
      break; 
      
    case ADC_MEASURE_ITEM_BUCK: /*1/4x */
      SAR_CTRL_SFRS->SAR_CFG0.SAR_INPUT_MODE_CH1= (uint8_t)ADC_AFE_INPUT_INP_BUF_INN_EXT;     
      SAR_CTRL_SFRS->SAR_CFG0.ADC_VREF_SEL_CH1  = (uint8_t)ADC_REF_VBG_2TIMES;
      SAR_CTRL_SFRS->SAR_CFG0.SAR_INPUT_GAIN_CH1= (uint8_t)ADC_AFE_GAIN_31_OF_32;
      SAR_CTRL_SFRS->SAR_CFG1.SAMPCYC_CH1       = 5U;/* 0.25us/count @4MHz*/
      
      SAR_CTRL_SFRS->SAR_CHAN_CFG.CH1_SEL       = (uint8_t)ADC_CH_SEL_VBUCK_GND;
      SAR_CTRL_SFRS->SAR_CHAN_CFG.CHAN_SEQ_NUM  = (uint8_t)ADC_SQ_CH1;
      SAR_CTRL_SFRS->SAR_CTRL.TRIGSEL           = (uint8_t)triggerSource;
      break;
      
    case ADC_MEASURE_ITEM_VDD1V5:/* 1/3x */
      SAR_CTRL_SFRS->SAR_CFG0.SAR_INPUT_MODE_CH1= (uint8_t)ADC_AFE_INPUT_INP_BUF_INN_EXT;     
      SAR_CTRL_SFRS->SAR_CFG0.ADC_VREF_SEL_CH1  = (uint8_t)ADC_REF_VBG_2TIMES;
      SAR_CTRL_SFRS->SAR_CFG0.SAR_INPUT_GAIN_CH1= (uint8_t)ADC_AFE_GAIN_31_OF_32;
      SAR_CTRL_SFRS->SAR_CFG1.SAMPCYC_CH1       = 5U;/* 0.25us/count @4MHz*/
      
      SAR_CTRL_SFRS->SAR_CHAN_CFG.CH1_SEL       = (uint8_t)ADC_CH_SEL_1V5_GND;
      SAR_CTRL_SFRS->SAR_CHAN_CFG.CHAN_SEQ_NUM  = (uint8_t)ADC_SQ_CH1;
      SAR_CTRL_SFRS->SAR_CTRL.TRIGSEL           = (uint8_t)triggerSource;
      break;
      
    case ADC_MEASURE_ITEM_GPIO:/*1/3x,*/
      SAR_CTRL_SFRS->SAR_CFG0.SAR_INPUT_MODE_CH1= (uint8_t)ADC_AFE_INPUT_INP_BUF_INN_EXT;     
      SAR_CTRL_SFRS->SAR_CFG0.ADC_VREF_SEL_CH1  = (uint8_t)ADC_REF_VBG_2TIMES;
      SAR_CTRL_SFRS->SAR_CFG0.SAR_INPUT_GAIN_CH1= (uint8_t)ADC_AFE_GAIN_31_OF_32;
      SAR_CTRL_SFRS->SAR_CFG1.SAMPCYC_CH1       = 5U;/* 0.25us/count @4MHz*/
      
      SAR_CTRL_SFRS->SAR_CHAN_CFG.CH1_SEL       = (uint8_t)ADC_CH_SEL_GPIO6_GND;
      SAR_CTRL_SFRS->SAR_CHAN_CFG.CHAN_SEQ_NUM  = (uint8_t)ADC_SQ_CH1;
      SAR_CTRL_SFRS->SAR_CTRL.TRIGSEL           = (uint8_t)triggerSource;
      break;

    default:
      break;
    }
}

void ADC_UnInit(AdcMeasureItem_t item)
{
    
  
}

/**
 * @brief Register a callback function to ADC interrupt handler.
 *
 * @param dev Pointer to the ADC device.
 * @param cb The callback function to be registered.
 * @return 0 for success or error code upon a failure.
 */
void ADC_RegisterIRQ(adc_cb_func_t callback)
{
    adcCallback = callback;
    SAR_CTRL_SFRS->SAR_INT.CLEAR.INT_SAR_DONE_CLR = 1U;
    SAR_CTRL_SFRS->SAR_INT.ENABLE.INT_SAR_DONE_ENA = 1U;
    NVIC_EnableIRQ(ADC_IRQn);
    
}

/**
 * @brief Unregister a callback function to ADC interrupt handler.
 *
 * @param dev Pointer to the ADC device.
 * @param cb The callback function to be unregistered.
 * @return 0 for success or error code upon a failure.
 */
void ADC_UnregisterIRQ(void)
{
    adcCallback = NULL;
    SAR_CTRL_SFRS->SAR_INT.CLEAR.INT_SAR_DONE_CLR  = 1U;
    SAR_CTRL_SFRS->SAR_INT.ENABLE.INT_SAR_DONE_ENA = 0U;
    NVIC_DisableIRQ(ADC_IRQn);
}

void ADC_Start(void)
{
    SAR_CTRL_SFRS->SAR_INT.CLEAR.INT_SAR_DONE_CLR  = 1U;
    SAR_CTRL_SFRS->SAR_CTRL.SAR_ENA_REQ = 1U;
    SAR_CTRL_SFRS->SAR_CTRL.CONVERT = 1U;
}


#endif