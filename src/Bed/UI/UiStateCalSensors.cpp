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
    back = make_unique< Button >(Point(bx, by), bWidth, btnHeight, "Back",
                                 droid21);

    bx = fsm->dc.getWidth() - 10 - bWidth;
    reset = make_unique< Button >(Point(bx, by), bWidth, btnHeight, "Reset",
                                  droid21);

    unsigned int space = back->getUpperLeftCorner().y() - 110;
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

    writeLine(0, "P1", state.press1_raw, state.press_1);
    writeLine(1, "P2", state.press2_raw, state.press_2);
    writeLine(2, "F1", state.flow1_raw,  state.flow_1);
    writeLine(3, "F2", state.flow2_raw,  state.flow_2);

    unsigned int txtOffset = (btnHeight - droid21.getHeight()) / 2;
    Point txt1(10, zero[0]->getUpperLeftCorner().y() + txtOffset);
    fsm->dc.write(txt1, "Set zero");

    Point txt2(10, max[0]->getUpperLeftCorner().y() + txtOffset);
    fsm->dc.write(txt2, "Set max");

    Event     event    = InputHandler::instance().popEvent();
    FsmState *nxtState = nullptr;
    bool     updateCal = false;

    // Handle sensors' zeroing
    for(int i = 0; i < 4; i++)
    {
        if(zero[i]->handleTouchEvent(event))
        {
            updateCal = true;

            switch(i)
            {
                case 0:     // Pressure 1
                    state.cal.pressSens[0].offset = state.press1_out;
                    break;

                case 1:     // Pressure 2
                    state.cal.pressSens[1].offset = state.press2_out;
                    break;

                case 2:     // Flow rate 1
                    state.cal.flowSens[0].offset = state.flow1_out;
                    break;

                case 3:     // Flow rate 2
                    state.cal.flowSens[1].offset = state.flow2_out;
                    break;
            }
        }

        zero[i]->draw(fsm->dc);
    }

    // Handle sensors' slope calibration
    for(int i = 0; i < 4; i++)
    {
        float output;

        if(max[i]->handleTouchEvent(event))
        {
            updateCal = true;

            switch(i)
            {
                case 0:     // Pressure 1, FS 10kPa
                    output = state.press1_out
                           - state.cal.pressSens[0].offset;
                    state.cal.pressSens[0].slope = 10000.0f / output;
                    break;

                case 1:     // Pressure 2, FS 10kPa
                    output = state.press2_out
                           - state.cal.pressSens[1].offset;
                    state.cal.pressSens[1].slope = 10000.0f / output;
                    break;

                case 2:     // Flow rate 1, FS 100SLPM
                    output = state.flow1_out
                           - state.cal.flowSens[0].offset;
                    state.cal.flowSens[0].slope = 100.0f / output;
                    break;

                case 3:     // Flow rate 2, FS 100SLPM
                    output = state.flow2_out
                           - state.cal.flowSens[1].offset;
                    state.cal.flowSens[1].slope = 100.0f / output;
                    break;
            }
        }

        max[i]->draw(fsm->dc);
    }

    // Go back to previous page?
    if(back->handleTouchEvent(event))
            nxtState = &fsm->mainPage;

    // Reset calibrations?
    if(reset->handleTouchEvent(event))
    {
        state.cal.loadDefaultValues();
        updateCal = true;
    }

    if(updateCal)
    {
        AnalogSensors::instance().applyCalibration(state.cal);
    }

    // Draw buttons
    back->draw(fsm->dc);
    reset->draw(fsm->dc);

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
