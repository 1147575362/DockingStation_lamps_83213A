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

#ifndef SPI_SFR_H__
#define SPI_SFR_H__

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
 * @brief A structure to represent Special Function Registers for SPI.
 */
typedef struct {

  uint8_t  RXDATA;                            /*<! Rx Data buffer +0x000 */
  uint8_t  _RESERVED_01[3];                   /* +0x001 */

  uint8_t  TXDATA;                            /*<! Tx Data buffer +0x004 */
  uint8_t  _RESERVED_05[3];                   /* +0x005 */

  union {
    struct {
      uint8_t  ENA_REQ                  :  1; /*!< SPI enable request */
      uint8_t  ENA_STS                  :  1; /*!< SPI enable status */
      uint8_t  SPR                      :  2; /*!< Standard clock divider selection */
      uint8_t  CPHA                     :  1; /*!< SPI clock phase */
      uint8_t  CPOL                     :  1; /*!< SPI clock polarity (Motorola SPI Frame Format) */
      uint8_t  SCKEXT                   :  2; /*!< Extended clock divider */
      uint8_t  ICNT                     :  8; /*!< Interrupt counter bits */
      uint8_t  LPBK                     :  8; /*!< Port wired-OR mode bit */
      uint8_t  FIFOSOFTRESET            :  8; /*!< FIFO Soft Reset */
    };
    uint32_t WORD;
  } SPICTRL; /* +0x008 */

  union {
    struct {
      uint8_t  XFERCNT                  :  1; /*!< Completed Transfer Count */
      uint8_t  RXFIFOUF                 :  1; /*!< Reception FIFO underflow */
      uint8_t  TXFIFOOF                 :  1; /*!< Transmit FIFO overflow */
      uint8_t  RXFIFOEMPTY              :  1; /*!< Reception FIFO empty */
      uint8_t  RXFIFOFULL               :  1; /*!< Reception FIFO full */
      uint8_t  TXFIFOEMPTY              :  1; /*!< Transmit FIFO empty */
      uint8_t  TXFIFOFULL               :  1; /*!< Transmit FIFO full */
      uint8_t                           :  1; /*   (reserved) */
      uint8_t  RXFIFOCOUNT              :  8; /*!< Reception FIFO Count */
      uint8_t  TXFIFOCOUNT              :  8; /*!< Transmit FIFO Count */
      uint8_t                           :  8; /*   (reserved) */
    };
    uint32_t WORD;
  } SPISTATUS; /* +0x00C */

  union {
    struct {
      uint8_t  INTXFERCNT               :  1; /*!< SPI Transfer Count Interrupt */
      uint8_t  INTRXFIFOUF              :  1; /*!< Reception FIFO Underflow Interrupt */
      uint8_t  INTTXFIFOOF              :  1; /*!< Transmit FIFO Overflow Interrupt */
      uint8_t                           :  5; /*   (reserved) */
      uint32_t                          : 24; /*   (reserved) */
    };
    uint32_t WORD;
  } SPIINTSTATUS; /* +0x010 */

  union {
    struct {
      uint8_t  INTXFERCNTENA            :  1; /*!< SPI Transfer Count Interrupt Enable */
      uint8_t  INTRXFIFOUFENA           :  1; /*!< Reception FIFO Underflow Interrupt Enable */
      uint8_t  INTTXFIFOOFENA           :  1; /*!< Transmit FIFO Overflow Interrupt Enable */
      uint8_t                           :  5; /*   (reserved) */
      uint32_t                          : 24; /*   (reserved) */
    };
    uint32_t WORD;
  } SPIINTENABLE; /* +0x014 */

  union {
    struct {
      uint8_t  INTXFERCNTCLR            :  1; /*!< SPI Transfer Count Interrupt */
      uint8_t  INTRXFIFOUFCLR           :  1; /*!< Reception FIFO Underflow Interrupt */
      uint8_t  INTTXFIFOOFCLR           :  1; /*!< Transmit FIFO Overflow Interrupt */
      uint8_t                           :  5; /*   (reserved) */
      uint32_t                          : 24; /*   (reserved) */
    };
    uint32_t WORD;
  } SPIINTCLEAR; /* +0x018 */

} SPI_SFRS_t;

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
 * @brief The starting address of SPI SFRS.
 */
#define SPI_SFRS ((__IO SPI_SFRS_t *)0x50010700)

#endif /* end of __SPI_SFR_H__ section */


