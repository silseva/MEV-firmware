/*
 * MEV board firmware
 * Copyright (C) 2022  Silvano Seva silvano.seva@polimi.it
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <drivers/flash.h>
#include <util/crc16.h>
#include <cstring>
#include "Persistence.h"

using namespace miosix;

typedef struct
{
    uint16_t crc;
    uint8_t  data[126];
}
dataBlock_t;

typedef struct
{
    uint32_t    magic;
    uint32_t    flags[32];
    dataBlock_t blocks[1024];
}
memory_t;

static const uint32_t MEM_MAGIC   = 0x4D424A46; // "MBJF"
static const uint32_t baseAddress = 0x080E0000;
memory_t *memory = ((memory_t *) baseAddress);


/**
 * \internal
 * Utility function to find the currently active data block inside memory, that
 * is the one containing the last saved settings. Blocks containing legacy data
 * are marked with numbers starting from 4096.
 *
 * @return number currently active data block or -1 if memory data is invalid.
 */
static int findActiveBlock()
{
    // Check for invalid memory data
    if(memory->magic != MEM_MAGIC)
        return -1;

    uint16_t block = 0;
    uint16_t bit   = 0;

    // Find the first 32-bit block not full of zeroes
    for(; block < 32; block++)
    {
        if(memory->flags[block] != 0x00000000)
        {
            break;
        }
    }

    // Find the last zero within a block
    for(; bit < 32; bit++)
    {
        if((memory->flags[block] & (1 << bit)) != 0)
        {
            break;
        }
    }

    block = (block * 32) + bit;
    block -= 1;

    return block;
}



bool loadDataFromFlash(void *data, const size_t size)
{
    int block = findActiveBlock();
    if(block < 0) return false;

    uint16_t crc = crc16(&(memory->blocks[block].data), size);
    if(crc != memory->blocks[block].crc)
        return false;

    memcpy(data, &(memory->blocks[block].data), size);

    return true;
}

void saveDataToFlash(void *data, const size_t size)
{
    uint32_t addr    = 0;
    int      block   = findActiveBlock();
    uint16_t prevCrc = 0;

    /*
     * Memory never initialised or save space finished: erase all the sector.
     * On STM32F405 the settings are saved in sector 11, starting at address
     * 0x08060000.
     */
    if((block < 0) || (block >= 2047))
    {
        flash_eraseSector(11);
        addr = reinterpret_cast< uint32_t > (&(memory->magic));
        flash_write(addr, &MEM_MAGIC, sizeof(MEM_MAGIC));
        block = 0;
    }
    else
    {
        prevCrc = memory->blocks[block].crc;
        block += 1;
    }

    dataBlock_t tmpBlock;
    memset(&tmpBlock, 0xFF, sizeof(dataBlock_t));
    memcpy(&(tmpBlock.data), data, size);
    tmpBlock.crc = crc16(&(tmpBlock.data), size);

    // New data is equal to the old one, avoid saving
    if((block != 0) && (tmpBlock.crc == prevCrc))
        return;

    // Save data
    addr = reinterpret_cast< uint32_t > (&(memory->blocks[block]));
    flash_write(addr, &tmpBlock, sizeof(dataBlock_t));

    // Update the flags marking used data blocks
    uint32_t flag = ~(1 << (block % 32));
    addr = reinterpret_cast< uint32_t > (&(memory->flags[block / 32]));
    flash_write(addr, &flag, sizeof(uint32_t));
}
