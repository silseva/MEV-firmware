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

#include <miosix.h>
#include <cmath>
#include "SensorSampler.h"
#include "BedState.h"

#define SAMPLER_PRINT

using namespace std;
using namespace miosix;

SensorSampler::SensorSampler() : sensors(AnalogSensors::instance())
{

}

SensorSampler::~SensorSampler()
{

}

void SensorSampler::run()
{
    unsigned long long time = getTick();
    uint8_t            turn = 0;
    uint8_t            tail = 0;

    sensors.selectInput(Sensor::PRESS_1);

    while(!should_stop)
    {
        if((turn % 2) == 0)
        {
            // Update pressure measurements
            state.press1_raw = sensors.getRawValue(Sensor::PRESS_1);
            state.press1_out = sensors.getVoltage(Sensor::PRESS_1);
            state.press_1    = sensors.getValue(Sensor::PRESS_1);

            state.press2_raw = 0;    // sensors.getRawValue(Sensor::PRESS_2);
            state.press2_out = 0.0f; // sensors.getVoltage(Sensor::PRESS_2);
            state.press_2    = 0.0f; // sensors.getValue(Sensor::PRESS_2);

            // Preset input multiplexer for next turn;
            sensors.selectInput(Sensor::FLOW_1);
        }
        else
        {
            // Update flow measurements
            state.flow1_raw  = sensors.getRawValue(Sensor::FLOW_1);
            state.flow1_out  = sensors.getVoltage(Sensor::FLOW_1);
            state.flow_1     = sensors.getValue(Sensor::FLOW_1);

            state.flow2_raw  = sensors.getRawValue(Sensor::FLOW_2);
            state.flow2_out  = sensors.getVoltage(Sensor::FLOW_2);
            state.flow_2     = sensors.getValue(Sensor::FLOW_2);

            // Flow rate is in l/min while update step is in ms, hence we have
            // to divide the flow rate by 60 s/min * 1000 ms/s.
            //
            // Update volumes only if valve controller is running and flow
            // measurements contain valid data.
            if(state.enabled)
            {
                if(std::isnan(state.flow_1) == false)
                {
                    state.volume_1 += (state.flow_1 / 60000.0f)
                                    * static_cast< float > (updateStep);
                }

                if(std::isnan(state.flow_2) == false)
                {
                    state.volume_2 += (state.flow_2 / 60000.0f)
                                    * static_cast< float > (updateStep);
                }
            }

            // Preset input multiplexer for next turn;
            sensors.selectInput(Sensor::PRESS_1);
        }

        if(state.resetVolumes)
        {
            state.volume_1 = 0.0f;
            state.volume_2 = 0.0f;
            state.resetVolumes = false;
        }

        #ifdef SAMPLER_PRINT
        printf("%.3f,%.3f,%.3f,%.3f,%.3f, %d,%d\n", state.press_1, state.flow_1,
                                                    state.flow_2,  state.volume_1,
                                                    state.volume_2,
                                                    hpOutputs::out_1::value(),
                                                    hpOutputs::out_2::value());
        #endif

        turn += 1;
        time += updateStep;
        Thread::sleepUntil(time);
    }
}

