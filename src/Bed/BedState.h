/*
 * MEV board firmware
 * Copyright (C) 2021 - 2024  Silvano Seva silvano.seva@polimi.it
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

#include "common/RingBuffer.h"
#include "AnalogSensors.h"

typedef struct
{
    unsigned long long int timestamp;
    float   pressure;
    float   flow1;
    float   flow2;
    float   volume1;
    float   volume2;
    uint8_t valves;
}
loggerSample_t;

struct StateData
{
    SensorCalibration cal;

    bool  enabled;
    float tIns;
    float IE;
    float Fsample;

    uint16_t press1_raw;    // Output value of pressure sensor 1 in ADC counts
    uint16_t press2_raw;    // Output value of pressure sensor 2 in ADC counts
    uint16_t flow1_raw;     // Output value of flow sensor 1 in ADC counts
    uint16_t flow2_raw;     // Output value of flow sensor 2 in ADC counts

    float press1_out;       // Output value of pressure sensor 1 in volt
    float press2_out;       // Output value of pressure sensor 2 in volt
    float flow1_out;        // Output value of flow sensor 1 in volt
    float flow2_out;        // Output value of flow sensor 2 in volt

    float press_1;          // Output value of pressure sensor 1 in Pa
    float press_2;          // Output value of pressure sensor 2 in Pa
    float flow_1;           // Output value of flow sensor 1 in SLPM
    float flow_2;           // Output value of flow sensor 2 in SLPM
    float volume_1;
    float volume_2;
    bool  resetVolumes;

    RingBuffer< loggerSample_t, 131072 > log;   // 4MB buffer, 128k entries
};

extern StateData state;
