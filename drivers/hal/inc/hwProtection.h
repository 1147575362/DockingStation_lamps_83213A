#ifndef __HW_PROTECTION_H__
#define __HW_PROTECTION_H__
#include <rugbyHP.h>
#include <core_cm0.h>

typedef enum{
  OVT_PROT_90C = 0,
  OVT_PROT_95C,
  OVT_PROT_100C,
  OVT_PROT_105C,
  OVT_PROT_110C,
  OVT_PROT_115C,
  OVT_PROT_120C,
  OVT_PROT_125C,
  OVT_PROT_130C,
  OVT_PROT_135C,
  OVT_PROT_140C,
  OVT_PROT_145C,
  OVT_PROT_150C,
  OVT_PROT_155C,
  OVT_PROT_160C,
  OVT_PROT_165C,
}OverTempThres_t;

typedef enum{
  OVT_ACTION_HW_RESET = 0,
  OVT_ACTION_IRQ,
  OVT_ACTION_NONE,
}OverTempAction_t;

typedef enum{
  OV_VOLT_14_67V = 0U,
  OV_VOLT_15_16V,
  OV_VOLT_15_68V,
  OV_VOLT_16_23V,
  OV_VOLT_16_83V,
  OV_VOLT_17_48V,
  OV_VOLT_18_17V,
  OV_VOLT_18_93V,
  OV_VOLT_19_75V,
  OV_VOLT_20_64V,
}OverVoltThres_t;

typedef enum{
  OV_HYS_02_PERCENT = 0U,  /*  3600mV @15V */
  OV_HYS_06_PERCENT,       /*  720mV  @15V*/
  OV_HYS_09_PERCENT,       /*  1080mV @15V */
  OV_HYS_12_PERCENT,       /*  1440mV @15V */
}OverVoltHysteresis_t;


typedef enum{
  UV_VOLT_4_635V = 0,
  UV_VOLT_4_705V, 
  UV_VOLT_4_775V,
  UV_VOLT_4_845V, 
  UV_VOLT_4_915V, 
  UV_VOLT_4_995V,
  UV_VOLT_5_075V, 
  UV_VOLT_5_155V,
  UV_VOLT_5_235V, 
  UV_VOLT_5_325V, 
  UV_VOLT_5_415V,
  UV_VOLT_5_505V, 
  UV_VOLT_5_605V,
  UV_VOLT_5_705V, 
  UV_VOLT_5_805V, 
  UV_VOLT_5_915V,
  UV_VOLT_6_025V, 
  UV_VOLT_6_145V,
  UV_VOLT_6_265V, 
  UV_VOLT_6_385V, 
  UV_VOLT_6_515V,
  UV_VOLT_6_655V, 
  UV_VOLT_6_795V,
  UV_VOLT_6_945V, 
  UV_VOLT_7_095V, 
  UV_VOLT_7_255V,
  UV_VOLT_7_425V,
  UV_VOLT_7_605V, 
  UV_VOLT_7_785V,
  UV_VOLT_7_985V, 
  UV_VOLT_8_185V, 
  UV_VOLT_8_405V,
  UV_VOLT_8_625V, 
  UV_VOLT_8_865V,
  UV_VOLT_9_385V, 
  UV_VOLT_9_675V, 
  UV_VOLT_9_975V,
  UV_VOLT_10_29V,
  UV_VOLT_10_63V, 
}UnderVoltThres_t;

typedef enum{
  UV_HYS_04_PERCENT = 0U,    /*  475mV @15V */
  UV_HYS_06_PERCENT,         /*  835mV @15V */
  UV_HYS_08_PERCENT,        /*  1225mV @15V */
  UV_HYS_10_PERCENT,        /*  1635mV @15V */
}UnderVoltHysteresis_t;


typedef enum{
  VOLT_DEBOUNCE_TIME_0ms  = 0U,
  VOLT_DEBOUNCE_TIME_1ms  = 16U,
  VOLT_DEBOUNCE_TIME_2ms  = 32U,
  VOLT_DEBOUNCE_TIME_3ms  = 48U,
  VOLT_DEBOUNCE_TIME_4ms  = 64U,
  VOLT_DEBOUNCE_TIME_5ms  = 80U,
  VOLT_DEBOUNCE_TIME_6ms  = 96U,
  VOLT_DEBOUNCE_TIME_7ms  = 112U,
  VOLT_DEBOUNCE_TIME_8ms  = 128U,
  VOLT_DEBOUNCE_TIME_9ms  = 144U,
  VOLT_DEBOUNCE_TIME_10ms = 160U,
  VOLT_DEBOUNCE_TIME_11ms = 176U,
  VOLT_DEBOUNCE_TIME_12ms = 192U,
  VOLT_DEBOUNCE_TIME_13ms = 208U,
  VOLT_DEBOUNCE_TIME_14ms = 224U,
  VOLT_DEBOUNCE_TIME_15ms = 240U,
}VoltDebouceTime_t;

typedef void (*hwProtIsrCallback_t)(void);


void HW_PROT_SetOverVoltage(OverVoltThres_t threshold,OverVoltHysteresis_t hys, VoltDebouceTime_t triggerTime, VoltDebouceTime_t releaseTime);
void HW_PROT_RegisterOverVoltageIRQ(hwProtIsrCallback_t callback);
void HW_PROT_UnRegisterOverVoltageIRQ(void);

void HW_PROT_SetUnderVoltage(UnderVoltThres_t threshold,UnderVoltHysteresis_t hys, VoltDebouceTime_t triggerTime, VoltDebouceTime_t releaseTime);
void HW_PROT_RegisterUnderVoltageIRQ(hwProtIsrCallback_t callback);
void HW_PROT_UnRegisterUnderVoltageIRQ(void);

/* return 0: normal , 1: low, 2:high */
uint8_t HW_PROT_GetBattVoltState(void);
/* return 0: normal , others : over heat */
uint8_t HW_PROT_ChipIsOverHeat(void);




#endif