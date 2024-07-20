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
#include "miosix.h"
#include "mxgui/display.h"

#include "common/Persistence.h"
#include "BellJar/LevelController.h"
#include "BellJar/UI/UiFsmData.h"
#include "BellJar/BjState.h"

using namespace std;
using namespace mxgui;

BjState bjState;

int main()
{
    if(loadDataFromFlash(&bjState.ctParams, sizeof(PidParameters)) == false)
    {
        // Loading saved parameters went wrong, initialize them to safe values
        bjState.ctParams.uMin    = 0.0f;
        bjState.ctParams.uMax    = 1.0f;
        bjState.ctParams.k       = 0.0f;
        bjState.ctParams.Ti      = 0.0f;
        bjState.ctParams.Tsample = 0.0f;
    }

    // Initialize all the other non-persistent parameters
    bjState.ctMode     = CtrlMode::MAN;
    bjState.ctSetPoint = 0.0f;
    bjState.manOutput  = 0.0f;
    bjState.zeroLevel  = 0;
    bjState.maxLevel   = 4095;

    LevelController lc;
    lc.start();

    BjFsmData bjFsm;
    Fsm uiFsm(&bjFsm.mainPage, 50);
    uiFsm.start();

    unsigned long long time = miosix::getTick();

    while(1)
    {
        miosix::ledOn();
        printf("%.3f,%.3f,%.3f,%d,%d,%d\n", bjState.ctSetPoint, bjState.levelNorm,
                                            bjState.ctOutput,   bjState.levelRaw,
                                            bjState.zeroLevel,  bjState.maxLevel);
        miosix::ledOff();

        // Sample at 20Hz
        time += 50;
        miosix::Thread::sleepUntil(time);
    }

    return 0;
}
