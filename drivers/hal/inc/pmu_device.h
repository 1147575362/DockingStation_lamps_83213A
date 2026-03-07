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

#ifndef PMU_DEVICE_H__
#define PMU_DEVICE_H__

#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <rugbyHP.h>

typedef void (*bor_cb_func_t)(void);

#define MODULE_SOFT_RESET_ADC            (0x01U)
#define MODULE_SOFT_RESET_BUCK           (0x02U)
#define MODULE_SOFT_RESET_LINM           (0x04U)
#define MODULE_SOFT_RESET_LINS           (0x08U)
#define MODULE_SOFT_RESET_PWM_AUX        (0x10U)
#define MODULE_SOFT_RESET_PWM            (0x20U)
#define MODULE_SOFT_RESET_SPI            (0x40U)
#define MODULE_SOFT_RESET_UART           (0x80U)

typedef enum{
    POLARITY_NATIVE = 0,
    POLARITY_FLIP,
}BattMonitorPolarity_t;


/*
0x0: Vr: 1.243 V, Vf: 1.135 V,
0x1: Vr: 1.273 V, Vf: 1.165 V,
0x2: Vr: 1.298 V, Vf: 1.215 V,
0x3: Vr: 1.328 V, Vf: 1.255 V,
0x4: Vr: 1.358 V, Vf: 1.305 V(default),
0x5: Vr: 1.393 V, Vf: 1.355 V,
0x6: Vr: 1.428 V, Vf: 1.405 V,
0x7: Vr: 1.463 V, Vf: 1.465 V,
*/

typedef enum{
  BOR_1V5_THRS_1243mV = 0,
  BOR_1V5_THRS_1273mV,
  BOR_1V5_THRS_1298mV,
  BOR_1V5_THRS_1328mV,
  BOR_1V5_THRS_1358mV,
  BOR_1V5_THRS_1393mV,
  BOR_1V5_THRS_1428mV,
  BOR_1V5_THRS_1463mV,
}Bor1V5Thres_t;

/*
0x0: Vf: 2.223 V, Vr: 2.765 V,
0x1: Vf: 2.288 V, Vr: 2.825 V,
0x2: Vf: 2.358 V, Vr: 2.895 V,
0x3: Vf: 2.428 V, Vr: 2.975 V,
0x4: Vf: 2.503 V, Vr: 3.055 V(default),
0x5: Vf: 2.583 V, Vr: 3.135 V,
0x6: Vf: 2.668 V, Vr: 3.225 V,
0x7: Vf: 2.763 V, Vr: 3.315 V,
*/
typedef enum{
  BOR_3V3_THRS_2223mV = 0,
  BOR_3V3_THRS_2288mV,
  BOR_3V3_THRS_2358mV,
  BOR_3V3_THRS_2428mV,
  BOR_3V3_THRS_2503mV,
  BOR_3V3_THRS_2583mV,
  BOR_3V3_THRS_2668mV,
  BOR_3V3_THRS_2763mV,
}Bor3V3Thres_t;

/**
 * @brief The enumeration of brownout mode.
 */
typedef enum {
    PMU_BROWNOUT_RESET = 0U,
    PMU_BROWNOUT_INTERRUPT,
} PMU_BROWNOUT_MODE_t;

/**
 * @brief The enumeration of wake up timer mode.
 */
typedef enum {
    WAKEUP_TIMEER_DISABLE = 0U,
    WAKEUP_TIMEER_ENABLE,
} PMU_WAKEUP_TIMEER_MODE_t;

/**
 * @brief The enumeration of wake up timer mode.
 */
typedef enum {
    WAKEUP_TIMEER_INTERVAL_62_5us = 0U,
    WAKEUP_TIMEER_INTERVAL_125us,
    WAKEUP_TIMEER_INTERVAL_250us,
    WAKEUP_TIMEER_INTERVAL_500us,
    WAKEUP_TIMEER_INTERVAL_1ms,
    WAKEUP_TIMEER_INTERVAL_2ms,
    WAKEUP_TIMEER_INTERVAL_4ms,
    WAKEUP_TIMEER_INTERVAL_8ms,
    WAKEUP_TIMEER_INTERVAL_16ms,
    WAKEUP_TIMEER_INTERVAL_32ms,
    WAKEUP_TIMEER_INTERVAL_64ms,
    WAKEUP_TIMEER_INTERVAL_128ms,
    WAKEUP_TIMEER_INTERVAL_256ms,
    WAKEUP_TIMEER_INTERVAL_512ms,
    WAKEUP_TIMEER_INTERVAL_1024ms,
    WAKEUP_TIMEER_INTERVAL_2048ms
}PMU_WAKEUP_TIMEER_Interval_t;

void PMU_BORInit(Bor1V5Thres_t lowThreshold, Bor3V3Thres_t highThreshold);
void PMU_WakeTimerInit(PMU_WAKEUP_TIMEER_MODE_t mode, PMU_WAKEUP_TIMEER_Interval_t interval);
void PMU_EnterDeepSleepMode(void);

void PMU_RegisterBOR_IRQ(bor_cb_func_t callback);


#endif /* __PMU_DEVICE_H__ */
