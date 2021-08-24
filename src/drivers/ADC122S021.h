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

/**
 * Enumeration type for safe ADC channel selection.
 */
enum class AdcChannel : uint8_t
{
    _1 = 0,
    _2 = 1
};

/**
 * Driver for ADC122S021 ADC IC.
 */
class ADC122S021
{
public:

    /**
     * Singleton instance getter.
     *
     * @return reference to the singleton instance of this class.
     */
    static ADC122S021& instance();

    /**
     * Destructor.
     */
    ~ADC122S021();

    /**
     * Read the current value of one of the two ADC channel.
     * Returns a signalling NaN in case of an hardware failure.
     *
     * @param channel: channel number.
     * @return channel value or signalling NaN on failure.
     */
    float getValue(const AdcChannel channel);

    /**
     * Copy constructor, deleted as this class is singleton.
     */
    ADC122S021(const ADC122S021& other) = delete;

    /**
     * Assignment operator, deleted as this class is singleton.
     */
    ADC122S021& operator=(const ADC122S021& other) = delete;

    /**
     * operator== always return true: this class is singleton and is always
     * equal to itself.
     */
    bool operator==(const ADC122S021& other) const { return true; };

    /**
     * operator!= always return false: this class is singleton and is never
     * different from itself.
     */
    bool operator!=(const ADC122S021& other) const { return false; };

private:

    /**
     * Default constructor
     */
    ADC122S021();

};
