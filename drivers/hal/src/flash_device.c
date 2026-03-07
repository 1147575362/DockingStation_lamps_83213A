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

#include <stdint.h>
#include <string.h>
#include <flash_device.h>
#include <flash_sfrs.h>


static uint8_t flashBuff[FLASH_SECTOR_SIZE];

/* address + length can't overlap section address */
int8_t Flash_devCopyToFlash(uint32_t address, uint8_t *const buff, uint16_t length)
{
    int8_t result = -1;
    uint32_t sectorAddr = (address >> FLASH_SECTOR_SIZE_POS) << FLASH_SECTOR_SIZE_POS;
    uint32_t offsetAddr = address - sectorAddr;
    uint32_t addressInSector = offsetAddr + length;
    if ( (length != 0U) && (address >= FLASH_START_ADDRESS_USER_DATA) && ((address + length) <= FLASH_FINAL_ADDRESS) && (addressInSector <= FLASH_SECTOR_SIZE) ){
        for (uint32_t i = 0U; i < offsetAddr; i++){
            flashBuff[i] = ((uint8_t *)sectorAddr)[i];
        }
        
        for (uint32_t i = offsetAddr; i < addressInSector; i++){
            flashBuff[i] = buff[i - offsetAddr];
        }
        
        for (uint32_t i = addressInSector; i < FLASH_SECTOR_SIZE; i++){
            flashBuff[i] = ((uint8_t *)sectorAddr)[i];
        }
        
        f_FLASH_EraseSector(sectorAddr);
        
        uint32_t *pWord = (uint32_t *)((void *)flashBuff);
        for (uint32_t i = 0U; i < (FLASH_SECTOR_SIZE / 4U); i++){
            f_FLASH_WriteWord(sectorAddr + (i << 2U), pWord[i]);
        }
        result = 0;
    }
    return result;
}


void Flash_EraseSector(uint32_t sectorAddress)
{
    /* Erase sector data */
    f_FLASH_EraseSector(sectorAddress);
}

void Flash_WriteWord(uint32_t address,uint32_t data)
{
    f_FLASH_WriteWord(address, data);
}