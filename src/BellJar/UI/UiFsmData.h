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

#include <limits>
#include <mxgui/display.h>
#include "BellJar/BjState.h"
#include "UiStateMain.h"
#include "UiStateInputVal.h"
#include "UiStateConfAut.h"
#include "UiStateConfMan.h"
#include "UiStateConfSp.h"
#include "UiStateConfigPid.h"
#include "UiStateConfigInput.h"

class BjFsmData
{
public:

    BjFsmData() : dc(mxgui::DisplayManager::instance().getDisplay()),
                  kbInput(std::numeric_limits< float >::quiet_NaN()),
                  mainPage(this),   inputVal(this),   confirmAut(this),
                  confirmMan(this), confirmSp(this), setupPid(this),
                  setupInput(this) { }

    mxgui::DrawingContext dc;
    float kbInput;
    FsmState *prevState;

    BjMainPage    mainPage;
    BjInputValue  inputVal;
    BjConfirmAut  confirmAut;
    BjConfirmMan  confirmMan;
    BjConfirmSp   confirmSp;
    BjConfigPid   setupPid;
    BjConfigInput setupInput;
};
