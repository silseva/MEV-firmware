/***************************************************************************
 *   Copyright (C) 2021 by Silvano Seva                                    *
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

#include <misc_inst.h>
#include <cstring>
#include <image.h>
#include <vector>
#include <string>
#include "icons/warning_50x50.h"
#include "Rectangle.h"
#include "ConfirmBox.h"

using namespace std;
using namespace mxgui;

ConfirmBox::ConfirmBox (const unsigned int x, const unsigned int y) :
                        ConfirmBox(Point(x, y)) { }

ConfirmBox::ConfirmBox (const mxgui::Point p) : startPoint(p),
    btnYes(Point(p.x() + btnYes_xOff,
                 p.y() + btnY), btnWidth, btnHeight, "YES", droid21),
    btnNo(Point(p.x() + btnNo_xOff,
                p.y() + btnY), btnWidth, btnHeight, "NO",  droid21),
   yesPressed(false)
{
}

void ConfirmBox::draw(mxgui::DrawingContext& dc, const std::string& line)
{

    // Borders and grey background
    ShadowRectangle borders(startPoint, totalWidth, totalHeigth,
                            make_pair(lightGrey, darkGrey));

    Point fillStart(startPoint.x() + 1, startPoint.y() + 1);
    Point fillStop (fillStart.x()  + totalWidth  - 1,
                    fillStart.y()  + totalHeigth - 1);
    dc.clear(fillStart, fillStop, grey);
    borders.draw(dc);

    // Warning icon
    int xPos = startPoint.x() + (btnYes_xOff - iconSize)/2;
    int yPos = startPoint.y() + spacing;
    dc.drawImage(Point(xPos, yPos), warning_50x50);

    // Message text, adjust vertically and split in two lines if a newline
    // is present
    string::size_type newLinePos = line.find('\n');
    int numLines = 1;
    if(newLinePos != string::npos) numLines += 1;
    int textSpace = totalHeigth - spacing - btnHeight;
    int xOffset   = btnYes_xOff;
    int yOffset   = (textSpace - (numLines * droid21.getHeight() +
                                 (numLines - 1) * spacing))/2;

    int x = startPoint.x() + xOffset;
    int y = startPoint.y() + yOffset;

    dc.setFont(droid21);
    dc.setTextColor(black, grey);

    // First line
    string firstLine = line.substr(0, newLinePos);
    int lineLength   = droid21.calculateLength(firstLine.c_str());
    int lineXOffset  = (totalWidth - xOffset - lineLength)/2;
    dc.write(Point(x, y), firstLine.c_str());

    // Eventual second line
    if(numLines == 2)
    {
        string secondLine = line.substr(newLinePos+1);
        lineLength   = droid21.calculateLength(secondLine.c_str());
        lineXOffset  = (totalWidth - xOffset - lineLength)/2;
        y += spacing + droid21.getHeight();
        dc.write(Point(x, y), secondLine.c_str());
    }

    // "Yes" and "No" buttons
    btnYes.draw(dc);
    btnNo.draw(dc);
}

bool ConfirmBox::handleEvent(mxgui::Event& e, mxgui::DrawingContext& dc)
{
    yesPressed = btnYes.handleTouchEvent(e);
    btnYes.draw(dc);
    bool noPressed = btnNo.handleTouchEvent(e);
    btnNo.draw(dc);

    return yesPressed || noPressed;
}
