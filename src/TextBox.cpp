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

#include <misc_inst.h>
#include <Rectangle.h>
#include "TextBox.h"

using namespace std;
using namespace mxgui;

TextBox::TextBox(const mxgui::Point a, const mxgui::Point b,
                 const mxgui::Color bgColor, const mxgui::Color textColor,
                 const mxgui::Font& font) : a(a), b(b), bgColor(bgColor),
                 textColor(textColor), font(font) { }

TextBox::TextBox(const mxgui::Point a, const int width, const int height,
                 const mxgui::Color bgColor, const mxgui::Color textColor,
                 const mxgui::Font& font) : a(a),
                 b(a.x() + width, a.y() + height), bgColor(bgColor),
                 textColor(textColor), font(font) { }

void TextBox::draw(mxgui::DrawingContext& dc)
{
    // Draw borders
    ShadowRectangle rect(a, b, make_pair(darkGrey, lightGrey));
    rect.draw(dc);

    // Create the filled zone for text
    int xa = a.x() + 1;
    int ya = a.y() + 1;

    int xb = b.x() - 1;
    int yb = b.y() - 1;

    dc.clear(Point(xa, ya), Point(xb, yb), bgColor);

    // Write text, centered in the box
    int len = font.calculateLength(text.c_str());
    int dx = xb - xa;
    xa += (dx - len)/2;
    ya += ((yb - ya)/2 - font.getHeight()/2);

    dc.setFont(font);
    dc.setTextColor(textColor, bgColor);
    if(len <= dx) dc.write(Point(xa, ya), text.c_str());
}



