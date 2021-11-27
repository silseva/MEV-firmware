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

#pragma once

#include <mxgui/display.h>
#include <limits>
#include "UiStateMain.h"
#include "UiStateInputValue.h"
#include "../BedState.h"

class BedFsmData
{
public:

    BedFsmData(StateData& state) :
                  dc(mxgui::DisplayManager::instance().getDisplay()),
                  mainPage(this), inputVal(this), state(state) {}

    mxgui::DrawingContext dc;

    BedMainPage   mainPage;
    BedInputValue inputVal;
    StateData&    state;
};