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

#ifndef HWCFG_H__
#define HWCFG_H__

#include <rugbyHP.h>

#define E_HWCFG_SYSTEM_CLOCK_MAIN_RC_HF        (16000000U)
#define E_HWCFG_SYSTEM_CLOCK_MAIN_RC_LF        (10000U)

#define E_HWCFG_ADC_VBG              1220U       /* 1.22V */
#define E_HWCFG_ADC_PAT_LED          404U
#define E_HWCFG_ADC_VBAT_13V5        1632U
#define E_HWCFG_ADC_VBAT_8V0         967U
#define E_HWCFG_ADC_VBUCK_5V0        2115U

typedef enum {
  CURR_30MA = 0U,
}CurrValue_t;

typedef enum {
  OFF_CURR_2MA = 0U,
}OffCurrValue_t;

typedef struct{
  uint16_t ADC_CODE_PN_DELTA_1500mV;
  uint16_t ADC_CODE_PN_DELTA_4000mV;
  uint16_t LED_CURR_30mA;
  uint16_t LED_CURR_2mA;
  uint16_t LED_CURR_30mA_TRIM_CODE;
  uint16_t RESVD[3];
}LedChannelParam_t;/* 8*2 = 16 */


/**
 * @brief A structure to represent the data in hardware config block.
 */
typedef union {
    struct {/* 56*2 = 112 */
        struct {
            uint32_t crc32;
            struct {
                uint32_t  NAME;
                uint8_t   VERSION;
                uint8_t   LOT_NO[3];
                uint32_t  ATPR;
                uint32_t  CHIPID;
                uint32_t  TP_SVN_R;
            }CHIP;
            uint16_t FLASH_MAP_VERSION;
            uint16_t  CUSTOMER_INFO[3];
            
            uint16_t VREF_1P2_TRIM_CODE;
            uint16_t VREF_1P2_VOLT_mV;
            
            uint16_t ADC_CODE_PTAT_LED_25C;             /* OVERSAMPLING 11bits to 12bits  INP / INN BUFFERED gain=31/32ADC vref=1.2v*/
            uint16_t ADC_CODE_BUCK_VOUT_5V;             /* BUCK_ADC_CODE_VOUT_5V */
            uint16_t TRIM_CODE_LDO_5P0_OCP;             
            uint16_t TRIM_CODE_LDO_3P3V;
            uint16_t TRIM_CODE_LDO_3P3V_OCP;
            uint16_t TRIM_CODE_LDO_1P5V;
            uint16_t TRIM_CODE_LDO_1P5V_OCP;
            uint16_t TRIM_CODE_RC_LF_250KHZ;
            uint16_t TRIM_CODE_RC_HF_16MHZ;
            uint16_t RC_HF_16MHZ_FREQ_KHz;
            
            uint16_t ADC_CODE_0P5V;                     /* OVERSAMPLING 11bits to 12bits INP:BUFFERED  INN:EXT gain=22/32,ADC vref=2.4v */
            uint16_t ADC_CODE_1P0V;                     /* OVERSAMPLING 11bits to 12bits INP:BUFFERED  INN:EXT gain=22/32,ADC vref=2.4v */
            uint16_t ADC_CODE_VBAT_13P5V;               /* OVERSAMPLING 11bits to 12bits INP:BUFFERED INN:EXT gain=1/14,ADC vref=2.4v*/
            uint16_t ADC_CODE_VBAT_8P0V;                /* OVERSAMPLING 11bits to 12bits INP:BUFFERED INN:EXT gain=1/14,ADC vref=2.4v*/
            
            uint16_t LIN_SLAVE_TRIM_CODE_SLEWRATE;
            uint16_t LIN_SLAVE_TRIM_CODE_DUTY;
            uint16_t LIN_MASTER_TRIM_CODE_SLEWRATE;
            uint16_t LIN_MASTER_TRIM_CODE_DUTY;
            
            uint16_t ADC_REF_VBG_mV;
            uint16_t ADC_REF_2VBG_mV;
  
            uint16_t ADC_CODE_PTAT_BUCK_25C;            /* OVERSAMPLING 11bits to 12bits  INP / INN BUFFERED gain=31/32ADC vref=1.2v*/
            uint16_t BUCK_TRIM_CODE_DAC;
            uint16_t BUCK_TRIM_CODE_DATA0_EN_FPWM;
            uint16_t BUCK_TRIM_CODE_DATA0_CLP_DN;
            uint16_t BUCK_TRIM_CODE_DATA0_DLY;
            uint16_t BUCK_TRIM_CODE_DATA0_CLK;
            uint16_t BUCK_TRIM_CODE_DATA0_BIAS_CLP_UP;
            uint16_t BUCK_TRIM_CODE_DATA0_BUCK_DAC;
            uint16_t BUCK_TRIM_CODE_DATA0_IRAMP_COMP;
            uint16_t BUCK_TRIM_CODE_DATA0_ISNS;
            
            uint8_t  BUCK_TRIM_CODE_DATA1_DIS_AZCD_CAL;
            uint8_t  BUCK_TRIM_CODE_DATA1_HS_LMT;
            uint16_t BUCK_TRIM_CODE_DATA1_ZCD;
            uint16_t BUCK_TRIM_CODE_DATA1_IBAS_NRES;
            uint8_t  BUCK_TRIM_CODE_DATA1_LDO1V5_SEL;
            uint8_t  BUCK_TRIM_CODE_DATA1_R_I2V;
            uint16_t BUCK_TRIM_CODE_DATA1_SEL_FREQ_CODE;
            uint16_t BUCK_TRIM_CODE_DATA1_IRAMP_CLK;
            uint16_t BUCK_TRIM_CODE_DATA1_VBUCK_DAC_MAX;
            uint16_t BUCK_TRIM_CODE_DATA1_NCD;
            
        };
        /*  8*2 = 16*/
        uint16_t LED_TRIM_CODE_BIAS_30mA;
        uint16_t LED_TRIM_CODE_VFW_2mA;
        uint16_t TRIM_CODE_RESVD0;
        uint8_t  TRIM_CODE_LINS_TX_BIAS_BOOST;
        uint8_t  TRIM_CODE_LINM_TX_BIAS_BOOST;
        uint16_t LED_RESVD[4];
        /*  16*24 = 384*/
        LedChannelParam_t  LED[24];                     /* OVERSAMPLING 11bits to 12bits INP:EXT  INN:EXT 2/5, gain=31/32,ADC vref=2.4v */
    };
    uint8_t payload[512];

} HWCFG_SFRS_t;

/**
 * @brief The starting address of hardware config block.
 */
#define HWCFG_SFRS ((__IO HWCFG_SFRS_t *) (0x00010200))
#define HWCFG_SFRS_ADDR                   (0x00010200)






uint16_t HWCFG_GetFlashMapVersion(void);
uint8_t  HWCFG_GetRCHFClockCalibValue(void);
uint8_t  HWCFG_GetRCLFClockCalibValue(void);
uint16_t HWCFG_GetVBG(void);
uint16_t HWCFG_Get2VBG(void);
uint16_t HWCFG_GetADCCode_PTATLED(void);
uint16_t HWCFG_GetADCCode_VBAT13V5(void);
uint16_t HWCFG_GetADCCode_VBAT8V0(void);
uint16_t HWCFG_GetADCCode_VBUCK5V0(void);
uint16_t HWCFG_GetADCCodeLEDPN_4V0(uint8_t ledChannel);
uint16_t HWCFG_GetADCCodeLEDPN_1V5(uint8_t ledChannel);
uint16_t HWCFG_GetLINS_SlewRate(void);
uint16_t HWCFG_GetLINS_TxDuty(void);
uint16_t HWCFG_GetLINM_SlewRate(void);
uint16_t HWCFG_GetLINM_TxDuty(void);
uint32_t HWCFG_GetCalibrationVersion(void);
uint8_t  HWCFG_GetLEDBiasTrimValue(void);
uint16_t HWCFG_GetLEDTrimValue(uint8_t channel);
uint8_t  HWCFG_GetOffLEDTrimValue(void);
uint8_t  HWCFG_Get1V2TrimCode(void);
uint8_t  HWCFG_GetLDO1V5TrimCode(void);
uint8_t  HWCFG_GetLDO3V3TrimCode(void);
uint8_t  HWCFG_GetLDO1V5OCPTrimCode(void);
uint8_t  HWCFG_GetLDO3V3OCPTrimCode(void);
uint8_t  HWCFG_GetLDO5V0OCPTrimCode(void);
void HWCFG_TrimAccessUnlock(void);
void HWCFG_TrimAccessLockUntilReset(void);

uint8_t HWCFG_GetLINS_TxBiasBoost(void);
uint8_t HWCFG_GetLINM_TxBiasBoost(void);


#endif /* __HWCFG_H__ */

