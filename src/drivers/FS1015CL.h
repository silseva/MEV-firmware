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
#include <limits>
#include "ADC122S021.h"

/**
 * Driver for FS1015CL mass flow sensor.
 *
 * @param CH: template parameter specifying the ADC channel to which the
 * sensor's analog output is connected.
 */
template < AdcChannel CH >
class FS1015CL
{
public:

    /**
     * Constructor.
     *
     * @param adc: instance of the ADC122S021 driver used to sample sensor's
     * analog outuput.
     */
    FS1015CL(ADC122S021& adc) : adc(adc) { }

    /**
     * Destructor.
     */
    ~FS1015CL() { }

    /**
     * Read the current flow rate measured by the sensor, in standard l/min.
     *
     * @return flow rate in l/min.
     */
    float getFlowRate()
    {
        float voltage = adc.getVoltage(CH);

        // ADC failure
        if(voltage == std::numeric_limits< float >::signaling_NaN())
        {
            return voltage;
        }

        // Voltage < 0.5V sensor error
        // Voltage > 4.5V sensor over scale
        if((voltage < 0.5f) || (voltage > 4.5f))
        {
            return std::numeric_limits< float >::signaling_NaN();
        }

        return (voltage - OFFSET) * SLOPE;
    }

private:

    static constexpr float OFFSET = 0.5f;   ///< Output offset at 0 SLPM, in volt
    static constexpr float SLOPE  = 25.0f;  ///< Output slope in SLPM/volt
    ADC122S021& adc;                        ///< ADC instance
};
