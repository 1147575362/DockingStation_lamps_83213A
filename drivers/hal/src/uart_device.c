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

#include <appConfig.h>
#include <isrfuncs.h>
#include <gpio_device.h>
#include <uart_device.h>
#include <atomic.h>


/***************
GPIO1           ->GAP4
GPIO2           ->GAP3
GPIO3           ->GAP1
GPIO4(RXD)      ->GAP0
GPIO5(TXD)      ->GAP2
******/

static uint8_t  rxBuff[RX_BUFF_SIZE];
typedef struct{
  uint16_t txCompleted;
  uint16_t txInput;
  uint16_t txOutput;
  uint8_t  txBuff[TX_BUFF_SIZE];
}UartBuffer_t;


static uartRx_cb_func_t uartRxDataCallback = NULL;
//static uartTx_cb_func_t uartTxCompletedCallback = NULL;

int8_t UART_Init(Baudrate_DIV_16MHz_t baudRate, BitSize_t bitSize, Parity_t parity,StopBits_t stopBits)
{
    int8_t result = 0;
    UART_SFRS->BAUD.OSR       = BAUDRATE_OSR_16MHZ;
    UART_SFRS->BAUD.BAUDDIV   = (uint8_t)baudRate;
    UART_SFRS->MSGCTRL.SIZE   = (uint8_t)bitSize;
    UART_SFRS->MSGCTRL.STOP   = (uint8_t)stopBits;
    UART_SFRS->MSGCTRL.PARITY = (uint8_t)parity;

    GPIO_SFRS->GPIO_CFG[(uint8_t)GPIO_PORT_4].DIR   = (uint8_t)GPIO_DIR_INPUT;
    GPIO_SFRS->GPIO_CFG[(uint8_t)GPIO_PORT_5].DIR   = (uint8_t)GPIO_DIR_OUTPUT;
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_PORT_4].HWMODE = 2U;/* GPIO_PORT_4: RXD*/
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_PORT_5].HWMODE = 2U;/* GPIO_PORT_5: TXD*/
    
    UART_SFRS->MSGCTRL.UFIFOSOFTRESET = 1U;// FIFO reset
    UART_SFRS->MSGCTRL.TXXFERCNTCLR = 1U;
    UART_SFRS->MSGCTRL.RXXFERCNTCLR = 1U;
    UART_SFRS->MSGCTRL.ENABLE = 1U;
    return result;
}


void UART_RegisterIRQ(uartRx_cb_func_t rxDowncallback, uartTx_cb_func_t txDowncallback)
{
    UART_SFRS->MSGCTRL.UFIFOSOFTRESET = 1U;// FIFO reset
    /*  multi-Rx settings */
    uartRxDataCallback = rxDowncallback;
    UART_SFRS->MSGCTRL.RXXFERCNTCLR = 1U;
    UART_SFRS->INT.CLEAR.RXMULTDONE = 1U;
    UART_SFRS->FIFOLEVELCTL.RXMULTIPLEXFERDONECNT = RX_FIFO_INT_SIZE;
    UART_SFRS->INT.ENABLE.RXMULTDONE = 1U;

    /*  Received timeout settings */
    UART_SFRS->INT2.CLEAR.RXTOUT = 1U;
    UART_SFRS->INT2.ENABLE.RXTOUT = 1U;
    
    NVIC_EnableIRQ(UART_IRQn);
}

void UART_UnRegisterIRQ(void)
{
    uartRxDataCallback = NULL;
//    uartTxCompletedCallback = NULL;
    UART_SFRS->INT.ENABLE.RXMULTDONE = 0U;
    UART_SFRS->INT.ENABLE.TXDONE     = 0U;
    UART_SFRS->INT.ENABLE.BREAKKERR  = 0U;
    NVIC_DisableIRQ(UART_IRQn);
}


int8_t UART_SendBuff(uint8_t *buff, uint16_t length)
{
    for(uint16_t i = 0; i < length; i++){
        UART_REG_DATA = buff[i];
        while(UART_SFRS->INT.STATUS.TXDONE == 0U){}
        UART_SFRS->INT.CLEAR.TXDONE = 1U;
    }
    return 0;
}


void UART_Handler(void)
{
    if (uartRxDataCallback != NULL){
        uint8_t statusRXM = UART_SFRS->INT.IRQ.RXMULTDONE;
        uint8_t statusRXT = UART_SFRS->INT2.IRQ.RXTOUT;
        if ( (statusRXM != 0U) || (statusRXT != 0U) ){
            uint8_t length = UART_SFRS->FIFOSTATUS.RXCOUNT;
            for (uint8_t i = 0; i < length; i++){
                rxBuff[i] = UART_REG_DATA;
            }
//            UART_SFRS->INT.CLEAR.RXMULTDONE = 1U;
//            UART_SFRS->INT2.CLEAR.RXTOUT = 1U;
            UART_SFRS->INT.CLEAR.BYTE = 0xFFU;
            UART_SFRS->INT2.CLEAR.RXTOUT = 1U;
            UART_SFRS->MSGCTRL.RXXFERCNTCLR = 1U;/* clear fifo buff counter to 0 */
            uartRxDataCallback(rxBuff,length);
        }
    } 

}




