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

#ifndef __TRIM_MEM_MST_SFR_H__
#define __TRIM_MEM_MST_SFR_H__

#include <stdint.h>

/* -------  Start of section using anonymous unions and disabling warnings  ------- */
#if   defined (__CC_ARM)
  #pragma push
  #pragma anon_unions
#elif defined (__ICCARM__)
  #pragma language=extended
#elif defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
  #pragma clang diagnostic push
  #pragma clang diagnostic ignored "-Wc11-extensions"
  #pragma clang diagnostic ignored "-Wreserved-id-macro"
#elif defined (__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined (__TMS470__)
  /* anonymous unions are enabled by default */
#elif defined (__TASKING__)
  #pragma warning 586
#elif defined (__CSMC__)
  /* anonymous unions are enabled by default */
#else
  #warning Not supported compiler type
#endif

/**
 * @brief A structure to represent Special Function Registers for TRIM_MEM_MST.
 */
typedef struct {

  union {
    struct {
      uint8_t  EFUSE0_LOCK              :  1; /*!< The lock signal of eFUSE0 */
      uint8_t  EN_FPWM                  :  1; /*!< Enable force PWM option */
      uint8_t  TRIM_CLP_DN              :  2; /*!< Clamp down voltage for VC_EA */
      uint8_t  TRIM_DLY                 :  4; /*!< Trim delay time for better accuracy. */
      uint8_t  TRIM_CLK                 :  5; /*!< Trim internal BUCK clock(2Mhz/400Khz) */
      uint8_t  TRIM_BIAS_CLP_UP         :  3; /*!< Trim clamp up bias */
      uint8_t  TRIM_BUCK_DAC            :  5; /*!< Trim BUCK dac precision */
      uint8_t  TRIM_IRAMP_COMP          :  3; /*!< Trim the current which generates compensation ramp */
      uint8_t                           :  4; /*   (reserved) */
      uint8_t  TRIM_ISNS                :  4; /*!< Trim the offset current of HS current sensor. */
    };
    uint32_t WORD;
  } TRIM_DATA0; /* +0x000 */

  union {
    struct {
      uint8_t  EFUSE1_LOCK              :  1; /*!< The lock signal of eFUSE1 */
      uint8_t  DIS_AZCD_CAL             :  1; /*!< Disable auto zcd calculation. */
      uint8_t  TRIM_HS_LMT              :  2; /*!< Trim the current limit of high side NMOS. */
      uint8_t  TRIM_ZCD                 :  4; /*!< Trim the offset voltage of ZCD comparator. */
      uint8_t  TRIM_IBIAS_NRES          :  5;
      uint8_t  LDO1V5_SEL               :  1; /*!< Select 1p5v ldo type */
      uint8_t  TRIM_R_I2V               :  2; /*!< Trim the resistance of I to V sampling circuit. */
      uint8_t  SEL_FREQ                 :  8; /*!< Set BUCK clock frequency. */
      uint8_t  VBUCK_DAC_MAX            :  6; /*!< Set maximum output voltage code of BUCK */
      uint8_t  TRIM_NCD                 :  2; /*!< Trim the threshold current for NCD. */
    };
    uint32_t WORD;
  } TRIM_DATA1; /* +0x004 */

  union {
    struct {
      uint8_t  VDAC_TRIM                :  8; /*!< TRIM code for VDAC */
      uint8_t  RAMP_VAL                 :  8; /*!< VDAC ramp start code */
      uint8_t  RAMP_RATE                :  2; /*!< VDAC ramp rate to reach to VDAC_TRIM */
      uint8_t  RAMP_ENA                 :  1; /*!< Enable the Ramp feature for VDAC */
      uint8_t  BUCKDIS_RAMPDOWN_ENA     :  1; /*!< Enable of Ramp down after Core Buck disable */
      uint8_t  DTB_ANA_SEL              :  4; /*!< select for DTB */
      uint8_t  BUCK_DEBUG_CODE          :  8; /*!< Code used to set debug mode of BUCK control module */
    };
    uint32_t WORD;
  } BUCK_CFG0; /* +0x008 */

  union {
    struct {
      uint8_t  DBG_EN_BUCK_PRE          :  1; /*!< Enable V-I and comp_top to detect errors in debug mode */
      uint8_t  DBG_EN_BUCK              :  1; /*!< BUCK control enable in debug mode */
      uint8_t  DBG_EN_BUCK_PWM          :  1; /*!< BUCK enable of PWM Logic in debug mode */
      uint8_t  DBG_EN_BUCK_DRV          :  1; /*!< BUCK enable of power stage and driver in debug mode */
      uint8_t  PRE_ENA_DWELL            :  4; /*!< PRE_ENA Timeout Setting */
      uint8_t  DET_LOW_DWELL            :  4; /*!< DET_LOW Timeout Setting */
      uint8_t  START_ENA_DWELL          :  4; /*!< START_ENA Timeout Setting */
      uint8_t  START_PWM_DWELL          :  4; /*!< START_PWM Timeout Setting */
      uint8_t  START_DRV_DWELL          :  4; /*!< START_DRV Timeout Setting */
      uint8_t  START_OK_PRE_DWELL       :  4; /*!< START_OK_PRE Timeout Setting */
      uint8_t  HICCUP_DWELL             :  4; /*!< HICCUP Timeout Setting */
    };
    uint32_t WORD;
  } BUCK_CFG1; /* +0x00C */

  union {
    struct {
      uint8_t  BUCK_IGNORE              :  4; /*!< BUCK Status ignore */
      uint8_t  HICCUP_ENA               :  1; /*!< BUCK HICCUP Enable */
      uint8_t                           :  3; /*   (reserved) */
      uint8_t  CFG_CLPUP                :  3; /*!< Set clamp up configuration. */
      uint8_t  SR_CTRL                  :  1; /*!< Set slew rate. */
      uint8_t  CFG_UVLO                 :  1; /*!< Set UVLO threshold. */
      uint8_t  DIS_VOUT_OV              :  1; /*!< Set VOUT OV option. */
      uint8_t  EN_ACT_CLP               :  1; /*!< Set active clamp down option. */
      uint8_t                           :  1; /*   (reserved) */
      uint8_t  TRIM_CODE                :  8; /*!< Code used to set trim enable */
      uint8_t  EFUSE_MODE               :  1;
      uint8_t  EFUSE_SELECT             :  1;
      uint8_t  SET_VOUT_PD              :  1;
      uint8_t                           :  5; /*   (reserved) */
    };
    uint32_t WORD;
  } BUCK_CFG2; /* +0x010 */

  union {
    struct {
      uint8_t  IDLE_HOLD_DBNC_ENA       :  1; /*!< IDLE_HOLD debounce enable. */
      uint8_t  VOUT_LOW_DBNC_ENA        :  1; /*!< VOUT_LOW debounce enable. */
      uint8_t  VIN_UV_DBNC_ENA          :  1; /*!< VIN_UV debounce enable. */
      uint8_t  VOUT_SHORT_DBNC_ENA      :  1; /*!< VOUT_SHORT debounce enable. */
      uint8_t  VOUT_OK_DBNC_ENA         :  1; /*!< VOUT_OK debounce enable. */
      uint8_t  VOUT_OV_DBNC_ENA         :  1; /*!< VOUT_OV debounce enable. */
      uint8_t                           :  2; /*   (reserved) */
      uint8_t  IDLE_HOLD_POL            :  1; /*!< The valid level polarity for IDLE_HOLD signal */
      uint8_t  VOUT_LOW_POL             :  1; /*!< The valid level polarity for BUCK VOUT LOW signal */
      uint8_t  VIN_UV_POL               :  1; /*!< The valid level polarity for BUCK VIN UVLO signal */
      uint8_t  VOUT_SHORT_POL           :  1; /*!< The valid level polarity for BUCK VOUT SHORT signal */
      uint8_t  VOUT_OK_POL              :  1; /*!< The valid level polarity for BUCK VOUT OK signal */
      uint8_t  VOUT_OV_POL              :  1; /*!< The valid level polarity for BUCK VOUT OV signal */
      uint8_t                           :  2; /*   (reserved) */
      uint8_t  IDLE_HOLD_DBNC_THRES1    :  4; /*!< IDLE_HOLD debounce threshold from 0 to 1 */
      uint8_t  IDLE_HOLD_DBNC_THRES0    :  4; /*!< IDLE_HOLD debounce threshold from 1 to 0 */
      uint8_t  VOUT_LOW_DBNC_THRES1     :  4; /*!< VOUT_LOW debounce threshold from 0 to 1 */
      uint8_t  VOUT_LOW_DBNC_THRES0     :  4; /*!< VOUT_LOW debounce threshold from 1 to 0 */
    };
    uint32_t WORD;
  } BUCK_CFG3; /* +0x014 */

  union {
    struct {
      uint8_t  VIN_UV_DBNC_THRES1       :  4; /*!< VIN_UV debounce threshold from 0 to 1 */
      uint8_t  VIN_UV_DBNC_THRES0       :  4; /*!< VIN_UV debounce threshold from 1 to 0 */
      uint8_t  VOUT_SHORT_DBNC_THRES1   :  4; /*!< VOUT_SHORT debounce threshold from 0 to 1 */
      uint8_t  VOUT_SHORT_DBNC_THRES0   :  4; /*!< VOUT_SHORT debounce threshold from 1 to 0 */
      uint8_t  VOUT_OK_DBNC_THRES1      :  4; /*!< VOUT_OK debounce threshold from 0 to 1 */
      uint8_t  VOUT_OK_DBNC_THRES0      :  4; /*!< VOUT_OK debounce threshold from 1 to 0 */
      uint8_t  VOUT_OV_DBNC_THRES1      :  4; /*!< VOUT_OV debounce threshold from 0 to 1 */
      uint8_t  VOUT_OV_DBNC_THRES0      :  4; /*!< VOUT_OV debounce threshold from 1 to 0 */
    };
    uint32_t WORD;
  } BUCK_CFG4; /* +0x018 */

  union {
    struct {
      uint8_t  INT_START_ERROR_ENA      :  1; /*!< BUCK start error interrupt enable */
      uint8_t  INT_VIN_UV_ENA           :  1; /*!< Vin under-voltage interrupt enable */
      uint8_t  INT_VOUT_SHORT_ENA       :  1; /*!< Vout short interrupt enable */
      uint8_t  INT_VOUT_OK_ENA          :  1; /*!< Vout soft start OK interrupt enable */
      uint8_t  INT_VOUT_OV_ENA          :  1; /*!< Vout over-voltage interrupt enable */
      uint8_t  INT_EFUSE0_DONE_ENA      :  1; /*!< EFUSE macro0 done interrupt enable */
      uint8_t  INT_EFUSE1_DONE_ENA      :  1; /*!< EFUSE macro1 done interrupt enable */
      uint8_t                           :  1; /*   (reserved) */
      uint8_t  SEL_REG_EFUSE_CODE       :  8; /*!< Code used to set BUCK trim data sources */
      uint8_t  EFUSE_CODE               :  8; /*!< Code used to set EFUSE configurate enable */
      uint8_t  BUCK_ENA                 :  8; /*!< BUCK enable request */
    };
    uint32_t WORD;
  } BUCK_CFG5; /* +0x01C */

  union {
    struct {
      uint8_t  SEL_SSC_STEP             :  3; /*!< Set step period used in SSC. */
      uint8_t  SSC_ENA                  :  1; /*!< SSC enable. */
      uint8_t  DBG_ENTEST_BIAS          :  3; /*!< Debug signal in debug mode. */
      uint8_t  DBG_DLY_TEST             :  1; /*!< Debug signal in debug mode. */
      uint8_t  SSC_RAMP_CODE            :  5; /*!< Set SSC ramp code in debug mode. */
      uint8_t  DBG_EN_RCTEST            :  1; /*!< Debug signal in debug mode. */
      uint8_t                           :  2; /*   (reserved) */
      uint8_t  TRIM_IS_LSTRIG           :  4; /*!< Trim the derivative of the curve of LS current limit vs VEA-VCP_L */
      uint8_t  TRIM_IS_LSTRIG_PBIAS1    :  4; /*!< Trim the upper limit of the curve of LS current limit vs VEA-VCP_L */
      uint8_t  TRIM_IS_LSTRIG_PBIAS2    :  8; /*!< Trim the upper limit of the curve of LS current limit vs VEA-VCP_L */
    };
    uint32_t WORD;
  } BUCK_CFG6; /* +0x020 */

  union {
    struct {
      uint8_t  VOUT_LOW_RAW             :  1; /*!< Raw status of Buck output low */
      uint8_t  VIN_UV_RAW               :  1; /*!< Raw status of Buck Input UVLO */
      uint8_t  VOUT_SHORT_RAW           :  1; /*!< Raw status of Buck Output short */
      uint8_t  VOUT_OK_RAW              :  1; /*!< Raw status of Buck Output voltage OK */
      uint8_t  VOUT_OV_RAW              :  1; /*!< Raw status of Buck Output over-voltage */
      uint8_t  HS_ON_RAW                :  1; /*!< Raw status of Buck High Side NMOS ON */
      uint8_t  LS_ON_RAW                :  1; /*!< Raw status of Buck Low Side NMOS ON */
      uint8_t  HS_ILMT_COMP_OUT_RAW     :  1; /*!< Raw status of Buck High Side COMP OUT */
      uint8_t  HS_ILMT_RAW              :  1; /*!< Raw status of Buck High Side NMOS Over Current. */
      uint8_t  LS_ILMT_RAW              :  1; /*!< Raw status of Buck Low Side NMOS Over Current */
      uint8_t  ZCD_RAW                  :  1; /*!< Raw status of Buck Low Side NMOS is reach to zero current */
      uint8_t  CLP_L_RAW                :  1; /*!< Raw status of clamp low for EA */
      uint8_t  VBTPORN_RAW              :  1; /*!< Raw status of voltage for VBT */
      uint8_t  PWM_COMP_OUT_RAW         :  1; /*!< Raw status of Buck PWM Comparator output */
      uint8_t                           :  2; /*   (reserved) */
      uint8_t  INT_START_ERROR_STS      :  1; /*!< BUCK start error interrupt status */
      uint8_t  INT_VIN_UV_STS           :  1; /*!< Vin under-voltage interrupt status */
      uint8_t  INT_VOUT_SHORT_STS       :  1; /*!< Vout short interrupt status */
      uint8_t  INT_VOUT_OK_STS          :  1; /*!< Vout soft start OK interrupt status */
      uint8_t  INT_VOUT_OV_STS          :  1; /*!< Vout over-voltage interrupt status */
      uint8_t  INT_EFUSE0_DONE_STS      :  1; /*!< EFUSE macro0 done interrupt status */
      uint8_t  INT_EFUSE1_DONE_STS      :  1; /*!< EFUSE macro1 done interrupt status */
      uint8_t                           :  1; /*   (reserved) */
      uint8_t  EFUSE0_BUSY              :  1; /*!< EFUSE0 busy signal */
      uint8_t  EFUSE1_BUSY              :  1; /*!< EFUSE1 busy signal */
      uint8_t                           :  2; /*   (reserved) */
      uint8_t  TRIM_STATUS              :  1;
      uint8_t  DEBUG_STATUS             :  1;
      uint8_t                           :  2; /*   (reserved) */
    };
    uint32_t WORD;
  } BUCK_STS0; /* +0x024 */

  union {
    struct {
      uint8_t  EFUSE_STATE              :  4; /*!< EFUSE FSM state */
      uint8_t  BUCK_STATE               :  4; /*!< BUCK FSM state */
      uint8_t  BUCK_ERROR               :  8; /*!< BUCK error state */
      uint16_t                          : 16; /*   (reserved) */
    };
    uint32_t WORD;
  } BUCK_STS1; /* +0x028 */

  union {
    struct {
      uint8_t  PIB_CODE                 :  8; /*!< Code used to enable parallel interface bus control */
      uint8_t  PIB_ADDR_START           :  8; /*!< Parallel interface bus address */
      uint8_t  PIB_ADDR_NUM             :  8; /*!< Parallel interface bus address number to be accessed */
      uint8_t  PIB_TRG                  :  1;
      uint8_t  PIB_RNW                  :  1;
      uint8_t                           :  6; /*   (reserved) */
    };
    uint32_t WORD;
  } PIB_CTRL; /* +0x02C */

  union {
    struct {
      uint8_t  EFUSE_START              :  1;
      uint8_t  RAMP_UPDATE              :  1; /*!< Update RAMP Configuration */
      uint8_t                           :  6; /*   (reserved) */
      uint8_t  INT_START_ERROR_CLR      :  1; /*!< BUCK start error interrupt clear */
      uint8_t  INT_VIN_UV_CLR           :  1; /*!< Vin under-voltage interrupt clear */
      uint8_t  INT_VOUT_SHORT_CLR       :  1; /*!< Vout short interrupt clear */
      uint8_t  INT_VOUT_OK_CLR          :  1; /*!< Vout soft start OK interrupt clear */
      uint8_t  INT_VOUT_OV_CLR          :  1; /*!< Vout over-voltage interrupt clear */
      uint8_t  INT_EFUSE0_DONE_CLR      :  1; /*!< EFUSE macro0 done interrupt clear */
      uint8_t  INT_EFUSE1_DONE_CLR      :  1; /*!< EFUSE macro1 done interrupt clear */
      uint8_t                           :  1; /*   (reserved) */
      uint8_t  BUCK_ERROR_CLR           :  8; /*!< BUCK error flag clear */
      uint8_t  INT_START_ERROR_IRQ      :  1; /*!< BUCK start error interrupt activity */
      uint8_t  INT_VIN_UV_IRQ           :  1; /*!< Vin under-voltage interrupt activity */
      uint8_t  INT_VOUT_SHORT_IRQ       :  1; /*!< Vout short interrupt activity */
      uint8_t  INT_VOUT_OK_IRQ          :  1; /*!< Vout soft start OK interrupt activity */
      uint8_t  INT_VOUT_OV_IRQ          :  1; /*!< Vout over-voltage interrupt activity */
      uint8_t  INT_EFUSE0_DONE_IRQ      :  1; /*!< EFUSE macro0 done interrupt activity */
      uint8_t  INT_EFUSE1_DONE_IRQ      :  1; /*!< EFUSE macro1 done interrupt activity */
      uint8_t                           :  1; /*   (reserved) */
    };
    uint32_t WORD;
  } BUCK_CTRL_INT; /* +0x030 */

} TRIM_MEM_MST_SFRS_t;

/* --------  End of section using anonymous unions and disabling warnings  -------- */
#if   defined (__CC_ARM)
  #pragma pop
#elif defined (__ICCARM__)
  /* leave anonymous unions enabled */
#elif (__ARMCC_VERSION >= 6010050)
  #pragma clang diagnostic pop
#elif defined (__GNUC__)
  /* anonymous unions are enabled by default */
#elif defined (__TMS470__)
  /* anonymous unions are enabled by default */
#elif defined (__TASKING__)
  #pragma warning restore
#elif defined (__CSMC__)
  /* anonymous unions are enabled by default */
#else
  #warning Not supported compiler type
#endif

/**
 * @brief The starting address of TRIM_MEM_MST SFRS.
 */
#define TRIM_MEM_MST_SFRS ((__IO TRIM_MEM_MST_SFRS_t *)0x50010500)

#endif /* end of __TRIM_MEM_MST_SFR_H__ section */


