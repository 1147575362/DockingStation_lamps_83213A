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

#include <stddef.h>
#include <stdint.h>
#include <rugbyHP.h>
#include <hwcfg.h>

uint16_t HWCFG_GetFlashMapVersion(void)
{
    uint16_t version = (uint16_t)HWCFG_SFRS->FLASH_MAP_VERSION;
    return version;
}

uint8_t HWCFG_GetRCHFClockCalibValue(void)
{
    uint16_t version = (uint16_t)HWCFG_SFRS->FLASH_MAP_VERSION;
    uint8_t trim = (uint8_t)HWCFG_SFRS->TRIM_CODE_RC_HF_16MHZ;
    if (trim == 0xFFU || version == 0xFFFFU ){
        trim = 0xA4U;
    }
    return trim;
}

  
uint8_t HWCFG_GetRCLFClockCalibValue(void)
{
    uint16_t version = (uint16_t)HWCFG_SFRS->FLASH_MAP_VERSION;
    uint8_t trim =  (uint8_t)HWCFG_SFRS->TRIM_CODE_RC_LF_250KHZ;
    if (trim == 0xFFU || version == 0xFFFFU ){
        trim = 0x96U;
    }
    return trim;
}


uint16_t HWCFG_GetVBG(void)
{
    uint16_t version = (uint16_t)HWCFG_SFRS->FLASH_MAP_VERSION;
    uint16_t volt = HWCFG_SFRS->ADC_REF_VBG_mV;
    if (volt == 0xFFFFU || version == 0xFFFFU){
        volt = E_HWCFG_ADC_VBG;
    }
    return volt;
}

uint16_t HWCFG_Get2VBG(void)
{
    uint16_t version = (uint16_t)HWCFG_SFRS->FLASH_MAP_VERSION;
    uint16_t volt = HWCFG_SFRS->ADC_REF_2VBG_mV;
    if (volt == 0xFFFFU || version == 0xFFFFU){
        volt = E_HWCFG_ADC_VBG*2U;
    }
    return volt;
}


uint16_t HWCFG_GetADCCode_PTATLED(void)
{
    uint16_t version = (uint16_t)HWCFG_SFRS->FLASH_MAP_VERSION;
    uint16_t volt = HWCFG_SFRS->ADC_CODE_PTAT_LED_25C;
    if (volt == 0xFFFFU || version == 0xFFFFU){
        volt = E_HWCFG_ADC_PAT_LED;
    }
    return volt;
}

uint16_t HWCFG_GetLINS_SlewRate(void)
{
    uint16_t slewRate = HWCFG_SFRS->LIN_SLAVE_TRIM_CODE_SLEWRATE;
    uint16_t version = HWCFG_SFRS->FLASH_MAP_VERSION;
    if (slewRate > 0x0FU || version == 0xFFFFU){
        slewRate = (uint8_t)4U;
    }
    return slewRate;
}

uint16_t HWCFG_GetLINS_TxDuty(void)
{
    uint16_t txDuty = HWCFG_SFRS->LIN_SLAVE_TRIM_CODE_DUTY;
    uint16_t version = HWCFG_SFRS->FLASH_MAP_VERSION;
    if (txDuty > 0x07U || version == 0xFFFFU){
        txDuty = (uint8_t)5U;
    }
    return txDuty;
}


uint8_t HWCFG_GetLINS_TxBiasBoost(void)
{
    uint16_t version = HWCFG_SFRS->FLASH_MAP_VERSION;
    uint8_t txBiasBoost = (uint8_t)HWCFG_SFRS->TRIM_CODE_LINS_TX_BIAS_BOOST;
    if (txBiasBoost > 0x07U || version == 0xFFFFU){
        txBiasBoost = (uint8_t)4U;
    }
    return txBiasBoost;
}


uint16_t HWCFG_GetLINM_SlewRate(void)
{
    uint16_t slewRate = HWCFG_SFRS->LIN_MASTER_TRIM_CODE_SLEWRATE;
    uint16_t version = HWCFG_SFRS->FLASH_MAP_VERSION;
    if (slewRate > 0x0FU || version == 0xFFFFU){
        slewRate = (uint8_t)4U;
    }
    return slewRate;
}

uint16_t HWCFG_GetLINM_TxDuty(void)
{
    uint16_t txDuty = HWCFG_SFRS->LIN_MASTER_TRIM_CODE_DUTY;
    uint16_t version = HWCFG_SFRS->FLASH_MAP_VERSION;
    if (txDuty > 0x07U || version == 0xFFFFU){
        txDuty = (uint8_t)5U;
    }
    return txDuty;
}

uint8_t HWCFG_GetLINM_TxBiasBoost(void)
{
    uint8_t txBiasBoost = (uint8_t)HWCFG_SFRS->TRIM_CODE_LINM_TX_BIAS_BOOST;
    uint16_t version = HWCFG_SFRS->FLASH_MAP_VERSION;
    if (txBiasBoost > 0x07U || version == 0xFFFFU){
        txBiasBoost = (uint8_t)4U;
    }
    return txBiasBoost;
}


uint16_t HWCFG_GetADCCode_VBAT13V5(void)
{
    uint16_t volt = HWCFG_SFRS->ADC_CODE_VBAT_13P5V;
    uint16_t version = HWCFG_SFRS->FLASH_MAP_VERSION;
    if (volt == 0xFFFFU || version == 0xFFFFU){
        volt = E_HWCFG_ADC_VBAT_13V5;
    }
    return volt;
}

uint16_t HWCFG_GetADCCode_VBAT8V0(void)
{
    uint16_t volt = HWCFG_SFRS->ADC_CODE_VBAT_8P0V;
    uint16_t version = HWCFG_SFRS->FLASH_MAP_VERSION;
    if (volt == 0xFFFFU || version == 0xFFFFU){
        volt = E_HWCFG_ADC_VBAT_8V0;
    }
    return volt;
}


uint16_t HWCFG_GetADCCode_VBUCK5V0(void)
{
    uint16_t volt = HWCFG_SFRS->ADC_CODE_BUCK_VOUT_5V;
    uint16_t version = HWCFG_SFRS->FLASH_MAP_VERSION;
    if (volt > 0x0FFFU || version == 0xFFFFU){
        volt = E_HWCFG_ADC_VBUCK_5V0;
    }
    return volt;
}


uint16_t HWCFG_GetADCCodeLEDPN_4V0(uint8_t ledChannel)
{
    uint16_t code_val = 2708U;
    if (ledChannel < 24U){
        uint16_t version = HWCFG_SFRS->FLASH_MAP_VERSION;
        code_val = HWCFG_SFRS->LED[ledChannel].ADC_CODE_PN_DELTA_4000mV;
        if (code_val == 0xFFFFU || version == 0xFFFFU){
            code_val =  2708U; 
        }
    }
    return code_val;
}

uint16_t HWCFG_GetADCCodeLEDPN_1V5(uint8_t ledChannel)
{
    uint16_t code_val = 1015U;
    if (ledChannel < 24U){
        uint16_t version = HWCFG_SFRS->FLASH_MAP_VERSION;
        code_val = HWCFG_SFRS->LED[ledChannel].ADC_CODE_PN_DELTA_1500mV;
        if (code_val == 0xFFFFU || version == 0xFFFFU){
            code_val =  1015U; 
        }
    }
    return code_val;
}


/**
 * @brief Get the version of calibration data.
 *
 * @return The version for this calibration data block.
 */
uint32_t HWCFG_GetCalibrationVersion(void)
{
    uint32_t calibVersion = HWCFG_SFRS->CHIP.VERSION;
    return calibVersion;
}

uint8_t HWCFG_GetLEDBiasTrimValue(void)
{
    uint8_t trimValue = (uint8_t)HWCFG_SFRS->LED_TRIM_CODE_BIAS_30mA;
    uint16_t version  = HWCFG_SFRS->FLASH_MAP_VERSION;
    if (trimValue == 0xFFU || version == 0xFFFFU){
        trimValue =  0x80U;
    }
    return trimValue;
}


uint16_t HWCFG_GetLEDTrimValue(uint8_t ledChannel)
{
    uint16_t trimValue =  300U;
    if (ledChannel < 24U){
        uint16_t version  = HWCFG_SFRS->FLASH_MAP_VERSION;
        trimValue = HWCFG_SFRS->LED[ledChannel].LED_CURR_30mA_TRIM_CODE;
        if (trimValue > 0x1FFU || version == 0xFFFFU){
            trimValue =  300U; /* 100uA*300 = 30mA */
        }
    }
    return trimValue;
}


uint8_t HWCFG_GetOffLEDTrimValue(void)
{
    uint8_t trimValue = (uint8_t)HWCFG_SFRS->LED_TRIM_CODE_VFW_2mA;
    uint16_t version  = HWCFG_SFRS->FLASH_MAP_VERSION;
    if (trimValue == 0xFFU || version == 0xFFFFU){
        trimValue = 200U;
    }
    return trimValue;
}



uint8_t HWCFG_Get1V2TrimCode(void)
{
    uint8_t code = (uint8_t)HWCFG_SFRS->VREF_1P2_TRIM_CODE;
    uint16_t version  = HWCFG_SFRS->FLASH_MAP_VERSION;
    if (code == 0xFFU || version == 0xFFFFU){
        code = 0U;
    }
    return code;
}


uint8_t HWCFG_GetLDO1V5TrimCode(void)
{
    uint8_t trim = (uint8_t)HWCFG_SFRS->TRIM_CODE_LDO_1P5V;
    uint16_t version  = HWCFG_SFRS->FLASH_MAP_VERSION;
    if (trim > 0x03U || version == 0xFFFFU){
        trim = 0x1U;
    }
    return trim;
}

uint8_t HWCFG_GetLDO3V3TrimCode(void)
{
    uint8_t trim = (uint8_t)HWCFG_SFRS->TRIM_CODE_LDO_3P3V;
    uint16_t version  = HWCFG_SFRS->FLASH_MAP_VERSION;
    if (trim > 0x07U || version == 0xFFFFU){
        trim = 0x04U;
    }
    return trim;
}

uint8_t HWCFG_GetLDO1V5OCPTrimCode(void)
{
    uint8_t trim = (uint8_t)HWCFG_SFRS->TRIM_CODE_LDO_1P5V_OCP;
    uint16_t version  = HWCFG_SFRS->FLASH_MAP_VERSION;
    if (trim > 0x03U || version == 0xFFFFU){
        trim = 0x1U;
    }
    return trim;
}
uint8_t HWCFG_GetLDO3V3OCPTrimCode(void)
{
    uint8_t trim = (uint8_t)HWCFG_SFRS->TRIM_CODE_LDO_3P3V_OCP; 
    uint16_t version  = HWCFG_SFRS->FLASH_MAP_VERSION;
    if (trim > 0x0FU || version == 0xFFFFU){
        trim = 0x9U;
    }
    return trim;
}
uint8_t HWCFG_GetLDO5V0OCPTrimCode(void)
{
    uint8_t trim = (uint8_t)HWCFG_SFRS->TRIM_CODE_LDO_5P0_OCP;
    uint16_t version  = HWCFG_SFRS->FLASH_MAP_VERSION;
    if (trim > 0x07U || version == 0xFFFFU){
        trim = 0x05U;
    }
    return trim;
}



void HWCFG_TrimAccessUnlock(void)
{
    SYSCTRLA_SFRS->PMU_ACCESS_KEY.PMU_ACCESS_KEY        = 0x0AU;
    SYSCTRLA_SFRS->TRIM_ACCESS_KEY.TRIM_ACCESS_KEY      = 0x0EU;
    PMUAPRE5V_SFRS->CFG_ACCESS.CFG_ACCESS_KEY           = 0x0BU;
}


void HWCFG_TrimAccessLockUntilReset(void)
{
    SYSCTRLA_SFRS->TRIM_ACCESS_KEY.TRIM_LOCK = 1U;
}


    
