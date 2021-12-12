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
#include "UiStateMain.h"
#include "UiFsmData.h"

using namespace mxgui;
using namespace std;

const vector< string > BedMainPage::paramLabels = {"Pressure",
                                                   "Flow",
                                                   "Ti/Ratio",
                                                   "Status"};

BedMainPage::BedMainPage(BedFsmData* fsm) : fsm(fsm)
{
    statusBox = make_unique< DisplayBox >(Point(0,0), fsm->dc.getWidth(),
                                          sbHeight, leftMargin,
                                          fsm->dc.getWidth()/2 - 20, paramLabels,
                                          sbBgColor, sbLabColor, droid21b);

    unsigned int btnWidth = (fsm->dc.getWidth() - (2*leftMargin + btnSpace))/2;

    enable = make_unique< Button >
    (Point(leftMargin,
           statusBox->getLowerRightCorner().y() + btnSpace),
     btnWidth, btnHeight, "Enable", droid21);

    disable = make_unique< Button >
    (Point(enable->getLowerRightCorner().x() + btnSpace,
           enable->getUpperLeftCorner().y()),
     btnWidth, btnHeight, "Disable", droid21);

    setTin = make_unique< Button >
    (Point(enable->getUpperLeftCorner().x(),
           enable->getLowerRightCorner().y() + btnSpace),
     btnWidth, btnHeight, "Set T_ins", droid21);

    setIE = make_unique< Button >
    (Point(disable->getUpperLeftCorner().x(),
           disable->getLowerRightCorner().y() + btnSpace),
     btnWidth, btnHeight, "Set I/E", droid21);

    setFs = make_unique< Button >
    (Point(setTin->getUpperLeftCorner().x(),
           setTin->getLowerRightCorner().y() + btnSpace),
     btnWidth, btnHeight, "Set F_s", droid21);
}

BedMainPage::~BedMainPage() { }

void BedMainPage::enter()
{
    fsm->dc.clear(uiBkgColor);
}

FsmState *BedMainPage::update()
{
    Event event = InputHandler::instance().popEvent();
    bool enaPressed = enable->handleTouchEvent(event);
    bool disPressed = disable->handleTouchEvent(event);
    bool tinPressed = setTin->handleTouchEvent(event);
    bool ratPressed = setIE->handleTouchEvent(event);
    bool fsaPressed = setFs->handleTouchEvent(event);
    enable->draw(fsm->dc);
    disable->draw(fsm->dc);
    setTin->draw(fsm->dc);
    setIE->draw(fsm->dc);
    setFs->draw(fsm->dc);

    // Update pressure indicator
    char text[50] = {0};
    snprintf(text, sizeof(text), "%03.1f  %03.1f", fsm->state.press_1,
                                                   fsm->state.press_2);
    statusBox->setEntryValue(0, text,  black);

    // Update flow rate indicator
    snprintf(text, sizeof(text), "%03.2f  %03.2f", fsm->state.flow_1,
                                                   fsm->state.flow_2);
    statusBox->setEntryValue(1, text,  black);

    // Update Ti/Ratio indicator
    snprintf(text, sizeof(text), "%.1f / 1:%.2f", fsm->state.tIns,
                                                  fsm->state.IE);
    statusBox->setEntryValue(2, text,  black);

    // Update operating status indicator
    if(fsm->state.enabled)
        statusBox->setEntryValue(3, "RUNNING", green);
    else
        statusBox->setEntryValue(3, "STOPPED", red);

    statusBox->draw(fsm->dc);

    // Enable/disable
    if(enaPressed) fsm->state.enabled = true;
    if(disPressed) fsm->state.enabled = false;

    // Handle Ti input request
    if(tinPressed)
    {
        fsm->state.set_tIns = true;
        return &fsm->inputVal;
    }

    // Handle I/E ratio input request
    if(ratPressed)
    {
        fsm->state.set_ratio = true;
        return &fsm->inputVal;
    }

    // Handle sample frequency input request
    if(fsaPressed)
    {
        fsm->state.set_fsample = true;
        return &fsm->inputVal;
    }

    return nullptr;
}

void BedMainPage::leave()
{
    // This has just an aestethic function, to avoid having the buttons looking
    // as "pressed" when re-entering in this state.
    Event event(EventType::TouchUp);
    enable->handleTouchEvent(event);
    disable->handleTouchEvent(event);
    setTin->handleTouchEvent(event);
    setIE->handleTouchEvent(event);
    setFs->handleTouchEvent(event);

    enable->draw(fsm->dc);
    disable->draw(fsm->dc);
    setTin->draw(fsm->dc);
    setIE->draw(fsm->dc);
    setFs->draw(fsm->dc);
}
