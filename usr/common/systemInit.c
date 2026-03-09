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

#include <systemInit.h>
#include <rugbyHP.h>
#include <wdt_device.h>
#include <buck_device.h>
#include <pwmAux_device.h>
#include <uart_device.h>
#include <linSlaveTask.h>
#include <linStackTask.h>
#include <applicationTask.h>

void leds_driver_Init(void);
void gpios_init(void);
void pmu_init(void);


void leds_driver_Init(void)
{
    (void)PWM_SetLedChannelCurrent();
#if LED_NUM > 8U
    ADC_GeneralInit();
#endif
    PWM_Init(PWM_PRESCALER_DIVIDE_1, PWM_VALUE_MAX);
}


void gpios_init(void)
{
    /* set gpios to input with power up resistor */
#if LED_NUM > 8U
    GPIO_Init(GPIO_PORT_1, GPIO_DIR_OUTPUT,GPIO_PULL_NONE); GPIO_Set(GPIO_PORT_1, GPIO_LOW);
    GPIO_Init(GPIO_PORT_2, GPIO_DIR_OUTPUT,GPIO_PULL_NONE); GPIO_Set(GPIO_PORT_2, GPIO_LOW);
    GPIO_Init(GPIO_PORT_3, GPIO_DIR_OUTPUT,GPIO_PULL_NONE); GPIO_Set(GPIO_PORT_3, GPIO_LOW);
    GPIO_Init(GPIO_PORT_6, GPIO_DIR_OUTPUT,GPIO_PULL_NONE); GPIO_Set(GPIO_PORT_6, GPIO_LOW);
    GPIO_Init(GPIO_PORT_7, GPIO_DIR_OUTPUT,GPIO_PULL_NONE); GPIO_Set(GPIO_PORT_7, GPIO_LOW);
#endif
//    
//    GPIO_Init(GPIO_PORT_8, GPIO_DIR_OUTPUT,GPIO_PULL_NONE);
//    GPIO_Set(GPIO_PORT_8, GPIO_LOW);
//
//#if LIN_STACK_TYPE == LIN_STACK_TYPE_LIN2_2A
//    if(GPIO_SFRS->GPIO_CFG[(uint8_t)GPIO_PORT_1].ACTDET != 0U){
//        GPIO_SFRS->GPIO_CFG[(uint8_t)GPIO_PORT_1].CLR = 1U;
//        LINS_EventTriggered_ISR();
//    }
//    GPIO_RegisterIRQ(GPIO_PORT_1, GPIO_EDGE_FALLING, LINS_EventTriggered_ISR);
//#endif
}


void pmu_init(void)
{
    PMUAPRE5V_SFRS->CTRL.TRIM_BG_V2I    = HWCFG_Get1V2TrimCode();
    PMUA_SFRS->PMUTRIM.VDD1V5_LDO_TRIM  = HWCFG_GetLDO1V5TrimCode();
    PMUA_SFRS->PMUTRIM.VDD3V3_LDO_TRIM  = HWCFG_GetLDO3V3TrimCode();
    PMUA_SFRS->PMUTRIM.OCP_CTRL_1V5     = HWCFG_GetLDO1V5OCPTrimCode();
    PMUA_SFRS->PMUTRIM.OCP_CTRL_3V3     = HWCFG_GetLDO3V3OCPTrimCode();
    PMUA_SFRS->PMUTRIM.OCP_CTRL_5V0     = HWCFG_GetLDO5V0OCPTrimCode();
    /* Init set BOR voltage level for cpu low voltage safety*/
    PMU_BORInit(BOR_1V5_THRS_1393mV, BOR_3V3_THRS_2763mV);
}


void SYS_Init(void)
{
    SYSCTRLA_SFRS->DEBUG_ACCESS_KEY.DEBUG_ACCESS_KEY = 0x05U;
    SYSCTRLA_SFRS->PMU_ACCESS_KEY.PMU_ACCESS_KEY = 0x0AU;
    SYSCTRLA_SFRS->TRIM_ACCESS_KEY.TRIM_ACCESS_KEY = 0x0E;
    PMUAPRE5V_SFRS->CFG_ACCESS.CFG_ACCESS_KEY = 0x0B;
  
    /* Enable trim revise access enable*/
    HWCFG_TrimAccessUnlock();
    CRGA_SFRS->MODULERSTREQ = 0xFBU;
    /* Init system clock */
    Clock_SystemMainClockInit(SYS_MAIN_CLOCK_DIV);
    pmu_init();
    /* Init global timer engine for driving soft timer */
    SysTick_Init(SOFT_TIMER_INTERVAL *1000U * MAIN_CPU_CLOCK, SoftTimer_ExpireCallback);
#if WATCH_DOG_EN == 1U
      WDTA_Enable(WDTA_INTERVAL_18432MS); /* 18s */
#endif
    /* Init gpios settings */
    gpios_init();
    /* Init LED current and PWM settings */
    leds_driver_Init();
    
#if (UART_MILKY_WAY_EN == 1U)
    (void)UART_Init(BAUDRATE_DIV_16MHz_1000000, BITSIZE_8BITS, PARITY_NONE,STOPS_1BITS);
    UART_RegisterIRQ(0,APPL_UART_ISR);
#elif (CODE_DEBUG_EN == 1U)
    (void)UART_Init(BAUDRATE_DIV_16MHz_115200, BITSIZE_8BITS, PARITY_NONE,STOPS_1BITS);
    UART_RegisterIRQ(0,NULL);
#endif
    
    /* Init buck settings */
    BUCK_Init(BUCK_OUTPUT_4220mV);
    
    /* tasks init must be called before use. */
    TM_PostTask(TASK_ID_SOFT_TIMER);
    TM_PostTask(TASK_ID_SAFETY_MONITOR);
    TM_PostTask(TASK_ID_LINS);
    TM_PostTask(TASK_ID_PDS);
#if LIN_MASTER_EN == 1
    TM_PostTask(TASK_ID_LINM);
#endif
    TM_PostTask(TASK_ID_ADC_MEASURE);
    TM_PostTask(TASK_ID_COLOR_MIXING);
    TM_PostTask(TASK_ID_COLOR_COMPENSATION);
    TM_PostTask(TASK_ID_APPL);
    
}



