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

#include <memory>
#include "UiStateConfMan.h"
#include "UiFsmData.h"

using namespace mxgui;
using namespace std;

BjConfirmMan::BjConfirmMan(BjFsmData* fsm) : fsm(fsm)
{
    int cBox_x = (fsm->dc.getWidth()  - ConfirmBox::getWidth())/2;
    int cBox_y = (fsm->dc.getHeight() - ConfirmBox::getHeight())/2;
    cBox = make_unique< ConfirmBox >(cBox_x, cBox_y);
}

BjConfirmMan::~BjConfirmMan() { }

void BjConfirmMan::enter()
{
    fsm->dc.clear(lightGrey);
    cBox->draw(fsm->dc, "Switch to\nmanual mode?");
}

FsmState *BjConfirmMan::update()
{
    Event event = InputHandler::instance().popEvent();
    if(cBox->handleEvent(event, fsm->dc))
    {
        if(cBox->confirmed())
        {
            bjState.ctMode = CtrlMode::MAN;
        }

        return &fsm->mainPage;
    }

    return nullptr;
}

void BjConfirmMan::leave() { }
