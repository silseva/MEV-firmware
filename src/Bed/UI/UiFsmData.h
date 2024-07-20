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

#pragma once

#include <mxgui/display.h>
#include <limits>
#include "UiStateCalSensors.h"
#include "UiStateInputValue.h"
#include "UiStateSetup.h"
#include "UiStateMain.h"
#include "../BedState.h"

class BedFsmData
{
public:

    BedFsmData(StateData& state) :
                  dc(mxgui::DisplayManager::instance().getDisplay()),
                  kbInput(std::numeric_limits< float >::quiet_NaN()),
                  mainPage(this), inputVal(this), calSensors(this), setup(this),
                  state(state) {}

    mxgui::DrawingContext dc;
    float kbInput;
    FsmState *prevState;

    BedMainPage     mainPage;
    BedInputValue   inputVal;
    BedCalibSensors calSensors;
    BedSetupPage    setup;
    StateData&      state;
};
