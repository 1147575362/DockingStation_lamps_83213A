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

#include <linMasterTask.h>
#include <lin_device.h>

#if LIN_MASTER_EN == 1

static TaskState_t linmTaskState = TASK_STATE_INIT;
static LIN_Device_Frame_t activelinmFrame;
void linmSendTimerExpired(SoftTimer_t *timer);
void LIN_Master_ISR(LIN_Device_Frame_t const *frame);
void hanldeCommands(LIN_Device_Frame_t *frame);


static LIN_Device_Frame_t linm_cmd_table[] = {
    [0] = {
        .frame_id = 0x20U,
        .msg_type = LIN_MSG_TYPE_TX,
        .checksum = LIN_CHECKSUM_ENHANCED,
        .length   = LIN_BUFF_SIZE,
    },
    [1] = {
        .frame_id = 0x21U,
        .msg_type = LIN_MSG_TYPE_RX,
        .checksum = LIN_CHECKSUM_ENHANCED,
        .length   = LIN_BUFF_SIZE,
    },
};


void linmSendTimerExpired(SoftTimer_t *timer)
{
    static uint8_t colorIndex = 0U;
    static uint8_t index = 0U;
    linm_cmd_table[index].data[0] = 0x01U;
    linm_cmd_table[index].data[1] = 0x00U + (2U << 4U);
    linm_cmd_table[index].data[5] = 255U;
    linm_cmd_table[index].data[6] = 0U;
    
    if (colorIndex == 0U){
        linm_cmd_table[index].data[2] = 255U;
        linm_cmd_table[index].data[3] = 0U;
        linm_cmd_table[index].data[4] = 0U;
        colorIndex = 1U;
    }else if (colorIndex == 1U){
        linm_cmd_table[index].data[2] = 0U;
        linm_cmd_table[index].data[3] = 255U;
        linm_cmd_table[index].data[4] = 0U;
        colorIndex = 2U;
    }else{
        linm_cmd_table[index].data[2] = 0U;
        linm_cmd_table[index].data[3] = 0U;
        linm_cmd_table[index].data[4] = 255U;
        colorIndex = 0U;
    }
    
    (void)LINM_SendFrame(&linm_cmd_table[index]);
    index++;
    if (index >= sizeof(linm_cmd_table)/sizeof(LIN_Device_Frame_t)){
        index = 0U;
    }
}


static SoftTimer_t linmSendTimer = {
    .mode     = TIMER_PERIODIC_MODE,
    .interval = 1000U,
    .handler  = linmSendTimerExpired
};


void LIN_Master_ISR(LIN_Device_Frame_t const *frame)
{
    activelinmFrame = *frame;
    TM_PostTask(TASK_ID_LINM);
}

void hanldeCommands(LIN_Device_Frame_t *frame)
{
    /* Handle received command */
    
}

void LINM_TaskHandler(void)
{
    switch(linmTaskState){
    case TASK_STATE_ACTIVE:
      hanldeCommands(&activelinmFrame);
      break;
    case TASK_STATE_INIT:
      LINM_Init();
      LINM_RegisterLINM_IRQ(LIN_Master_ISR);
      
      SoftTimer_Start(&linmSendTimer);
      linmTaskState = TASK_STATE_ACTIVE;
      break;
    default:
      break;
    }
}

#endif
