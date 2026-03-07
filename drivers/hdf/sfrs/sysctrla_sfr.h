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

#ifndef SYSCTRLA_SFR_H__
#define SYSCTRLA_SFR_H__

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

typedef struct{
  uint8_t  _RESERVED_24;                      /* +0x024 */
  uint8_t  _RESERVED_25;                      /* +0x025 */
  uint16_t TRIM;                             /*<! LED trim (100uA step) +0x026 */
}TrimLed_t;


/**
 * @brief A structure to represent Special Function Registers for SYSCTRLA.
 */
typedef struct {

  uint8_t  RETAIN1;                           /*<! Firmware scratch register 1 (0x1) +0x000 */
  uint8_t  _RESERVED_01[3];                   /* +0x001 */

  union {
    struct {
      uint8_t  DEBUG_ACCESS_KEY         :  8;
      uint8_t                           :  8; /*   (reserved) */
      uint8_t                           :  8; /*   (reserved) */
      uint8_t                           :  7; /*   (reserved) */
      uint8_t  DEBUG_LOCK               :  1;
    };
    uint32_t WORD;
  } DEBUG_ACCESS_KEY; /* +0x004 */

  uint8_t  DEBUG_ACCESS_ENABLED;              /* +0x008 */
  uint8_t  _RESERVED_09[3];                   /* +0x009 */

  union {
    struct {
      uint8_t  TRIM_ACCESS_KEY          :  8;
      uint8_t                           :  8; /*   (reserved) */
      uint8_t                           :  8; /*   (reserved) */
      uint8_t                           :  7; /*   (reserved) */
      uint8_t  TRIM_LOCK                :  1;
    };
    uint32_t WORD;
  } TRIM_ACCESS_KEY; /* +0x00C */

  uint8_t  TRIM_ACCESS_ENABLED;               /* +0x010 */
  uint8_t  _RESERVED_11[3];                   /* +0x011 */

  union {
    struct {
      uint8_t  PMU_ACCESS_KEY           :  8;
      uint8_t                           :  8; /*   (reserved) */
      uint8_t                           :  8; /*   (reserved) */
      uint8_t                           :  7; /*   (reserved) */
      uint8_t  PMU_LOCK                 :  1;
    };
    uint32_t WORD;
  } PMU_ACCESS_KEY; /* +0x014 */

  uint8_t  PMU_ACCESS_ENABLED;                /* +0x018 */
  uint8_t  _RESERVED_19[3];                   /* +0x019 */

  union {
    struct {
      uint8_t  TRIM_HF_RC               :  8; /*!< High Frequency RC Oscillator trim */
      uint8_t  SSCENA                   :  1; /*!< SSC Enable */
      uint8_t  SSCDEEP                  :  3; /*!< SSC Depth Configuration */
      uint8_t                           :  4; /*   (reserved) */
      uint8_t  SSCDIV                   :  8; /*!< SSC Clock Divider */
      uint8_t                           :  8; /*   (reserved) */
    };
    uint32_t WORD;
  } HF_OSC_TRIM; /* +0x01C */

  union {
    struct {
      uint8_t  LEDBIASSEL               :  1; /*!< High Voltage LED bias select */
      uint8_t  LEDBIASREG               :  1; /*!< High Voltage LED bias select register */
      uint8_t  LEDBIASTRIMEN            :  1; /*!< LED bias trim eanble */
      uint8_t                           :  1; /*   (reserved) */
      uint32_t LEDBIASTRIM              :  8; /*!< LED bias current trim !!misaligned!! */
      uint8_t                           :  4; /*   (reserved) */
      uint16_t                          : 16; /*   (reserved) */
    };
    uint32_t WORD;
  } BIAS; /* +0x020 */

  TrimLed_t TRIMLED[24];

  uint8_t  TRIMVFW;                           /*<! PN Forward Voltage Current trim (10uA step) +0x084 */
  uint8_t  _RESERVED_85[3];                   /* +0x085 */

  uint32_t NAME;                              /*<! ASIC name +0x088 */

  uint16_t REV;                               /*<! Silicon Revision +0x08C */
  uint8_t  _RESERVED_8E[2];                   /* +0x08E */

  union {
    struct {
      uint8_t  ENABORTESTMODE           :  1; /*!< BOR Testmode Enable */
      uint8_t  EN_COMPOT                :  1;
      uint8_t  EN_BG_HCOMP              :  1;
      uint8_t  BLANK_OT                 :  1;
      uint8_t  DBG_ADC_TSN_ATB01        :  1;
      uint8_t  DBG_ADC_TSP_ATB02        :  1;
      uint8_t  DBG_VBG_TEST_ATB03       :  1;
      uint8_t  DBG_VREF_1P2_BUF_ATB04   :  1;
      uint8_t  DBG_IBP_1U_ATB05         :  1;
      uint8_t  DBG_VPRE_REG_ATB06       :  1;
      uint8_t                           :  6; /*   (reserved) */
      uint16_t                          : 16; /*   (reserved) */
    };
    uint32_t WORD;
  } TESTMODE; /* +0x090 */

} SYSCTRLA_SFRS_t;

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
 * @brief The starting address of SYSCTRLA SFRS.
 */
#define SYSCTRLA_SFRS ((__IO SYSCTRLA_SFRS_t *)0x50013000)

#endif /* end of __SYSCTRLA_SFR_H__ section */


