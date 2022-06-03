/*
 * MEV board firmware
 * Copyright (C) 2021  Silvano Seva silvano.seva@polimi.it
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

#include <cstdint>
#include "common/PidRegulator.h"

enum class CtrlMode : uint8_t
{
    MAN  = 0,
    AUTO = 1
};

struct BjState
{
    CtrlMode      ctMode;           ///< Controller operating mode
    PidParameters ctParams;         ///< Controller tuning parameters
    float         ctSetPoint;       ///< Controller et point
    float         ctOutput;         ///< Controller output
    float         manOutput;        ///< Output value for manual mode

    uint16_t      levelRaw;        ///< Raw value of BJ level in ADC counts
    uint16_t      levelAdj;        ///< Adjusted value of BJ level w.r.t zero
    float         levelNorm;       ///< Normalised value of BJ level in range 0.0 - 1.0
    uint16_t      zeroLevel;       ///< Raw value in ADC counts correspoding to BJ zero level
    uint16_t      maxLevel;        ///< Raw value in ADC counts correspoding to BJ max level
};

extern BjState bjState;
