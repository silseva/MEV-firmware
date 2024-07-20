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

#include <miosix.h>
#include "ValveController.h"
#include "drivers/hwmapping.h"

using namespace std;
using namespace miosix;

ValveController::ValveController(StateData& state) : state(state)
{
    hpOutputs::out_1::mode(Mode::OUTPUT);
    hpOutputs::out_2::mode(Mode::OUTPUT);
    hpOutputs::out_1::low();
    hpOutputs::out_2::low();
}

ValveController::~ValveController()
{
    hpOutputs::out_1::low();
    hpOutputs::out_2::low();
    hpOutputs::out_1::mode(Mode::INPUT);
    hpOutputs::out_2::mode(Mode::INPUT);
}

void ValveController::run()
{
    while(1)
    {
        if((state.enabled) && (state.tIns > 0) && (state.IE > 0))
        {
            uint32_t tIns = static_cast< uint32_t >(state.tIns * 1000.0f);
            uint32_t tEsp = static_cast< uint32_t >(state.tIns * state.IE
                                                               * 1000.0f);

            // Force EV1 and EV2 to closed state, wait 50ms to compensate for
            // valve closing time. Reset inh/exh. volume measurements before a
            // new cycle begins.
            hpOutputs::out_1::low();
            hpOutputs::out_2::low();
            Thread::sleep(50);
            state.resetVolumes = true;

            // Open EV1
            hpOutputs::out_1::high();
            miosix::ledOn();
            Thread::sleep(tIns);

            // Force EV1 and EV2 to closed state, wait 50ms to compensate for
            // valve closing time.
            hpOutputs::out_1::low();
            hpOutputs::out_2::low();
            Thread::sleep(50);

            // Open EV2
            hpOutputs::out_2::high();
            miosix::ledOff();
            Thread::sleep(tEsp);
        }
        else
        {
            hpOutputs::out_1::low();
            hpOutputs::out_2::low();

            miosix::ledOn();
            Thread::sleep(50);
            miosix::ledOff();
            Thread::sleep(50);
        }
    }
}

