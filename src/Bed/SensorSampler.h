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

#include "common/ActiveObject.h"
#include "drivers/hwmapping.h"
#include "drivers/ADC122S021.h"
#include "drivers/FSP2000.h"
#include "drivers/FS1015CL.h"
#include "AnalogSensors.h"

/**
 * Sensor sampler class for collecting all the measurements, active object.
 */
class SensorSampler : public ActiveObject
{
public:

    /**
     * Constructor.
     */
    SensorSampler();

    /**
     * Destructor.
     */
    virtual ~SensorSampler();

private:

    /**
     * Worker function of the sensor sampler, called by the active object
     * thread.
     */
    virtual void run() override;

    static constexpr uint32_t updateStep = 40;  ///< 40ms update step (25Hz)
    AnalogSensors&            sensors;          ///< Analog sensors manager
};
