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

#ifndef BUCK_DEVICE_H__
#define BUCK_DEVICE_H__

#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <rugbyHP_sfrs.h>

typedef void (*BuckIsrCallback_t)(uint8_t status);

typedef enum{
  BUCK_RAMP_RATE_8us = 0U,
  BUCK_RAMP_RATE_16us,
  BUCK_RAMP_RATE_31us,
  BUCK_RAMP_RATE_62us,
}Buck_RAMP_RATE_t;

typedef enum{
  BUCK_SSC_STEP_8STEPS_1_PEROIDS = 0U,
  BUCK_SSC_STEP_4STEPS_1_PEROIDS,
  BUCK_SSC_STEP_2STEPS_1_PEROIDS,
  BUCK_SSC_STEP_1STEPS_1_PEROIDS,
  BUCK_SSC_STEP_1STEPS_2_PEROIDS,
  BUCK_SSC_STEP_1STEPS_3_PEROIDS,
  BUCK_SSC_STEP_1STEPS_4_PEROIDS,
  BUCK_SSC_STEP_1STEPS_5_PEROIDS,
}Buck_SSC_STEP_t;

typedef enum{
  BUCK_OUTPUT_2500mV = 16U,
  BUCK_OUTPUT_2660mV = 17U,
  BUCK_OUTPUT_2810mV = 18U,
  BUCK_OUTPUT_2970mV = 19U,
  BUCK_OUTPUT_3130mV = 20U,
  BUCK_OUTPUT_3280mV = 21U,
  BUCK_OUTPUT_3440mV = 22U,
  BUCK_OUTPUT_3590mV = 23U,
  BUCK_OUTPUT_3750mV = 24U,
  BUCK_OUTPUT_3910mV = 25U,
  BUCK_OUTPUT_4060mV = 26U,
  BUCK_OUTPUT_4220mV = 27U,
  BUCK_OUTPUT_4380mV = 28U,
  BUCK_OUTPUT_4530mV = 29U,
  BUCK_OUTPUT_4690mV = 30U,
  BUCK_OUTPUT_4840mV = 31U,
  BUCK_OUTPUT_5000mV = 32U,
  BUCK_OUTPUT_5160mV = 33U,
  BUCK_OUTPUT_5310mV = 34U,
  BUCK_OUTPUT_5470mV = 35U,
}BuckOutputVoltage_t;


void BUCK_Init(BuckOutputVoltage_t enumBuckVolt);
void BUCK_RegisterEmergencyIRQ(BuckIsrCallback_t callback);
void BUCK_DisableBuck(void);
void BUCK_EnableBuck(void);


#endif













