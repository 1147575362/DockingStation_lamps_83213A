


#include <appConfig.h>

#if (LED_NUM > 8U)

#include <adc_device.h>

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

    SAR_CTRL_SFRS->SAR_CFG0.SAR_INPUT_MODE_CH1  = (uint8_t)ADC_AFE_INPUT_INP_EXT_INN_EXT;     /* LED PN/PTAT of Buck     */
    SAR_CTRL_SFRS->SAR_CFG0.SAR_INPUT_MODE_CH2  = (uint8_t)ADC_AFE_INPUT_INP_BUF_INN_EXT;     /* VBAT                    */
    SAR_CTRL_SFRS->SAR_CFG0.SAR_INPUT_MODE_CH3  = (uint8_t)ADC_AFE_INPUT_INP_BUF_INN_EXT;     /* VBUCK                   */
    SAR_CTRL_SFRS->SAR_CFG0.SAR_INPUT_MODE_CH4  = (uint8_t)ADC_AFE_INPUT_INP_BUF_INN_BUF;     /* PTAT of LED             */
    
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
    
    SAR_CTRL_SFRS->SAR_CFG2.TS_SEL_CH1          = (uint8_t)PTAT_BUCK;
    SAR_CTRL_SFRS->SAR_CFG2.TS_SEL_CH4          = (uint8_t)PTAT_LED;
    IOCTRLA_SFRS->LEDPIN.GAIN_SEL               = (uint8_t)ADC_LED_GAIN_2_OF_5;
    SAR_CTRL_SFRS->SAR_CHAN_CFG.TEST_BATTERY_GAIN_CHOOSE = (uint8_t)ADC_VBAT_GAIN_1_OF_14;
    
    SAR_CTRL_SFRS->OT_CTRL_CFG.ADC_SAMP_OT_ENA  = 1U;
    SAR_CTRL_SFRS->OT_CTRL_CFG.EN_OVERTEMP      = 1U;
    
    SAR_CTRL_SFRS->SAR_CTRL.SAR_AFE_EN          = 1U;
    SAR_CTRL_SFRS->SAR_CTRL.SAR_PREAMP_EN       = 1U;
    SAR_CTRL_SFRS->SAR_CTRL.SAR_ENA_REQ         = 1U;       /* SAR ADC Enable. Set to enable the SAR analog & digital part */
    
    IOCTRLA_SFRS->LEDPIN.VFW_ENA      = 1U;           /* Enable 2mA led current source */
    IOCTRLA_SFRS->LEDPIN.SENSE_ENA    = 1U;           /* LED Forward Voltage Sense Enable. Set to enable LED forward voltage sense module.*/
    
    SAR_CTRL_SFRS->SAR_CHAN_CFG.TEST_BATTERY_GAIN_CHOOSE      = (uint8_t)ADC_VBAT_GAIN_1_OF_14;
    
    SAR_CTRL_SFRS->SAR_CHAN_CFG.CH1_SEL       = (uint8_t)ADC_CH_SEL_PTAT_DIFF;
    SAR_CTRL_SFRS->SAR_CHAN_CFG.CH2_SEL       = (uint8_t)ADC_CH_SEL_VBAT_GND;
    SAR_CTRL_SFRS->SAR_CHAN_CFG.CH3_SEL       = (uint8_t)ADC_CH_SEL_VBUCK_GND;
    SAR_CTRL_SFRS->SAR_CHAN_CFG.CH4_SEL       = (uint8_t)ADC_CH_SEL_PTAT_DIFF;
    SAR_CTRL_SFRS->SAR_CHAN_CFG.CHAN_SEQ_NUM  = (uint8_t)ADC_SQ_CH1_CH2_CH3_CH4;
  
    SAR_CTRL_SFRS->SAR_CTRL.TRIGSEL             = (uint8_t)ADC_TRIGGER_SOURCE_PWM_PERIOD;
    
    ADC_RegisterIRQ(NULL);
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
    SAR_CTRL_SFRS->SAR_INT.CLEAR.INT_SAR_DONE_CLR  = 1U;
    SAR_CTRL_SFRS->SAR_INT.ENABLE.INT_SAR_DONE_ENA = 0U;
    NVIC_DisableIRQ(ADC_IRQn);
}

void ADC_Start(void)
{
    SAR_CTRL_SFRS->SAR_INT.CLEAR.INT_SAR_DONE_CLR  = 1U;
    SAR_CTRL_SFRS->SAR_CTRL.CONVERT = 1U;
}

































#endif