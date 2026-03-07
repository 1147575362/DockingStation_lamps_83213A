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

#ifndef LIN_DEVICE_H__
#define LIN_DEVICE_H__

#include <stdint.h>
#include <stdbool.h>
#include <rugbyHP.h>
#include "lins_sfr.h"
#include "linm_sfr.h"

#define E_LIN_STATUS_COMPLETE       (1U << 0)
#define E_LIN_STATUS_WAKEUP         (1U << 1)
#define E_LIN_STATUS_ERROR          (1U << 2)
#define E_LIN_STATUS_INT_REQ        (1U << 3)
#define E_LIN_STATUS_DATA_REQ       (1U << 4)
#define E_LIN_STATUS_ABORT          (1U << 5)
#define E_LIN_STATUS_IDLE_TIMEOUT   (1U << 6)
#define E_LIN_STATUS_ACTIVE         (1U << 7)

#define E_LIN_ERROR_BIT             (1U << 0)
#define E_LIN_ERROR_CHECKSUM        (1U << 1)
#define E_LIN_ERROR_TIMEOUT         (1U << 2)
#define E_LIN_ERROR_PARITY          (1U << 3)


#define E_LIN_TIME_INACTIVE_SEC_4       (0U)
#define E_LIN_TIME_INACTIVE_SEC_6       (1U)
#define E_LIN_TIME_INACTIVE_SEC_8       (2U)
#define E_LIN_TIME_INACTIVE_SEC_10      (3U)
#define E_LIN_TIME_INACTIVE_MAX         (3U) /*E_LIN_TIME_INACTIVE_SEC_10*/

#define E_LIN_TIME_WAKEUP_REPEAT_MS_180 (0U)
#define E_LIN_TIME_WAKEUP_REPEAT_MS_200 (1U)
#define E_LIN_TIME_WAKEUP_REPEAT_MS_220 (2U)
#define E_LIN_TIME_WAKEUP_REPEAT_MS_240 (3U)
#define E_LIN_TIME_WAKEUP_REPEAT_MAX    (3U)/*E_LIN_TIME_WAKEUP_REPEAT_MS_240*/



typedef enum{
    LIN_SLEW_RATE_1_07V_PER_US = 0U,  /* Slew from 40%*Vbat to 60%*Vbat @Vbat=13V,Cbus=1nF */
    LIN_SLEW_RATE_1_33V_PER_US,
    LIN_SLEW_RATE_1_59V_PER_US,
    LIN_SLEW_RATE_1_85V_PER_US,
    LIN_SLEW_RATE_2_11V_PER_US,
    LIN_SLEW_RATE_2_37V_PER_US,
    LIN_SLEW_RATE_2_63V_PER_US,
    LIN_SLEW_RATE_2_89V_PER_US,
    LIN_SLEW_RATE_3_15V_PER_US,
    LIN_SLEW_RATE_3_41V_PER_US,
    LIN_SLEW_RATE_3_67V_PER_US,
    LIN_SLEW_RATE_3_93V_PER_US,
    LIN_SLEW_RATE_4_19V_PER_US,
    LIN_SLEW_RATE_4_45V_PER_US,
    LIN_SLEW_RATE_4_71V_PER_US,
    LIN_SLEW_RATE_10_0V_PER_US
}LinTxSlewRate_t;

typedef enum{
    LIN_TX_PULL_DOWN_CAPACITY_061mA = 0U,
    LIN_TX_PULL_DOWN_CAPACITY_083mA,
    LIN_TX_PULL_DOWN_CAPACITY_105mA,
    LIN_TX_PULL_DOWN_CAPACITY_126mA
}LinTxPullDownCapacity_t;

typedef enum{
    LIN_RX_RISING_DELAY_2_323_US = 0U,
    LIN_RX_RISING_DELAY_1_344_US,
    LIN_RX_RISING_DELAY_0_968_US,
    LIN_RX_RISING_DELAY_0_767_US
}LinRxRisingDelay_t;



#define LIN_BUFF_SIZE                  8U

/* Defines for LIN device to handle message direction.  */
typedef enum{
    LIN_MSG_TYPE_RX = 0U,  /*received data from sender for slave; */
    LIN_MSG_TYPE_TX,       /*data request from sender for slave; */
    LIN_MSG_TYPE_TX_EVENT, /*event trigger data request from sender for slave; */
}LinMessageType_t;


typedef enum{
    E_LIN_BAUDRATE_10417,
    E_LIN_BAUDRATE_19200,
    E_LIN_BAUDRATE_20000,
}LIN_BaudRate_t;

#define E_LIN_BAUDRATE_MAX   (3U)/*E_LIN_BAUDRATE_20000*/


typedef enum{
    LIN_CHECKSUM_CLASSIC = 0U,  /*!< classic checksum does not include ID Byte.  */ 
    LIN_CHECKSUM_ENHANCED       /*!< "enhanced" checksum includes ID Byte.  */ 
}ChecksumType_t;


typedef enum{
  AA_SWITCH_OFF = 0U,
  AA_SWITCH_ON,
}AutoaddressingSwitchState_t;

/**
* @brief A structure to represent LIN frame information.
*/
typedef struct { 
    uint8_t             frame_id;                  /*!< frame ID.  */ 
    LinMessageType_t    msg_type;                  /*!< Type of message, rx,tx or event trigger.  */ 
    uint8_t             checksum;                  /*!< Type of checksum (enhanced or classic).  */ 
    uint8_t             length;                    /*!< Number of bytes in the data. */
    union{
        uint8_t         data[LIN_BUFF_SIZE];       /*!< data buffer. */
        uint16_t        data16[LIN_BUFF_SIZE/2];
        uint32_t        data32[LIN_BUFF_SIZE/4];
    };
    uint8_t             responseValid;
    uint8_t             frameIsValid;
    uint8_t             eventTriggered;                  /*!< event triggered when the msg_type is LIN_MSG_TYPE_TX_EVENT */
    uint8_t             linkedEventTriggerFidTableIndex; /*!< event triggered when the msg_type is LIN_MSG_TYPE_TX_EVENT */
} LIN_Device_Frame_t;


typedef void (*lin_cb_func_t)(LIN_Device_Frame_t const *frame);
typedef void (*lin_stack_cb_func_t)(void);
typedef void (*lin_wakeup_cb_func_t)(void);

void LINS_Init(void);
void LINS_AutoAddressingSwitch(AutoaddressingSwitchState_t state);
void LINS_SetSlaveModuleToSleep(void);
void LINS_RegisterLINStack_IRQ(lin_stack_cb_func_t callback,lin_wakeup_cb_func_t wakeupCallback);

void LINM_Init(void);
void LINM_RegisterLINM_IRQ(lin_cb_func_t cb);
void LINM_UnRegisterLINM_IRQ(void);
int8_t LINM_SendFrame(LIN_Device_Frame_t *frame);

#if LIN_STACK_TYPE == LIN_STACK_TYPE_PRIVATE

typedef void (*ls_DataReqCallback_t)(LIN_Device_Frame_t const *frame);
typedef void (*ls_DataRspCallback_t)(LIN_Device_Frame_t *const frame);
typedef void (*ls_SleepReqCallback_t)(void);

typedef struct{
  ls_DataReqCallback_t         dataReqCallback;   /* received data from master */
  ls_DataRspCallback_t         dataRspCallback;   /* send data to master */
  ls_SleepReqCallback_t        sleepReqCallback;
}ls_LinsPrivateFramesCallback_t;

void LINS_RegisterPrivateServices(LIN_Device_Frame_t *frameTable, uint8_t tableSize, const ls_LinsPrivateFramesCallback_t *pFunCallback);

#endif


#endif /* __LIN_DEVICE_H__ */
