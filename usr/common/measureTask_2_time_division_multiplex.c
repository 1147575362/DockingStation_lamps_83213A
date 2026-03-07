#include <appConfig.h>

#if (LED_NUM > 8U)


#include <crc32.h>
#include <measureTask.h>
#include <gpio_device.h>

#define AVERAGE_MEASURE_POS     (4U)
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
  uint32_t startCode;
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

void add_data_to_buff(uint32_t *original, uint16_t newData, uint16_t *bufferIndex,uint16_t *buff);
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
    adcResult.vChipTemp.target = get_chip_temp_volt(&adcResult.vChipTemp);
    adcResult.chipTemperature = get_chip_temperature(adcResult.vChipTemp.target);
    /* calculate battery volt */
    adcResult.vBatt.target = get_volt(&adcResult.vBatt);
    /* calculate buck volt */
    adcResult.vBuck.target = get_volt(&adcResult.vBuck);
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
        adcResult.vLedPN[ledIndex][i].target = get_volt(&adcResult.vLedPN[ledIndex][i]);
        if (adcResult.vLedPN[ledIndex][i].target != (int16_t)INVALID_VOLT){
            if (i == (uint8_t)LED_RED){
                adcResult.ledPNTemperature[ledIndex][i] = get_led_temperature(LP_GetTempTableRed(LED0),  (adcResult.vLedPN[ledIndex][i].target - typicalVolt[0]) );
            }else if (i == (uint8_t)LED_GREEN){
                adcResult.ledPNTemperature[ledIndex][i] = get_led_temperature(LP_GetTempTableGreen(LED0),(adcResult.vLedPN[ledIndex][i].target - typicalVolt[1]) );
            }else{
                adcResult.ledPNTemperature[ledIndex][i] = get_led_temperature(LP_GetTempTableBlue(LED0), (adcResult.vLedPN[ledIndex][i].target - typicalVolt[2]) );
            }
/*
#if CODE_DEBUG_EN == 1
        (void)DEBUG_OUT("[%d,%d]:%3d  \r\n",ledIndex,i,adcResult.vLedPN[ledIndex][i].target);
#endif  
*/        
        }else{
            adcResult.ledPNTemperature[ledIndex][i] = ROOM_TEMPERATURE;
        } 
    }
/*
#if CODE_DEBUG_EN == 1
    (void)DEBUG_OUT("\r\n");
#endif
*/
    if (++ledIndex >= LED_NUM){
        ledIndex = 0U;
    }
}


void adcCodeUpdate(void)
{
    uint16_t pnCodeR,pnCodeG,pnCodeB;
    uint16_t code = ADC_GetVTempCode();
    if (code != INVALID_ADC_CODE){
        add_data_to_buff(&adcResult.vChipTemp.startCode, code, &adcResult.vChipTemp.count,adcResult.vChipTemp.buff);
    }
    code = ADC_GetVBattCode();
    if (code != INVALID_ADC_CODE){
        add_data_to_buff(&adcResult.vBatt.startCode, code, &adcResult.vBatt.count,adcResult.vBatt.buff);
    }
    
    code = ADC_GetVBuckCode();
    if (code != INVALID_ADC_CODE){
        add_data_to_buff(&adcResult.vBuck.startCode, code, &adcResult.vBuck.count,adcResult.vBuck.buff);
    }
    
    for (uint8_t i = 0; i < LED_NUM; i++){
        (void)ADC_GetLedPNCode(i, &pnCodeR, &pnCodeG, &pnCodeB);
        if ((pnCodeR != INVALID_ADC_CODE) && (pnCodeG != INVALID_ADC_CODE) && (pnCodeB != INVALID_ADC_CODE)){
            add_data_to_buff(&adcResult.vLedPN[i][LED_RED].startCode,   pnCodeR, &adcResult.vLedPN[i][LED_RED].count,   adcResult.vLedPN[i][LED_RED].buff);
            add_data_to_buff(&adcResult.vLedPN[i][LED_GREEN].startCode, pnCodeG, &adcResult.vLedPN[i][LED_GREEN].count, adcResult.vLedPN[i][LED_GREEN].buff);
            add_data_to_buff(&adcResult.vLedPN[i][LED_BLUE].startCode,  pnCodeB, &adcResult.vLedPN[i][LED_BLUE].count,  adcResult.vLedPN[i][LED_BLUE].buff);
        }else{
            adcResult.vLedPN[i][LED_RED].startCode   = INVALID_PARAM;
            adcResult.vLedPN[i][LED_GREEN].startCode = INVALID_PARAM;
            adcResult.vLedPN[i][LED_BLUE].startCode  = INVALID_PARAM;
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
    uint16_t version = HWCFG_GetFlashMapVersion();
    if (version == 0xFFFFU){
        dataIntegrityError = TRUE;
    }else{
        HWCFG_SFRS_t *param = (HWCFG_SFRS_t *)HWCFG_SFRS_ADDR;
        uint32_t crcCalculated = CRC32_GetRunTimeCRC32( param->payload + 4U ,508U);
        uint32_t crcFromFlash = HWCFG_SFRS->crc32;
        if (crcCalculated != crcFromFlash){
            dataIntegrityError = TRUE;
        }
    }
    

    uint32_t bandgap_mV  = HWCFG_GetVBG();
    uint16_t tSensorCode = HWCFG_GetADCCode_PTATLED();
    int32_t tSensor_mV;
    bandgap_mV = (bandgap_mV*tSensorCode) >> 12U;/* 0.5mV */
    tSensor_mV = (int32_t)((uint32_t)((bandgap_mV * MEASURE_ADC_GAIN_FACTOR) >> MEASURE_TEMP_GAIN_POS));
    adcResult.vChipTemp.startCode = INVALID_PARAM;
    /* T = volt /0.4 *- 271 */
    adcResult.tChip.gain        = MEASURE_TEMP_GAIN;
    adcResult.tChip.offset      = 25*(int32_t)(1UL << MEASURE_TEMP_GAIN_POS) - MEASURE_TEMP_GAIN*tSensor_mV;/* 30 degree centigrade*/
    
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
    
    adcResult.vBatt.startCode    = INVALID_PARAM;
    /* buck volt*/
    uint16_t buck5V0 = HWCFG_SFRS->ADC_CODE_BUCK_VOUT_5V;/*HWCFG_GetADCCode_VBUCK5V0();*/
    adcResult.vBuck.calibP.gain         = (5000- 0)*MEASURE_GAIN/((int32_t)buck5V0 - 0);
    adcResult.vBuck.calibP.offset       = 0;
    adcResult.vBuck.startCode           = INVALID_PARAM;
    
    /* led parameters init */
    uint16_t codeH,codeL;
    uint8_t index;
    for (uint8_t i = 0U; i <(LED_NUM/2U); i++){
        index = i >> 1U;
        codeH = HWCFG_GetADCCodeLEDPN_4V0((uint8_t)PHY_CHANNEL_R + 3U*index);
        codeL = HWCFG_GetADCCodeLEDPN_1V5((uint8_t)PHY_CHANNEL_R + 3U*index);
        adcResult.vLedPN[i*2U][PHY_CHANNEL_R].calibP.gain      = (4000 - 1500)*MEASURE_GAIN/((int32_t)codeH - (int32_t)codeL);
        adcResult.vLedPN[i*2U][PHY_CHANNEL_R].calibP.offset    = 1500*MEASURE_GAIN - adcResult.vLedPN[i][PHY_CHANNEL_R].calibP.gain*(int32_t)codeL;
        adcResult.vLedPN[i*2U][PHY_CHANNEL_R].startCode        = INVALID_PARAM;
        
        codeH = HWCFG_GetADCCodeLEDPN_4V0((uint8_t)PHY_CHANNEL_G + 3U*index);
        codeL = HWCFG_GetADCCodeLEDPN_1V5((uint8_t)PHY_CHANNEL_G + 3U*index);
        adcResult.vLedPN[i*2U][PHY_CHANNEL_G].calibP.gain      = (4000 - 1500)*MEASURE_GAIN/((int32_t)codeH - (int32_t)codeL);
        adcResult.vLedPN[i*2U][PHY_CHANNEL_G].calibP.offset    = 1500*MEASURE_GAIN - adcResult.vLedPN[i][PHY_CHANNEL_G].calibP.gain*(int32_t)codeL;
        adcResult.vLedPN[i*2U][PHY_CHANNEL_G].startCode        = INVALID_PARAM;
        
        codeH = HWCFG_GetADCCodeLEDPN_4V0((uint8_t)PHY_CHANNEL_B + 3U*index);
        codeL = HWCFG_GetADCCodeLEDPN_1V5((uint8_t)PHY_CHANNEL_B + 3U*index);
        adcResult.vLedPN[i*2U][PHY_CHANNEL_B].calibP.gain      = (4000 - 1500)*MEASURE_GAIN/((int32_t)codeH - (int32_t)codeL);
        adcResult.vLedPN[i*2U][PHY_CHANNEL_B].calibP.offset    = 1500*MEASURE_GAIN - adcResult.vLedPN[i][PHY_CHANNEL_B].calibP.gain*(int32_t)codeL;
        adcResult.vLedPN[i*2U][PHY_CHANNEL_B].startCode        = INVALID_PARAM;

        adcResult.vLedPN[i*2U + 1U][PHY_CHANNEL_R].calibP.gain      = adcResult.vLedPN[i*2U][PHY_CHANNEL_R].calibP.gain;
        adcResult.vLedPN[i*2U + 1U][PHY_CHANNEL_R].calibP.offset    = adcResult.vLedPN[i*2U][PHY_CHANNEL_R].calibP.offset;
        adcResult.vLedPN[i*2U + 1U][PHY_CHANNEL_R].startCode        = adcResult.vLedPN[i*2U][PHY_CHANNEL_R].startCode;

        adcResult.vLedPN[i*2U + 1U][PHY_CHANNEL_G].calibP.gain      = adcResult.vLedPN[i*2U][PHY_CHANNEL_G].calibP.gain;
        adcResult.vLedPN[i*2U + 1U][PHY_CHANNEL_G].calibP.offset    = adcResult.vLedPN[i*2U][PHY_CHANNEL_G].calibP.offset;
        adcResult.vLedPN[i*2U + 1U][PHY_CHANNEL_G].startCode        = adcResult.vLedPN[i*2U][PHY_CHANNEL_G].startCode;

        adcResult.vLedPN[i*2U + 1U][PHY_CHANNEL_B].calibP.gain      = adcResult.vLedPN[i*2U][PHY_CHANNEL_B].calibP.gain;
        adcResult.vLedPN[i*2U + 1U][PHY_CHANNEL_B].calibP.offset    = adcResult.vLedPN[i*2U][PHY_CHANNEL_B].calibP.offset;
        adcResult.vLedPN[i*2U + 1U][PHY_CHANNEL_B].startCode        = adcResult.vLedPN[i*2U][PHY_CHANNEL_B].startCode;

    }
    
    for (uint8_t led = 0U; led < LED_NUM; led++){
        for (uint8_t channel  = 0U; channel < 3U; channel++){
            adcResult.vLedPN[led][channel].target    = (int16_t)INVALID_VOLT;
            adcResult.vLedPN[led][channel].startCode  = INVALID_PARAM;
            adcResult.ledPNTemperature[led][channel] = ROOM_TEMPERATURE;
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
    *pnVoltR = adcResult.vLedPN[ledIndex][0].target;
    *pnVoltG = adcResult.vLedPN[ledIndex][1].target;
    *pnVoltB = adcResult.vLedPN[ledIndex][2].target;
}



void MES_MnftGetLedInfo(LedNum_t ledNo, uint16_t *pnRVolt,uint16_t *pnGVolt,uint16_t *pnBVolt)
{
    int32_t sLedCode = 0;
    uint32_t uLedCode[3];
    uint8_t led_no = (uint8_t)ledNo;
    uint8_t lastCHSEQ;
    uint8_t lastCH2;
    if (led_no < LED_NUM){
        ADC_UnregisterIRQ();
        lastCHSEQ   = SAR_CTRL_SFRS->SAR_CHAN_CFG.CHAN_SEQ_NUM;
        lastCH2     = SAR_CTRL_SFRS->SAR_CHAN_CFG.CH1_SEL;
        
        if ((led_no - ((led_no >> 1)*2U)) == 0U){
#if LED_BOARD_TYPE == BOARD_TYPE_DP
      GPIO_SetGPIOs((1U << GPIO_PORT_2) | (1U << GPIO_PORT_3), (1U << GPIO_PORT_2));
#elif LED_BOARD_TYPE == BOARD_TYPE_SY
      GPIO_SetGPIOs((1U << GPIO_PORT_2) | (1U << GPIO_PORT_3), (1U << GPIO_PORT_3));  /* 0,2,4,6,8,10,12,14,16 */
#elif LED_BOARD_TYPE == BOARD_TYPE_S2
      GPIO_SetGPIOs((1U << GPIO_PORT_2) | (1U << GPIO_PORT_3), (1U << GPIO_PORT_2));
#endif
        }else{
#if LED_BOARD_TYPE == BOARD_TYPE_DP
      GPIO_SetGPIOs((1U << GPIO_PORT_2) | (1U << GPIO_PORT_3), (1U << GPIO_PORT_3));
#elif LED_BOARD_TYPE == BOARD_TYPE_SY
      GPIO_SetGPIOs((1U << GPIO_PORT_2) | (1U << GPIO_PORT_3), (1U << GPIO_PORT_2));  /* 0,2,4,6,8,10,12,14,16 */
#elif LED_BOARD_TYPE == BOARD_TYPE_S2
      GPIO_SetGPIOs((1U << GPIO_PORT_2) | (1U << GPIO_PORT_3), (1U << GPIO_PORT_3));
#endif
        }
        SAR_CTRL_SFRS->SAR_CTRL.SAR_ENA_REQ = 0U;
        PWM_Reset();
        while(PWM_UpdateFinished() == TRUE){}
        
        SAR_CTRL_SFRS->SAR_CTRL.TRIGSEL = (uint8_t)ADC_TRIGGER_SOURCE_SOFT_INPUT;
        SAR_CTRL_SFRS->SAR_CTRL.SAR_ENA_REQ = 1U;
        SAR_CTRL_SFRS->SAR_CHAN_CFG.CHAN_SEQ_NUM  = (uint8_t)ADC_SQ_CH1;
        
        
        SAR_CTRL_SFRS->SAR_CHAN_CFG.CH1_SEL = (uint8_t)ADC_CH_SEL_VBUCK_LED0;
        SAR_CTRL_SFRS->SAR_INT.CLEAR.INT_SAR_DONE_CLR = 1U;
        SAR_CTRL_SFRS->SAR_INT.ENABLE.INT_SAR_DONE_ENA = 1U;
        SAR_CTRL_SFRS->SAR_CTRL.CONVERT = 1U;
        while(SAR_CTRL_SFRS->SAR_INT.STATUS.INT_SAR_DONE_STS == 0U){}
        
        
        for (uint8_t channel = 0U; channel < 3U; channel++){
            uLedCode[channel] = 0U;
            SAR_CTRL_SFRS->SAR_CHAN_CFG.CH1_SEL = (uint8_t)ADC_CH_SEL_VBUCK_LED0 + (led_no >> 1) * 3U + channel;
            for (uint16_t i = 0U; i < 16U; i++){
                SAR_CTRL_SFRS->SAR_INT.CLEAR.INT_SAR_DONE_CLR = 1U;
                SAR_CTRL_SFRS->SAR_INT.ENABLE.INT_SAR_DONE_ENA = 1U;
                SAR_CTRL_SFRS->SAR_CTRL.CONVERT = 1U;
                while(SAR_CTRL_SFRS->SAR_INT.STATUS.INT_SAR_DONE_STS == 0U){}
                uLedCode[channel] += SAR_CTRL_SFRS->ADC_DATA0.DATA0;
            }
            uLedCode[channel] = (uLedCode[channel] >> 3);
            sLedCode = (int32_t)uLedCode[channel];
            sLedCode = sLedCode* adcResult.vLedPN[led_no >> 1][channel].calibP.gain + adcResult.vLedPN[led_no >> 1][channel].calibP.offset;
            uLedCode[channel] = (uint32_t)sLedCode;
        }
        *pnRVolt = (uint16_t)(uLedCode[PHY_CHANNEL_R] >> MEASURE_GAIN_POS);
        *pnGVolt = (uint16_t)(uLedCode[PHY_CHANNEL_G] >> MEASURE_GAIN_POS);
        *pnBVolt = (uint16_t)(uLedCode[PHY_CHANNEL_B] >> MEASURE_GAIN_POS);
        (void)sLedCode;
        SAR_CTRL_SFRS->SAR_CTRL.TRIGSEL =  (uint8_t)ADC_TRIGGER_SOURCE_PWM_PERIOD;
        SAR_CTRL_SFRS->SAR_CHAN_CFG.CHAN_SEQ_NUM = lastCHSEQ;
        SAR_CTRL_SFRS->SAR_CHAN_CFG.CH1_SEL = lastCH2;
        ADC_RegisterIRQ(NULL);
    }
}






















































#endif