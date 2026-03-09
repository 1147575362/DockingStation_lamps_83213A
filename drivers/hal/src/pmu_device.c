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
#include <stddef.h>
#include <pmu_device.h>
#include <errno.h>
#include <isrfuncs.h>
#include <gpio_device.h>
#include <lin_device.h>
#include <wica_sfr.h>
#include <pwm_device.h>
#include <buck_device.h>
#include <wdt_device.h>

   
static bor_cb_func_t bor_callback = NULL;
static __INLINE void WDTA_Start(void);


void BOR_Handler(void) 
{
    if ( bor_callback != NULL ){
        bor_callback();
    }
}



void PMU_RegisterBOR_IRQ(bor_cb_func_t callback)
{
    bor_callback = callback;
}
   
void PMU_BORInit(Bor1V5Thres_t lowThreshold, Bor3V3Thres_t highThreshold)
{
    PMUAPRE5V_SFRS->BOR.S_BOR_1V5 = (uint8_t)lowThreshold;
    PMUAPRE5V_SFRS->BOR.S_BOR_3V3 = (uint8_t)highThreshold;
    PMUAPRE5V_SFRS->BOR.BOR_1V5_ACTION = (uint8_t)PMU_BROWNOUT_RESET;
    PMUAPRE5V_SFRS->BOR.BOR_3V3_ACTION = (uint8_t)PMU_BROWNOUT_RESET;
    
    PMUAPRE5V_SFRS->BOR.BOR_LOCK = 1U;
    /*   CRGA_SFRS->OVTEMPCONFIG.DISTEMPSENS = 0U;  enable temperature sensor */
    EVTHOLD_SFRS->HOLD = 0U; /* clear  Lullaby_Handler after wake up  */
}

void PMU_WakeTimerInit(PMU_WAKEUP_TIMEER_MODE_t mode, PMU_WAKEUP_TIMEER_Interval_t interval)
{
  if (mode == WAKEUP_TIMEER_DISABLE){
      WICA_SFRS->CTRL.TIMERENA = 0U;
  }else{
      WICA_SFRS->CTRL.TIMERTAPSEL = (uint8_t)interval;
      WICA_SFRS->CTRL.TIMERENA    = 1U;
  }
}

void PMU_EnterDeepSleepMode(void)
{
    PMUAPRE5V_SFRS->CFG_ACCESS.CFG_ACCESS_KEY = 0x0BU;
    BUCK_DisableBuck();                 /* disable buck*/ 
    CRGA_SFRS->MODULERSTREQ     |= MODULE_SOFT_RESET_PWM | MODULE_SOFT_RESET_ADC | MODULE_SOFT_RESET_BUCK;
    LINS_SFRS->CTRL.SLEEP       = 0U;
    LINM_SFRS->CTRL.SLEEP       = 0U;
    
    PMUAPRE5V_SFRS->LINS.SWON             	= 0U;

    /* disbale temperature sensor when sleep */
    SAR_CTRL_SFRS->OT_CTRL_CFG.ADC_SAMP_OT_ENA = 0U;
    SAR_CTRL_SFRS->OT_CTRL_CFG.EN_OVERTEMP = 0U;
    
    PMUA_SFRS->VBATCTRL.BAT_UV_EN = 0U;
    PMUA_SFRS->VBATCTRL.BAT_OV_EN = 0U;
    
    SYSCTRLA_SFRS->BIAS.LEDBIASTRIMEN = 0U;
    PMUAPRE5V_SFRS->CTRL.PD1V5_ENA_HIBERNATE = 1U; 
    /* clear all of wake up flags */
    /* Disable linm wakeup detect and IRQ (LINM_REG_CTRL.SLEEP = 0U;)*/
    WICA_SFRS->CTRL.LINMIRQCLR = 1U;
    WICA_SFRS->CTRL.LINMIRQENA = 0U;
    /* Enable lins wakeup detect and IRQ  when LINS is in sleep mode(LINS_REG_CTRL.SLEEP = 1U;)  */
    WICA_SFRS->CTRL.LINSIRQCLR = 1U;
    WICA_SFRS->CTRL.LINSIRQENA = 1U;
    NVIC_EnableIRQ(WICA_IRQn);
    PMUAPRE5V_SFRS->LINS.LINS_RX_BIAS_BOOST = 0U;
#if (LIN_STACK_TYPE == LIN_STACK_TYPE_PRIVATE)  
    /* Set lin to sleep mode */
    if (LINS_SFRS->CTRL.SLEEP == 1U){
        LINS_SFRS->CTRL.SLEEP = 0U;
    }
    LINS_SFRS->CTRL.SLEEP = 1U;
    /*  Enter sleep mode  */
    PMUA_SFRS->CTRL.HIBERNATE = 1U;
#else
    NVIC_EnableIRQ(Lullaby_IRQn);               /*  Enable  Lullaby interrupt*/
    EVTHOLD_SFRS->HOLD = 1U;                    /*  Trigger ISR then entering sleep in ISR */
#endif
    /* Waiting to enter sleep mode */
    for(;;){
    }
}

/* Please make sure here and don't delete these functions!!!! or it would cause severe error*/
/* hibernate wake up interrupt ISR */
void Lullaby_Handler(void)
{
    /* Set lin to sleep mode */
    if (LINS_SFRS->CTRL.SLEEP == 1U){
        LINS_SFRS->CTRL.SLEEP = 0U;
    }
    LINS_SFRS->CTRL.SLEEP = 1U;
    /*  Enter sleep mode  */
    PMUA_SFRS->CTRL.HIBERNATE = 1U;
}


#pragma optimize=none
static __INLINE void WDTA_Start(void)
{
    WDTA_SFRS->CTRL.WDTA_TIMEOUT_SEL     = (uint8_t)WDTA_INTERVAL_9216MS;
    WDTA_SFRS->CLEAR = 0x3c574ad6U;
    WDTA_SFRS->STOP  = 0U;/* start wdt*/
}


void NMI_Handler(void)
{
    WDTA_Start();
    for(;;){}
}

void HardFault_Handler(void)
{
    WDTA_Start();
    for(;;){}
}

void SVC_Handler(void)
{
    WDTA_Start();
    for(;;){}
}

void PendSV_Handler(void)
{
    WDTA_Start();
    for(;;){}
}

void BTE_Handler(void)
{
    WDTA_Start();
    for(;;){}
}

void SDIO_Handler(void)
{
    WDTA_Start();
    for(;;){}
}


