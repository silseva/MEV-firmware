/*
 * MEV board firmware
 * Copyright (C) 2022  Silvano Seva silvano.seva@polimi.it
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
#include "UiStateConfig.h"
#include "UiFsmData.h"

using namespace mxgui;
using namespace std;


CfgEntry::CfgEntry(const mxgui::Point startPoint, const std::string& label,
                   float& refValue) : startPoint(startPoint), label(label),
                   value(refValue)
{

}

CfgEntry::~CfgEntry()
{

}

bool CfgEntry::update(mxgui::Event& e, mxgui::DrawingContext& dc)
{
    int btnOffset = dc.getWidth()
                   - spacing
                   - btnWidth;

    Point btnPoint(startPoint.x() + btnOffset, startPoint.y());
    Point lblPoint(startPoint.x() + spacing,
                   startPoint.y() + (droid21.getHeight() / 2));
    Point txtPoint(lblPoint.x() + 3*spacing, lblPoint.y());

    char text[32];
    snprintf(text, sizeof(text), "%.3f", value);

    dc.setTextColor(black, lightGrey);
    dc.write(lblPoint, label.c_str());
    dc.write(txtPoint, text);

    Button bt(btnPoint, btnWidth, btnHeight, "Set", droid21);
    bool pressed = bt.handleTouchEvent(e);
    bt.draw(dc);

    return pressed;
}


BjConfig::BjConfig(BjFsmData* fsm) : valueToChange(-1), fsm(fsm)
{
    int x = 0;
    int y = spacing;
    entries.push_back( make_unique < CfgEntry >(Point(x, y), "Kp",
                                                bjState.ctParams.k));
    y += 40;
    entries.push_back( make_unique < CfgEntry >(Point(x, y), "Ti",
                                                bjState.ctParams.Ti));
    y += 40;
    entries.push_back( make_unique < CfgEntry >(Point(x, y), "Td",
                                                bjState.ctParams.Td));
    y += 40;
    entries.push_back( make_unique < CfgEntry >(Point(x, y), "N",
                                                bjState.ctParams.N));
    y += 40;
    entries.push_back( make_unique < CfgEntry >(Point(x, y), "Ts",
                                                bjState.ctParams.Tsample));
    entries.shrink_to_fit();

    unsigned int bWidth = (fsm->dc.getWidth() - (2*spacing + btnSpace))/2;
    unsigned int bx = spacing;// fsm->dc.getWidth()  - spacing - bWidth;
    unsigned int by = y + 60;

    zero = make_unique< Button >(Point(bx, by), bWidth, btnHeight, "Zero",
                                droid21);

    unsigned int offset = fsm->dc.getWidth() - 2*bWidth - 2*spacing;
    bx = zero->getLowerRightCorner().x() + offset;
    max = make_unique< Button >(Point(bx, by), bWidth,
                                btnHeight, "Max", droid21);

    by = fsm->dc.getHeight() - spacing - btnHeight;
    ret  = make_unique< Button >(Point(bx, by), bWidth, btnHeight, "Back",
                                 droid21);
}

BjConfig::~BjConfig()
{

}

void BjConfig::enter()
{
    fsm->dc.clear(lightGrey);
}

FsmState *BjConfig::update()
{
    if(valueToChange >= 0)
    {
        switch(valueToChange)
        {
            case 0:
                bjState.ctParams.k = fsm->kbInput;
                break;

            case 1:
                bjState.ctParams.Ti = fsm->kbInput;
                break;

            case 2:
                bjState.ctParams.Td = fsm->kbInput;
                break;

            case 3:
                bjState.ctParams.N = fsm->kbInput;
                break;

            case 4:
                bjState.ctParams.Tsample = fsm->kbInput;
                break;

            default:
                break;
        }

        valueToChange = -1;
        fsm->kbInput  = numeric_limits< float >::quiet_NaN();
    }

    Event     event    = InputHandler::instance().popEvent();
    FsmState *nxtState = nullptr;

    for(size_t i = 0; i < entries.size(); i++)
    {
        bool pressed = entries[i]->update(event, fsm->dc);
        if(pressed) valueToChange = i;
    }

    if(valueToChange >= 0)
    {
        fsm->prevState = this;
        nxtState = &fsm->inputVal;
    }

    bool retPressed  = ret->handleTouchEvent(event);
    bool maxPressed  = max->handleTouchEvent(event);
    bool zeroPressed = zero->handleTouchEvent(event);
    ret->draw(fsm->dc);
    max->draw(fsm->dc);
    zero->draw(fsm->dc);

    if(retPressed)  nxtState = &fsm->mainPage;
    if(maxPressed)  bjState.maxLevel  = bjState.levelRaw;
    if(zeroPressed) bjState.zeroLevel = bjState.levelRaw;

    return nxtState;
}

void BjConfig::leave()
{

}
