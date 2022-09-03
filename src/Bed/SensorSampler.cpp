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
#include "SensorSampler.h"
#include "BedState.h"

#define SAMPLER_PRINT

using namespace std;
using namespace miosix;

SensorSampler::SensorSampler() : adc(ADC122S021::instance()),
                                 flow1(adc), flow2(adc)
{
    i2c::i2c_1::init();
    i2c::i2c_2::init();
}

SensorSampler::~SensorSampler()
{

}

void SensorSampler::run()
{
    unsigned long long time = getTick();

    while(!should_stop)
    {
        // Time step for measurements update, in milliseconds.
        // Default to 1Hz if no sampling frequency is specified.
        uint32_t updateStep = 1000;
        if(state.Fsample > 0)
        {
            updateStep = 1000 / static_cast< uint32_t >(state.Fsample);
        }


        state.press_1 = press1.getPressure();
        state.press_2 = press2.getPressure();
        state.flow_1  = flow1.getFlowRate();
        state.flow_2  = flow2.getFlowRate();

        // Flow rate is in l/min while update step is in ms, hence we have to
        // divide the flow rate by 60 s/min * 1000 ms/s.
        state.volume_1 += (state.flow_1 / 60000.0f)
                        * static_cast< float > (updateStep);

        state.volume_2 += (state.flow_2 / 60000.0f)
                        * static_cast< float > (updateStep);

        if(state.resetVolumes)
        {
            state.volume_1 = 0.0f;
            state.volume_2 = 0.0f;
            state.resetVolumes = false;
        }

        #ifdef SAMPLER_PRINT
        printf("%.3f,%.3f,%.3f,%.3f,%.3f,%.3f, %d,%d\n",
                                              state.press_1,  state.press_2,
                                              state.flow_1,   state.flow_2,
                                              state.volume_1, state.volume_2,
                                              hpOutputs::out_1::value(),
                                              hpOutputs::out_2::value());
        #endif

        Thread::sleepUntil(time);
    }
}

