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

#ifndef PWM_SFR_H__
#define PWM_SFR_H__

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


typedef union {
  struct {
    uint16_t PFALL                   : 16; /*!< Pulse Fall */
    uint16_t PRISE                   : 16; /*!< Pulse Rise */
  };
  uint32_t WORD;
} PWM_PULSE_t; /* +0x01C */
/**
 * @brief A structure to represent Special Function Registers for PWM.
 */
typedef struct {

  union {
    struct {
      uint8_t                           :  8; /*   (reserved) */
      uint8_t  PRESCALESEL              :  8; /*!< Prescaler select */
      uint16_t PERIOD                   : 16;
    };
    uint32_t WORD;
  } BASE; /* +0x000 */

  uint16_t PWMCNT;                            /* +0x004 */
  uint8_t  _RESERVED_06[2];                   /* +0x006 */

  union {
    struct {
      uint32_t ENAREQ                   : 24;
      uint8_t  ENAREQALL                :  1;
      uint8_t  CLRREQALL                :  1;
      uint8_t  FORCEINACTIVE            :  1;
      uint8_t                           :  5; /*   (reserved) */
    };
    uint32_t WORD;
  } ENAREQ; /* +0x008 */

  uint32_t ENASTS;                            /* +0x00C */

  uint32_t INIT;                              /* +0x010 */

  uint32_t INVERT;                            /* +0x014 */

  uint8_t  UPDATE;                            /* +0x018 */
  uint8_t  _RESERVED_19[3];                   /* +0x019 */

  PWM_PULSE_t PULSE[24];

  uint32_t INTPOSEDGENA;                      /*<! Interrupt enable +0x07C */

  uint32_t INTNEGEDGENA;                      /*<! Interrupt enable +0x080 */

  uint32_t INTPOSEDGCLR;                      /*<! Interrupt clear +0x084 */

  uint32_t INTNEGEDGCLR;                      /*<! Interrupt clear +0x088 */

  uint32_t INTPOSEDGSTS;                      /*<! Interrupt status +0x08C */

  uint32_t INTNEGEDGSTS;                      /*<! Interrupt status +0x090 */

  uint32_t INTPOSEDGIRQ;                      /*<! Interrupt active +0x094 */

  uint32_t INTNEGEDGIRQ;                      /*<! Interrupt active +0x098 */

  union {
    struct {
      union {
        struct {
          uint8_t  PERIOD               :  1; /*!< Period Interrupt enable */
          uint8_t  UPD                  :  1; /*!< Updated Interrupt enable */
          uint8_t                       :  6; /*   (reserved) */
        };
        uint8_t BYTE;
      } ENABLE;
      union {
        struct {
          uint8_t  PERIOD               :  1; /*!< Period Interrupt clear */
          uint8_t  UPD                  :  1; /*!< Updated Interrupt clear */
          uint8_t                       :  6; /*   (reserved) */
        };
        uint8_t BYTE;
      } CLEAR;
      union {
        struct {
          uint8_t  PERIOD               :  1; /*!< Period Interrupt status */
          uint8_t  UPD                  :  1; /*!< Updated Interrupt status */
          uint8_t                       :  6; /*   (reserved) */
        };
        uint8_t BYTE;
      } STATUS;
      union {
        struct {
          uint8_t  PERIOD               :  1; /*!< Period Interrupt active */
          uint8_t  UPD                  :  1; /*!< Updated Interrupt active */
          uint8_t                       :  6; /*   (reserved) */
        };
        uint8_t BYTE;
      } IRQ;
    };
    uint32_t WORD;
  } INTPWM; /* +0x09C */

} PWM_SFRS_t;

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
 * @brief The starting address of PWM SFRS.
 */
#define PWM_SFRS ((__IO PWM_SFRS_t *)0x50011000)

#endif /* end of __PWM_SFR_H__ section */


