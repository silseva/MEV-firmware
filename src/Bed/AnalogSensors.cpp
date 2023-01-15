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

#include <limits>
#include <miosix.h>
#include "AnalogSensors.h"
#include "drivers/MPX5010.h"
#include "drivers/FS1015CL.h"
#include "drivers/hwmapping.h"
#include "drivers/calibration.h"

using namespace miosix;
using namespace std;

typedef struct
{
    uint8_t    muxSel;
    AdcChannel channel;
}
AdChConfig_t;

static constexpr AdChConfig_t AdChConfig[4] =
{
    {1, AdcChannel::_1},    // PRESS_1
    {1, AdcChannel::_2},    // PRESS_2
    {0, AdcChannel::_1},    // FLOW_1
    {0, AdcChannel::_2},    // FLOW_2
};


static ADC122S021& Adc = ADC122S021::instance();    // ADC driver
static FS1015CL < AdcChannel::_1 > flow1(Adc);      // First flow sensor.
static FS1015CL < AdcChannel::_2 > flow2(Adc);      // Second flow sensor.
static MPX5010  < AdcChannel::_1 > press1(Adc);     // First pressure sensor.
static MPX5010  < AdcChannel::_2 > press2(Adc);     // Second pressure sensor.


AnalogSensors& AnalogSensors::instance()
{
    static AnalogSensors sensors;
    return sensors;
}

AnalogSensors::AnalogSensors()
{
    adc::muxs::mode(Mode::OUTPUT);
    adc::muxs::high();

    adCal_t cal;
    bool ok = readAnalogCalibrationData(cal);

    if(ok)
    {
        press1.calibrateSupplyVoltage(cal.SENS_SUPPLY_VOLTAGE);
        press2.calibrateSupplyVoltage(cal.SENS_SUPPLY_VOLTAGE);
        Adc.setConversionParameters(AdcChannel::_1, cal.ADC_SLOPE[0],
                                    cal.ADC_OFFSET[0]);
        Adc.setConversionParameters(AdcChannel::_2, cal.ADC_SLOPE[1],
                                    cal.ADC_OFFSET[1]);
    }
}

AnalogSensors::~AnalogSensors()
{

}

uint16_t AnalogSensors::getRawValue(const Sensor sensor)
{
    if(sensor > Sensor::FLOW_2)
        return 0xFFFF;

    auto ch = selectInput(sensor);
    return Adc.getRawValue(ch);
}

float AnalogSensors::getVoltage(const Sensor sensor)
{
    if(sensor > Sensor::FLOW_2)
        return 0xFFFF;

    auto ch = selectInput(sensor);
    return Adc.getVoltage(ch);
}

float AnalogSensors::getValue(const Sensor sensor)
{
    float value = std::numeric_limits< float >::signaling_NaN();

    if(sensor > Sensor::FLOW_2)
        return value;

    selectInput(sensor);

    // Acquire measurement
    switch(sensor)
    {
        case Sensor::PRESS_1: value = press1.getDiffPressure(); break;
        case Sensor::PRESS_2: value = press2.getDiffPressure(); break;
        case Sensor::FLOW_1:  value = flow1.getFlowRate();      break;
        case Sensor::FLOW_2:  value = flow2.getFlowRate();      break;
        default:                                                break;
    }

    return value;
}

void AnalogSensors::applyCalibration(const SensorCalibration& cal)
{
    // Tune flow sensors
    flow1.setOutputParameters(cal.flowSens[0].offset, cal.flowSens[0].slope);
    flow2.setOutputParameters(cal.flowSens[1].offset, cal.flowSens[1].slope);

    // Tune pressure sensors
    press1.setOutputParameters(cal.pressSens[0].offset, cal.pressSens[0].slope);
    press2.setOutputParameters(cal.pressSens[1].offset, cal.pressSens[1].slope);
}

AdcChannel AnalogSensors::selectInput(const Sensor sensor)
{
    // Select mux channel, wait 30ms to allow the LPF to settle
    uint8_t index  = static_cast< uint8_t > (sensor);
    auto    config = AdChConfig[index];

    if(config.muxSel != adc::muxs::value())
    {
        (config.muxSel == 1) ? adc::muxs::high() : adc::muxs::low();
        delayMs(50);
    }

    return config.channel;
}
