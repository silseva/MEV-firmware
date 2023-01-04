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
#include "UiStateInputVal.h"
#include "UiFsmData.h"

using namespace mxgui;
using namespace std;

BjInputValue::BjInputValue(BjFsmData* fsm) : fsm(fsm)
{
    int kb_x = (fsm->dc.getWidth()  - Keypad::getWidth())/2;
    int kb_y = (fsm->dc.getHeight() - Keypad::getHeight())/2;
    kb = make_unique< Keypad >(kb_x, kb_y);
}

BjInputValue::~BjInputValue()
{

}

void BjInputValue::enter()
{
    fsm->dc.clear(lightGrey);
    kb->clear();
    kb->draw(fsm->dc);
}

FsmState *BjInputValue::update()
{
    Event     event    = InputHandler::instance().popEvent();
    FsmState *nxtState = nullptr;

    if(kb->handleEvent(event, fsm->dc))
    {
        fsm->kbInput = kb->getNumber();
        nxtState     = fsm->prevState;

        if(fsm->kbInput != numeric_limits< float >::quiet_NaN())
        {
            if(fsm->prevState == (&fsm->mainPage))
            {
                nxtState = &fsm->confirmSp;
            }
        }
    }

    return nxtState;
}

void BjInputValue::leave()
{

}
