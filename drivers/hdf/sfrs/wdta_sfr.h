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

#ifndef WDTA_SFR_H__
#define WDTA_SFR_H__

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
 * @brief A structure to represent Special Function Registers for WDTA.
 */
typedef struct {

  union {
    struct {
      uint8_t  WDTA_RUNNING             :  1; /*!< Analog-Watchdog Running status */
      uint8_t  UPDATE                   :  1; /*!< Window Mode Enable */
      uint8_t                           :  6; /*   (reserved) */
      uint8_t  WDTA_TIMEOUT_SEL         :  3; /*!< Timeout select */
      uint8_t  WDTA_WINOPEN_ENA         :  1; /*!< Window Mode Enable */
      uint8_t  WDTA_WINOPEN_SEL         :  2; /*!< Window Mode open select */
      uint8_t  WDTA_WINOPEN_FLAG        :  1; /*!< Window open flag */
      uint8_t                           :  1; /*   (reserved) */
      uint8_t                           :  8; /*   (reserved) */
      uint8_t                           :  7; /*   (reserved) */
      uint8_t  STOP_LOCK                :  1;
    };
    uint32_t WORD;
  } CTRL; /* +0x000 */

  uint32_t STOP;                              /* +0x004 */

  uint32_t CLEAR;                             /* +0x008 */

  uint32_t CNTVAL;                            /*<! Counter value +0x00C */

  union {
    struct {
      union {
        struct {
          uint8_t  WDTA                 :  1; /*!< WDTA timeout Interrupt Enable */
          uint8_t  WINOPEN              :  1; /*!< Window Open Interrupt Enable */
          uint8_t                       :  6; /*   (reserved) */
        };
        uint8_t BYTE;
      } ENABLE;
      union {
        struct {
          uint8_t  WDTA                 :  1; /*!< WDTA timeout Interrupt Clear */
          uint8_t  WINOPEN              :  1; /*!< Window Open Interrupt Clear */
          uint8_t                       :  6; /*   (reserved) */
        };
        uint8_t BYTE;
      } CLEAR;
      union {
        struct {
          uint8_t  WDTA                 :  1; /*!< WDTA timeout Status */
          uint8_t  WINOPEN              :  1; /*!< Window Open Status */
          uint8_t                       :  6; /*   (reserved) */
        };
        uint8_t BYTE;
      } STATUS;
      union {
        struct {
          uint8_t  WDTA                 :  1; /*!< WDTA timeout Interrupt */
          uint8_t  WINOPEN              :  1; /*!< Window Open Interrupt */
          uint8_t                       :  6; /*   (reserved) */
        };
        uint8_t BYTE;
      } IRQ;
    };
    uint32_t WORD;
  } INT; /* +0x010 */

} WDTA_SFRS_t;

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
 * @brief The starting address of WDTA SFRS.
 */
#define WDTA_SFRS ((__IO WDTA_SFRS_t *)0x50010200)

#endif /* end of __WDTA_SFR_H__ section */


