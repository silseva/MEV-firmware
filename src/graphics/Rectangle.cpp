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
#include "Rectangle.h"

using namespace mxgui;

Rectangle::Rectangle(const Point a, const Point b, const Color color) : a(a),
                     b(b), border(color) { }

Rectangle::Rectangle(const Point a, const int width, const int height,
                     const Color color) :
                     Rectangle(a, Point(a.x() + width, a.y() + height), color)
                     { }

void Rectangle::draw(mxgui::DrawingContext& dc)
{
    dc.line(a, Point(b.x(),a.y()), border);    // Upper edge
    dc.line(Point(b.x(),a.y()), b, border);    // Right edge
    dc.line(b, Point(a.x(),b.y()), border);    // Lower edge
    dc.line(Point(a.x(),b.y()), a, border);    // Left edge
}

FilledRectangle::FilledRectangle(const mxgui::Point a, const mxgui::Point b,
                                 const mxgui::Color lineColor,
                                 const mxgui::Color fillColor) :
                                 Rectangle(a, b, lineColor), fill(fillColor) {}

FilledRectangle::FilledRectangle(const mxgui::Point a, const int width,
                                 const int height, const mxgui::Color lineColor,
                                 const mxgui::Color fillColor) :
                                 Rectangle(a, width, height, lineColor),
                                 fill(fillColor) { }

void FilledRectangle::draw(mxgui::DrawingContext& dc)
{
    Rectangle::draw(dc);
    Point start(a.x() + 1, a.y() + 1);
    Point end  (b.x() - 1, b.y() - 1);
    dc.clear(start, end, fill);
}

ShadowRectangle::ShadowRectangle(const mxgui::Point a, const mxgui::Point b,
                                 const std::pair< Color, Color > colors) :
                                 Rectangle(a, b, colors.first),
                                 colors(colors),filled(false) {}

ShadowRectangle::ShadowRectangle(const mxgui::Point a, const int width,
                                 const int height,
                                 const std::pair< Color, Color > colors) :
                                 Rectangle(a, width, height, colors.first),
                                 colors(colors), filled(false) {}

void ShadowRectangle::draw(mxgui::DrawingContext& dc)
{
    // Fill area then draw the borders
    if(filled) dc.clear(a, b, fillColor);

    dc.line(a, Point(b.x(),a.y()), colors.first);   // Upper edge
    dc.line(Point(b.x(),a.y()), b, colors.second);  // Right edge
    dc.line(b, Point(a.x(),b.y()), colors.second);  // Lower edge
    dc.line(Point(a.x(),b.y()), a, colors.first);   // Left edge

}



