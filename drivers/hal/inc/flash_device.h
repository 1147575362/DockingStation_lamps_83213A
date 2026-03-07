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

#ifndef FLASH_DEVICE_H__
#define FLASH_DEVICE_H__

#include <stdint.h>

/**
 * @brief Erase a sector flash memory and wait for completion
 *
 * @param addr: starting address of the block
 * @return none
 */
void Flash_EraseSector(uint32_t sectorAddress);

/**
 * @brief write flash memory and wait for completion
 *
 * @param addr: starting address of the data
 * @return none
 */
void Flash_WriteWord(uint32_t address,uint32_t data);
/**
 * @brief Copy a data block from RAM to flash memory.
 *
 * @param destAddress: destination address in flash memory
 * @param buff: source address in RAM
 * @param length: size of data in bytes
 * @return none
 */
int8_t Flash_devCopyToFlash(uint32_t destAddress, uint8_t *const buff, uint16_t length);

#endif /* __FLASH_DEVICE_H__ */