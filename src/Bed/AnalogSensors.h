/*
 * MEV board firmware
 * Copyright (C) 2021 - 2022 Silvano Seva silvano.seva@polimi.it
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
#include "drivers/ADC122S021.h"

enum class Sensor : uint8_t
{
    PRESS_1 = 0,
    PRESS_2 = 1,
    FLOW_1  = 2,
    FLOW_2  = 3,
};

class AnalogSensors
{
public:

    /**
     * Singleton instance getter.
     *
     * @return reference to the singleton instance of this class.
     */
    static AnalogSensors& instance();

    /**
     * Destructor.
     */
    ~AnalogSensors();

    /**
     * Read the current value of one of the two ADC channel and returns it as
     * raw value expressed in ADC counts.
     * Returns 0xFFFF in case of an hardware failure.
     *
     * @param channel: channel number.
     * @return channel raw value or 0xFFFF on failure.
     */
    uint16_t getRawValue(const Sensor sensor);

    /**
     * Read the current voltage of one of the two ADC channel.
     * Returns a signalling NaN in case of an hardware failure.
     *
     * @param channel: channel number.
     * @return channel value or signalling NaN on failure.
     */
    float getValue(const Sensor sensor);

    /**
     * Copy constructor, deleted as this class is singleton.
     */
    AnalogSensors(const AnalogSensors& other) = delete;

    /**
     * Assignment operator, deleted as this class is singleton.
     */
    AnalogSensors& operator=(const AnalogSensors& other) = delete;

    /**
     * operator== always return true: this class is singleton and is always
     * equal to itself.
     */
    bool operator==(const AnalogSensors& other) const { return true; };

    /**
     * operator!= always return false: this class is singleton and is never
     * different from itself.
     */
    bool operator!=(const AnalogSensors& other) const { return false; };

private:

    /**
     * Default constructor
     */
    AnalogSensors();

    /**
     * Select multiplexer input given the sensor to sample.
     *
     * @param sensor: sensor to be sampled.
     * @return corresponding ADC channel number.
     */
    AdcChannel selectInput(const Sensor sensor);
};
