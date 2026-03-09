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

#include <safetyMonitorTask.h>
#include <buck_device.h>
#include <pmu_device.h>

#define OVER_TEMPERATURE_HIGH           (130)   /*celsius degree */
#define OVER_TEMPERATURE_RELEASE        (OVER_TEMPERATURE_HIGH - 10)   /*celsius degree */


#define LED_PN_SHORT_VOLT                       (500)   /* mV */
#define LED_PN_OPEM_VOLT                        (4000)  /* mV */

#define INTENSITY_RATIO_TYPICAL                 (1024U)                                 /* 100% of max intensity */
#define INTENSITY_RATIO_HIGH_TEMPERATURE        (INTENSITY_RATIO_TYPICAL*4U/5U)         /* 80% of max intensity */
#define INTENSITY_RATIO_BATT_ISSUE              (0U)                                    /* 0% of max intensity */


static TaskState_t safeMonitorState = TASK_STATE_INIT;
static BatteryState_t battState = BATT_STATE_NORMAL;
static BatteryState_t battLastState = BATT_STATE_NORMAL;
static ChipTemperatureState_t chipTemperatureState = CHIP_TEMPERATURE_STATE_NORMAL;
static ChipTemperatureState_t chipTemperatureLastState = CHIP_TEMPERATURE_STATE_NORMAL;
static LedPNState_t ledPNState[LED_NUM];

static void UnderVoltageProtection_ISR(void);
static void OverVoltageProtection_ISR(void);
static void safetyHandle(void);
static void safetyInit(void);
static void safeMonitorTimerExpired(SoftTimer_t *timer);

static SoftTimer_t safeMonitorTimer = {
    .mode     = TIMER_PERIODIC_MODE,
    .interval = 200U,
    .handler  = safeMonitorTimerExpired
};


static void safeMonitorTimerExpired(SoftTimer_t *timer)
{
    TM_PostTask(TASK_ID_SAFETY_MONITOR);
}

void SAFM_TaskHandler(void)
{
    switch(safeMonitorState){
    case TASK_STATE_INIT:
      safetyInit();
      SoftTimer_Start(&safeMonitorTimer);
      safeMonitorState = TASK_STATE_ACTIVE;
      break;
    case TASK_STATE_ACTIVE:
      safetyHandle();
      break;
    default:
      break;
    }
}


static void safetyHandle(void)
{
    /*  Led PN Voltage Protection Handling     */
    int16_t pnVoltR,pnVoltG,pnVoltB;
    uint8_t pnState;
    for (uint8_t ledIndex = 0U; ledIndex < LED_NUM; ledIndex++){
        pnState = 0U;
        MES_GetRunTimeLedPNVolt((LedNum_t)ledIndex,&pnVoltR, &pnVoltG,&pnVoltB);
        if(PWM_LedIsOn(ledIndex) == 0U){
            pnVoltR = INVALID_PARAM;
            pnVoltG = INVALID_PARAM;
            pnVoltB = INVALID_PARAM;            
        } 
        if ( (pnVoltR != (int16_t)INVALID_PARAM) && (pnVoltG != (int16_t)INVALID_PARAM)  && (pnVoltB != (int16_t)INVALID_PARAM) ){
            if ( (pnVoltR <= LED_PN_SHORT_VOLT) || (pnVoltG <= LED_PN_SHORT_VOLT) || (pnVoltB <= LED_PN_SHORT_VOLT)){
                pnState = 1U;           /* short  */
            }
            if ( (pnVoltR >= LED_PN_OPEM_VOLT) || (pnVoltG >= LED_PN_OPEM_VOLT) || (pnVoltB >= LED_PN_OPEM_VOLT)){
                pnState |= (1U << 1);   /* open  */
            }
        }
        ledPNState[ledIndex] = (LedPNState_t)pnState;
    }
    
    /*  Temperature Protection Handling     */
    int16_t chipTemperature;
    if (MES_GetChipTemperature(&chipTemperature) == 0){
        if (chipTemperature >= OVER_TEMPERATURE_HIGH){
            chipTemperatureState = CHIP_TEMPERATURE_STATE_HIGH;
        }else{
            if (chipTemperature <= OVER_TEMPERATURE_RELEASE){
                chipTemperatureState = CHIP_TEMPERATURE_STATE_NORMAL;
            }
        }
    }
    
    /*  Battery Voltage Protection Handling     */
    battState = (BatteryState_t)HW_PROT_GetBattVoltState();
    if ((chipTemperatureLastState != chipTemperatureState) || (battLastState != battState)){
        chipTemperatureLastState = chipTemperatureState;
        battLastState = battState;
        if (battState == BATT_STATE_LOW){
            /* Turn off LED when Battery voltage is low */
            (void)CLM_SetLimitedMaxIntensity(INTENSITY_RATIO_BATT_ISSUE);
            TM_PostTask(TASK_ID_COLOR_COMPENSATION);
            /* LED PN detection disbaled when Voltage is low */
            for (uint8_t ledIndex = 0U; ledIndex < LED_NUM; ledIndex++){
                ledPNState[ledIndex] = LED_PN_NORMAL;
            }
            CRGA_SFRS->MODULERSTREQ |= MODULE_SOFT_RESET_BUCK;/* turn off buck when Vbat is low! */
        }else{
            if (chipTemperatureState == CHIP_TEMPERATURE_STATE_HIGH){
                (void)CLM_SetLimitedMaxIntensity(INTENSITY_RATIO_HIGH_TEMPERATURE);
                TM_PostTask(TASK_ID_COLOR_COMPENSATION);
            }else{
                (void)CLM_SetLimitedMaxIntensity(INTENSITY_RATIO_TYPICAL);
                TM_PostTask(TASK_ID_COLOR_COMPENSATION);
            }
            BUCK_Init(BUCK_OUTPUT_3910mV);
            /* Restart OV UV ISR   */
            PMUA_SFRS->PMUIRQ.CLEAR.OV  = 1U;
            PMUA_SFRS->PMUIRQ.CLEAR.UV  = 1U;
            NVIC_EnableIRQ(VBAT_IRQn);
        }
    }

}


static void UnderVoltageProtection_ISR(void)
{
    TM_PostTask(TASK_ID_SAFETY_MONITOR);
}


static void OverVoltageProtection_ISR(void)
{
    TM_PostTask(TASK_ID_SAFETY_MONITOR);
}


static void safetyInit(void)
{
    for (uint8_t ledIndex = 0U; ledIndex < LED_NUM; ledIndex++){
        ledPNState[ledIndex] = LED_PN_NORMAL;
    }
    HW_PROT_SetUnderVoltage(UV_VOLT_5_505V,UV_HYS_04_PERCENT, VOLT_DEBOUNCE_TIME_10ms, VOLT_DEBOUNCE_TIME_10ms);
    HW_PROT_RegisterUnderVoltageIRQ(UnderVoltageProtection_ISR);
    
    HW_PROT_SetOverVoltage(OV_VOLT_20_64V, OV_HYS_12_PERCENT,   VOLT_DEBOUNCE_TIME_10ms, VOLT_DEBOUNCE_TIME_10ms);
    HW_PROT_RegisterOverVoltageIRQ(OverVoltageProtection_ISR);
}


BatteryState_t SAFM_GetBatteryState(void)
{
    return battState;
}


ChipTemperatureState_t SAFM_GetChipTemperatureState(void)
{
    return chipTemperatureState;
}


LedPNState_t SAFM_GetLedPNStateState(LedNum_t ledIndex)
{
    return ledPNState[(uint8_t)ledIndex];
}
