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
     * Read the current value of one of the two ADC channel and returns it as
     * raw value expressed in ADC counts.
     * Returns 0xFFFF in case of an hardware failure.
     *
     * @param channel: channel number.
     * @return channel raw value or 0xFFFF on failure.
     */
    uint16_t getRawValue(const AdcChannel channel);

    /**
     * Read the current voltage of one of the two ADC channel.
     * Returns a signalling NaN in case of an hardware failure.
     *
     * @param channel: channel number.
     * @return channel value or signalling NaN on failure.
     */
    float getVoltage(const AdcChannel channel);

    /**
     * Set values for conversion offset and slope of a specific channel.
     *
     * @param channel: channel number.
     * @param slope: conversion slope, in counts/V.
     * @param offset: conversion offset, in counts.
     */
    void setConversionParameters(const AdcChannel channel, const float slope,
                                 const float offset);

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

    float CH_OFFSET[2];    ///< Channels' conversion offset
    float CH_SLOPE[2];     ///< Channels' conversion slope
};
