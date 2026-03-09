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

#include <colorCompensationTask.h>

#define COLOR_COMP_INTERVAL 400U
#define COLOR_COMP_DIMMING_INTERVAL 100U /* (COLOR_COMP_INTERVAL - 100U)*/
static TaskState_t ccpState = TASK_STATE_INIT;
void doColorCompensationHandle(void);
void colorCompTimerExpired(SoftTimer_t *timer);
static uint8_t ledNo = LED3;

static SoftTimer_t colorCompTimer = {
    .mode     = TIMER_PERIODIC_MODE,
    .interval = COLOR_COMP_INTERVAL, /* color compensation detect time interval */
    .handler  = colorCompTimerExpired
};


void colorCompTimerExpired(SoftTimer_t *timer)
{
    /* color compensation only starts until the diming operation has been finished */
    if ( CLM_FadingIsFinished((LedNum_t)ledNo) != 0U){
        TM_PostTask(TASK_ID_COLOR_COMPENSATION);
    }
}

void CCP_TaskHandler(void)
{
    switch(ccpState){
    case TASK_STATE_INIT:
      SoftTimer_Start(&colorCompTimer);
      ccpState = TASK_STATE_ACTIVE;
      break;
    case TASK_STATE_ACTIVE:
      doColorCompensationHandle(); /* do color compensation operation if temperature has changed */
      break;
    default:
      break;
    }
}

void doColorCompensationHandle(void)
{
    /* Get current LED temperature */
    if (PWM_LedIsOn(ledNo) == TRUE){
        ColorParam_t const *color = CLM_GetCurrentColorParams((LedNum_t)ledNo);
        if(color != NULL){
            switch(color->colorMode){
            case COLOR_MODE_XYY:
              (void)CLM_SetXYY((LedNum_t)ledNo,MES_GetLedTemperature((LedNum_t)ledNo), color->XYY.x, color->XYY.y, color->XYY.Y, COLOR_COMP_DIMMING_INTERVAL);
              break;
            case COLOR_MODE_LUV:
              (void)CLM_SetLUV((LedNum_t)ledNo,MES_GetLedTemperature((LedNum_t)ledNo), color->LUV.u, color->LUV.v, color->LUV.L, COLOR_COMP_DIMMING_INTERVAL);
              break;
            case COLOR_MODE_sRGBL:
              (void)CLM_SetSRGBL((LedNum_t)ledNo,MES_GetLedTemperature((LedNum_t)ledNo), color->RGBL.red, color->RGBL.green, color->RGBL.blue,color->RGBL.level,COLOR_COMP_DIMMING_INTERVAL);
              break;
            case COLOR_MODE_XY_ABS_Y:
              (void)CLM_SetXYAbsY((LedNum_t)ledNo,MES_GetLedTemperature((LedNum_t)ledNo), color->XYY.x, color->XYY.y, color->XYY.Y, COLOR_COMP_DIMMING_INTERVAL);
              break;
            case COLOR_MODE_ABS_LUV:
              (void)CLM_SetAbsLUV((LedNum_t)ledNo,MES_GetLedTemperature((LedNum_t)ledNo), color->LUV.u, color->LUV.v, color->LUV.L, COLOR_COMP_DIMMING_INTERVAL);
              break;
            case COLOR_MODE_sRGB_ABS_L:
              (void)CLM_SetSRGBAbsL((LedNum_t)ledNo,MES_GetLedTemperature((LedNum_t)ledNo), color->RGBL.red, color->RGBL.green, color->RGBL.blue,color->RGBL.level,COLOR_COMP_DIMMING_INTERVAL);
              break;
            case COLOR_MODE_RGBL:
              (void)CLM_SetRGBL((LedNum_t)ledNo,MES_GetLedTemperature((LedNum_t)ledNo), color->RGBL.red, color->RGBL.green, color->RGBL.blue,color->RGBL.level,COLOR_COMP_DIMMING_INTERVAL);
              break;
            case COLOR_MODE_RGB:
              (void)CLM_SetRGB((LedNum_t)ledNo,MES_GetLedTemperature((LedNum_t)ledNo), color->RGB.red, color->RGB.green, color->RGB.blue,COLOR_COMP_DIMMING_INTERVAL);
              break;
            case COLOR_MODE_HSL:
              (void)CLM_SetHSL((LedNum_t)ledNo,MES_GetLedTemperature((LedNum_t)ledNo), color->HSL.hue, color->HSL.saturation, color->HSL.level, COLOR_COMP_DIMMING_INTERVAL);
              break;
            default:
              break;
            }
        }
    }
    ledNo++;
    if (ledNo >= (uint8_t)LED_NUM){
        ledNo = (uint8_t)LED0;
    }
}

