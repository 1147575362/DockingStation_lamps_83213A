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

#ifndef IOCTRLA_SFR_H__
#define IOCTRLA_SFR_H__

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


typedef union{
  struct {
    uint8_t  HWMODE                   :  3; /*!< hardware mode */
    uint8_t  PULLMODE                 :  2; /* 01: pull none, 00: pull up, 11: pull down, 10: pull up and down*/
    uint8_t  RDENA                    :  1; /*!< read enable */
    uint8_t  SL                       :  1; /*   (reserved) */
    uint8_t  CS                       :  1; /*   (reserved) */
    uint8_t  PDVR                     :  2; /*   (reserved) */
    uint8_t                           :  6;
    uint8_t                           :  8;
    uint8_t  MUXSEL                   :  8;
  };
  uint32_t WORD;
}IOCTRLA_GPIO_t;
/**
 * @brief A structure to represent Special Function Registers for IOCTRLA.
 */
typedef struct {

  IOCTRLA_GPIO_t        GPIO[9]; /* +0x000 */
  union {
    struct {
      uint8_t  LINS_HWMODE              :  1; /*!< LIN Slave hardware mode */
      uint8_t                           :  4; /*   (reserved) */
      uint8_t  LINS_TXENA               :  1; /*!< LIN transmit enable */
      uint8_t                           :  2; /*   (reserved) */
      uint8_t  LINM_HWMODE              :  1; /*!< LIN Master hardware mode */
      uint8_t                           :  4; /*   (reserved) */
      uint8_t  LINM_TXENA               :  1; /*!< LIN transmit enable */
      uint8_t                           :  2; /*   (reserved) */
      uint8_t                           :  8; /*   (reserved) */
      uint8_t                           :  1; /*   (reserved) */
      uint8_t  PMODE                    :  1; /*!< LIN Power Mode */
      uint8_t                           :  6; /*   (reserved) */
    };
    uint32_t WORD;
  } LIN; /* +0x024 */

  union {
    struct {
      uint8_t  LINS_RX_GF_ENA           :  8; /*!< LINS RXD Glitch Filter enables */
      uint8_t  LINS_DBNC_THRES0_1ST     :  8; /*!< 1st Stage LINS Debounce Threshold for 1 to 0 */
      uint8_t  LINS_DBNC_THRES1_1ST     :  8; /*!< 1st Stage LINS Debounce Threshold for 0 to 1 */
      uint8_t                           :  8; /*   (reserved) */
    };
    uint32_t WORD;
  } LINSGFCONF; /* +0x028 */

  union {
    struct {
      uint8_t  LINS_DBNC_THRES0_2ND     :  8; /*!< 2nd Stage LINS Debounce Threshold for 1 to 0 */
      uint8_t  LINS_DBNC_THRES1_2ND     :  8; /*!< 2nd Stage LINS Debounce Threshold for 0 to 1 */
      uint8_t  LINS_DBNC_THRES0_3RD     :  8; /*!< 3rd Stage LINS Debounce Threshold for 1 to 0 */
      uint8_t  LINS_DBNC_THRES1_3RD     :  8; /*!< 3rd Stage LINS Debounce Threshold for 0 to 1 */
    };
    uint32_t WORD;
  } LINSGFCONF1; /* +0x02C */

  union {
    struct {
      uint8_t  LINM_RX_GF_ENA           :  8; /*!< LINM RXD Glitch Filter enables */
      uint8_t  LINM_DBNC_THRES0_1ST     :  8; /*!< LINM Debounce Threshold for 1 to 0 */
      uint8_t  LINM_DBNC_THRES1_1ST     :  8; /*!< LINM Debounce Threshold for 0 to 1 */
      uint8_t                           :  8; /*   (reserved) */
    };
    uint32_t WORD;
  } LINMGFCONF; /* +0x030 */

  union {
    struct {
      uint8_t  LINM_DBNC_THRES0_2ND     :  8; /*!< 2nd Stage LINM Debounce Threshold for 1 to 0 */
      uint8_t  LINM_DBNC_THRES1_2ND     :  8; /*!< 2nd Stage LINM Debounce Threshold for 0 to 1 */
      uint8_t  LINM_DBNC_THRES0_3RD     :  8; /*!< 3rd Stage LINM Debounce Threshold for 1 to 0 */
      uint8_t  LINM_DBNC_THRES1_3RD     :  8; /*!< 3rd Stage LINM Debounce Threshold for 0 to 1 */
    };
    uint32_t WORD;
  } LINMGFCONF1; /* +0x034 */

  union {
    struct {
      uint8_t  LINSTXDMONITORENA        :  1; /*!< LINS TxD Monitor enable */
      uint8_t  LINMTXDMONITORENA        :  1; /*!< LINM TxD Monitor enable */
      uint8_t                           :  6; /*   (reserved) */
      uint8_t  LINSTXDTIMEOUTDOMINANT   :  1; /*!< Tx Dominant Timeout */
      uint8_t  LINMTXDTIMEOUTDOMINANT   :  1; /*!< Tx Dominant Timeout */
      uint8_t                           :  6; /*   (reserved) */
      uint16_t                          : 16; /*   (reserved) */
    };
    uint32_t WORD;
  } LINTXDMONITOR; /* +0x038 */

  union {
    struct {
      uint8_t                           :  8; /*   (reserved) */
      uint8_t  SENSE_ENA                :  1; /*!< LED Forward Voltage Sense Enable */
      uint8_t  SENSE_CTRL               :  3; /*!< LED */
      uint8_t                           :  3; /*   (reserved) */
      uint8_t  VFW_ENA                  :  1; /*!< LED Forward Voltage Current Enable */
      uint8_t  GAIN_SEL                 :  1; /*!< LED Sense AFE gain select */
      uint8_t  PN_OP_START_BIAS_BOOST   :  2; /*!< PN OP START BIAS BOOST */
      uint8_t                           :  3; /*   (reserved) */
      uint8_t  OP_GBW_SEL               :  2; /*!< LED SENSE OP GBW SEL */
      uint8_t  BUCK_OUT_PORN_STS        :  8; /*!< BUCK ouput volatage POR flag */
    };
    uint32_t WORD;
  } LEDPIN; /* +0x03C */

  uint32_t HWMODE;                            /*<! LED hardware mode +0x040 */

  uint32_t LEDDATA;                           /*<! LED Data Out +0x044 */

  uint32_t LEDPUENA;                          /*<! LED Pullup Enable +0x048 */

  union {
    struct {
      uint8_t  ADC_CON_SEL              :  1; /*!< Hardware/Firmware Select <br>NOTE:The write operation of this field register takes effect by configuring 'SYSCTRLA_SFRS->DEBUG_ACCESS_KEY.DEBUG_ACCESS_KEY = 0x05' */
      uint8_t  GPIO_CON_SEL             :  1; /*!< Hardware/Firmware Select <br>NOTE:The write operation of this field register takes effect by configuring 'SYSCTRLA_SFRS->DEBUG_ACCESS_KEY.DEBUG_ACCESS_KEY = 0x05' */
      uint8_t  ADC_SEL_SEL              :  1; /*!< Hardware/Firmware Select <br>NOTE:The write operation of this field register takes effect by configuring 'SYSCTRLA_SFRS->DEBUG_ACCESS_KEY.DEBUG_ACCESS_KEY = 0x05' */
      uint8_t                           :  5; /*   (reserved) */
      uint32_t ADC_CON_REG              : 12; /*!< Firmware Debug Value !!misaligned!! */
      uint8_t  ADC_GPIO_REG             :  4; /*!< Select which GPIO forward voltage to ADC by firmware */
      uint8_t  ADC_LED_REG              :  8; /*!< Select which channel of LED forward voltage to ADC by firmware */
    };
    uint32_t WORD;
  } ANALOGTESTMUXOVERRIDE1; /* +0x04C */

  union {
    struct {
      uint8_t  GPIOCONREG               :  8; /*!< Firmware Debug Value */
      uint8_t                           :  8; /*   (reserved) */
      uint16_t ADCSELREG                : 16; /*!< Firmware Debug Value */
    };
    uint32_t WORD;
  } ANALOGTESTMUXOVERRIDE2; /* +0x050 */

  uint32_t LEDSEL;                            /*<! LED test analog channel select, one hot code <br> NOTE:exclusive with DFT_MODE_ENA_LED +0x054 */

  union {
    struct {
      union {
        struct {
          uint8_t  LINS_TXD_DOM         :  1; /*!< LINS TXD Dominant Monitor interrupt enable */
          uint8_t  LINM_TXD_DOM         :  1; /*!< LINM TXD Dominant Monitor interrupt enable */
          uint8_t                       :  6; /*   (reserved) */
        };
        uint8_t BYTE;
      } ENABLE;
      union {
        struct {
          uint8_t  LINS_TXD_DOM         :  1; /*!< LINS TXD Dominant Monitor interrupt clear */
          uint8_t  LINM_TXD_DOM         :  1; /*!< LINM TXD Dominant Monitor interrupt clear */
          uint8_t                       :  6; /*   (reserved) */
        };
        uint8_t BYTE;
      } CLEAR;
      union {
        struct {
          uint8_t  LINS_TXD_DOM         :  1; /*!< LINS TXD Dominant Monitor interrupt status */
          uint8_t  LINM_TXD_DOM         :  1; /*!< LINM TXD Dominant Monitor interrupt status */
          uint8_t                       :  6; /*   (reserved) */
        };
        uint8_t BYTE;
      } STATUS;
      union {
        struct {
          uint8_t  LINS_TXD_DOM         :  1; /*!< LINS TXD Dominant Monitor interrupt active */
          uint8_t  LINM_TXD_DOM         :  1; /*!< LINM TXD Dominant Monitor interrupt active */
          uint8_t                       :  6; /*   (reserved) */
        };
        uint8_t BYTE;
      } IRQ;
    };
    uint32_t WORD;
  } IRQ; /* +0x058 */

  union {
    struct {
      uint8_t                           :  8; /*   (reserved) */
      uint8_t                           :  8; /*   (reserved) */
      uint8_t                           :  8; /*   (reserved) */
      uint8_t                           :  7; /*   (reserved) */
      uint8_t  FILT_UNLOCK              :  1;
    };
    uint32_t WORD;
  } FILT_ACCESS; /* +0x05C */

  union {
    struct {
      uint8_t  MCU_CLK_SL               :  1;
      uint8_t  MCU_INT_SL               :  1;
      uint8_t  MCU_DATA_SL              :  1;
      uint8_t                           :  5; /*   (reserved) */
      uint8_t  MCU_CLK_PDRV             :  2;
      uint8_t  MCU_INT_PDRV             :  2;
      uint8_t  MCU_DATA_PDRV            :  2;
      uint8_t                           :  2; /*   (reserved) */
      uint16_t                          : 16; /*   (reserved) */
    };
    uint32_t WORD;
  } MCU_INF_CONF; /* +0x060 */

} IOCTRLA_SFRS_t;

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
 * @brief The starting address of IOCTRLA SFRS.
 */

#define IOCTRLA_REG_GPIO(index)                 (*(__IO                   IOCTRLA_GPIO_t *)(0x50012000+ index*4)) /* index = 0-8 */
#define IOCTRLA_SFRS ((__IO IOCTRLA_SFRS_t *)0x50012000)

#endif /* end of __IOCTRLA_SFR_H__ section */


