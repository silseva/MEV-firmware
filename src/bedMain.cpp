/*
 * MEV board firmware
 * Copyright (C) 2021 - 2024  Silvano Seva silvano.seva@polimi.it
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

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include "miosix.h"

#include "Bed/ValveController.h"
#include "Bed/SensorSampler.h"
#include "Bed/AnalogSensors.h"
#include "Bed/UI/UiFsmData.h"
#include "common/Persistence.h"
#include "common/RingBuffer.h"
#include "common/Fsm.h"

using namespace std;
using namespace mxgui;
using namespace miosix;

StateData state;

int main()
{
    state.resetVolumes = true;
    state.enabled      = false;
    state.tIns         = 0.0f;
    state.IE           = 0.0f;
    state.Fsample      = 0.0f;

    state.cal.loadDefaultValues();

    if(loadDataFromFlash(&(state.cal), sizeof(SensorCalibration)) == true)
    {
        AnalogSensors::instance().applyCalibration(state.cal);
    }

    SensorSampler sampler;
    sampler.start();

    BedFsmData UiFsm(state);
    Fsm uiFsm(&UiFsm.mainPage, 50);
    uiFsm.start();

    ValveController vc(state);
    vc.start();

    while(1)
    {
        #ifndef LOG_PRINT
        if(getchar() == 'd')
        {
            loggerSample_t sample;
            while(state.log.pop(sample))
            {
                printf("%llu,%.3f,%.3f,%.3f,%.3f,%.3f,%d,%d\n",
                       sample.timestamp,       sample.pressure,
                       sample.flow1,           sample.flow2,
                       sample.volume1,         sample.volume2,
                      (sample.valves & 0x01), (sample.valves >> 1));
            }
        }
        #endif

        Thread::sleep(250);
    }

    return 0;
}
