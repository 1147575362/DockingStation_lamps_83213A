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

#ifndef FLASH_SFR_H__
#define FLASH_SFR_H__

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
 * @brief A structure to represent Special Function Registers for FLASH.
 */
typedef struct {

  uint32_t ADDR;                              /* +0x000 */

  uint32_t DATA;                              /* +0x004 */

  uint32_t UNLOCK_WRITE;                      /* +0x008 */

  uint32_t WRITE_START;                       /* +0x00C */

  uint32_t UNLOCK_ERASE;                      /* +0x010 */

  uint32_t ERASE_START;                       /* +0x014 */

  uint32_t _RESERVED_18[2];

  uint8_t  CTRL;                              /* +0x020 */
  uint8_t  _RESERVED_21[3];                   /* +0x021 */

  uint32_t CODE_PROT;                         /* +0x024 */

  uint32_t _RESERVED_28[2];

  uint32_t UNLOCK_CTRL_OP;                    /*<! Flash Control Operation Register Unlock value +0x030 */

  union {
    struct {
      uint8_t  CHIP                     :  1; /*!< CHIP bit */
      uint8_t  SIZE                     :  2; /*!< SIZE of the write operation */
      uint8_t                           :  5; /*   (reserved) */
      uint32_t                          : 24; /*   (reserved) */
    };
    uint32_t WORD;
  } CTRL_OP; /* +0x034 */

  union {
    struct {
      uint16_t OSC_TRIM                 : 16; /*!< Oscillator Trim Value */
      uint8_t  SDIO_TIMING_CFG          :  1; /*!< SDIO interface timing configuration */
      uint8_t  SLEEPDEEP_CFG            :  1; /*!< Deep Sleep VDD_IO configuration */
      uint8_t                           :  6; /*   (reserved) */
      uint8_t                           :  8; /*   (reserved) */
    };
    uint32_t WORD;
  } TRIM; /* +0x038 */

} FLASH_SFRS_t;

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
 * @brief The starting address of FLASH SFRS.
 */
#define FLASH_SFRS ((__IO FLASH_SFRS_t *)0x50020020)

#endif /* end of __FLASH_SFR_H__ section */


