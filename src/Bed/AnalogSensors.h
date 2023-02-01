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

/**
 * Enumerating type for sensors' identification.
 */
enum class Sensor : uint8_t
{
    PRESS_1 = 0,
    PRESS_2 = 1,
    FLOW_1  = 2,
    FLOW_2  = 3,
};

/**
 * Container class to handle sensors' calibration parameters.
 */
struct SensorCalibration
{
    /**
     * Default constructor, load all fields with their default values.
     */
    SensorCalibration()
    {
        loadDefaultValues();
    }

    /**
     * Reset all the fields with their default values;
     */
    void loadDefaultValues()
    {
        for(int i = 0; i < 2; i++)
        {
            flowSens[i].offset  = 0.5f;
            flowSens[i].slope   = 25.0f;
            pressSens[i].offset = 0.2f;
            pressSens[i].slope  = 2222.222f;
        }
    }

    typedef struct
    {
        float offset;
        float slope;
    }
    cal_t;

    cal_t flowSens[2];
    cal_t pressSens[2];
};

/**
 * Analog sensors' manager class.
 */
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
     * Read the output value of a given sensor as raw ADC counts.
     * Returns 0xFFFF in case of an hardware failure.
     *
     * @param channel: channel number.
     * @return sensor raw output value or 0xFFFF on failure.
     */
    uint16_t getRawValue(const Sensor sensor);

    /**
     * Read the output voltage of a given sensor.
     * Returns a signalling NaN in case of an hardware failure.
     *
     * @param channel: channel number.
     * @return sensor output voltage or signalling NaN on failure.
     */
    float getVoltage(const Sensor sensor);

    /**
     * Read the output of a given sensor.
     * Returns a signalling NaN in case of an hardware failure.
     *
     * @param channel: channel number.
     * @return sensor output or signalling NaN on failure.
     */
    float getValue(const Sensor sensor);

    /**
     * Select multiplexer input given the sensor to sample.
     *
     * @param sensor: sensor to be sampled.
     * @return corresponding ADC channel number.
     */
    AdcChannel selectInput(const Sensor sensor);

    /**
     * Apply external calibration parameters to all the sensors.
     *
     * @param cal: data structure holding the sensors' calibration parameters.
     */
    void applyCalibration(const SensorCalibration& cal);

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
};
