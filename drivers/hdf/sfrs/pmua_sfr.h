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

#ifndef PMUA_SFR_H__
#define PMUA_SFR_H__

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
 * @brief A structure to represent Special Function Registers for PMUA.
 */
typedef struct {

  union {
    struct {
      uint8_t  HIBERNATE                :  1;
      uint8_t  FASTBOOT                 :  1; /*!< Fast boot */
      uint8_t  BG_OK                    :  1; /*!< the flag of bandgap OK */
      uint8_t  IGNORE_CIFS              :  1; /*!< Ignore QACKs */
      uint8_t                           :  1; /*   (reserved) */
      uint8_t  DBG_1P3V                 :  1;
      uint8_t  DBG_IPRE                 :  1;
      uint8_t  DBG_OT_COMP              :  1;
      uint32_t                          : 24; /*   (reserved) */
    };
    uint32_t WORD;
  } CTRL; /* +0x000 */

  union {
    struct {
      uint8_t  OCP_CTRL_5V0             :  3; /*!< OCP trim for 5V LDO(default 80mA) */
      uint8_t                           :  1; /*   (reserved) */
      uint8_t  OCP_CTRL_3V3             :  4; /*!< OCP trim for 3V3 LDO(default 30mA) */
      uint8_t  OCP_CTRL_1V5             :  8; /*!< OCP trim for 1.5V LDO(default 30mA) */
      uint8_t                           :  8; /*   (reserved) */
      uint8_t  VDD1V5_LDO_TRIM          :  2;
      uint8_t                           :  2; /*   (reserved) */
      uint8_t  VDD3V3_LDO_TRIM          :  3;
      uint8_t                           :  1; /*   (reserved) */
    };
    uint32_t WORD;
  } PMUTRIM; /* +0x004 */

  union {
    struct {
      uint8_t  ENABLE_1V5               :  4; /*!< Enable 1.5V dwell time */
      uint8_t  ATTACH_1V5               :  4; /*!< Attach 1.5V dwell time */
      uint8_t  ATTACH_3V3               :  4; /*!< Attach 3.3V dwell time */
      uint8_t  POWER_DOWN_MCU           :  4; /*!< Power down MCU dwell time */
      uint16_t                          : 16; /*   (reserved) */
    };
    uint32_t WORD;
  } DWELL; /* +0x008 */

  union {
    struct {
      uint8_t  BAT_UV_EN                :  1; /*!< Battery Under Voltage Monitor Enable */
      uint8_t  BAT_OV_EN                :  1; /*!< Battery Over Voltage Monitor Enable */
      uint8_t  UV_POL                   :  1; /*!< Battery Voltage Monitor Under Voltage Interrupt Event Polarity */
      uint8_t  OV_POL                   :  1; /*!< Battery Voltage Monitor Over Voltage Interrupt Event Polarity */
      uint8_t                           :  4; /*   (reserved) */
      uint8_t                           :  8; /*   (reserved) */
      uint8_t                           :  8; /*   (reserved) */
      uint8_t  LOW                      :  1; /*!< Battery Voltage Low Status */
      uint8_t  HIGH                     :  1; /*!< Battery Voltage High Status */
      /*uint8_t  LOW_DBNC                 :  1; !< Battery Voltage Low Status after debouncing */
      /*uint8_t  HIGH_DBNC                :  1; !< Battery Voltage High Status after deboucing */
      uint8_t  BATT_STATE               :  2;
      uint8_t                           :  4; /*   (reserved) */
    };
    uint32_t WORD;
  } VBATCTRL; /* +0x00C */

  union {
    struct {
      uint8_t  UVLEVEL                  :  8; /*!< Battery Voltage Monitor Under Voltage Select */
      uint8_t  UVHYS                    :  8; /*!< Battery Voltage Monitor Under Voltage Hysterisis Select */
      uint8_t  OVLEVEL                  :  8; /*!< Battery Voltage Monitor Over Voltage Select */
      uint8_t  OVHYS                    :  8; /*!< Battery Voltage Monitor Over Voltage Hysterisis Select */
    };
    uint32_t WORD;
  } VBATTRIM; /* +0x010 */

  union {
    struct {
      uint8_t  UV                       :  1; /*!< under voltage signal debounce enable */
      uint8_t  OV                       :  1; /*!< over voltage signal debounce enable */
      uint8_t  UVSTRB0SEL               :  1; /*!< Low frequency strobing select for debouncing */
      uint8_t  UVSTRB1SEL               :  1; /*!< Low frequency strobing select for debouncing */
      uint8_t  OVSTRB0SEL               :  1; /*!< Low frequency strobing select for debouncing */
      uint8_t  OVSTRB1SEL               :  1; /*!< Low frequency strobing select for debouncing */
      uint8_t                           :  2; /*   (reserved) */
      uint32_t                          : 24; /*   (reserved) */
    };
    uint32_t WORD;
  } VBATDBNC; /* +0x014 */

  union {
    struct {
      uint8_t  UVTHRES1                 :  8; /*!< Under Voltage debouncing threshold for 0to1 Transition */
      uint8_t  OVTHRES1                 :  8; /*!< Over Voltage debouncing threshold for 0to1 Transition */
      uint8_t  UVTHRES0                 :  8; /*!< Under Voltage debouncing threshold for 1to0 Transition */
      uint8_t  OVTHRES0                 :  8; /*!< Over Voltage debouncing threshold for 1to0 Transition */
    };
    uint32_t WORD;
  } VBATDBNCTHRES; /* +0x018 */

  union {
    struct {
      union {
        struct {
          uint8_t  UV                   :  1; /*!< under voltage interrupt enable */
          uint8_t  OV                   :  1; /*!< over voltage interrupt enable */
          uint8_t                       :  6; /*   (reserved) */
        };
        uint8_t BYTE;
      } ENABLE;
      union {
        struct {
          uint8_t  UV                   :  1; /*!< under voltage interrupt clear */
          uint8_t  OV                   :  1; /*!< over voltage interrupt clear */
          uint8_t                       :  6; /*   (reserved) */
        };
        uint8_t BYTE;
      } CLEAR;
      union {
        struct {
          uint8_t  UV                   :  1; /*!< under voltage interrupt status */
          uint8_t  OV                   :  1; /*!< over voltage interrupt status */
          uint8_t                       :  6; /*   (reserved) */
        };
        uint8_t BYTE;
      } STATUS;
      union {
        struct {
          uint8_t  UV                   :  1; /*!< under voltage interrupt active */
          uint8_t  OV                   :  1; /*!< over voltage interrupt active */
          uint8_t                       :  6; /*   (reserved) */
        };
        uint8_t BYTE;
      } IRQ;
    };
    uint32_t WORD;
  } PMUIRQ; /* +0x01C */

} PMUA_SFRS_t;

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
 * @brief The starting address of PMUA SFRS.
 */
#define PMUA_SFRS ((__IO PMUA_SFRS_t *)0x50000040)

#endif /* end of __PMUA_SFR_H__ section */


