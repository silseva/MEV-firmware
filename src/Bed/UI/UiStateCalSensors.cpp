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
#include "UiStateCalSensors.h"
#include "UiFsmData.h"
#include "Bed/AnalogSensors.h"

using namespace mxgui;
using namespace std;

BedCalibSensors::BedCalibSensors(BedFsmData* fsm) : fsm(fsm)
{
    unsigned int bWidth = (fsm->dc.getWidth() - (2*spacing + btnSpace))/2;
    unsigned int bx     = 10;
    unsigned int by     = fsm->dc.getHeight() - 10 - btnHeight;
    ret = make_unique< Button >(Point(bx, by), bWidth, btnHeight, "Back",
                                 droid21);

    unsigned int space = ret->getUpperLeftCorner().y() - 110;
    space -= 2*btnHeight + 10;
    bx     = fsm->dc.getWidth() - 5 - 4*50;
    by     = 110 + (space / 2);

    char name[3] = "P1";
    for(int i = 0; i < 4; i++)
    {
        if(i >= 2) name[0] = 'F';
        name[1] = '1' + (i % 2);

        zero[i] = make_unique< Button >(Point(bx, by), 40, btnHeight, name,
                                        droid21);

        max[i] = make_unique< Button >(Point(bx, by + btnHeight + 10), 40,
                                       btnHeight, name, droid21);
        bx += 50;
    }

}

BedCalibSensors::~BedCalibSensors()
{

}

void BedCalibSensors::enter()
{

}

FsmState *BedCalibSensors::update()
{
    fsm->dc.clear(lightGrey);
    fsm->dc.setTextColor(black, lightGrey);

    AnalogSensors& sens = AnalogSensors::instance();

    writeLine(0, "P1", sens.getRawValue(Sensor::PRESS_1), sens.getValue(Sensor::PRESS_1));
    writeLine(1, "P2", sens.getRawValue(Sensor::PRESS_2), sens.getValue(Sensor::PRESS_2));
    writeLine(2, "F1", sens.getRawValue(Sensor::FLOW_1),  sens.getValue(Sensor::FLOW_1));
    writeLine(3, "F2", sens.getRawValue(Sensor::FLOW_2),  sens.getValue(Sensor::FLOW_2));

    unsigned int txtOffset = (btnHeight - droid21.getHeight()) / 2;
    Point txt1(10, zero[0]->getUpperLeftCorner().y() + txtOffset);
    fsm->dc.write(txt1, "Set zero");

    Point txt2(10, max[0]->getUpperLeftCorner().y() + txtOffset);
    fsm->dc.write(txt2, "Set max");

    Event     event    = InputHandler::instance().popEvent();
    FsmState *nxtState = nullptr;

    for(int i = 0; i < 4; i++)
    {
        bool zeroPressed = zero[i]->handleTouchEvent(event);
        zero[i]->draw(fsm->dc);
        max[i]->draw(fsm->dc);
    }

    bool retPressed  = ret->handleTouchEvent(event);
    ret->draw(fsm->dc);

    if(retPressed)  nxtState = &fsm->mainPage;

    return nxtState;
}

void BedCalibSensors::leave()
{

}

void BedCalibSensors::writeLine(const int pos, const char* label,
                                const uint16_t raw, const float conv)
{
    char str[32];
    const int yPos = (droid21.getHeight() + 5) * pos;

    Point p1(10, 10 + yPos);
    snprintf(str, sizeof(str), "%s   raw: %d", label, raw);
    fsm->dc.write(p1, str);

    Point p2(p1.x() + (fsm->dc.getWidth() - 20)/2, p1.y());
    snprintf(str, sizeof(str), "conv: %.2f", conv);
    fsm->dc.write(p2, str);
}

