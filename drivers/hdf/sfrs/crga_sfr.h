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

#ifndef __CRGA_SFR_H__
#define __CRGA_SFR_H__

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
 * @brief A structure to represent Special Function Registers for CRGA.
 */
typedef struct {

  union {
    struct {
      uint8_t  LFRCSTS                  :  8; /*!< Slow oscillator status */
      uint8_t  CLKLFSEL                 :  8; /*!< LF Clock Source select */
      uint16_t                          : 16; /*   (reserved) */
    };
    uint32_t WORD;
  } LFCLKCTRL; /* +0x000 */

  union {
    struct {
      uint8_t  HFRCENA                  :  1; /*!< Fast oscillator enable */
      uint8_t  HFRCSTS                  :  1; /*!< Fast oscillator status */
      uint8_t                           :  6; /*   (reserved) */
      uint8_t  SYSCLKSEL                :  8; /*!< Clock select */
      uint8_t  DIVSYSCLK                :  3; /*!< Clock div select */
      uint8_t                           :  1; /*   (reserved) */
      uint8_t  HF_RC_LDO_VSEL           :  2;
      uint8_t                           :  2; /*   (reserved) */
      uint8_t                           :  8; /*   (reserved) */
    };
    uint32_t WORD;
  } SYSCLKCTRL; /* +0x004 */

  union {
    struct {
      uint8_t  PORFLAG                  :  1; /*!< Power on reset flag */
      uint8_t  OVFLAG                   :  1; /*!< Over Voltage Monitor flag */
      uint8_t                           :  1; /*   (reserved) */
      uint8_t  UVFLAG                   :  1; /*!< Under Voltage Monitor flag */
      uint8_t                           :  1; /*   (reserved) */
      uint8_t  WDTFLAG                  :  1; /*!< Watchdog bark flag */
      uint8_t  OVTEMPFLAG               :  1; /*!< Over Temp Violation flag */
      uint8_t                           :  1; /*   (reserved) */
      uint8_t  PORFLAGCLR               :  1; /*!< POR flag clear */
      uint8_t  OVFLAGCLR                :  1; /*!< OV flag clear */
      uint8_t                           :  1; /*   (reserved) */
      uint8_t  UVFLAGCLR                :  1; /*!< UV flag clear */
      uint8_t                           :  1; /*   (reserved) */
      uint8_t  WDTFLAGCLR               :  1; /*!< WDT flag clear */
      uint8_t  OVTEMPFLAGCLR            :  1; /*!< OVTEMP flag clear */
      uint8_t                           :  1; /*   (reserved) */
      uint8_t  HARDRSTREQ               :  8; /*!< Hard reset request */
      uint8_t  SOFTRSTREQ               :  8; /*!< Soft reset request */
    };
    uint32_t WORD;
  } RESETCTRL; /* +0x008 */

  uint8_t  MODULERSTREQ;                      /*<! Module Soft reset request +0x00C */
  uint8_t  _RESERVED_0D[3];                   /* +0x00D */

  uint8_t  WDTACTION;                         /*<! Watchdog action +0x010 */
  uint8_t  _RESERVED_11[3];                   /* +0x011 */

  uint8_t  KILLLFRC;                          /*<! Kill slow RC oscillator +0x014 */
  uint8_t  _RESERVED_15[3];                   /* +0x015 */

  union {
    struct {
      uint8_t  OVTEMP                   :  8; /*!< Over Temperature action */
      uint8_t                           :  8; /*   (reserved) */
      uint8_t                           :  8; /*   (reserved) */
      uint8_t                           :  7; /*   (reserved) */
      uint8_t  OVTEMP_LOCK              :  1;
    };
    uint32_t WORD;
  } OVTEMPACTION; /* +0x018 */

  union {
    struct {
      uint8_t                           :  4; /*   (reserved) */
      uint8_t  SEL_TSCH                 :  2; /*!< Selecting the temperature sensor channel */
      uint8_t                           :  2; /*   (reserved) */
      uint32_t                          : 24; /*   (reserved) */
    };
    uint32_t WORD;
  } OVTEMPCONFIG; /* +0x01C */

  union {
    struct {
      uint8_t  OV_ACTION                :  1; /*!< Over Voltage action */
      uint8_t                           :  3; /*   (reserved) */
      uint8_t  UV_ACTION                :  1; /*!< Under Voltage action */
      uint8_t                           :  3; /*   (reserved) */
      uint8_t                           :  8; /*   (reserved) */
      uint8_t                           :  8; /*   (reserved) */
      uint8_t                           :  3; /*   (reserved) */
      uint8_t  OV_LOCK                  :  1;
      uint8_t                           :  3; /*   (reserved) */
      uint8_t  UV_LOCK                  :  1;
    };
    uint32_t WORD;
  } OVUVACTION; /* +0x020 */

  uint8_t  MODULEGATEENA;                     /*<! Module Gate enable request +0x024 */
  uint8_t  _RESERVED_25[3];                   /* +0x025 */

} CRGA_SFRS_t;

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
 * @brief The starting address of CRGA SFRS.
 */
#define CRGA_SFRS ((__IO CRGA_SFRS_t *)0x50000000)

#endif /* end of __CRGA_SFR_H__ section */


