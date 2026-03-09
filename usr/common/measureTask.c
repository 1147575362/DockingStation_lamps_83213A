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

#include <crc32.h>
#include <measureTask.h>

#define AVERAGE_MEASURE_POS     (4U)
#define AVERAGE_MEASURE_GAIN    (1U << AVERAGE_MEASURE_POS)

#if AVERAGE_MEASURE_POS == 0U
  #error AVERAGE_MEASURE_POS should be higher than 0!
#endif

#define MEASURE_GAIN_POS         12U
#define MEASURE_GAIN             4096 /*(1UL << MEASURE_GAIN_POS)*/

#define MEASURE_TEMP_GAIN_POS    14U
#define MEASURE_TEMP_GAIN        40960 /*16384*2.5 *2*/
#define MEASURE_ADC_GAIN_FACTOR  16913U /* 16384 / ADC_GAIN(31/32) = 16384 * 32/31 = 16913*/

typedef struct{
  int32_t gain;
  int32_t offset;
}calibParam_t;

typedef struct{
  uint32_t startCode;/* means the measurement is started or not */
  uint16_t buff[AVERAGE_MEASURE_GAIN];
  uint16_t count;
  calibParam_t calibP;
  int16_t target;
}AdcDatabuff_t;

typedef struct{
  AdcDatabuff_t vBatt;
  AdcDatabuff_t vBuck;
  AdcDatabuff_t vLedPN[LED_NUM][3];
  AdcDatabuff_t vChipTemp;
  calibParam_t  tChip;
  int16_t       chipTemperature;
  int16_t       ledPNTemperature[LED_NUM][3];
}AdcResult_t;

static uint8_t dataIntegrityError = FALSE;


void add_data_to_buff(uint16_t *original, uint16_t newData, uint16_t *bufferIndex,uint16_t *buff);
static TaskState_t adcTaskState = TASK_STATE_INIT;
static AdcResult_t adcResult;

static uint8_t measLedNo = 0U;
static uint8_t measLedColorNo = 0U;
static AdcTriggerSource_t measSyncMode = ADC_TRIGGER_SOURCE_SOFT_INPUT;
static AdcMeasureItem_t  measItem = ADC_MEASURE_ITEM_NONE;
static volatile uint32_t measStart = 0U;
static volatile uint16_t measGeneralAdcCode[4];
void measureDoneISR(ADCMeasureParam_t item, uint16_t *const result);
void measureParamStart(void);
void measureParamInit(void);

static uint32_t updateParamIndex = 0U;
static uint32_t updateParamStart = 0U;
void updateTimerExpired(SoftTimer_t *timer);

int16_t get_chip_temp_volt(AdcDatabuff_t *const param);

int16_t get_volt(AdcDatabuff_t *const param);
int16_t get_chip_temperature(int16_t vTemp);
int8_t get_led_temperature(TempParams_t const table[],int16_t deltaPNVolt);
void updateSystemInfo(void);
void updateLedTemperature(LedNum_t index);
void updateParams(void);

static volatile uint32_t adcConvertDone = 0U;

static SoftTimer_t updateTimer = {
    .mode     = TIMER_PERIODIC_MODE,
    .interval = 50U,
    .handler  = updateTimerExpired
};


void add_data_to_buff(uint32_t *original, uint16_t newData, uint16_t *bufferIndex,uint16_t *buff)
{
    if (*original == INVALID_PARAM){
        *original = newData;
        *bufferIndex = 0;
        for (uint8_t i = 0U; i < AVERAGE_MEASURE_GAIN; i++){
            buff[i] = newData;
        }
    }else{
        buff[*bufferIndex] = newData;
    }
    (*bufferIndex) ++;
    if ((*bufferIndex) >= AVERAGE_MEASURE_GAIN){
        (*bufferIndex) = 0U;
    }
}


void updateTimerExpired(SoftTimer_t *timer)
{
    static uint8_t measWaitCount = 3U;
    /*  update then start next measurement  */
    if (++measWaitCount >= 3U){
        measWaitCount = 0U;
        measStart = 1U; 
    }
    updateParamStart = 1U;
    TM_PostTask(TASK_ID_ADC_MEASURE);
}


void measureDoneISR(ADCMeasureParam_t param, uint16_t *const result)
{
    measGeneralAdcCode[0] = result[0];
    measGeneralAdcCode[1] = result[1];
    measGeneralAdcCode[2] = result[2];
    measGeneralAdcCode[3] = result[3];
    measStart = 1U;
    adcConvertDone = 1U;
    TM_PostTask(TASK_ID_ADC_MEASURE);
}

void measureParamStart(void)
{
    switch(measItem){
    case ADC_MEASURE_ITEM_NONE:
      measSyncMode = ADC_TRIGGER_SOURCE_SOFT_INPUT;
      measItem = ADC_MEASURE_ITEM_VPTATBUCK_VBAT_VBUCK_VPTATLED;
      break;
    case ADC_MEASURE_ITEM_VPTATBUCK_VBAT_VBUCK_VPTATLED:
      add_data_to_buff(&adcResult.vBatt.startCode,     measGeneralAdcCode[1], &adcResult.vBatt.count,adcResult.vBatt.buff);
      add_data_to_buff(&adcResult.vBuck.startCode,     measGeneralAdcCode[2], &adcResult.vBuck.count,adcResult.vBuck.buff);
      add_data_to_buff(&adcResult.vChipTemp.startCode, measGeneralAdcCode[3], &adcResult.vChipTemp.count,adcResult.vChipTemp.buff);
      
      for (measLedNo = 0U; measLedNo < LED_NUM; measLedNo++){
          if (PWM_LedIsOn(measLedNo) != 0U){
              break;
          }else{
              adcResult.vLedPN[measLedNo][0].startCode = INVALID_PARAM;
              adcResult.vLedPN[measLedNo][1].startCode = INVALID_PARAM;
              adcResult.vLedPN[measLedNo][2].startCode = INVALID_PARAM;
          }
      }
      if (measLedNo < LED_NUM){
          measSyncMode   = ADC_TRIGGER_SOURCE_PWM_PERIOD;
          measItem    = ADC_MEASURE_ITEM_VLED_VBAT_VBUCK_VPTATLED;
          measLedColorNo = 0U;
      }else{
          measSyncMode = ADC_TRIGGER_SOURCE_SOFT_INPUT;
          measItem  = ADC_MEASURE_ITEM_NONE;
      }
      
      break;
    case ADC_MEASURE_ITEM_VLED_VBAT_VBUCK_VPTATLED:
      add_data_to_buff(&adcResult.vLedPN[measLedNo][measLedColorNo].startCode, measGeneralAdcCode[0], &adcResult.vLedPN[measLedNo][measLedColorNo].count,adcResult.vLedPN[measLedNo][measLedColorNo].buff);
      add_data_to_buff(&adcResult.vBatt.startCode,     measGeneralAdcCode[1], &adcResult.vBatt.count,adcResult.vBatt.buff);
      add_data_to_buff(&adcResult.vBuck.startCode,     measGeneralAdcCode[2], &adcResult.vBuck.count,adcResult.vBuck.buff);
      add_data_to_buff(&adcResult.vChipTemp.startCode, measGeneralAdcCode[3], &adcResult.vChipTemp.count,adcResult.vChipTemp.buff);
      
      measLedColorNo++;
      if (measLedColorNo >= 3U){
          measLedColorNo = 0U;
          measLedNo++;
          for (; measLedNo < LED_NUM; measLedNo++){
              if (PWM_LedIsOn(measLedNo) != 0U){
                  break;
              }else{
                  adcResult.vLedPN[measLedNo][0].startCode = INVALID_PARAM;
                  adcResult.vLedPN[measLedNo][1].startCode = INVALID_PARAM;
                  adcResult.vLedPN[measLedNo][2].startCode = INVALID_PARAM;
              }
          }
          if (measLedNo >= LED_NUM){
              measSyncMode = ADC_TRIGGER_SOURCE_SOFT_INPUT;
              measItem  = ADC_MEASURE_ITEM_NONE;
          }
      }
      break;
    default:
      break;
    }
    if (measItem != ADC_MEASURE_ITEM_NONE){
        ADC_Init(measItem,measLedNo,(LedType_t)measLedColorNo,measSyncMode);
        ADC_Start();
    }
}


int16_t get_chip_temp_volt(AdcDatabuff_t *const param)
{
    uint32_t bandgap_mV  = HWCFG_GetVBG();
    
    int16_t volt = (int16_t)INVALID_VOLT;
    uint32_t sum = 0;
    if (param->startCode != INVALID_PARAM){
        for (uint8_t i = 0U; i < AVERAGE_MEASURE_GAIN; i++){
            sum += param->buff[i];
        }
        sum  = sum >> (AVERAGE_MEASURE_POS - 1U);
        sum = (bandgap_mV*sum) >> 12U;/*1mV */
        volt = (int16_t)((uint32_t)((sum*MEASURE_ADC_GAIN_FACTOR) >> MEASURE_TEMP_GAIN_POS));   
    }
    return volt;
}

int16_t get_volt(AdcDatabuff_t *const param)
{
    int16_t volt = (int16_t)INVALID_VOLT;
    int32_t sVolt;
    uint32_t uVolt;
    uint32_t sum = 0;
    if (param->startCode != INVALID_PARAM){
        for (uint8_t i = 0U; i < AVERAGE_MEASURE_GAIN; i++){
            sum += param->buff[i];
        }
        sum  = sum >> (AVERAGE_MEASURE_POS - 1U);
        sVolt = param->calibP.gain * (int32_t)sum + param->calibP.offset;
        if (sVolt <= 0){
            sVolt = 0;
        }
        uVolt = (uint32_t)sVolt;
        uVolt = uVolt >> MEASURE_GAIN_POS;
        volt = (int16_t)uVolt;
    }
    return volt;
}


int16_t get_chip_temperature(int16_t vTemp)
{
    int32_t  stemp;
    uint32_t uTemp;
    uint8_t  negative = 0U;
    stemp = adcResult.tChip.gain*vTemp + adcResult.tChip.offset;
    if (stemp < 0){
        stemp = -stemp;
        negative = 1U;
    }
    uTemp = (uint32_t)stemp;
    uTemp = uTemp >> MEASURE_TEMP_GAIN_POS;
    stemp = (int32_t)uTemp;
    if (negative != 0U){
        stemp = -stemp;
    }
    return (int16_t)stemp;
}


void updateSystemInfo(void)
{
    /* calculate chip temperature */
    adcResult.vChipTemp.target = get_chip_temp_volt(&adcResult.vChipTemp);
    adcResult.chipTemperature = get_chip_temperature(adcResult.vChipTemp.target);
#if CODE_DEBUG_EN == 1
    (void)DEBUG_OUT("VTEMP:%d  ",adcResult.chipTemperature);
#endif
    /* calculate battery volt */
    adcResult.vBatt.target = get_volt(&adcResult.vBatt);
#if CODE_DEBUG_EN == 1
    (void)DEBUG_OUT("VBATT:%4d  ",adcResult.vBatt.target);
#endif
    /* calculate buck volt */
    adcResult.vBuck.target = get_volt(&adcResult.vBuck);
#if CODE_DEBUG_EN == 1
    (void)DEBUG_OUT("VBUCK:%4d  ",adcResult.vBuck.target);
#endif
}


int8_t get_led_temperature(TempParams_t const table[],int16_t deltaPNVolt)
{
    int8_t temperature;
    uint8_t minPos = 0U;
    uint8_t maxPos = TEMP_CMP_NUM_MAX-1U;
    uint8_t tempPos = ((TEMP_CMP_NUM_MAX-1U)>> 1U);
    for(;;){
        if (deltaPNVolt > table[tempPos].deltaVoltPN){
            maxPos = tempPos;
        }else if (deltaPNVolt < table[tempPos].deltaVoltPN){
            minPos = tempPos;
        }else{
            break;
        }
        tempPos = (minPos + maxPos) >> 1U;
        if ((minPos + 1U)>= maxPos){
            break;
        }
    }
    if (deltaPNVolt <= table[TEMP_CMP_NUM_MAX-1U].deltaVoltPN){
        tempPos = TEMP_CMP_NUM_MAX-1U;
    }
    tempPos = tempPos << DELTA_TEMP_POS;
    temperature = (int8_t)tempPos;
    temperature = temperature + MINIMUM_TEMP;
    return temperature;
}

void updateLedTemperature(LedNum_t ledIndex)
{
    int16_t typicalVolt[3];
    ColorTemperature_t temperature;
    temperature.value[0] = ROOM_TEMPERATURE;
    temperature.value[1] = ROOM_TEMPERATURE;
    temperature.value[2] = ROOM_TEMPERATURE;
    
    (void)CLM_GetLedPNVolts(ledIndex,temperature, &typicalVolt[0], &typicalVolt[1], &typicalVolt[2]);
    for (uint8_t i = 0U; i < 3U; i++){
        adcResult.vLedPN[ledIndex][i].target = get_volt(&adcResult.vLedPN[ledIndex][i]);
        if (adcResult.vLedPN[ledIndex][i].target != (int16_t)INVALID_VOLT){
            if (i == (uint8_t)LED_RED){
                adcResult.ledPNTemperature[ledIndex][i] = get_led_temperature(LP_GetTempTableRed(),  (adcResult.vLedPN[ledIndex][i].target - typicalVolt[i]) );
            }else if (i == (uint8_t)LED_GREEN){
                adcResult.ledPNTemperature[ledIndex][i] = get_led_temperature(LP_GetTempTableGreen(),(adcResult.vLedPN[ledIndex][i].target - typicalVolt[i]) );
            }else{
                adcResult.ledPNTemperature[ledIndex][i] = get_led_temperature(LP_GetTempTableBlue(), (adcResult.vLedPN[ledIndex][i].target - typicalVolt[i]) );
            }
        }else{
            adcResult.ledPNTemperature[ledIndex][i] = ROOM_TEMPERATURE;
        }
#if CODE_DEBUG_EN == 1
        (void)DEBUG_OUT("TLED[%d][%d]:%3d  ",ledIndex,i,adcResult.vLedPN[ledIndex][i].target);
#endif  
    }
#if CODE_DEBUG_EN == 1
    (void)DEBUG_OUT("\r\n");
#endif
}


void updateParams(void)
{
    if( updateParamIndex == 0U){
        /* update system information */
        updateSystemInfo();
    }else if( updateParamIndex == 1U){
        /* update led temperature information LED0-LED3*/
#if LED_NUM > 1U
        for (uint8_t i = (uint8_t)LED0; i <(LED_NUM/2U); i++){
            updateLedTemperature((LedNum_t)i);
        }
#endif
    }else{
        /* update led temperature information LED4-LED7*/
        for (uint8_t i = (LED_NUM/2U); i < LED_NUM; i++){
            updateLedTemperature((LedNum_t)i);
        }
#if CODE_DEBUG_EN == 1
        (void)DEBUG_OUT("\n");
#endif
    }
    
    if (++updateParamIndex > 2U){
        updateParamIndex = 0U;
    }
}


void MES_TaskHandler(void)
{
    switch(adcTaskState){
    case TASK_STATE_INIT:
      measureParamInit();
      ADC_GeneralInit();/*  init ADC */
      ADC_RegisterIRQ(measureDoneISR);
      /* start updating timer */
      SoftTimer_Start(&updateTimer);
      adcTaskState = TASK_STATE_ACTIVE;
      break;
    case TASK_STATE_ACTIVE:
      /* start parameters updating  */
      if (updateParamStart != 0U){
          updateParamStart = 0U;
          updateParams();
      }
      /*  start parameters measuring */
      if (measStart != 0U){
          measStart = 0U;
          measureParamStart();
      }
      break;
    default:
      break;
    }
}


void measureParamInit(void)
{
    uint32_t crc = CRC32_GetRunTimeCRC32((uint8_t *)HWCFG_SFRS->payload+4U ,508U);
    if (crc != HWCFG_SFRS->crc32){
        dataIntegrityError = TRUE;
    }

    uint32_t bandgap_mV  = 1220U;
    uint16_t tSensorCode = 404;//HWCFG_SFRS->ADC_CODE_PTAT_LED_25C;
    int32_t tSensor_mV;
    bandgap_mV = (bandgap_mV*tSensorCode) >> 12U;/* 0.5mV */
    tSensor_mV = (int32_t)((uint32_t)((bandgap_mV * MEASURE_ADC_GAIN_FACTOR) >> MEASURE_TEMP_GAIN_POS));
    adcResult.vChipTemp.startCode = INVALID_PARAM;
    /* T = volt /0.4 *- 271 */
    adcResult.tChip.gain        = MEASURE_TEMP_GAIN;
    adcResult.tChip.offset      = 25*(int32_t)(1UL << MEASURE_TEMP_GAIN_POS) - MEASURE_TEMP_GAIN*tSensor_mV;/* 25 degree centigrade*/
    
    /* battery volt*/
    uint16_t batt13V5   = 1632U;//HWCFG_SFRS->ADC_CODE_VBAT_13P5V;
    uint16_t batt8V0    = 967U;//HWCFG_SFRS->ADC_CODE_VBAT_8P0V;
    
    if (batt8V0 == 0xFFFFU){
        adcResult.vBatt.calibP.gain      = (13500 - 0)*MEASURE_GAIN/((int32_t)batt13V5 - 0);
        adcResult.vBatt.calibP.offset    = 0;
    }else{
        adcResult.vBatt.calibP.gain      = (13500 - 8000)*MEASURE_GAIN/((int32_t)batt13V5 - (int32_t)batt8V0);
        adcResult.vBatt.calibP.offset    = 8000*MEASURE_GAIN - adcResult.vBatt.calibP.gain*(int32_t)batt8V0;
    }
    
    adcResult.vBatt.startCode    = INVALID_PARAM;
    /* buck volt*/
    uint16_t buck5V0 = 2115;//HWCFG_SFRS->BUCK_ADC_CODE_VOUT_5V;
    adcResult.vBuck.calibP.gain         = (5000- 0)*MEASURE_GAIN/((int32_t)buck5V0 - 0);
    adcResult.vBuck.calibP.offset       = 0;
    adcResult.vBuck.startCode           = INVALID_PARAM;
    
    /* led parameters init */
    uint16_t codeH,codeL;
    for (uint8_t i = 0; i < LED_NUM; i++){
        codeH = 2708;//HWCFG_SFRS->LED[PHY_CHANNEL_R(i)].ADC_CODE_PN_DELTA_4000mV;
        codeL = 1015;//HWCFG_SFRS->LED[PHY_CHANNEL_R(i)].ADC_CODE_PN_DELTA_1500mV;
        adcResult.vLedPN[i][0].calibP.gain        = (4000 - 1500)*MEASURE_GAIN/((int32_t)codeH - (int32_t)codeL);
        adcResult.vLedPN[i][0].calibP.offset      = 1500*MEASURE_GAIN - adcResult.vLedPN[i][0].calibP.gain*(int32_t)codeL;
        adcResult.vLedPN[i][0].startCode = INVALID_PARAM;
        
        codeH = 2708;//HWCFG_SFRS->LED[PHY_CHANNEL_G(i)].ADC_CODE_PN_DELTA_4000mV;
        codeL = 1015;//HWCFG_SFRS->LED[PHY_CHANNEL_G(i)].ADC_CODE_PN_DELTA_1500mV;
        adcResult.vLedPN[i][1].calibP.gain        = (4000 - 1500)*MEASURE_GAIN/((int32_t)codeH - (int32_t)codeL);
        adcResult.vLedPN[i][1].calibP.offset      = 1500*MEASURE_GAIN - adcResult.vLedPN[i][1].calibP.gain*(int32_t)codeL;
        adcResult.vLedPN[i][1].startCode = INVALID_PARAM;
        
        codeH = 2708;//HWCFG_SFRS->LED[PHY_CHANNEL_B(i)].ADC_CODE_PN_DELTA_4000mV;
        codeL = 1015;//HWCFG_SFRS->LED[PHY_CHANNEL_B(i)].ADC_CODE_PN_DELTA_1500mV;
        adcResult.vLedPN[i][2].calibP.gain       = (4000 - 1500)*MEASURE_GAIN/((int32_t)codeH - (int32_t)codeL);
        adcResult.vLedPN[i][2].calibP.offset     = 1500*MEASURE_GAIN - adcResult.vLedPN[i][2].calibP.gain*(int32_t)codeL;
        adcResult.vLedPN[i][2].startCode = INVALID_PARAM;
        
        for (uint8_t channel  = 0U; channel < 3U; channel++){
            adcResult.vLedPN[i][channel].target    = (int16_t)INVALID_VOLT;
            adcResult.ledPNTemperature[i][channel] = ROOM_TEMPERATURE;
        }
    }
    
}

/*
 *  Get factory calibration data state
 *  @return 0: calibration data OK, 1 : calibration data error
 */
uint8_t MES_DataIntegrityError(void)
{
    return dataIntegrityError;
  
}

/*
 *  Get battery voltage in mV
 *  @param [out] battVolt: 0-65535
 *  @return 0: get voltage success, other : failed
 */
int8_t MES_GetBatteryVolt(uint16_t *battVolt)
{
    int8_t result = -1;
    if (adcResult.vBatt.startCode != INVALID_PARAM){
        *battVolt = (uint16_t)adcResult.vBatt.target;
        result = 0;
    }
    return result;
}

/*
 *  Get buck voltage in mV
 *  @param [out] buckVolt: 0-65535
 *  @return 0: get voltage success, other : failed
 */
int8_t MES_GetBuckVolt(uint16_t *buckVolt)
{
    int8_t result = -1;
    if (adcResult.vBuck.startCode != INVALID_PARAM){
        *buckVolt = (uint16_t)adcResult.vBuck.target;
        result = 0;
    }
    return result;
}


int16_t MES_GetChipTemperature(int16_t *chipTemperature)
{
    int8_t result = -1;
    if (adcResult.vChipTemp.startCode != INVALID_PARAM){
        *chipTemperature = adcResult.chipTemperature;
        result = 0;
    }
    return result;
}

int16_t MES_GetChipTempVolt(uint16_t *chipTempVolt)
{
    int8_t result = -1;
    if (adcResult.vChipTemp.startCode != INVALID_PARAM){
        *chipTempVolt = (uint16_t)adcResult.vChipTemp.target;
        result = 0;
    }
    return result;
}



ColorTemperature_t MES_GetLedTemperature(LedNum_t ledNo)
{
    ColorTemperature_t temperature;
    temperature.value[0] = (int8_t)adcResult.ledPNTemperature[ledNo][0];
    temperature.value[1] = (int8_t)adcResult.ledPNTemperature[ledNo][1];
    temperature.value[2] = (int8_t)adcResult.ledPNTemperature[ledNo][2];
    return temperature;
}

int8_t MES_GetLedRGBTemperature(LedNum_t index, int8_t *rTemp,int8_t *gTemp,int8_t *bTemp)
{
    *rTemp = (int8_t)adcResult.ledPNTemperature[index][0];
    *gTemp = (int8_t)adcResult.ledPNTemperature[index][1];
    *bTemp = (int8_t)adcResult.ledPNTemperature[index][2];
    return 0;
}

/*
 *  Get Runtime Led PN voltage in mV
 *  @param [out] pnVoltR/pnVoltG/pnVoltB: 0-4000mV
 *  @return :none
 */
void MES_GetRunTimeLedPNVolt(LedNum_t ledIndex,uint16_t *pnVoltR, uint16_t *pnVoltG,uint16_t *pnVoltB)
{
    *pnVoltR = (uint16_t)adcResult.vLedPN[ledIndex][0].target;
    *pnVoltG = (uint16_t)adcResult.vLedPN[ledIndex][1].target;
    *pnVoltB = (uint16_t)adcResult.vLedPN[ledIndex][2].target;
}


void MES_MnftGetLedInfo(LedNum_t ledNo, uint16_t *pnRVolt,uint16_t *pnGVolt,uint16_t *pnBVolt)
{
    int32_t sLedCode = 0;
    uint32_t uLedCode[3];
    for(uint8_t i = 0U; i < LED_NUM; i++){
        (void)CLM_SetPWMs((LedNum_t)i,0U, 0U, 0U, 0U);  /* turn off all of channels */
    }
    while(PWM_UpdateFinished() != TRUE){}
    
    for (uint8_t led = 0U; led < 3U; led++){
        uLedCode[led] = 0U;
        ADC_Init(ADC_MEASURE_ITEM_LED,(uint8_t)ledNo,(LedType_t)led,ADC_TRIGGER_SOURCE_SOFT_INPUT);
        for (uint16_t i = 0U; i < 16U; i++){
            adcConvertDone = 0U;
            ADC_Start();/* soft trigger for adc conversion   */
            while(adcConvertDone == 0U){}
            uLedCode[led] += measGeneralAdcCode[0];
        }
        uLedCode[led] = (uLedCode[led] >> 4);
        sLedCode = (int32_t)uLedCode[led];
        sLedCode = sLedCode* adcResult.vLedPN[ledNo][led].calibP.gain + adcResult.vLedPN[ledNo][led].calibP.offset;
        uLedCode[led] = (uint32_t)sLedCode;
    }
    *pnRVolt = (uint16_t)(uLedCode[0] >> MEASURE_GAIN_POS);
    *pnGVolt = (uint16_t)(uLedCode[1] >> MEASURE_GAIN_POS);
    *pnBVolt = (uint16_t)(uLedCode[2] >> MEASURE_GAIN_POS);
    (void)sLedCode;
}























#endif












