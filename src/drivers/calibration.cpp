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

#include <miosix.h>
#include <string.h>
#include "calibration.h"
#include "flash.h"

static constexpr uint32_t OTP_BASE_ADDR = 0x1FFF7820;
static constexpr uint32_t OTP_LOCK_ADDR = 0x1FFF7A00;

static uint32_t crc32(const void *data, const size_t len)
{
    const uint8_t *ptr = reinterpret_cast< const uint8_t * >(data);
    uint32_t crc = 0xFFFFFFFF;
    uint32_t mask;

    for(size_t i = 0; i < len; i++)
    {
        uint32_t byte = ptr[i];
        crc = crc ^ byte;

        for(uint8_t j = 0; j < 8; j++)
        {
            mask = -(crc & 1);
            crc  = (crc >> 1) ^ (0xEDB88320 & mask);
        }
    }

    return ~crc;
}


adCal_t defaultAnalogCalibrationData()
{
    adCal_t defaultCal;

    defaultCal.SENS_SUPPLY_VOLTAGE = 5.0f;
    defaultCal.ADC_OFFSET[0]       = 0.0f;
    defaultCal.ADC_OFFSET[1]       = 0.0f;
    defaultCal.ADC_SLOPE[0]        = 4096.0f / 5.0f;
    defaultCal.ADC_SLOPE[1]        = 4096.0f / 5.0f;

    // Compute CRC32
    size_t   len = (sizeof(adCal_t) - sizeof(uint32_t)) / sizeof(uint8_t);
    uint32_t crc = crc32(&(defaultCal.SENS_SUPPLY_VOLTAGE), len);
    defaultCal.crc = crc;

    return defaultCal;
}

bool readAnalogCalibrationData(adCal_t& cal)
{
    // Check if the data block is locked and thus valid.
    uint8_t lockStatus = reinterpret_cast< uint8_t * >(OTP_LOCK_ADDR)[0];
    if(lockStatus != 0x00) return false;

    // Read data
    uint8_t *src = reinterpret_cast < uint8_t *  >(OTP_BASE_ADDR);
    memcpy(&cal, src, sizeof(adCal_t));

    // Check CRC
    size_t   len = (sizeof(adCal_t) - sizeof(uint32_t)) / sizeof(uint8_t);
    uint32_t crc = crc32(&(cal.SENS_SUPPLY_VOLTAGE), len);
    if(crc != cal.crc) return false;

    return true;
}

void writeAnalogCalibrationData(adCal_t& cal)
{
    // Update CRC32 before saving into flash
    size_t   len = (sizeof(adCal_t) - sizeof(uint32_t)) / sizeof(uint8_t);
    uint32_t crc = crc32(&(cal.SENS_SUPPLY_VOLTAGE), len);
    cal.crc      = crc;

    // Write data
    flash_write(OTP_BASE_ADDR, &cal, sizeof(adCal_t) / sizeof(uint8_t));

    // Lock the sector
    uint8_t byte = 0;
    flash_write(OTP_LOCK_ADDR, &byte, 1);
}
