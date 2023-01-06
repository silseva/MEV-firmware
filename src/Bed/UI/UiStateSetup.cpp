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

#include <mxgui/level2/input.h>
#include <miosix.h>
#include "UiStateSetup.h"
#include "UiFsmData.h"

using namespace mxgui;
using namespace std;


BedSetupPage::BedSetupPage(BedFsmData* fsm) : inputSource(KbInputSource::NONE),
                                              fsm(fsm)
{
    unsigned int btnWidth = (fsm->dc.getWidth() - (2*leftMargin + btnSpace))/2;
    unsigned int bx       = leftMargin;
    unsigned int by       = leftMargin;

    setTin = make_unique< Button > (Point(bx, by), btnWidth, btnHeight,
                                    "Set T_ins", droid21);

    by = setTin->getLowerRightCorner().y() + btnSpace;
    setIE = make_unique< Button > (Point(bx, by), btnWidth, btnHeight,
                                   "Set I/E", droid21);

    by = setIE->getLowerRightCorner().y() + btnSpace;
    setFs = make_unique< Button > (Point(bx, by), btnWidth, btnHeight,
                                   "Set F_s", droid21);

    by = fsm->dc.getHeight() - leftMargin - btnHeight;
    ret = make_unique < Button > (Point(bx, by), btnWidth, btnHeight,
                                   "Back", droid21);
}

BedSetupPage::~BedSetupPage() { }

void BedSetupPage::enter()
{
    fsm->dc.clear(uiBkgColor);
}

FsmState *BedSetupPage::update()
{
    if(fsm->kbInput != numeric_limits< float >::quiet_NaN())
    {
        switch(inputSource)
        {
            case KbInputSource::T_INS:
                fsm->state.tIns = fsm->kbInput;
                break;

            case KbInputSource::RATIO:
                fsm->state.IE = fsm->kbInput;
                break;

            case KbInputSource::FSAMP:
                fsm->state.Fsample = fsm->kbInput;
                break;

            default:
                break;
        }
    }

    inputSource = KbInputSource::NONE;
    fsm->kbInput  = numeric_limits< float >::quiet_NaN();

    Event event = InputHandler::instance().popEvent();
    bool tinPressed = setTin->handleTouchEvent(event);
    bool ratPressed = setIE->handleTouchEvent(event);
    bool fsaPressed = setFs->handleTouchEvent(event);
    bool retPressed = ret->handleTouchEvent(event);

    setTin->draw(fsm->dc);
    setIE->draw(fsm->dc);
    setFs->draw(fsm->dc);
    ret->draw(fsm->dc);

    // Handle Ti input request
    if(tinPressed)
    {
        inputSource    = KbInputSource::T_INS;
        fsm->prevState = this;
        return &fsm->inputVal;
    }

    // Handle I/E ratio input request
    if(ratPressed)
    {
        inputSource    = KbInputSource::RATIO;
        fsm->prevState = this;
        return &fsm->inputVal;
    }

    // Handle sample frequency input request
    if(fsaPressed)
    {
        inputSource    = KbInputSource::FSAMP;
        fsm->prevState = this;
        return &fsm->inputVal;
    }

    if(retPressed) return &fsm->mainPage;

    return nullptr;
}

void BedSetupPage::leave()
{

}
