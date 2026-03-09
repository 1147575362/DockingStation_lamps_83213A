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

#include <applicationTask.h>
#include <measureTask.h>
#include <pdsTask.h>
#include <linStackTask.h>
#include <uart_device.h>


#if UART_MILKY_WAY_EN == 1U
static uint8_t  rxBuff[RX_DATA_SIZE];
static uint8_t  rxBuffLength;
void handleColorFromUart(uint8_t* buff, uint8_t length);
#endif

/*static uint8_t ledNum = LED0;*/
static TaskState_t applState = TASK_STATE_INIT;
void ApplTimerExpired(SoftTimer_t *timer);




static uint8_t LED_State = 0U;

uint8_t APPL_GetLEDState(void)
{
    return LED_State;
}





static SoftTimer_t ApplTimer = {
    .mode     = TIMER_PERIODIC_MODE,
    .interval = 20U,
    .handler  = ApplTimerExpired
};


void ApplTimerExpired(SoftTimer_t *timer)
{
    ColorTemperature_t temp;
    temp.value[0] = ROOM_TEMPERATURE;
    temp.value[1] = ROOM_TEMPERATURE;
    temp.value[2] = ROOM_TEMPERATURE;
    static uint8_t index = 0U;
    static uint8_t colorIndex = 0U;
    static uint8_t red = 0,green = 0, blue = 0;
    if (index < LED_NUM){
        (void)CLM_SetRGBL((LedNum_t)index,temp, red, green, blue,1024U, 1000U);
    }
    index++;
    if (index >= LED_NUM){
        index = 0;
        colorIndex++;
        if (colorIndex >=4U){
            colorIndex = 0U;
        }
        
        if (colorIndex == 0U){
            red = 0;green = 0; blue = 0;
        }else if (colorIndex == 1U){
            red = 255;green = 0; blue = 0;
        }else if (colorIndex == 2U){
            red = 0;green = 255; blue = 0;
        }else{
            red = 0;green = 0; blue = 255;
        }
    }

    static uint8_t DoorSigMcu;
    static uint8_t TouchIo;
    DoorSigMcu = GPIO_Read(GPIO_PORT_LINS);
    TouchIo = GPIO_Read(GPIO_PORT_1);

}



void APPL_TaskHandler(void)
{
    switch(applState){
    case TASK_STATE_INIT:
      SoftTimer_Start(&ApplTimer);
      (void)CLM_SetIntensityDegradation(FALSE);
      applState = TASK_STATE_ACTIVE;
      break;
    case TASK_STATE_ACTIVE:
#if UART_MILKY_WAY_EN == 1U
      handleColorFromUart(rxBuff, rxBuffLength);
#endif
      break;
    default:
      break;
    }
}


void APPL_HandleColorControlCommands(LIN_Device_Frame_t const *frame)
{
    ColorCtrlFrame_t const *color = (ColorCtrlFrame_t const*)((void const*)frame->data);
    uint8_t nodeAddress = color->nodeAddress & 0x3FU;
    uint8_t ledNumMsb = color->nodeAddress >> 6U;
    uint8_t ledNum = color->channel + (ledNumMsb << 4U);
    if ( (nodeAddress == ls_read_nad()) || (nodeAddress == LIN_NAD_WILDCARD)){
        if (ledNum < LED_NUM){
//            LedNum_t ledIndex = color->channel;
            LedNum_t ledIndex = (LedNum_t)ledNum;
            switch(color->command){
            case CMD_COLOR_CTRL_XYY:
              (void)CLM_SetXYY(ledIndex,MES_GetLedTemperature(ledIndex), color->ColorXY.x, color->ColorXY.y, (uint16_t)color->ColorXY.Y*10U, (uint16_t)color->ColorXY.transitionTime*100U);
              break;
            case CMD_COLOR_CTRL_XYY_EXT:
              (void)CLM_SetXYY(ledIndex,MES_GetLedTemperature(ledIndex), color->ColorXY_EXT.x, color->ColorXY_EXT.y, (uint16_t)color->ColorXY_EXT.Y, 0U);
              break;
            case CMD_COLOR_CTRL_HSL:
              (void)CLM_SetHSL(ledIndex,MES_GetLedTemperature(ledIndex), color->ColorHSL.hue, color->ColorHSL.saturation, (uint16_t)color->ColorHSL.level*10U, (uint16_t)color->ColorHSL.transitionTime*100U);
              break;
            case CMD_COLOR_CTRL_RGBL:
              if (color->ColorRGBL.level > 0U){
                  LED_State = 1U;
              }else{
                  LED_State = 0U;
              }
              (void)CLM_SetRGBL(ledIndex,MES_GetLedTemperature(ledIndex), color->ColorRGBL.red, color->ColorRGBL.green, color->ColorRGBL.blue,(uint16_t)color->ColorRGBL.level*10U, (uint16_t)color->ColorRGBL.transitionTime*100U);
              break;
            case CMD_COLOR_CTRL_RGB:
              (void)CLM_SetRGB(ledIndex,MES_GetLedTemperature(ledIndex), color->ColorRGB.red, color->ColorRGB.green, color->ColorRGB.blue, (uint16_t)color->ColorRGB.transitionTime*100U);
              break;
            case CMD_COLOR_CTRL_PWM:
              (void)CLM_SetPWMs(ledIndex,(uint16_t)color->ColorPWM.red*257U, (uint16_t)color->ColorPWM.green*257U, (uint16_t)color->ColorPWM.blue*257U, (uint16_t)color->ColorPWM.transitionTime*100U);
              break;
            case CMD_COLOR_CTRL_LUV:
              (void)CLM_SetLUV(ledIndex,MES_GetLedTemperature(ledIndex), color->ColorLUV.u, color->ColorLUV.v, (uint16_t)color->ColorLUV.level*10U, (uint16_t)color->ColorLUV.transitionTime*100U);
              break;
              
            case CMD_COLOR_CTRL_XY_ABS_Y:
              (void)CLM_SetXYAbsY(ledIndex,MES_GetLedTemperature(ledIndex), color->ColorXY_Intensity.x, color->ColorXY_Intensity.y, color->ColorXY_Intensity.intensity, 0U);
              break;
            case CMD_COLOR_CTRL_ABS_L_UV:
              (void)CLM_SetAbsLUV(ledIndex,MES_GetLedTemperature(ledIndex), color->ColorIntensity_UV.u, color->ColorIntensity_UV.v, color->ColorIntensity_UV.intensity, 0U);
              break;
            case CMD_COLOR_CTRL_sRGB_ABS_L:
              (void)CLM_SetSRGBAbsL(ledIndex,MES_GetLedTemperature(ledIndex), color->ColorSRGB_Intensity.red, color->ColorSRGB_Intensity.green, color->ColorSRGB_Intensity.blue,color->ColorSRGB_Intensity.intensity, 0U);
              break;
              
            default:
              break;
            }
        }
    }
}


uint8_t APPL_HandleLedParamConfigRequest(uint8_t reqSid,uint8_t *const dataBuff,DiagRspInfo_t *const diagRsp)
{
    uint8_t response = FALSE;
    CommLedParamInfo_t *const info    = (CommLedParamInfo_t *)((void *)dataBuff);
    CommLedParamInfo_t *const infoRsp = (CommLedParamInfo_t *)((void *)diagRsp->payload);

    /* handle response data here  */
    diagRsp->type = PDU_TYPE_SINGLE_PDU;
    diagRsp->sid = reqSid;
    diagRsp->packLength = SF_MAX_DATA_LENGTH;   /* Valid data length not includes sid*/
    diagRsp->payload[0] = 0xFFU;                /* failed   */
    
    infoRsp->command = info->command;
    switch(info->command){
    case APP_DIAG_SET_LED_PHY_PARAM: 
      {
      LedcolorParam_t ledColorParam = info->ledPhysicalParam.colorParam;
      ColorTemperature_t temperature;
      temperature.value[0] = info->ledPhysicalParam.temperature[0];
      temperature.value[1] = info->ledPhysicalParam.temperature[1];
      temperature.value[2] = info->ledPhysicalParam.temperature[2];
      if (info->ledPhysicalParam.ledIndex <= (LedNum_t)(LED_NUM -1U)){
          (void)CLM_SetLedPhyParams(info->ledPhysicalParam.ledIndex,temperature, &ledColorParam);
          diagRsp->payload[0] = 0x00U;
      }
      response = TRUE;
      break;
      }
    case APP_DIAG_SET_LED_GENERAL_PARAM:
      {
      Coordinate_t    whitePoint = info->ledGeneralParam.whitePoint;
      uint16_t minIntensity[3];
      minIntensity[0] = info->ledGeneralParam.minIntensity[0];
      minIntensity[1] = info->ledGeneralParam.minIntensity[1];
      minIntensity[2] = info->ledGeneralParam.minIntensity[2];
      (void)CLM_SetWhitePointParams(&whitePoint);
      (void)CLM_SetMinimumIntensity(minIntensity[0],minIntensity[1],minIntensity[2]);
      diagRsp->payload[0] = 0x00U;
      response = TRUE;
      break;
      }
    case APP_DIAG_SET_LED_TYPICAL_PN_VOLT: 
      {
      int16_t volt_R,volt_G,volt_B;
      ColorTemperature_t temperature;
      volt_R = info->ledTypicalPNVolt.ledTypicalPNVolt[0];
      volt_G = info->ledTypicalPNVolt.ledTypicalPNVolt[1];
      volt_B = info->ledTypicalPNVolt.ledTypicalPNVolt[2];
      
      temperature.value[0] = info->ledTypicalPNVolt.temperature;
      temperature.value[1] = info->ledTypicalPNVolt.temperature;
      temperature.value[2] = info->ledTypicalPNVolt.temperature;
      
      (void)CLM_SetLedPNVolts((LedNum_t)info->ledTypicalPNVolt.ledIndex,temperature, volt_R, volt_G, volt_B);
      diagRsp->payload[0] = 0x00U;
      response = TRUE;
      break;
      }
    default:
      break;
    }
    return response;
}

uint8_t APPL_PrepareLedParamRequest(uint8_t reqSid,uint8_t *const dataBuff, DiagRspInfo_t *const diagRsp)
{
    uint8_t response = FALSE;
    CommLedParamInfo_t *const info    = (CommLedParamInfo_t *)((void *)dataBuff);
    CommLedParamInfo_t *const infoRsp = (CommLedParamInfo_t *)((void *)diagRsp->payload);

    diagRsp->sid = reqSid;
    infoRsp->command = info->command;
    switch(info->command){
    case APP_DIAG_GET_LED_PHY_PARAM:
      {
      LedcolorParam_t ledColorParam;
      ColorTemperature_t temperature;
      temperature.value[0] = info->ledPhysicalParam.temperature[0];
      temperature.value[1] = info->ledPhysicalParam.temperature[1];
      temperature.value[2] = info->ledPhysicalParam.temperature[2];
      
      if (info->ledPhysicalParam.ledIndex <= (LedNum_t)(LED_NUM -1U)){
         (void)CLM_GetLedPhyParams((LedNum_t)info->ledPhysicalParam.ledIndex,temperature, &ledColorParam);
          infoRsp->ledPhysicalParam.colorParam      = ledColorParam;
          infoRsp->ledPhysicalParam.ledIndex        = info->ledPhysicalParam.ledIndex;
          infoRsp->ledPhysicalParam.temperature[0]  = info->ledPhysicalParam.temperature[0];
          infoRsp->ledPhysicalParam.temperature[1]  = info->ledPhysicalParam.temperature[1];
          infoRsp->ledPhysicalParam.temperature[2]  = info->ledPhysicalParam.temperature[2];
          
          diagRsp->packLength = 2U + (uint16_t)sizeof(CommLedPhysicalParam_t);
          diagRsp->type = (diagRsp->packLength > SF_MAX_DATA_LENGTH)? PDU_TYPE_MULTI_PDU:PDU_TYPE_SINGLE_PDU;
          response = TRUE;
      }
      break;
      }
    case APP_DIAG_GET_LED_GENERAL_PARAM:
      {
      Coordinate_t    whitePoint;
      uint16_t minIntensity[3];
      (void)CLM_GetWhitePointParams(&whitePoint);
      (void)CLM_GetMinimumIntensity(&minIntensity[0],&minIntensity[1],&minIntensity[2]);
      infoRsp->ledGeneralParam.minIntensity[0] = minIntensity[0];
      infoRsp->ledGeneralParam.minIntensity[1] = minIntensity[1];
      infoRsp->ledGeneralParam.minIntensity[2] = minIntensity[2];
      infoRsp->ledGeneralParam.whitePoint = whitePoint;
      diagRsp->packLength = 2U + (uint16_t)sizeof(CommLedGeneralParam_t);
      diagRsp->type = (diagRsp->packLength > SF_MAX_DATA_LENGTH)? PDU_TYPE_MULTI_PDU:PDU_TYPE_SINGLE_PDU;
      response = TRUE;
      break;
      }
    case APP_DIAG_GET_LED_TYPICAL_PN_VOLT:
      {
      int16_t volt_R,volt_G,volt_B;
      ColorTemperature_t temperature;
      temperature.value[0] = info->ledTypicalPNVolt.temperature;
      temperature.value[1] = info->ledTypicalPNVolt.temperature;
      temperature.value[2] = info->ledTypicalPNVolt.temperature;
      (void)CLM_GetLedPNVolts((LedNum_t)info->ledTypicalPNVolt.ledIndex,temperature, &volt_R, &volt_G, &volt_B);
      infoRsp->ledTypicalPNVolt.ledIndex    = info->ledTypicalPNVolt.ledIndex;
      infoRsp->ledTypicalPNVolt.temperature = info->ledTypicalPNVolt.temperature;
      infoRsp->ledTypicalPNVolt.ledTypicalPNVolt[0] = volt_R;
      infoRsp->ledTypicalPNVolt.ledTypicalPNVolt[1] = volt_G;
      infoRsp->ledTypicalPNVolt.ledTypicalPNVolt[2] = volt_B;
      diagRsp->packLength = 2U + (uint16_t)sizeof(CommLedTypicalPNVolt_t);
      diagRsp->type = (diagRsp->packLength > SF_MAX_DATA_LENGTH)? PDU_TYPE_MULTI_PDU:PDU_TYPE_SINGLE_PDU;
      response = TRUE;
      break;
      }
    case APP_DIAG_GET_LED_RUN_TIME_INFO:
      {
      uint8_t ledNo;
      uint16_t battVolt,buckVolt;
      int16_t chipTemperature;
      ColorTemperature_t temperature;
      int16_t sum;
      
      (void)MES_GetBatteryVolt(&battVolt);
      (void)MES_GetBuckVolt(&buckVolt);
      (void)MES_GetChipTemperature(&chipTemperature);
      
      infoRsp->ledRunTimeParam.battVolt = battVolt;
      infoRsp->ledRunTimeParam.buckVolt = buckVolt;
      infoRsp->ledRunTimeParam.chipTemp = chipTemperature;
      infoRsp->ledRunTimeParam.ledNum   = LED_NUM;
      for (ledNo = 0U; ledNo < LED_NUM; ledNo++){
        temperature = MES_GetLedTemperature((LedNum_t)ledNo);
        sum = (int16_t)temperature.value[0] + (int16_t)temperature.value[1] + (int16_t)temperature.value[1] + (int16_t)temperature.value[2];
        infoRsp->ledRunTimeParam.ledTemperature[ledNo] = (int16_t)(sum/4);
      }
      diagRsp->packLength = 2U + (uint16_t)sizeof(CommLedRunTimeParam_t);
      diagRsp->type = (diagRsp->packLength > SF_MAX_DATA_LENGTH)? PDU_TYPE_MULTI_PDU:PDU_TYPE_SINGLE_PDU;
      response = TRUE;
      break;
      }
      
    case APP_DIAG_GET_LED_RGB_RUN_TEMP:
      {
        int8_t rTemp,gTemp,bTemp;
        if ((uint8_t)info->LedRunTimePNTemp.ledIndex < LED_NUM ){
            (void)MES_GetLedRGBTemperature((LedNum_t)info->LedRunTimePNTemp.ledIndex, &rTemp,&gTemp,&bTemp);
            infoRsp->LedRunTimePNTemp.ledIndex = info->LedRunTimePNTemp.ledIndex;
            infoRsp->LedRunTimePNTemp.ledTemp[0] = rTemp;
            infoRsp->LedRunTimePNTemp.ledTemp[1] = gTemp;
            infoRsp->LedRunTimePNTemp.ledTemp[2] = bTemp;
            diagRsp->packLength = 2U + (uint16_t)sizeof(CommLedRunTimePNTemp_t);
            diagRsp->type = (diagRsp->packLength > SF_MAX_DATA_LENGTH)? PDU_TYPE_MULTI_PDU:PDU_TYPE_SINGLE_PDU;
            response = TRUE;
        }
        break;
      }
      
    case APP_DIAG_GET_LED_STATIC_PN_VOLT:
      {
      /* force to measure PN volt when Led is off for getting initialized PN voltage */
      uint16_t volt_R,volt_G,volt_B;
      MES_MnftGetLedInfo((LedNum_t)info->ledStaticPNVolt.ledIndex, &volt_R,&volt_G,&volt_B);
      infoRsp->ledStaticPNVolt.ledIndex = info->ledStaticPNVolt.ledIndex;
      infoRsp->ledStaticPNVolt.ledPNVolt[0] = volt_R;
      infoRsp->ledStaticPNVolt.ledPNVolt[1] = volt_G;
      infoRsp->ledStaticPNVolt.ledPNVolt[2] = volt_B;
      diagRsp->packLength = 2U + (uint16_t)sizeof(CommLedStaticPNVolt_t);
      diagRsp->type = (diagRsp->packLength > SF_MAX_DATA_LENGTH)? PDU_TYPE_MULTI_PDU:PDU_TYPE_SINGLE_PDU;
      response = TRUE;
      break;
      }
      
    case APP_DIAG_GET_LED_RGB_RUN_VOLT:
      {
        uint16_t battVolt,buckVolt;
        int16_t chipTemperature;
        int16_t volt_R,volt_G,volt_B;
        if ((uint8_t)info->LedRunTimePNTemp.ledIndex < LED_NUM ){
            (void)MES_GetBatteryVolt(&battVolt);
            (void)MES_GetBuckVolt(&buckVolt);
            (void)MES_GetChipTemperature(&chipTemperature);
            
            infoRsp->ledRunTimePNVolt.battVolt = battVolt;
            infoRsp->ledRunTimePNVolt.buckVolt = buckVolt;
            infoRsp->ledRunTimePNVolt.chipTemp = chipTemperature;
            
            MES_GetRunTimeLedPNVolt((LedNum_t)info->ledRunTimePNVolt.ledIndex,&volt_R, &volt_G, &volt_B);
            infoRsp->ledRunTimePNVolt.ledIndex = info->ledRunTimePNVolt.ledIndex;
            infoRsp->ledRunTimePNVolt.ledPNVolt[0] = (uint16_t)volt_R;
            infoRsp->ledRunTimePNVolt.ledPNVolt[1] = (uint16_t)volt_G;
            infoRsp->ledRunTimePNVolt.ledPNVolt[2] = (uint16_t)volt_B;
            diagRsp->packLength = 2U + (uint16_t)sizeof(CommLedRunTimePNVolt_t);
            diagRsp->type = (diagRsp->packLength > SF_MAX_DATA_LENGTH)? PDU_TYPE_MULTI_PDU:PDU_TYPE_SINGLE_PDU;
            response = TRUE;
        }
        break;
      }
    case APP_DIAG_GET_SDK_VERSION:
      infoRsp->Version.sdkApplication   = SDK_APPL_VERSION;
      infoRsp->Version.linStack         = ls_get_lin_version();
      infoRsp->Version.ledLibrary       = CLM_GetColorLibraryVersion();
      diagRsp->packLength = 2U + (uint16_t)sizeof(CommVersion_t);
      diagRsp->type = (diagRsp->packLength > SF_MAX_DATA_LENGTH)? PDU_TYPE_MULTI_PDU:PDU_TYPE_SINGLE_PDU;
      response = TRUE;
      break;
    default:
      break;
    }
    return response;
}




#if UART_MILKY_WAY_EN == 1U

void APPL_UART_ISR(uint8_t* buff, uint8_t length)
{
    rxBuffLength = length;
    memcpy(rxBuff,buff,length);
    TM_PostTask(TASK_ID_APPL);
  
}

void handleColorFromUart(uint8_t* buff, uint8_t length)
{
    APPXYY_t *color = (APPXYY_t *)buff;
    for (uint8_t i = 0; i < LED_NUM; i++){
        (void)CLM_SetXYY((LedNum_t)i,MES_GetLedTemperature((LedNum_t)i), color->xyY[i].x, color->xyY[i].y, (uint16_t)color->xyY[i].Y*10U, 0);
    }
}
#endif

