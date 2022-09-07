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

#pragma once

typedef struct
{
    uint32_t crc;                   //< CRC32 of the other fields
    float    SENS_SUPPLY_VOLTAGE;   //< Sensors' power supply voltage
    float    ADC_OFFSET[2];         //< ADC offset
    float    ADC_SLOPE[2];          //< ADC slope in counts/V
}
__attribute__((__packed__)) adCal_t;


/**
 * Get default calibration values.
 *
 * - supply voltage: 5.0V
 * - ADC offset: 0
 * - ADC slope: 819.2 counts/V
 *
 * @return an adCal_t variable with fields set to default values.
 */
adCal_t defaultAnalogCalibrationData();

/**
 * Read calibration values from OTP memory.
 *
 * @param cal: structure holding the values read from OTP memory.
 * @return true on success, false if OTP is not locked or there is a CRC mismatch.
 */
bool readAnalogCalibrationData(adCal_t& cal);

/**
 * Write calibration values to OTP memory.
 *
 * @param cal: structure holding the values to be written to OTP memory.
 */
void writeAnalogCalibrationData(adCal_t& cal);
