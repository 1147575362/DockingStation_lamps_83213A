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

#ifndef __SAR_CTRL_SFR_H__
#define __SAR_CTRL_SFR_H__

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
 * @brief A structure to represent Special Function Registers for SAR_CTRL.
 */
typedef struct {

  union {
    struct {
      uint8_t  SAR_ENA_REQ              :  1; /*!< SAR ADC Enable */
      uint8_t  SAR_PREAMP_EN            :  1; /*!< adc pre-amp enable */
      uint8_t  SAR_AFE_EN               :  1; /*!< ADC AFE Enable */
      uint8_t  CONVERT                  :  1; /*!< ADC START Register */
      uint8_t  DIG_RESET                :  1; /*!< SAR Digital Part Reset */
      uint8_t  SAR_BUSY                 :  1; /*!< SAR ADC busy */
      uint8_t                           :  2; /*   (reserved) */
      uint8_t  CONT                     :  1; /*!< Continuous Conversion Enable */
      uint8_t                           :  3; /*   (reserved) */
      uint8_t  AVER_SEL                 :  2; /*!< Select average numbers of adc sample data */
      uint8_t  EN_AVER                  :  1; /*!< Enable sample data average */
      uint8_t                           :  1; /*   (reserved) */
      uint8_t  PWMSEL                   :  8; /*!< PWM Trigger Signal Selects */
      uint8_t  TRIGSEL                  :  4; /*!< SAR Converion Trigger Selects */
      uint8_t  ADC_SEL_VIN_VCM_EXT      :  1; /*!< Select External Inputs to ADC */
      uint8_t                           :  3; /*   (reserved) */
    };
    uint32_t WORD;
  } SAR_CTRL; /* +0x000 */

  union {
    struct {
      uint8_t  ADC_VREF_SEL_CH1         :  2; /*!< adc vref select for channel 1 */
      uint8_t  ADC_VREF_SEL_CH2         :  2; /*!< adc vref select for channel 2 */
      uint8_t  ADC_VREF_SEL_CH3         :  2; /*!< adc vref select for channel 3 */
      uint8_t  ADC_VREF_SEL_CH4         :  2; /*!< adc vref select for channel 4 */
      uint8_t  SAR_INPUT_GAIN_CH1       :  2; /*!< choose ADC input gain for channel 1 */
      uint8_t  SAR_INPUT_GAIN_CH2       :  2; /*!< choose ADC input gain for channel 2 */
      uint8_t  SAR_INPUT_GAIN_CH3       :  2; /*!< choose ADC input gain for channel 3 */
      uint8_t  SAR_INPUT_GAIN_CH4       :  2; /*!< choose ADC input gain for channel 4 */
      uint8_t  SAR_INPUT_MODE_CH1       :  2; /*!< ADC AFE Input Modes for channel 1 */
      uint8_t  SAR_INPUT_MODE_CH2       :  2; /*!< ADC AFE Input Modes for channel 2 */
      uint8_t  SAR_INPUT_MODE_CH3       :  2; /*!< ADC AFE Input Modes for channel 3 */
      uint8_t  SAR_INPUT_MODE_CH4       :  2; /*!< ADC AFE Input Modes for channel 4 */
      uint8_t                           :  8; /*   (reserved) */
    };
    uint32_t WORD;
  } SAR_CFG0; /* +0x004 */

  union {
    struct {
      uint8_t  ROUND                    :  8; /*!< ADC round enable */
      uint8_t  SAMPCYC_CH1              :  4; /*!< Sample cycle for channel 1 */
      uint8_t  SAMPCYC_CH2              :  4; /*!< Sample cycle for channel 2 */
      uint8_t  SAMPCYC_CH3              :  4; /*!< Sample cycle for channel 3 */
      uint8_t  SAMPCYC_CH4              :  4; /*!< Sample cycle for channel 4 */
      uint8_t                           :  8; /*   (reserved) */
    };
    uint32_t WORD;
  } SAR_CFG1; /* +0x008 */

  union {
    struct {
      uint8_t  SAR_CLK_DIV              :  8; /*!< SAR ADC reference clock div select */
      uint8_t  TS_SEL_CH1               :  1; /*!< Temp sensor select for channel 1. */
      uint8_t  TS_SEL_CH2               :  1; /*!< Temp sensor select for channel 2. */
      uint8_t  TS_SEL_CH3               :  1; /*!< Temp sensor select for channel 3. */
      uint8_t  TS_SEL_CH4               :  1; /*!< Temp sensor select for channel 4. */
      uint8_t                           :  4; /*   (reserved) */
      uint8_t  TRIG_DLY                 :  8; /*!< Trigger delay used for non temp sensor sampling channel. */
      uint8_t  TRIG_DLY_TS              :  8; /*!< Trigger delay used for temp sensor sampling channel. */
    };
    uint32_t WORD;
  } SAR_CFG2; /* +0x00C */

  union {
    struct {
      uint8_t  TS_CFG                   :  3; /*!< Choose the OT level from 90~160 degree. */
      uint8_t                           :  1; /*   (reserved) */
      uint8_t  SEL_OT_DBNC_THRES1       :  2; /*!< Select OT flag debounce threshold when a 0 turns into a 1 */
      uint8_t  SEL_OT_DBNC_THRES0       :  2; /*!< Select OT flag debounce threshold when a 1 turns into a 0 */
      uint8_t  EN_COMPOT                :  1;
      uint8_t  EN_OVERTEMP              :  1;
      uint8_t                           :  6; /*   (reserved) */
      uint8_t  DBG_OT                   :  1; /*!< OT debug enable signal */
      uint8_t  DBG_TS_CH                :  1; /*!< Select temperature sensor channel */
      uint8_t                           :  6; /*   (reserved) */
      uint8_t  START_OT_DET             :  1; /*!< Set 1 to launch a OT detect operation. */
      uint8_t  AUTO_OT_DET_ENA          :  1; /*!< Auto OT detection enable */
      uint8_t  ADC_SAMP_OT_ENA          :  1; /*!< ADC sampling temperature sensor enable */
      uint8_t  OT_BUSY                  :  1;
      uint8_t                           :  4; /*   (reserved) */
    };
    uint32_t WORD;
  } OT_CTRL_CFG; /* +0x010 */

  union {
    struct {
      uint8_t  CHAN_SEQ_NUM             :  2; /*!< Channel Sequence number */
      uint8_t  CH1_SEL                  :  6; /*!< Channel1 Selection */
      uint8_t  CH2_SEL                  :  8; /*!< Channel2 Selection */
      uint8_t  CH3_SEL                  :  8; /*!< Channel3 Selection */
      uint8_t  CH4_SEL                  :  6; /*!< Channel4 Selection */
      uint8_t                           :  1; /*   (reserved) */
      uint8_t  TEST_BATTERY_GAIN_CHOOSE :  1; /*!< VBAT Gain Select */
    };
    uint32_t WORD;
  } SAR_CHAN_CFG; /* +0x014 */

  union {
    struct {
      uint16_t DATA0                    : 16;
      uint16_t DATA1                    : 16;
    };
    uint32_t WORD;
  } ADC_DATA0; /* +0x018 */

  union {
    struct {
      uint16_t DATA2                    : 16;
      uint16_t DATA3                    : 16;
    };
    uint32_t WORD;
  } ADC_DATA1; /* +0x01C */

  union {
    struct {
      uint16_t DATA_OT0                 : 16;
      uint16_t DATA_OT1                 : 16;
    };
    uint32_t WORD;
  } ADC_OT_DATA; /* +0x020 */

  union {
    struct {
      union {
        struct {
          uint8_t  INT_SAR_DONE_ENA     :  1; /*!< SAR convert done interrupt enable */
          uint8_t  INT_OT_ENA           :  1; /*!< Over-temperature interrupt enable */
          uint8_t                       :  6; /*   (reserved) */
        };
        uint8_t BYTE;
      } ENABLE;
      union {
        struct {
          uint8_t  INT_SAR_DONE_CLR     :  1; /*!< SAR convert done interrupt clear */
          uint8_t  INT_OT_CLR           :  1; /*!< Over-temperature interrupt clear */
          uint8_t                       :  6; /*   (reserved) */
        };
        uint8_t BYTE;
      } CLEAR;
      union {
        struct {
          uint8_t  INT_SAR_DONE_STS     :  1; /*!< SAR convert done flag status */
          uint8_t  INT_OT0_STS          :  1; /*!< Over-temperature flag of OT channel 0 */
          uint8_t  INT_OT1_STS          :  1; /*!< Over-temperature flag of OT channel 1 */
          uint8_t                       :  5; /*   (reserved) */
        };
        uint8_t BYTE;
      } STATUS;
      union {
        struct {
          uint8_t  INT_SAR_DONE_IRQ     :  1; /*!< SAR convert done interrupt */
          uint8_t  INT_OT_IRQ           :  1; /*!< Over-temperature interrupt */
          uint8_t                       :  6; /*   (reserved) */
        };
        uint8_t BYTE;
      } IRQ;
    };
    uint32_t WORD;
  } SAR_INT; /* +0x024 */

} SAR_CTRL_SFRS_t;

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
 * @brief The starting address of SAR_CTRL SFRS.
 */
#define SAR_CTRL_SFRS ((__IO SAR_CTRL_SFRS_t *)0x50010600)

#endif /* end of __SAR_CTRL_SFR_H__ section */


