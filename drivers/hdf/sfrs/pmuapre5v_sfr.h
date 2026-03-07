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

#ifndef PMUAPRE5V_SFR_H__
#define PMUAPRE5V_SFR_H__

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
 * @brief A structure to represent Special Function Registers for PMUAPRE5V.
 */
typedef struct {

  uint8_t  RETAIN0;                           /*<! Firmware scratch register 0 +0x000 */
  uint8_t  _RESERVED_01[3];                   /* +0x001 */

  union {
    struct {
      uint8_t  CFG_ACCESS_KEY           :  4;
      uint8_t  CFG_ACCESS_ENABLED       :  1;
      uint8_t                           :  3; /*   (reserved) */
      uint8_t                           :  8; /*   (reserved) */
      uint8_t                           :  8; /*   (reserved) */
      uint8_t                           :  7; /*   (reserved) */
      uint8_t  CFG_LOCK                 :  1;
    };
    uint32_t WORD;
  } CFG_ACCESS; /* +0x004 */

  union {
    struct {
      uint8_t  LINS_RXD_HIGH_RST_ENA    :  8; /*!< Enable signal that LINs RXD glitch filter at sleep mode is asynchronous reset by high of LINs_RXD */
      uint8_t  LINS_SLEEP_GF_THRES0     :  4;
      uint8_t  LINS_SLEEP_GF_THRES1     :  4;
      uint8_t  LINM_RXD_HIGH_RST_ENA    :  8; /*!< Enable signal that LINs RXD glitch filter at sleep mode is asynchronous reset by high of LINs_RXD */
      uint8_t  LINM_SLEEP_GF_THRES0     :  4;
      uint8_t  LINM_SLEEP_GF_THRES1     :  4;
    };
    uint32_t WORD;
  } LIN_SLEEP_GF; /* +0x008 */

  union {
    struct {
      uint8_t                           :  2; /*   (reserved) */
      uint8_t  LINS_WUP_DETECT_THRES    :  6; /*!< LIN Slave Wakeup Detection Threshold */
      uint8_t  LINS_BUS_INACTIVE        :  2; /*!< LIN Slave Bus Inactivity Time */
      uint8_t  LINS_BUS_DOMN_RLS_WUP_ENA :  1; /*!< LIN Slave Bus Dominant Release Wakeup Enable */
      uint8_t  LINS_BUS_IDLE_TO_DOMN    :  1; /*!< Dominant Bus Idle Timeout */
      uint8_t                           :  4; /*   (reserved) */
      uint8_t                           :  2; /*   (reserved) */
      uint8_t  LINM_WUP_DETECT_THRES    :  6; /*!< LIN Master Wakeup Detection Threshold */
      uint8_t  LINM_BUS_INACTIVE        :  2; /*!< LIN Master Bus Inactivity Time */
      uint8_t  LINM_BUS_DOMN_RLS_WUP_ENA :  1; /*!< LIN Master Bus Dominant Release Wakeup Enable */
      uint8_t  LINM_BUS_IDLE_TO_DOMN    :  1; /*!< Dominant Bus Idle Timeout */
      uint8_t                           :  4; /*   (reserved) */
    };
    uint32_t WORD;
  } LIN_WUP; /* +0x00C */

  union {
    struct {
      uint8_t  LINS_PU30K_ENA           :  1; /*!< LIN 30K pullup enable */
      uint8_t  LINS_RXENA               :  1; /*!< LIN receive  enable */
      uint8_t                           :  2; /*   (reserved) */
      uint8_t  SWON                     :  1; /*!< LIN Dual Mode Switch On */
      uint8_t  SWOFF_TIMEOUT            :  1; /*!< Switch off in dominant TimeOut condition */
      uint8_t  LINS_PUOFF_TIMEOUT       :  1; /*!< LINS Pullup Disable in dominant TimeOut condition */
      uint8_t                           :  1; /*   (reserved) */
      uint8_t  LINS_RX_BIAS_BOOST       :  2;
      uint8_t  LINS_TX_BIAS_BOOST       :  3; /*!< LIN Slave IO TX Bias select */
      uint8_t  LIN_TX_S_DT              :  3; /*!< LIN Slave duty cycle adjust */
      uint8_t  LIN_TX_S_SL              :  8; /*!< LIN Slave TX driver slew rate select */
      uint8_t  LIN_DBG                  :  4; /*!< LIN DEBUG */
      uint8_t                           :  3; /*   (reserved) */
      uint8_t  SWON_LOCK                :  1; /*!< SWON Lock Bit */
    };
    uint32_t WORD;
  } LINS; /* +0x010 */

  union {
    struct {
      uint8_t                           :  2; /*   (reserved) */
      uint8_t  LINM_PU1K_ENA            :  1; /*!< LIN 1K  pullup enable */
      uint8_t  LINM_RXENA               :  1; /*!< LIN receive  enable */
      uint8_t                           :  3; /*   (reserved) */
      uint8_t  LINM_PUOFF_TIMEOUT       :  1; /*!< LINM 1K Pullup disable in dominant TimeOut condition */
      uint8_t                           :  8; /*   (reserved) */
      uint8_t                           :  3; /*   (reserved) */
      uint8_t  LINM_RX_BIAS_BOOST       :  2;
      uint8_t  LINM_TX_BIAS_BOOST       :  3; /*!< LIN Master IO TX Bias select */
      uint8_t  LIN_TX_M_DT              :  3; /*!< LIN Master duty cycle adjust */
      uint8_t  LIN_TX_M_SL              :  4; /*!< LIN Master TX driver slew rate select */
      uint8_t                           :  1; /*   (reserved) */
    };
    uint32_t WORD;
  } LINM; /* +0x014 */

  union {
    struct {
      uint8_t  PD1V5_ENA_HIBERNATE      :  1; /*!< enable of 1V5 Power Domain at Hibernate mode */
      uint8_t  OPT_EN_LP                :  1;
      uint8_t  DBG_EN_LP_BG             :  1;
      uint8_t  DBG_EN_LP_1V5            :  1;
      uint8_t  DBG_EN_LP_5V0            :  1;
      uint8_t  DBG_EN_LP_BOR1V5         :  1;
      uint8_t  DBG_DIS_LDO_1V5          :  1;
      uint8_t  DBG_EN_LDO_3V3           :  1;
      uint8_t  DBG_EN_BOR_1V5           :  1;
      uint8_t  DBG_EN_BOR_3V3           :  1;
      uint8_t  DBG_DISCHARGE_1V5        :  1;
      uint8_t  DBG_DISCHARGE_3V3        :  1;
      uint8_t  DBG_ISO                  :  1;
      uint8_t  OVERTEMP_ENA             :  1; /*!< OverTemp Monitor Enable bit */
      uint8_t  VDD1V5_LDO_CHOOSE        :  1;
      uint8_t                           :  1; /*   (reserved) */
      uint8_t  TRIM_BG_V2I              :  4;
      uint32_t TRIM_BG                  :  6;
      uint8_t  DBG_ACCESS_KEY           :  4;
      uint8_t  DBG_ACCESS_ENABLED       :  1;
      uint8_t  DBG_LOCK                 :  1;
    };
    uint32_t WORD;
  } CTRL; /* +0x018 */

  union {
    struct {
      uint8_t  S_BOR_3V3                :  3; /*!< BOR 3v3 threshold */
      uint8_t                           :  1; /*   (reserved) */
      uint8_t  S_BOR_1V5                :  3; /*!< BOR 1v5 threshold */
      uint8_t                           :  1; /*   (reserved) */
      uint8_t  BOR_3V3_ACTION           :  1; /*!< BOR 3v3 action */
      uint8_t  BOR_1V5_ACTION           :  1; /*!< BOR 1v5 action */
      uint8_t  BOR_LOCK                 :  1;
      uint8_t                           :  1; /*   (reserved) */
      uint8_t  BOR_3V3_FLAG             :  1; /*!< BOR 3v3 flag */
      uint8_t  BOR_1V5_FLAG             :  1; /*!< BOR 1v5 flag */
      uint8_t  BOR_3V3_FLAG_CLR         :  1; /*!< BOR 3v3 clear */
      uint8_t  BOR_1V5_FLAG_CLR         :  1; /*!< BOR 1v5 clear */
      uint16_t                          : 16; /*   (reserved) */
    };
    uint32_t WORD;
  } BOR; /* +0x01C */

  union {
    struct {
      uint8_t  ENA_BOR1V5_DEGLITCH      :  1;
      uint8_t                           :  3; /*   (reserved) */
      uint8_t  SEL_BOR1V5_NEG_DEGLITCH  :  2;
      uint8_t  SEL_BOR1V5_POS_DEGLITCH  :  2;
      uint8_t  ENA_BOR3V3_DEGLITCH      :  1;
      uint8_t                           :  3; /*   (reserved) */
      uint8_t  SEL_BOR3V3_NEG_DEGLITCH  :  2;
      uint8_t  SEL_BOR3V3_POS_DEGLITCH  :  2;
      uint16_t                          : 16; /*   (reserved) */
    };
    uint32_t WORD;
  } BORDEGLITCH; /* +0x020 */

  union {
    struct {
      uint8_t  PMUA_PRE5V_CLKMUX_SEL    :  1; /*!< 5V domain Clock select */
      uint8_t                           :  3; /*   (reserved) */
      uint8_t  EN_HIGH_BW_LF_RC         :  1;
      uint8_t                           :  3; /*   (reserved) */
      uint8_t  TRIM_LF_RC               :  8; /*!< LF RC oscillator trim */
      uint8_t  CLK_LF_RC_STS            :  1; /*!< Slow oscillator status */
      uint8_t  CLK_HF_RC_STS            :  1; /*!< Fast oscillator status */
      uint8_t                           :  6; /*   (reserved) */
      uint8_t                           :  8; /*   (reserved) */
    };
    uint32_t WORD;
  } CLK_CTRL; /* +0x024 */

  union {
    struct {
      uint8_t  DFT_CODE                 :  8;
      uint8_t                           :  1; /*   (reserved) */
      uint8_t  DFT_LOCK                 :  1;
      uint8_t  DFT_ACCESS_ENABLED       :  1;
      uint8_t  DFT_TESTMODE_START       :  1;
      uint8_t                           :  4; /*   (reserved) */
      uint16_t                          : 16; /*   (reserved) */
    };
    uint32_t WORD;
  } DFT; /* +0x028 */

} PMUAPRE5V_SFRS_t;

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
 * @brief The starting address of PMUAPRE5V SFRS.
 */
#define PMUAPRE5V_SFRS ((__IO PMUAPRE5V_SFRS_t *)0x50015000)

#endif /* end of __PMUAPRE5V_SFR_H__ section */


