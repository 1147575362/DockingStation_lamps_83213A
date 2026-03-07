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

#ifndef WICA_SFR_H__
#define WICA_SFR_H__

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
 * @brief A structure to represent Special Function Registers for WICA.
 */
typedef struct {

  union {
    struct {
      uint8_t                           :  2; /*   (reserved) */
      uint8_t  TIMERENA                 :  1; /*!< Wakeup Timer Enable */
      uint8_t                           :  1; /*   (reserved) */
      uint8_t  TIMERTAPSEL              :  4; /*!< WakeUp Timer Tap Select */
      uint8_t  LINSIRQENA               :  1; /*!< LIN Slave Wakeup Interrupt Enable */
      uint8_t                           :  1; /*   (reserved) */
      uint8_t  TIMERIRQENA              :  1; /*!< Timer Wakeup Interrupt Enable */
      uint8_t  LINMIRQENA               :  1; /*!< LIN Master Wakeup Interrupt Enable */
      uint8_t  LINSIRQCLR               :  1; /*!< clear the wulin_irq */
      uint8_t                           :  1; /*   (reserved) */
      uint8_t  TIMERIRQCLR              :  1; /*!< clear the wutimer_irq */
      uint8_t  LINMIRQCLR               :  1; /*!< clear the wulin_irq */
      uint16_t                          : 16; /*   (reserved) */
    };
    uint32_t WORD;
  } CTRL; /* +0x000 */

  union {
    struct {
      uint8_t  LINS                     :  1; /*!< LIN Slave Wakeup Status */
      uint8_t                           :  1; /*   (reserved) */
      uint8_t  TIMER                    :  1; /*!< Wakeup Timer Status */
      uint8_t  LINM                     :  1; /*!< LIN Master Wakeup Status */
      uint8_t                           :  4; /*   (reserved) */
      uint8_t                           :  8; /*   (reserved) */
      uint16_t TIMERCNT                 : 16; /*!< Wakeup Timer Counter Value */
    };
    uint32_t WORD;
  } STATUS; /* +0x004 */

} WICA_SFRS_t;

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
 * @brief The starting address of WICA SFRS.
 */
#define WICA_SFRS ((__IO WICA_SFRS_t *)0x50010100)

#endif /* end of __WICA_SFR_H__ section */


