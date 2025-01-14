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
#include "UiStateCalSensors.h"
#include "UiFsmData.h"
#include "common/Persistence.h"
#include "Bed/AnalogSensors.h"

using namespace mxgui;
using namespace std;

BedCalibSensors::BedCalibSensors(BedFsmData* fsm) : sensorToUpdate(-1), fsm(fsm)
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
    bx     = fsm->dc.getWidth() - 5 - 3*50;
    by     = 110 + (space / 2);

    char name[3] = "P1";
    for(int i = 0; i < 3; i++)
    {
        if(i >= 1)
        {
            name[0] = 'F';
            name[1] = '1' + (i / 2);
        }

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
    writeLine(1, "F1", state.flow1_raw,  state.flow_1);
    writeLine(2, "F2", state.flow2_raw,  state.flow_2);

    unsigned int txtOffset = (btnHeight - droid21.getHeight()) / 2;
    Point txt1(10, zero[0]->getUpperLeftCorner().y() + txtOffset);
    fsm->dc.write(txt1, "Set zero");

    Point txt2(10, max[0]->getUpperLeftCorner().y() + txtOffset);
    fsm->dc.write(txt2, "Set max");

    Event     event    = InputHandler::instance().popEvent();
    FsmState *nxtState = nullptr;
    bool     updateCal = false;

    // Update flow sensor calibration with user input
    if(sensorToUpdate >= 0)
    {
        float fullScale = fsm->kbInput;
        float output    = 0.0f;

        switch(sensorToUpdate)
        {
            case 1:
                output = state.flow1_out
                       - state.cal.flowSens[0].offset;
                state.cal.flowSens[0].slope = fullScale / output;
                break;

            case 2:
                output = state.flow2_out
                       - state.cal.flowSens[1].offset;
                state.cal.flowSens[1].slope = fullScale / output;
                break;

            default:
                break;
        }

        updateCal      = true;
        sensorToUpdate = -1;
        fsm->kbInput   = numeric_limits< float >::quiet_NaN();
    }

    // Handle sensors' zeroing
    for(int i = 0; i < 3; i++)
    {
        if(zero[i]->handleTouchEvent(event))
        {
            updateCal = true;

            switch(i)
            {
                case 0:     // Pressure 1
                    state.cal.pressSens[0].offset = state.press1_out;
                    break;

                case 1:     // Flow rate 1
                    state.cal.flowSens[0].offset = state.flow1_out;
                    break;

                case 2:     // Flow rate 2
                    state.cal.flowSens[1].offset = state.flow2_out;
                    break;
            }
        }

        zero[i]->draw(fsm->dc);
    }

    // Handle pressure sensor slope calibration
    if(max[0]->handleTouchEvent(event))
    {
        float output = state.press1_out - state.cal.pressSens[0].offset;
        state.cal.pressSens[0].slope = 10000.0f / output;
        updateCal = true;
    }
    max[0]->draw(fsm->dc);

    // Handle flow sensor slope calibration
    for(int i = 1; i < 3; i++)
    {
        if(max[i]->handleTouchEvent(event))
            sensorToUpdate = i;

        max[i]->draw(fsm->dc);
    }

    // Go to user input?
    if(sensorToUpdate >= 0)
    {
        fsm->prevState = this;
        nxtState = &fsm->inputVal;
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
    saveDataToFlash(&(state.cal), sizeof(SensorCalibration));
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
