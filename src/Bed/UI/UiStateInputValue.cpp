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
#include "UiStateInputValue.h"
#include "UiFsmData.h"

using namespace mxgui;
using namespace std;

BedInputValue::BedInputValue(BedFsmData* fsm) : fsm(fsm)
{
    int kb_x = (fsm->dc.getWidth()  - Keypad::getWidth())/2;
    int kb_y = (fsm->dc.getHeight() - Keypad::getHeight())/2;
    kb = make_unique< Keypad >(kb_x, kb_y);
}

BedInputValue::~BedInputValue(){ }

void BedInputValue::enter()
{
    fsm->dc.clear(lightGrey);
    kb->clear();
    kb->draw(fsm->dc);
}

FsmState *BedInputValue::update()
{
    Event event = InputHandler::instance().popEvent();
    if(kb->handleEvent(event, fsm->dc))
    {
        float value = kb->getNumber();
        if(value != numeric_limits< float >::quiet_NaN())
        {
            value = max(0.0f, value);

            if(fsm->state.set_tIns)
            {
                fsm->state.tIns = value;
                fsm->state.set_tIns = false;
            }

            if(fsm->state.set_ratio)
            {
                fsm->state.IE = value;
                fsm->state.set_ratio = false;
            }

            if(fsm->state.set_fsample)
            {
                fsm->state.Fsample = value;
                fsm->state.set_fsample = false;
            }

            return &fsm->mainPage;
        }
    }

    return nullptr;
}

void BedInputValue::leave()
{
}
