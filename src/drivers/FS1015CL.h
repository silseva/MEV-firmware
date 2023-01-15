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
    FS1015CL(ADC122S021& adc) : OFFSET(0.5f), SLOPE(25.0f), adc(adc) { }

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

        // Voltage < 0.45V sensor error
        // Voltage > 4.65V sensor over scale
        if((voltage < 0.45f) || (voltage > 4.65f))
        {
            return std::numeric_limits< float >::signaling_NaN();
        }

        return (voltage - OFFSET) * SLOPE;
    }

    /**
     * Set sensor's output characteristic parameters.
     *
     * @param offset: output offset at 0 SLPM, in volt.
     * @param slope: output slope in SLPM/volt
     */
    void setOutputParameters(const float offset, const float slope)
    {
        OFFSET = offset;
        SLOPE  = slope;
    }

private:

    float OFFSET;       ///< Output offset at 0 SLPM, in volt
    float SLOPE;        ///< Output slope in SLPM/volt
    ADC122S021& adc;    ///< ADC instance
};
