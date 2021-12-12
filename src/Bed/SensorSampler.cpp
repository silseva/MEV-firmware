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
        state.press_1 = press1.getPressure();
        state.press_2 = press2.getPressure();
        state.flow_1  = flow1.getFlowRate();
        state.flow_2  = flow2.getFlowRate();

        #ifdef SAMPLER_PRINT
        printf("%.3f,%.3f,%.3f,%.3f,%d,%d\n", state.press_1, state.press_2,
                                              state.flow_1,  state.flow_2,
                                              hpOutputs::out_1::value(),
                                              hpOutputs::out_2::value());
        #endif

        // If sampling frequency is set to 0Hz, default to 1Hz
        if(state.Fsample > 0)
        {
            time += 1000 / static_cast< uint32_t >(state.Fsample);
        }
        else
        {
            time += 1000;
        }

        Thread::sleepUntil(time);
    }
}

