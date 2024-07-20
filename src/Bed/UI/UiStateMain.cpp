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

#include <mxgui/level2/input.h>
#include <miosix.h>
#include "UiStateMain.h"
#include "UiFsmData.h"

using namespace mxgui;
using namespace std;

const vector< string > BedMainPage::paramLabels = {"Pressure",
                                                   "Volume",
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

    setup = make_unique< Button >
    (Point(enable->getUpperLeftCorner().x(),
           enable->getLowerRightCorner().y() + btnSpace),
     btnWidth, btnHeight, "Setup", droid21);

    calib = make_unique< Button >
    (Point(disable->getUpperLeftCorner().x(),
           disable->getLowerRightCorner().y() + btnSpace),
     btnWidth, btnHeight, "Calib", droid21);
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
    bool setPressed = setup->handleTouchEvent(event);
    bool calPressed = calib->handleTouchEvent(event);

    enable->draw(fsm->dc);
    disable->draw(fsm->dc);
    setup->draw(fsm->dc);
    calib->draw(fsm->dc);

    // Update pressure indicator
    char text[50] = {0};
    snprintf(text, sizeof(text), "%03.1f", fsm->state.press_1);
    statusBox->setEntryValue(0, text,  black);

    // Update flow rate indicator
    snprintf(text, sizeof(text), "%02.2f  %02.2f", fsm->state.volume_1,
                                                   fsm->state.volume_2);
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
    if(calPressed) return &fsm->calSensors;
    if(setPressed) return &fsm->setup;

    return nullptr;
}

void BedMainPage::leave()
{
    // This has just an aestethic function, to avoid having the buttons looking
    // as "pressed" when re-entering in this state.
    Event event(EventType::TouchUp);
    enable->handleTouchEvent(event);
    disable->handleTouchEvent(event);
    setup->handleTouchEvent(event);
    calib->handleTouchEvent(event);

    enable->draw(fsm->dc);
    disable->draw(fsm->dc);
    setup->draw(fsm->dc);
    calib->draw(fsm->dc);
}
