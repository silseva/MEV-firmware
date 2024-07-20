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

#include <array>
#include <memory>
#include "UiStateConfigInput.h"
#include "UiFsmData.h"

using namespace mxgui;
using namespace std;

const vector< string > BjConfigInput::labels =
{
    "Raw level",
    "Norm. level",
    "Zero",
    "Max"
};

BjConfigInput::BjConfigInput(BjFsmData* fsm) : fsm(fsm)
{
    levels = make_unique< DisplayBox >(Point(0, 0), fsm->dc.getWidth(), 130, 10,
                                       fsm->dc.getWidth()/2, labels, lightGrey,
                                       black, droid21);

    unsigned int bWidth = (fsm->dc.getWidth() - (2*spacing + btnSpace))/2;
    unsigned int bx = spacing;
    unsigned int by = levels->getLowerRightCorner().y() + 10;

    zero = make_unique< Button >(Point(bx, by), bWidth, btnHeight, "Set zero",
                                 droid21);

    by  = fsm->dc.getHeight() - spacing - btnHeight;
    ret = make_unique< Button >(Point(bx, by), bWidth, btnHeight, "Back",
                                 droid21);

    bx  = fsm->dc.getWidth() - spacing - bWidth;
    nxt = make_unique< Button >(Point(bx, by), bWidth, btnHeight, "Next",
                                 droid21);

    by  = levels->getLowerRightCorner().y() + 10;
    max = make_unique< Button >(Point(bx, by), bWidth, btnHeight, "Set max",
                                 droid21);
}

BjConfigInput::~BjConfigInput()
{

}

void BjConfigInput::enter()
{

}

FsmState *BjConfigInput::update()
{
    fsm->dc.clear(lightGrey);

    char str[32];
    snprintf(str, sizeof(str), "%d", bjState.levelRaw);
    levels->setEntryValue(0, str, black);

    snprintf(str, sizeof(str), "%.3f", bjState.levelNorm);
    levels->setEntryValue(1, str, black);

    snprintf(str, sizeof(str), "%d", bjState.zeroLevel);
    levels->setEntryValue(2, str, black);

    snprintf(str, sizeof(str), "%d", bjState.maxLevel);
    levels->setEntryValue(3, str, black);

    // Finally, draw
    levels->draw(fsm->dc);

    Event     event    = InputHandler::instance().popEvent();
    FsmState *nxtState = nullptr;

    bool zeroPressed = zero->handleTouchEvent(event);
    bool maxPressed  = max->handleTouchEvent(event);
    bool retPressed  = ret->handleTouchEvent(event);
    bool nxtPressed  = nxt->handleTouchEvent(event);

    zero->draw(fsm->dc);
    max->draw(fsm->dc);
    ret->draw(fsm->dc);
    nxt->draw(fsm->dc);

    if(zeroPressed) bjState.zeroLevel = bjState.levelRaw;
    if(maxPressed)  bjState.maxLevel  = bjState.levelRaw;
    if(retPressed)  nxtState = &fsm->mainPage;
    if(nxtPressed)  nxtState = &fsm->setupPid;

    return nxtState;
}

void BjConfigInput::leave()
{

}
