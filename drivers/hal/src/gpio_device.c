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

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <gpio_device.h>
#include <isrfuncs.h>

#ifdef GPIO_SFRS
static gpio_cb_func_t gpioCallback[9]= {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};


void GPIO_Handler( void )
{
    for (uint8_t i = (uint8_t)GPIO_PORT_1; i<= (uint8_t)GPIO_PORT_9; i++ ){
        if (GPIO_SFRS->GPIO_CFG[i].ACTDET == 1U){
            GPIO_SFRS->GPIO_CFG[i].CLR = 1U;
            if (gpioCallback[i] !=NULL){
                gpioCallback[i]();
            }
        }
    }
}


void GPIO_Init(GpioPort_t port,GpioDir_t dir,GpioPullMode_t pullMode)
{
    IOCTRLA_SFRS->GPIO[(uint8_t)port].HWMODE    = (uint8_t)GPIO_MUX_GPIO;
    IOCTRLA_SFRS->GPIO[(uint8_t)port].PULLMODE  = (uint8_t)pullMode;
    GPIO_SFRS->GPIO_CFG[(uint8_t)port].DIR      = (uint8_t)dir;
    IOCTRLA_SFRS->GPIO[(uint8_t)port].RDENA     = 1U;
}


void GPIO_RegisterIRQ(GpioPort_t port, GpioISREdge_t edge, gpio_cb_func_t callback)
{
    GPIO_SFRS->GPIO_CFG[(uint8_t)port].CLR = 1U;
    GPIO_SFRS->GPIO_CFG[(uint8_t)port].IE = 1U;
    GPIO_SFRS->GPIO_CFG[(uint8_t)port].EDGE = (uint8_t)edge;
    gpioCallback[(uint8_t)port] = callback;
    NVIC_EnableIRQ(GPIO_IRQn);
}

void GPIO_UnRegisterIRQ(GpioPort_t port)
{
    gpioCallback[(uint8_t)port] = NULL;
}

void GPIO_Set(GpioPort_t port, GpioSetState_t state)
{
    if(port <= GPIO_PORT_8){
        GPIO_SFRS->GPADATA[1U << (uint8_t)port] =  (uint8_t)state << (uint8_t)port;
    }else{
        GPIO_SFRS->GPBDATA[1U << ((uint8_t)port - (uint8_t)GPIO_PORT_9)]=  (uint8_t)state << ((uint8_t)port - (uint8_t)GPIO_PORT_9);
    }
}

void GPIO_SetGPIOs(uint8_t portsMask, uint8_t value)
{
    GPIO_SFRS->GPADATA[portsMask] = value;/* value */
}


uint8_t GPIO_Read(GpioPort_t port)
{
    uint8_t data;
    if(port <= GPIO_PORT_8){
        data = GPIO_SFRS->GPADATA[1U << (uint8_t)port];
    }else{
        data = GPIO_SFRS->GPBDATA[1U << ((uint8_t)port - (uint8_t)GPIO_PORT_9)];
    }
    return (uint8_t)(data != 0U);
}

#endif