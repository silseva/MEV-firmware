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

#include "Rectangle.h"
#include "DisplayBox.h"

#include <stdio.h>

using namespace mxgui;

DisplayBox::DisplayBox(const mxgui::Point a, const int width, const int height,
                       const int sideMargin, const int entryMargin,
                       std::vector< std::string >& labels,
                       const mxgui::Color bgColor, const mxgui::Color labelColor,
                       const mxgui::Font& font) :
                       DisplayBox(a, Point(a.x() + width, a.y() + height),
                                sideMargin, entryMargin, labels, bgColor,
                                labelColor, font)
                       { }

DisplayBox::DisplayBox(const mxgui::Point a, const mxgui::Point b,
                       const int sideMargin, const int entryMargin,
                       std::vector< std::string >& labels,
                       const mxgui::Color bgColor, const mxgui::Color labelColor,
                       const mxgui::Font& font) :
                       a(a), b(b), sideMargin(sideMargin),
                       entryMargin(entryMargin), labels(labels),
                       bgColor(bgColor), labelColor(labelColor), font(font)
{
    // Compute vertical spacing and initialise the entry vector
    size_t nElems = labels.size();
    ySpacing = (b.y() - a.y() - nElems * font.getHeight())/(nElems + 1);
    entries.assign(nElems, std::make_pair("", white));
}

void DisplayBox::setEntryValue(const int entry, const std::string& text,
                               const mxgui::Color color)
{
    if(static_cast< size_t >(entry) >= entries.size()) return;
    entries[entry].first  = text;
    entries[entry].second = color;
}


void DisplayBox::draw(mxgui::DrawingContext& dc)
{
    // Draw background
    FilledRectangle rect(a, b, bgColor, bgColor);
    rect.draw(dc);

    // Draw labels and entries
    dc.setFont(font);
    int x = a.x() + sideMargin;
    int y = a.y() + ySpacing;

    for(size_t i = 0; i < labels.size(); i++)
    {
        Point pl(x, y);
        dc.setTextColor(labelColor, bgColor);
        dc.write(pl, labels[i].c_str());

        Point pe(x + entryMargin, y);
        dc.setTextColor(entries[i].second, bgColor);
        dc.write(pe, entries[i].first.c_str());

        y += font.getHeight() + ySpacing;
    }
}

