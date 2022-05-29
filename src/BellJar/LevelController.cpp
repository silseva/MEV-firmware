/*
 * MEV board firmware
 * Copyright (C) 2022  Silvano Seva silvano.seva@polimi.it
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
#include <cassert>
#include "LevelController.h"

using namespace std;
using namespace miosix;

LevelController::LevelController() : adc(ADC122S021::instance()),
                                     blower(Blower::instance()),
                                     pid(bjState.ctParams), rMode(bjState.ctMode)
{

}

LevelController::~LevelController()
{

}

void LevelController::run()
{
    unsigned long long time = getTick();

    while(1)
    {
        updateMeasurements();

        // Handle switching between man/auto operating mode
        if(rMode != bjState.ctMode)
        {
            switch(bjState.ctMode)
            {
                case CtrlMode::MAN:
                    pid.enableTracking();
                    break;

                case CtrlMode::AUTO:
                    pid.disableTracking();
                    break;

                default:
                    // UH-OH!
                    assert(false);
                    break;
            }

            rMode = bjState.ctMode;
        }

        // Update tracking output, if in manual mode
        if(bjState.ctMode == CtrlMode::MAN)
            pid.setTrackingOutput(bjState.manOutput);

        // Controller step
        bjState.ctOutput = pid.computeAction(bjState.ctSetPoint,
                                             bjState.currLevelNorm);

        // Update actuation
        blower.setValue(bjState.ctOutput);

        time += static_cast< uint32_t >(bjState.ctParams.Tsample * 1000.0f);
        Thread::sleepUntil(time);
    }
}

void LevelController::updateMeasurements()
{
    uint16_t level = adc.getRawValue(AdcChannel::_1);

    // Bad measurement, return. Allowed range is 0 - 0x0FFF
    if(level > 0x1000) return;

    bjState.currLevelRaw = level;

    if(level < bjState.levelZero)
    {
        bjState.currLevelAdj  = 0;
        bjState.currLevelNorm = 0.0f;
    }
    else
    {
        // Adust to zero level
        bjState.currLevelAdj  = bjState.currLevelRaw
                                - bjState.levelZero;

        // Normalise to range 0.0 - 1.0
        float levelNorm = static_cast< float >(bjState.currLevelAdj)
                        / static_cast< float >(0x0FFF - bjState.levelZero);
        bjState.currLevelNorm = std::min(levelNorm, 1.0f);
    }
}

