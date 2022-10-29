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
#include "UiStateConfSp.h"
#include "UiFsmData.h"

using namespace mxgui;
using namespace std;

BjConfirmSp::BjConfirmSp(BjFsmData* fsm) : fsm(fsm)
{
    int cBox_x = (fsm->dc.getWidth()  - ConfirmBox::getWidth())/2;
    int cBox_y = (fsm->dc.getHeight() - ConfirmBox::getHeight())/2;
    cBox = make_unique< ConfirmBox >(cBox_x, cBox_y);
}

BjConfirmSp::~BjConfirmSp() { }

void BjConfirmSp::enter()
{
    fsm->dc.clear(lightGrey);

    std::string text(20, '\0');
    snprintf(&text[0], text.size(), "Confirm\n%s = %.1f?",
             (bjState.ctMode == CtrlMode::MAN) ? "OUT" : "SP",
             fsm->kbInput);

    cBox->draw(fsm->dc, text);
}

FsmState *BjConfirmSp::update()
{
    Event event = InputHandler::instance().popEvent();
    if(cBox->handleEvent(event, fsm->dc))
    {
        bool isValid =  (fsm->kbInput != numeric_limits< float >::quiet_NaN())
                     && (fsm->kbInput <= 100.0f)
                     && (fsm->kbInput >= 0.0f);

        if(cBox->confirmed() && isValid)
        {
            // Set-point range is 0.0 - 1.0
            float sp = fsm->kbInput / 100.0f;

            if(bjState.ctMode == CtrlMode::MAN)
            {
                bjState.manOutput = sp;
            }
            else
            {
                bjState.ctSetPoint = sp;
            }
        }

        fsm->kbInput   = numeric_limits< float >::quiet_NaN();
        fsm->prevState = nullptr;

        return &fsm->mainPage;
    }

    return nullptr;
}

void BjConfirmSp::leave() { }
