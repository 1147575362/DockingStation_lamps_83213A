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

#ifndef LINM_SFR_H__
#define LINM_SFR_H__

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
 * @brief A structure to represent Special Function Registers for LINM.
 */

typedef union{
  struct{
    uint8_t  DATA;                          /*<! Data Buffer 1 +0x000 */
    uint8_t  _RESERVED_01[3];                   /* +0x001 */
  };
  uint32_t WORD;
}LIN_DATABUF_t;


typedef struct {
  LIN_DATABUF_t        DATABUFF[8];             /*<! Data Buffer 1 +0x000 */
  union {
    struct {
      uint8_t  STARTREQ                 :  1; /*!< Start Request */
      uint8_t  WAKEUPREQ                :  1; /*!< WakeUp Request */
/*    uint8_t  RSTERR                   :  1; !< Reset Error */
/*    uint8_t  RSTINT                   :  1; !< Reset interrupt */
      uint8_t  RST_INT_ERR              :  2; /*!< Reset Reset interrupt, Error */
      uint8_t  DATAACK                  :  1; /*!< Data Acknowledgement */
      uint8_t  TRANSMIT                 :  1; /*!< Transmit Operation */
      uint8_t  SLEEP                    :  1; /*!< Sleep Request */
      uint8_t  STOP                     :  1; /*!< Stop Register */
      uint32_t                          : 24; /*   (reserved) */
    };
    uint32_t WORD;
  } CTRL; /* +0x020 */

  union {
    struct {
      uint8_t  COMPLETE                 :  1;
      uint8_t  WAKEUP                   :  1;
      uint8_t  ERROR                    :  1; /*!< Lin Error */
      uint8_t  INTR                     :  1; /*!< Interupt Request */
      uint8_t  DATAREQ                  :  1; /*!< Data Request */
      uint8_t  ABORTED                  :  1;
      uint8_t  BUSIDLETIMEOUT           :  1; /*!< BUS Idle Timeout */
      uint8_t  ACTIVE                   :  1; /*!< Lin Bus Active */
      uint32_t                          : 24; /*   (reserved) */
    };
    uint32_t WORD;
  } STATUS; /* +0x024 */

  union {
    struct {
      uint8_t  BITERR                   :  1; /*!< Bit Error in Byte field */
      uint8_t  CHK                      :  1; /*!< Checksum Error */
      uint8_t  TIMEOUT                  :  1; /*!< Timeout Error */
      uint8_t                           :  1; /*   (reserved) */
      uint8_t  BITMON                   :  1; /*!< Bit Monitor Error */
      uint8_t  SBITERR                  :  1; /*!< Start Bit Error in Byte field */
      uint8_t  FRAMEERR                 :  1; /*!< Byte Field Framing Error */
      uint8_t  BITMONSTOP               :  1; /*!< Bit Monitor Error occurred in Stop Bit */
      uint8_t  BITMONDATA               :  8; /*!< Bit Monitor Error occurred in Start or Data Bits */
      uint16_t                          : 16; /*   (reserved) */
    };
    uint32_t WORD;
  } ERROR; /* +0x028 */

  union {
    struct {
      uint8_t  LENGTH                   :  4; /*!< Data Length */
      uint8_t                           :  1; /*   (reserved) */
      uint8_t  DISAUTOSLEEP             :  1; /*!< Disable Auto Sleep */
      uint8_t  DISBITMON                :  1; /*!< Disable Bit Monitor */
      uint8_t  ENHCHK                   :  1; /*!< Enhancement Check */
      uint32_t                          : 24; /*   (reserved) */
    };
    uint32_t WORD;
  } DL; /* +0x02C */

  uint8_t  BTDIV07;                           /*<! Bt Div LSBs +0x030 */
  uint8_t  _RESERVED_31[3];                   /* +0x031 */

  union {
    struct {
      uint8_t  BTDIV8                   :  1; /*!< Bt Div Most Significant bit */
      uint8_t  BTMULT                   :  5; /*!< Bt Div Most Significant bit */
      uint8_t  PRESCL                   :  2; /*!< Prescaler Register */
      uint32_t                          : 24; /*   (reserved) */
    };
    uint32_t WORD;
  } BITTIME; /* +0x034 */

  uint8_t  ID;                                /* +0x038 */
  uint8_t  _RESERVED_39[3];                   /* +0x039 */

  union {
    struct {
      uint8_t  WUPREPEAT                :  2; /*!< wakeup repeat time */
      uint8_t  BUSINACTIVE              :  2; /*!< Bus Inactivity Time */
      uint8_t                           :  4; /*   (reserved) */
      uint32_t                          : 24; /*   (reserved) */
    };
    uint32_t WORD;
  } BUSTIME; /* +0x03C */

  union {
    struct {
      uint8_t  COMPLETETX               :  1; /*!< Complete TX */
      uint8_t  BUSIDLETIMEOUTDOMINANT   :  1; /*!< Dominant Bus Idle Timeout */
      uint8_t  BUSIDLEMONITOR           :  2; /*!< Bus Idle Monitor Status */
      uint8_t                           :  4; /*   (reserved) */
      uint32_t                          : 24; /*   (reserved) */
    };
    uint32_t WORD;
  } STATUSEXT; /* +0x040 */

  uint32_t _RESERVED_44;

  union {
    struct {
      uint8_t  INTERBYTECNT             :  8; /*!< Inter-Byte Space Bit Count */
      uint8_t  BITMONMODE               :  8; /*!< BIT Monitor Mode */
      uint16_t                          : 16; /*   (reserved) */
    };
    uint32_t WORD;
  } CONF; /* +0x048 */

} LINM_SFRS_t;

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
 * @brief The starting address of LINM SFRS.
 */
#define LINM_SFRS ((__IO LINM_SFRS_t *)0x50010400)

#endif /* end of __LINM_SFR_H__ section */


