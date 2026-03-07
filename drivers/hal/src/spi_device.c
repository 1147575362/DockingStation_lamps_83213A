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
#include <gpio_device.h>
#include <spi_device.h>
#include <atomic.h>
#include <isrfuncs.h>



int8_t SPI_Init(SpiClkDevider_t devider)
{
    GPIO_SFRS->GPIO_CFG[(uint8_t)GPIO_PORT_3].DIR   = (uint8_t)GPIO_DIR_OUTPUT;
    GPIO_SFRS->GPIO_CFG[(uint8_t)GPIO_PORT_4].DIR   = (uint8_t)GPIO_DIR_OUTPUT;
    GPIO_SFRS->GPIO_CFG[(uint8_t)GPIO_PORT_5].DIR   = (uint8_t)GPIO_DIR_INPUT;
    

    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_PORT_3].HWMODE = 5U;/* GPIO_PORT_3: SPI Master Clock Out.*/
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_PORT_4].HWMODE = 4U;/* GPIO_PORT_4: SPI Master MOSI*/
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_PORT_5].HWMODE = 4U;/* GPIO_PORT_5: SPI Master MISO.*/

    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_PORT_3].PULLMODE = (uint8_t)GPIO_PULL_UP;
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_PORT_4].PULLMODE = (uint8_t)GPIO_PULL_UP;    
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_PORT_5].PULLMODE = (uint8_t)GPIO_PULL_UP; 

    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_PORT_3].RDENA  = 0U;
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_PORT_4].RDENA  = 0U;
    IOCTRLA_SFRS->GPIO[(uint8_t)GPIO_PORT_5].RDENA  = 1U;    
    
    
    SPI_SFRS->SPICTRL.FIFOSOFTRESET = 1U;
    SPI_SFRS->SPICTRL.LPBK = 0U;
    SPI_SFRS->SPICTRL.SCKEXT = ((uint8_t)devider >> 2);
    SPI_SFRS->SPICTRL.SPR    = (uint8_t)devider &0x03U;
    SPI_SFRS->SPICTRL.ENA_REQ = 1U;
    return 0;
}

/*#pragma optimize=none*/
int8_t SPI_SendBuff(uint8_t * const buff, uint16_t length)
{
    uint8_t data = 0U;
    SPI_SFRS->SPICTRL.FIFOSOFTRESET = 1U;
    for (uint16_t i = 0U; i < length; i++){
        SPI_SFRS->TXDATA = buff[i];
        while(SPI_SFRS->SPISTATUS.RXFIFOEMPTY == 1U){}/* waiting for rx  completed*/
        data = SPI_SFRS->RXDATA;
        
    }
    return (int8_t)data;
}

int8_t SPI_ReadBuff(uint8_t * const buff, uint16_t length)
{
    SPI_SFRS->SPICTRL.FIFOSOFTRESET = 1U;
    for (uint16_t i = 0; i < length; i++){
        SPI_SFRS->TXDATA = 0xFFU;
        while(SPI_SFRS->SPISTATUS.RXFIFOEMPTY == 1U){}/* waiting for rx completed */
        buff[i] = SPI_SFRS->RXDATA;
    }
    return 0;
}



uint8_t SPI_ReadWriteOneByte(uint8_t data)
{
    uint8_t temp;

    /* Send data */
    SPI_SFRS->TXDATA = data;
    /*wait TXFIFO empty*/
    while(SPI_SFRS->SPISTATUS.TXFIFOEMPTY==0U){}
    /*wait RXFIFO not empty */
    while(SPI_SFRS->SPISTATUS.RXFIFOEMPTY==1U){}
    /*read data*/
    temp = SPI_SFRS->RXDATA;

    return temp;
}

void SPI_Handler(void)
{
  
  
}