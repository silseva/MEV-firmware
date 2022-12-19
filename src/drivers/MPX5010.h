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

#include <cstdint>
#include <limits>
#include "hwmapping.h"
#include "ADC122S021.h"

/**
 * Driver for MPX5010 differential pressure sensor.
 *
 * @param CH: template parameter specifying the ADC channel to which the
 * sensor's analog output is connected.
 */
template < AdcChannel CH >
class MPX5010
{
public:

    /**
     * Constructor.
     * Calibration offset and slope are configured for a default supply of 5V.
     *
     * @param adc: instance of the ADC122S021 driver used to sample sensor's
     * analog outuput.
     */
    MPX5010(ADC122S021& adc) : OFFSET(0.2f), SLOPE(2222.222f), adc(adc) { }

    /**
     * Destructor.
     */
    ~MPX5010() { }

    /**
     * Read the current differential pressure measured by the sensor.
     *
     * @return differential pressure in Pa.
     */
    float getDiffPressure()
    {
        float voltage = adc.getVoltage(CH);

        // ADC failure
        if(voltage == std::numeric_limits< float >::signaling_NaN())
        {
            return voltage;
        }

        return (voltage - OFFSET) * SLOPE;
    }

    /**
     * Tune offset and slope coefficients basing on sensor's effective supply
     * voltage.
     *
     * @param vSupply: value of sensor's power supply in volt.
     */
    void calibrateSupplyVoltage(const float vSupply)
    {
        OFFSET = 0.04f * vSupply;
        SLOPE  = 1000.0f / (0.09f * vSupply);
    }

private:

    float OFFSET;       ///< Output offset at 0 Pa, in volt
    float SLOPE;        ///< Output slope in Pa/volt
    ADC122S021& adc;    ///< ADC instance
};
