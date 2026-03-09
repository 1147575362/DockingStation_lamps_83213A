
#include <appConfig.h>
#include <applicationTask.h>

#if (LED_NUM > 8U)


#include <crc32.h>
#include <measureTask.h>
#include <gpio_device.h>

#define AVERAGE_MEASURE_POS     (5U)
#define AVERAGE_MEASURE_GAIN    (1U << AVERAGE_MEASURE_POS)

#define MEASURE_GAIN_POS         12U
#define MEASURE_GAIN             4096 /*(1UL << MEASURE_GAIN_POS)*/

#define MEASURE_TEMP_GAIN_POS    14U
#define MEASURE_TEMP_GAIN        40960 /*16384*2.5 *2*/
#define MEASURE_ADC_GAIN_FACTOR  16913U /* 16384 / ADC_GAIN(31/32) = 16384 * 32/31 = 16913*/

#define UPDATE_TIEMS_LEDS               (1U << 0)
#define UPDATE_TIEMS_BATT_BUCK_TEMP     (1U << 1)

typedef struct{
  int32_t gain;
  int32_t offset;
}calibParam_t;

typedef struct{
  calibParam_t calibP;
  uint16_t  target;
}AdcDatabuff_t;

typedef struct{
  AdcDatabuff_t vBatt;
  AdcDatabuff_t vBuck;
  AdcDatabuff_t vLedPN[LED_NUM][3];
  AdcDatabuff_t vChipTemp;
  calibParam_t  tChip;
  uint16_t      battVolt;
  uint16_t      buckVolt;
  int16_t       chipTemperature;
  uint16_t      ledPNVolt[LED_NUM][3];
  int16_t       ledPNTemperature[LED_NUM][3];
}AdcResult_t;

static uint32_t sumchiptemp;
static uint32_t sumvbat;
static uint32_t sumvbuck;
static uint32_t sumR[LED_NUM];
static uint32_t sumG[LED_NUM];
static uint32_t sumB[LED_NUM];

static uint8_t dataIntegrityError = FALSE;

static TaskState_t adcTaskState = TASK_STATE_INIT;
static AdcResult_t adcResult;
void measureParamInit(void);
void adcCodeUpdate(void);

static uint8_t updateItems = UPDATE_TIEMS_BATT_BUCK_TEMP;
void updateTimerExpired(SoftTimer_t *timer);

int16_t get_chip_temp_volt(AdcDatabuff_t *const param);
int16_t get_volt(AdcDatabuff_t *const param);
int16_t get_chip_temperature(int16_t vTemp);
int8_t get_led_temperature(TempParams_t const table[],int16_t deltaPNVolt);
void updateLedsTemperature(void);
void updateParams(void);

void updateSystemInfo(void);

static volatile uint32_t adcConvertDone = 0U;

static SoftTimer_t updateTimer = {
    .mode     = TIMER_PERIODIC_MODE,
    .interval = 50U,
    .handler  = updateTimerExpired
};

void updateTimerExpired(SoftTimer_t *timer)
{
    static uint8_t measWaitCount = 0U;
    measWaitCount ++;
    if ( measWaitCount <= LED_NUM ){
        updateItems |= UPDATE_TIEMS_LEDS;
    }else{
        updateItems |= UPDATE_TIEMS_BATT_BUCK_TEMP;
        measWaitCount = 0;
    }
    TM_PostTask(TASK_ID_ADC_MEASURE);
}



int16_t get_chip_temp_volt(AdcDatabuff_t *const param)
{
    uint32_t bandgap_mV  = HWCFG_GetVBG();
    int16_t volt = (int16_t)INVALID_PARAM;
    if (param->target != INVALID_PARAM){
        uint32_t sum = (bandgap_mV*param->target) >> 12U;/*1mV */
        volt = (int16_t)((uint32_t)((sum*MEASURE_ADC_GAIN_FACTOR) >> MEASURE_TEMP_GAIN_POS));   
    }
    return volt;
}

int16_t get_volt(AdcDatabuff_t *const param)
{
    int16_t volt = (int16_t)INVALID_PARAM;
    uint32_t uVolt;
    if (param->target != INVALID_PARAM){
        int32_t sVolt = param->calibP.gain * (int32_t)param->target + param->calibP.offset;
        if(sVolt <=0){
            sVolt = -sVolt;
            uVolt = (uint32_t)sVolt;
            uVolt = uVolt >> MEASURE_GAIN_POS;
            volt = (int16_t)uVolt;
            volt = -volt;
        }else{
            uVolt = (uint32_t)sVolt;
            uVolt = uVolt >> MEASURE_GAIN_POS;
            volt = (int16_t)uVolt;       
        }
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
    int16_t tempVolt = get_chip_temp_volt(&adcResult.vChipTemp);
    adcResult.chipTemperature = get_chip_temperature(tempVolt);
    /* calculate battery volt */
    adcResult.battVolt = (uint16_t)get_volt(&adcResult.vBatt);
    /* calculate buck volt */
    adcResult.buckVolt = (uint16_t)get_volt(&adcResult.vBuck);
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

void updateLedsTemperature(void)
{
    static uint8_t ledIndex = 0U;
    int16_t typicalVolt[3];
    ColorTemperature_t temperature;
    temperature.value[0] = ROOM_TEMPERATURE;
    temperature.value[1] = ROOM_TEMPERATURE;
    temperature.value[2] = ROOM_TEMPERATURE;
    
    (void)CLM_GetLedPNVolts((LedNum_t)ledIndex,temperature, &typicalVolt[0], &typicalVolt[1], &typicalVolt[2]);
    for (uint8_t i = 0U; i < 3U; i++){
        adcResult.ledPNVolt[ledIndex][i] = (uint16_t)get_volt(&adcResult.vLedPN[ledIndex][i]);
        if (adcResult.vLedPN[ledIndex][i].target != INVALID_PARAM){
            if (i == (uint8_t)LED_RED){
                adcResult.ledPNTemperature[ledIndex][i] = (int16_t)get_led_temperature(LP_GetTempTableRed((LedNum_t)ledIndex),  ((int16_t)adcResult.ledPNVolt[ledIndex][i] - typicalVolt[0]) );
            }else if (i == (uint8_t)LED_GREEN){
                adcResult.ledPNTemperature[ledIndex][i] = (int16_t)get_led_temperature(LP_GetTempTableGreen((LedNum_t)ledIndex),((int16_t)adcResult.ledPNVolt[ledIndex][i] - typicalVolt[1]) );
            }else{
                adcResult.ledPNTemperature[ledIndex][i] = (int16_t)get_led_temperature(LP_GetTempTableBlue((LedNum_t)ledIndex), ((int16_t)adcResult.ledPNVolt[ledIndex][i] - typicalVolt[2]) );
            }
        }else{
            adcResult.ledPNTemperature[ledIndex][i] = ROOM_TEMPERATURE;
        } 
    }
    if (++ledIndex >= LED_NUM){
        ledIndex = 0U;
    }
}


void adcCodeUpdate(void)
{
    uint16_t pnCodeR,pnCodeG,pnCodeB;
    uint16_t code = ADC_GetVTempCode();
    if (code != INVALID_ADC_CODE){
        if (adcResult.vChipTemp.target == INVALID_PARAM){
            adcResult.vChipTemp.target = (code << 1U);
            sumchiptemp = (uint32_t)adcResult.vChipTemp.target * (AVERAGE_MEASURE_GAIN-1U) + ((uint32_t)code << 1U);
            adcResult.vChipTemp.target = (uint16_t)(sumchiptemp >> AVERAGE_MEASURE_POS); 
            sumchiptemp = (sumchiptemp* (AVERAGE_MEASURE_GAIN-1U))>> AVERAGE_MEASURE_POS;
        }else{
            sumchiptemp = sumchiptemp + ((uint32_t)code << 1U);
            adcResult.vChipTemp.target = (uint16_t)(sumchiptemp >> AVERAGE_MEASURE_POS); 
            sumchiptemp = (sumchiptemp * (AVERAGE_MEASURE_GAIN-1U))>> AVERAGE_MEASURE_POS;        
        }
    }
    code = ADC_GetVBattCode();
    if (code != INVALID_ADC_CODE){
        if (adcResult.vBatt.target == INVALID_PARAM){
            adcResult.vBatt.target = (code << 1U);
            sumvbat = (uint32_t)adcResult.vBatt.target * (AVERAGE_MEASURE_GAIN-1U) + ((uint32_t)code << 1U);
            adcResult.vBatt.target = (uint16_t)(sumvbat >> AVERAGE_MEASURE_POS); 
            sumvbat = (sumvbat* (AVERAGE_MEASURE_GAIN-1U))>> AVERAGE_MEASURE_POS;
        }else{
            sumvbat = sumvbat + ((uint32_t)code << 1U);
            adcResult.vBatt.target = (uint16_t)(sumvbat >> AVERAGE_MEASURE_POS); 
            sumvbat = (sumvbat * (AVERAGE_MEASURE_GAIN-1U))>> AVERAGE_MEASURE_POS;    
        }
    }
    
    code = ADC_GetVBuckCode();
    if (code != INVALID_ADC_CODE){
        if (adcResult.vBuck.target == INVALID_PARAM){
            adcResult.vBuck.target = (code << 1U);
            sumvbuck = (uint32_t)adcResult.vBuck.target * (AVERAGE_MEASURE_GAIN-1U) + ((uint32_t)code << 1U);
            adcResult.vBuck.target = (uint16_t)(sumvbuck >> AVERAGE_MEASURE_POS); 
            sumvbuck = (sumvbuck* (AVERAGE_MEASURE_GAIN-1U))>> AVERAGE_MEASURE_POS;
        }else{
            sumvbuck = sumvbuck + ((uint32_t)code << 1U);
            adcResult.vBuck.target = (uint16_t)(sumvbuck >> AVERAGE_MEASURE_POS); 
            sumvbuck = (sumvbuck * (AVERAGE_MEASURE_GAIN-1U))>> AVERAGE_MEASURE_POS; 
        }
    }
    
    for (uint8_t i = 0; i < LED_NUM; i++){
        (void)ADC_GetLedPNCode(i, &pnCodeR, &pnCodeG, &pnCodeB);
        if ((pnCodeR != INVALID_ADC_CODE) && (pnCodeG != INVALID_ADC_CODE) && (pnCodeB != INVALID_ADC_CODE)){
            
            if (adcResult.vLedPN[i][0].target == INVALID_PARAM){
                adcResult.vLedPN[i][0].target = (pnCodeR << 1U);
                sumR[i] = (uint32_t)adcResult.vLedPN[i][0].target * (AVERAGE_MEASURE_GAIN-1U) + (uint16_t)(pnCodeR << 1U);
                adcResult.vLedPN[i][0].target = (uint16_t)(sumR[i] >> AVERAGE_MEASURE_POS); 
                sumR[i] = (sumR[i] * (AVERAGE_MEASURE_GAIN-1U))>> AVERAGE_MEASURE_POS;
            }
            
            if (adcResult.vLedPN[i][1].target == INVALID_PARAM){
                adcResult.vLedPN[i][1].target = (pnCodeG << 1U);
                sumG[i] = (uint32_t)adcResult.vLedPN[i][1].target * (AVERAGE_MEASURE_GAIN-1U) + (uint16_t)(pnCodeG << 1U);
                adcResult.vLedPN[i][1].target = (uint16_t)(sumG[i] >> AVERAGE_MEASURE_POS); 
                sumG[i] = (sumG[i] * (AVERAGE_MEASURE_GAIN-1U))>> AVERAGE_MEASURE_POS; 
            }
            
            if (adcResult.vLedPN[i][2].target == INVALID_PARAM){
                adcResult.vLedPN[i][2].target = (pnCodeB << 1U);
                sumB[i] = (uint32_t)adcResult.vLedPN[i][2].target * (AVERAGE_MEASURE_GAIN-1U) + (uint16_t)(pnCodeB << 1U);
                adcResult.vLedPN[i][2].target = (uint16_t)(sumB[i] >> AVERAGE_MEASURE_POS); 
                sumB[i] = (sumB[i] * (AVERAGE_MEASURE_GAIN-1U))>> AVERAGE_MEASURE_POS;
            }  
            
            if ((adcResult.vLedPN[i][0].target != INVALID_PARAM) && (adcResult.vLedPN[i][1].target != INVALID_PARAM) && (adcResult.vLedPN[i][2].target != INVALID_PARAM)){            
                sumR[i] = sumR[i] + ((uint32_t)pnCodeR << 1U);
                sumG[i] = sumG[i] + ((uint32_t)pnCodeG << 1U);
                sumB[i] = sumB[i] + ((uint32_t)pnCodeB << 1U); 
                
                adcResult.vLedPN[i][0].target = (uint16_t)(sumR[i] >> AVERAGE_MEASURE_POS); 
                sumR[i] = (sumR[i] * (AVERAGE_MEASURE_GAIN-1U))>> AVERAGE_MEASURE_POS;     
                
                adcResult.vLedPN[i][1].target = (uint16_t)(sumG[i] >> AVERAGE_MEASURE_POS); 
                sumG[i] = (sumG[i] * (AVERAGE_MEASURE_GAIN-1U))>> AVERAGE_MEASURE_POS;      
                
                adcResult.vLedPN[i][2].target = (uint16_t)(sumB[i] >> AVERAGE_MEASURE_POS); 
                sumB[i] = (sumB[i] * (AVERAGE_MEASURE_GAIN-1U))>> AVERAGE_MEASURE_POS;      
            }                 
             
        }else{
            adcResult.vLedPN[i][0].target  = INVALID_PARAM;
            adcResult.vLedPN[i][1].target  = INVALID_PARAM;
            adcResult.vLedPN[i][2].target  = INVALID_PARAM;
        }
    }
}


void MES_TaskHandler(void)
{
    switch(adcTaskState){
    case TASK_STATE_INIT:
      measureParamInit();
      SoftTimer_Start(&updateTimer);
      adcTaskState = TASK_STATE_ACTIVE;
      break;
    case TASK_STATE_ACTIVE:
      if ((updateItems & UPDATE_TIEMS_LEDS) == UPDATE_TIEMS_LEDS){
          updateItems &=(~UPDATE_TIEMS_LEDS);
          updateLedsTemperature();
      }else if( (updateItems & UPDATE_TIEMS_BATT_BUCK_TEMP) == UPDATE_TIEMS_BATT_BUCK_TEMP){
          updateItems &=(~UPDATE_TIEMS_BATT_BUCK_TEMP);
          updateSystemInfo();
      }else{
          /* purpose to empty */
      }
      adcCodeUpdate();
      break;
    default:
      break;
    }
}


void measureParamInit(void)
{
    HWCFG_SFRS_t *param = (HWCFG_SFRS_t *)HWCFG_SFRS_ADDR;
    uint32_t crc = CRC32_GetRunTimeCRC32( param->payload + 4U,508U);
    if (crc != HWCFG_SFRS->crc32){
        dataIntegrityError = TRUE;
    }
    uint32_t bandgap_mV  = HWCFG_GetVBG();
    uint16_t tSensorCode = HWCFG_GetADCCode_PTATLED();
    int32_t tSensor_mV;
    bandgap_mV = (bandgap_mV*tSensorCode) >> 12U;/* 0.5mV */
    tSensor_mV = (int32_t)((uint32_t)((bandgap_mV * MEASURE_ADC_GAIN_FACTOR) >> MEASURE_TEMP_GAIN_POS));
    adcResult.vChipTemp.target = INVALID_PARAM;
    /* T = volt /0.4 *- 271 */
    adcResult.tChip.gain        = MEASURE_TEMP_GAIN;
    adcResult.tChip.offset      = 25*(int32_t)(1UL << MEASURE_TEMP_GAIN_POS) - MEASURE_TEMP_GAIN*tSensor_mV;/* 25 degree centigrade*/
    
    /* battery volt*/
    uint16_t batt13V5   = HWCFG_GetADCCode_VBAT13V5();
    uint16_t batt8V0    = HWCFG_GetADCCode_VBAT8V0();
    
    if (batt8V0 == 0xFFFFU){
        adcResult.vBatt.calibP.gain      = (13500 - 0)*MEASURE_GAIN/((int32_t)batt13V5 - 0);
        adcResult.vBatt.calibP.offset    = 0;
    }else{
        adcResult.vBatt.calibP.gain      = (13500 - 8000)*MEASURE_GAIN/((int32_t)batt13V5 - (int32_t)batt8V0);
        adcResult.vBatt.calibP.offset    = 8000*MEASURE_GAIN - adcResult.vBatt.calibP.gain*(int32_t)batt8V0;
    }
    
    adcResult.vBatt.target    = INVALID_PARAM;
    /* buck volt*/
    uint16_t buck5V0 = HWCFG_SFRS->ADC_CODE_BUCK_VOUT_5V;/*HWCFG_GetADCCode_VBUCK5V0();*/
    adcResult.vBuck.calibP.gain         = (5000- 0)*MEASURE_GAIN/((int32_t)buck5V0 - 0);
    adcResult.vBuck.calibP.offset       = 0;
    adcResult.vBuck.target           = INVALID_PARAM;
    
    /* led parameters init */
    uint16_t codeH,codeL;
    uint8_t index;
    for (uint8_t i = 0U; i < LED_NUM; i++){
      
#if (LED_NUM > 8U && LED_NUM <= 16U)
        index = i >> 1U;
#elif (LED_NUM > 16U && LED_NUM <= 24U)
        index = i / 3U;
#elif (LED_NUM > 24U && LED_NUM <= 32U)
        index = i >> 2U;;
#elif (LED_NUM > 32U && LED_NUM <= 40U)
        index = i / 5U;
#else
        #error LED_NUM must be within 8-40!
#endif
        codeH = HWCFG_GetADCCodeLEDPN_4V0((uint8_t)0U + 3U*index);
        codeL = HWCFG_GetADCCodeLEDPN_1V5((uint8_t)0U + 3U*index);
        adcResult.vLedPN[i][0].calibP.gain      = (4000 - 1500)*MEASURE_GAIN/((int32_t)codeH - (int32_t)codeL);
        adcResult.vLedPN[i][0].calibP.offset    = 1500*MEASURE_GAIN - adcResult.vLedPN[i][0].calibP.gain*(int32_t)codeL;
        adcResult.vLedPN[i][0].target           = INVALID_PARAM;
        
        codeH = HWCFG_GetADCCodeLEDPN_4V0((uint8_t)1U + 3U*index);
        codeL = HWCFG_GetADCCodeLEDPN_1V5((uint8_t)1U + 3U*index);
        adcResult.vLedPN[i][1].calibP.gain      = (4000 - 1500)*MEASURE_GAIN/((int32_t)codeH - (int32_t)codeL);
        adcResult.vLedPN[i][1].calibP.offset    = 1500*MEASURE_GAIN - adcResult.vLedPN[i][1].calibP.gain*(int32_t)codeL;
        adcResult.vLedPN[i][1].target = INVALID_PARAM;
        
        codeH = HWCFG_GetADCCodeLEDPN_4V0((uint8_t)2U + 3U*index);
        codeL = HWCFG_GetADCCodeLEDPN_1V5((uint8_t)2U + 3U*index);
        adcResult.vLedPN[i][2].calibP.gain      = (4000 - 1500)*MEASURE_GAIN/((int32_t)codeH - (int32_t)codeL);
        adcResult.vLedPN[i][2].calibP.offset    = 1500*MEASURE_GAIN - adcResult.vLedPN[i][2].calibP.gain*(int32_t)codeL;
        adcResult.vLedPN[i][2].target           = INVALID_PARAM;
    }
    
    for (uint8_t led = 0U; led < LED_NUM; led++){
        for (uint8_t channel  = 0U; channel < 3U; channel++){
            adcResult.vLedPN[led][channel].target    = (int16_t)INVALID_PARAM;
            adcResult.ledPNTemperature[led][channel] = ROOM_TEMPERATURE;
        }
    }
    
    for (uint8_t i = 0; i < LED_NUM; i++){
      sumR[i] = 0;
      sumG[i] = 0;
      sumB[i] = 0;
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
    if (adcResult.vBatt.target != INVALID_PARAM){
        *battVolt = adcResult.battVolt;
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
    if (adcResult.vBuck.target != INVALID_PARAM){
        *buckVolt = adcResult.buckVolt;
        result = 0;
    }
    return result;
}


int16_t MES_GetChipTemperature(int16_t *chipTemperature)
{
    int8_t result = -1;
    if (adcResult.vChipTemp.target != INVALID_PARAM){
        *chipTemperature = adcResult.chipTemperature;
        result = 0;
    }
    return result;
}


/*
 *  Get Runtime Led PN voltage in mV
 *  @param [out] pnVoltR/pnVoltG/pnVoltB: 0-4000mV
 *  @return :none
 */
ColorTemperature_t MES_GetLedTemperature(LedNum_t ledNo)
{
    ColorTemperature_t temperature;
    temperature.value[0] = (int8_t)adcResult.ledPNTemperature[ledNo][0];
    temperature.value[1] = (int8_t)adcResult.ledPNTemperature[ledNo][1];
    temperature.value[2] = (int8_t)adcResult.ledPNTemperature[ledNo][2];
    return temperature;
}
/*
 *  Get Runtime Led temperature in C
 *  @param [out] 
 *  @return :0
 */
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
void MES_GetRunTimeLedPNVolt(LedNum_t ledIndex,int16_t *pnVoltR, int16_t *pnVoltG,int16_t *pnVoltB)
{
    *pnVoltR = (int16_t)adcResult.ledPNVolt[ledIndex][0];
    *pnVoltG = (int16_t)adcResult.ledPNVolt[ledIndex][1];
    *pnVoltB = (int16_t)adcResult.ledPNVolt[ledIndex][2];
}



void MES_MnftGetLedInfo(LedNum_t ledNo, uint16_t *pnRVolt,uint16_t *pnGVolt,uint16_t *pnBVolt)
{
    if ((uint8_t)ledNo < LED_NUM){
        uint16_t pnRcode,pnGCode,pnBCode;
        int32_t pnR,pnG,pnB;
        
        PWM_GetStaticPNVolt((uint8_t)ledNo, &pnRcode,&pnGCode,&pnBCode);
        pnR = (int32_t)pnRcode;
        pnG = (int32_t)pnGCode;
        pnB = (int32_t)pnBCode;
        
        uint8_t ledNum = PWM_GetPhyLedIndex((uint8_t)ledNo);
        pnR = pnR* adcResult.vLedPN[ledNum][PHY_CHANNEL_R].calibP.gain + adcResult.vLedPN[ledNum][PHY_CHANNEL_R].calibP.offset;
        pnG = pnG* adcResult.vLedPN[ledNum][PHY_CHANNEL_G].calibP.gain + adcResult.vLedPN[ledNum][PHY_CHANNEL_G].calibP.offset;
        pnB = pnB* adcResult.vLedPN[ledNum][PHY_CHANNEL_B].calibP.gain + adcResult.vLedPN[ledNum][PHY_CHANNEL_B].calibP.offset;
        
        *pnRVolt = (uint16_t)((uint32_t)pnR >> MEASURE_GAIN_POS);
        *pnGVolt = (uint16_t)((uint32_t)pnG >> MEASURE_GAIN_POS);
        *pnBVolt = (uint16_t)((uint32_t)pnB >> MEASURE_GAIN_POS);
    }
}






















































#endif