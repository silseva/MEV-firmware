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

const vector< string > BjMainPage::paramLabels = {"Set point",
                                                  "Level",
                                                  "Control",
                                                  "Mode"};

BjMainPage::BjMainPage(BjFsmData* fsm) : fsm(fsm)
{
    statusBox = make_unique< DisplayBox >(Point(0,0), fsm->dc.getWidth(),
                                          sbHeight, leftMargin,
                                          fsm->dc.getWidth()/2, paramLabels,
                                          sbBgColor, sbLabColor, droid21b);

    unsigned int btnWidth = (fsm->dc.getWidth() - (2*leftMargin + btnSpace))/2;
    Point manStart(leftMargin, fsm->dc.getHeight()/2);
    man = make_unique< Button >(manStart, btnWidth, btnHeight, "Manual",
                                droid21);

    Point autoStart(man->getLowerRightCorner().x() + btnSpace,
                    man->getUpperLeftCorner().y());
    aut = make_unique< Button >(autoStart, btnWidth, btnHeight, "Auto", droid21);

    Point spStart(man->getUpperLeftCorner().x(),
                  man->getLowerRightCorner().y() + btnSpace);
    set = make_unique< Button >(spStart, btnWidth, btnHeight, "Set", droid21);
}

BjMainPage::~BjMainPage() { }

void BjMainPage::enter()
{
    fsm->dc.clear(uiBkgColor);
}

FsmState *BjMainPage::update()
{
    Event event = InputHandler::instance().popEvent();
    bool manPressed = man->handleTouchEvent(event);
    bool autPressed = aut->handleTouchEvent(event);
    bool setPressed = set->handleTouchEvent(event);
    man->draw(fsm->dc);
    aut->draw(fsm->dc);
    set->draw(fsm->dc);

    statusBox->setEntryValue(0, "99",   black);
    statusBox->setEntryValue(1, "100",  black);
    statusBox->setEntryValue(2, "30%",  black);
    statusBox->setEntryValue(3, "AUTO", green);
    statusBox->draw(fsm->dc);

    if(manPressed) return &fsm->confirmMan;
    if(autPressed) return &fsm->confirmAut;
    if(setPressed) return &fsm->inputSp;
    return nullptr;
}

void BjMainPage::leave()
{
    // This has just an aestethic function, to avoid having the buttons looking
    // as "pressed" when re-entering in this state.
    Event event(EventType::TouchUp);
    man->handleTouchEvent(event);
    aut->handleTouchEvent(event);
    set->handleTouchEvent(event);
    man->draw(fsm->dc);
    aut->draw(fsm->dc);
    set->draw(fsm->dc);
}
