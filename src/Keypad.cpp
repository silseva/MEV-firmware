/***************************************************************************
 *   Copyright (C) 2013 by Terraneo Federico                               *
 *                 2021 by Silvano Seva                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   As a special exception, if other files instantiate templates or use   *
 *   macros or inline functions from this file, or you compile this file   *
 *   and link it with other works to produce a work based on this file,    *
 *   this file does not by itself cause the resulting work to be covered   *
 *   by the GNU General Public License. However the source code for this   *
 *   file must still be made available in accordance with the GNU General  *
 *   Public License. This exception does not invalidate any other reasons  *
 *   why a work based on this file might be covered by the GNU General     *
 *   Public License.                                                       *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, see <http://www.gnu.org/licenses/>   *
 ***************************************************************************/

#include <Rectangle.h>
#include <misc_inst.h>
#include <cstring>
#include <limits>
#include "Keypad.h"

using namespace std;
using namespace mxgui;

Keypad::Keypad (const unsigned int x, const unsigned int y) :
                           Keypad (Point(x, y)) { }

Keypad::Keypad (const mxgui::Point p) :
      startPoint(p), inputPos(0), okPressed(0), dotPressed(false),
      textBox(Point(startPoint.x() + btnSpacing, startPoint.y() + btnSpacing),
              Point(startPoint.x() + btnSpacing + textBoxWidth,
                    startPoint.y() + btnSpacing + btnHeight),
                    white, black, droid21)
{

    // Starting coordinates for keyboard buttons
    int x = textBox.getUpperLeftCorner().x();
    int y = textBox.getLowerRightCorner().y() + btnSpacing;

    int pos = 0;
    for(int col = 0; col < 4; col++)
    {
        for(int row = 0; row < 3; row++)
        {
            Point p(x, y);
            Button *b = new Button(p, btnWidth, btnHeight, labels[pos], droid21);
            x = b->getLowerRightCorner().x() + btnSpacing;
            keyboard[pos] = b;
            pos++;
        }

        y += btnHeight + btnSpacing;
        x = 20;
    }

    // Finally, clear the input buffer
    memset(input, 0x00, sizeof(input));
}

void Keypad::draw(mxgui::DrawingContext& dc)
{

    ShadowRectangle borders(startPoint, totalWidth, totalHeigth,
                            make_pair(lightGrey, darkGrey));

    Point fillStart(startPoint.x() + 1, startPoint.y() + 1);
    Point fillStop (fillStart.x() + totalWidth  - 1,
                    fillStart.y() + totalHeigth - 1);
    dc.clear(fillStart, fillStop, grey);
    borders.draw(dc);
    textBox.draw(dc);
    for(uint8_t i = 0; i < 12; i++) keyboard[i]->draw(dc);
}

bool Keypad::handleEvent(mxgui::Event& e, mxgui::DrawingContext& dc)
{
    /*
     * Scan through the buttons to check if any of them has been pressed:
     * - when "OK" button is pressed, input text is disabled.
     * - once the dot button has been pressed, avoids adding two dots in the
     *   input string.
     * - text box is updated only when a button is pressed, to avoid flickering
     */
    for(int i = 0; i < 12; i++)
    {
        bool pressed = keyboard[i]->handleTouchEvent(e);

        bool doubleDot = dotPressed && (i == 9);
        if(pressed && (i == 11)) okPressed  = true;
        if(pressed && !okPressed && !doubleDot && (inputPos < sizeof(input)))
        {
            input[inputPos] = labels[i].at(0);
            inputPos++;
            if(i == 9) dotPressed = true;
        }

        keyboard[i]->draw(dc);

        if(pressed)
        {
            textBox.write(input);
            textBox.draw(dc);
        }
    }

    return okPressed;
}

void Keypad::clear()
{
    okPressed  = false;
    dotPressed = false;
    inputPos   = 0;
    memset(input, 0x00, sizeof(input));
    textBox.write(input);
}

float Keypad::getNumber()
{
    if(!okPressed) return numeric_limits< float >::quiet_NaN();
    return atof(input);
}
