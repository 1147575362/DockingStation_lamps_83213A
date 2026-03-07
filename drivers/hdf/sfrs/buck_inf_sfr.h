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

#ifndef BUCK_INF_SFR_H__
#define BUCK_INF_SFR_H__

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
 * @brief A structure to represent Special Function Registers for BUCK_INF.
 */
typedef struct {
    uint32_t TRIM_DATA0;
    uint32_t TRIM_DATA1;

  union {
    struct {
      uint8_t  VDAC_TRIM                :  8; /*!< TRIM code for VDAC */
      uint8_t  RAMP_VAL                 :  8; /*!< VDAC ramp start code */
      uint8_t  RAMP_RATE                :  2; /*!< VDAC ramp rate to reach to VDAC_TRIM */
      uint8_t  RAMP_ENA                 :  1; /*!< Enable the Ramp feature for VDAC */
      uint8_t  BUCKDIS_RAMPDOWN_ENA     :  1; /*!< Enable of Ramp down after Core Buck disable */
      uint8_t  DTB_ANA_SEL              :  4; /*!< select for DTB */
      uint8_t  RAMP_UPDATE              :  1; /*!< Update VDAC ramp parameters */
      uint8_t  EN_FDET_OUTOV            :  1; /*!< Force enable VOUT OVP detect in BUCK auto mode */
      uint8_t                           :  2; /*   (reserved) */
      uint8_t  DBG_SEL_OVST_OVSD        :  1; /*!< Control BUCK vout ov threshold in debug mode */
      uint8_t                           :  3; /*   (reserved) */
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
      uint8_t  DIS_PS_MD                :  1; /*!< Disable pfm_ps_st2 output. */
      uint8_t  BUCK_DEBUG_CODE          :  8; /*!< Code used to set debug mode of BUCK control module */
      uint8_t  BUCK_ENA                 :  1; /*!< BUCK enable request */
      uint8_t  SET_VOUT_PD              :  1;
      uint8_t                           :  2; /*   (reserved) */
      uint8_t  BUCK_ERROR_CLR           :  3; /*!< BUCK error flag clear */
      uint8_t                           :  1; /*   (reserved) */
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

  uint32_t BUCK_CFG5;

  union {
    struct {
      uint8_t  SEL_SSC_STEP             :  3; /*!< Set step period used in SSC. */
      uint8_t  SSC_ENA                  :  1; /*!< SSC enable. */
      uint8_t  SOFT_RSTN_REQ            :  1; /*!< Generate a pulse used to reset BUCK 5V logic. */
      uint8_t                           :  3; /*   (reserved) */
      uint8_t  SSC_RAMP_CODE            :  8; /*!< Set SSC ramp code in debug mode */
      uint8_t  TRIM_IS_LSTRIG           :  4; /*!< Trim the derivative of the curve of LS current limit vs VEA-VCP_L */
      uint8_t  TRIM_IS_LSTRIG_PBIAS1    :  4; /*!< Trim the upper limit of the curve of LS current limit vs VEA-VCP_L */
      uint8_t  TRIM_IS_LSTRIG_PBIAS2    :  8; /*!< Trim the upper limit of the curve of LS current limit vs VEA-VCP_L */
    };
    uint32_t WORD;
  } BUCK_CFG6; /* +0x020 */

  union {
    struct {
      uint8_t  BUCK_DEBUG_DATA0         :  8;
      uint8_t  BUCK_DEBUG_DATA1         :  8;
      uint8_t  BUCK_DEBUG_DATA2         :  8;
      uint8_t  BUCK_DEBUG_DATA3         :  8;
    };
    uint32_t WORD;
  } BUCK_DBG; /* +0x024 */

  union {
    struct {
      uint8_t  VOUT_LOW_RAW             :  1; /*!< Raw status of Buck output low */
      uint8_t  VIN_UV_RAW               :  1; /*!< Raw status of Buck Input UVLO */
      uint8_t  VOUT_SHORT_RAW           :  1; /*!< Raw status of Buck Output short */
      uint8_t  VOUT_OK_RAW              :  1; /*!< Raw status of Buck Output voltage OK */
      uint8_t  VOUT_OV_RAW              :  1; /*!< Raw status of Buck Output over-voltage */
      uint8_t  HS_ON_RAW                :  1; /*!< Raw status of Buck High Side NMOS ON */
      uint8_t  LS_ON_RAW                :  1; /*!< Raw status of Buck Low Side NMOS ON */
      uint8_t                           :  1; /*   (reserved) */
      uint8_t  HS_ILMT_RAW              :  1; /*!< Raw status of Buck High Side NMOS Over Current. */
      uint8_t  LS_ILMT_RAW              :  1; /*!< Raw status of Buck Low Side NMOS Over Current */
      uint8_t  ZCD_RAW                  :  1; /*!< Raw status of Buck Low Side NMOS is reach to zero current */
      uint8_t  ZCD_COMP_OUT_RAW         :  1; /*!< Raw status of ZCD comparator output */
      uint8_t  CLP_L_RAW                :  1; /*!< Raw status of clamp low for EA */
      uint8_t  VBTPORN_RAW              :  1; /*!< Raw status of voltage for VBT */
      uint8_t  PWM_COMP_OUT_RAW         :  1; /*!< Raw status of Buck PWM Comparator output */
      uint8_t                           :  1; /*   (reserved) */
      uint8_t  BUCK_OFF                 :  1; /*!< Status of Buck off */
      uint8_t  BUCK_READY               :  1; /*!< Status of Buck ready */
      uint8_t  BUCK_IDLE                :  1; /*!< Status of Buck idle */
      uint8_t                           :  5; /*   (reserved) */
      uint8_t  EFUSE0_BUSY              :  1;
      uint8_t  EFUSE1_BUSY              :  1;
      uint8_t  DEBUG_STATUS             :  1;
      uint8_t                           :  5; /*   (reserved) */
    };
    uint32_t WORD;
  } BUCK_STS0; /* +0x028 */

  union {
    struct {
      uint8_t  EFUSE_STATE              :  4; 
      uint8_t  BUCK_STATE               :  4; /*!< BUCK FSM state */
      uint8_t  BUCK_ERROR               :  8; /*!< BUCK error flag */
      uint16_t                          : 16; /*   (reserved) */
    };
    uint32_t WORD;
  } BUCK_STS1; /* +0x02C */

  union {
    struct {
        union{
            struct{
                uint8_t  INT_START_ERROR_ENA      :  1; /*!< BUCK start error interrupt enable */
                uint8_t  INT_VIN_UV_ENA           :  1; /*!< Vin under-voltage interrupt enable */
                uint8_t  INT_VOUT_SHORT_ENA       :  1; /*!< Vout short interrupt enable */
                uint8_t  INT_VOUT_OK_ENA          :  1; /*!< Vout soft start OK interrupt enable */
                uint8_t  INT_VOUT_OV_ENA          :  1; /*!< Vout over-voltage interrupt enable */
                uint8_t  INT_VOUT_OC_ENA          :  1; /*!< Vout over-current interrupt enable */
                uint8_t  INT_EFUSE0_DONE_ENA      :  1;
                uint8_t  INT_EFUSE1_DONE_ENA      :  1;
            };
            uint8_t BYTE;
        }ENABLE;
        union{
            struct{
                uint8_t  INT_START_ERROR_CLR      :  1; /*!< BUCK start error interrupt clear */
                uint8_t  INT_VIN_UV_CLR           :  1; /*!< Vin under-voltage interrupt clear */
                uint8_t  INT_VOUT_SHORT_CLR       :  1; /*!< Vout short interrupt clear */
                uint8_t  INT_VOUT_OK_CLR          :  1; /*!< Vout soft start OK interrupt clear */
                uint8_t  INT_VOUT_OV_CLR          :  1; /*!< Vout over-voltage interrupt clear */
                uint8_t  INT_VOUT_OC_CLR          :  1; /*!< Vout over-current interrupt clear */
                uint8_t  INT_EFUSE0_DONE_CLR      :  1;
                uint8_t  INT_EFUSE1_DONE_CLR      :  1;
            };
            uint8_t BYTE;
        }CLEAR;
        union{
            struct{
                uint8_t  INT_START_ERROR_STS      :  1; /*!< BUCK start error interrupt status */
                uint8_t  INT_VIN_UV_STS           :  1; /*!< Vin under-voltage interrupt status */
                uint8_t  INT_VOUT_SHORT_STS       :  1; /*!< Vout short interrupt status */
                uint8_t  INT_VOUT_OK_STS          :  1; /*!< Vout soft start OK interrupt status */
                uint8_t  INT_VOUT_OV_STS          :  1; /*!< Vout over-voltage interrupt status */
                uint8_t  INT_VOUT_OC_STS          :  1; /*!< Vout over-current interrupt status */
                uint8_t  INT_EFUSE0_DONE_STS      :  1;
                uint8_t  INT_EFUSE1_DONE_STS      :  1;
            };
            uint8_t BYTE;
        }STATUS;
        union{
            struct{
                uint8_t  INT_START_ERROR_IRQ      :  1; /*!< BUCK start error interrupt activity */
                uint8_t  INT_VIN_UV_IRQ           :  1; /*!< Vin under-voltage interrupt activity */
                uint8_t  INT_VOUT_SHORT_IRQ       :  1; /*!< Vout short interrupt activity */
                uint8_t  INT_VOUT_OK_IRQ          :  1; /*!< Vout soft start OK interrupt activity */
                uint8_t  INT_VOUT_OV_IRQ          :  1; /*!< Vout over-voltage interrupt activity */
                uint8_t  INT_VOUT_OC_IRQ          :  1; /*!< Vout over-current interrupt activity */
                uint8_t  INT_EFUSE0_DONE_IRQ      :  1;
                uint8_t  INT_EFUSE1_DONE_IRQ      :  1;
            };
            uint8_t BYTE;
        }IRQ;
    };
    uint32_t WORD;
  } BUCK_CTRL_INT; /* +0x030 */

} BUCK_INF_SFRS_t;

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
 * @brief The starting address of BUCK_INF SFRS.
 */
#define BUCK_INF_SFRS ((__IO BUCK_INF_SFRS_t *)0x50010500)

#endif /* end of __BUCK_INF_SFR_H__ section */


