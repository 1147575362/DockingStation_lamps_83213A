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
 
#ifndef SYSTICK_SFRS_H__
#define SYSTICK_SFRS_H__

#include <stdint.h>
#include <core_cm0.h>
/**
 * @brief Enable SysTick timer.
 */
static __INLINE void f_SYSTICK_Enable(void)
{
    SysTick->CTRL |= (SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk);
}

/**
 * @brief Disable SysTick timer.
 */
static __INLINE void f_SYSTICK_Disable(void)
{
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

/**
 * @brief Check if SysTick timer enable.
 *
 * @return 0 if disable.
 */
static __INLINE uint8_t f_SYSTICK_IsEnabled(void)
{
    
    return (uint8_t)(SysTick->CTRL & SysTick_CTRL_ENABLE_Msk);
}

/**
 * @brief Enable SysTick timer interrupt.
 */
static __INLINE void f_SYSTICK_IntEnable(void)
{
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}

/**
 * @brief Disable SysTick timer interrupt.
 */
static __INLINE void f_SYSTICK_IntDisable(void)
{
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}

/**
 * @brief Set Reload value of SysTick timer.
 *
 * @param ticks The tick number to be set.
 */
static __INLINE void f_SYSTICK_SetLoad(uint32_t ticks)
{
    SysTick->LOAD = ticks;
} 

/**
 * @brief Set counter value of SysTick timer.
 *
 * @param ticks The tick number to be set.
 */
static __INLINE void f_SYSTICK_SetCount(uint32_t ticks)
{
    SysTick->VAL = ticks;
} 

/**
 * @brief Get counter value of SysTick timer.
 *
 * @return The current counter of Systick timer.
 */
static __INLINE uint32_t f_SYSTICK_GetCount(void)
{
    return SysTick->VAL;
} 

/**
 * @brief Get load value of SysTick timer.
 *
 * @return The load setting of Systick timer.
 */
static __INLINE uint32_t f_SYSTICK_GetLoad(void)
{
    return SysTick->LOAD;
} 
#endif /* __SYSTICK_SFRS_H__ */
