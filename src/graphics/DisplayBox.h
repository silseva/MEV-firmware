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

#ifndef DISPLAYBOX_H
#define DISPLAYBOX_H

#include <display.h>
#include <point.h>
#include <color.h>
#include <misc_inst.h>
#include <vector>
#include <string>

/**
 * Class for drawing a display box containing labels and text fields representing
 * values or parameters.
 */
class DisplayBox
{
public:

    /**
     * Constructor.
     * \param a: point corresponding to the display box's upper left corner.
     * \param b: point corresponding to the display box's bottom right corner.
     * \param sideMargin: distance of text from the left border of the screen.
     * \param entryMargin: distance between the the beginning of the label and
     *                     the beginning of the corresponding entry.
     * \param labels: vector containing the text for the labels.
     * \param bgColor: background color.
     * \param font: text font.
     */
    DisplayBox(const mxgui::Point a, const mxgui::Point b, const int sideMargin,
               const int entryMargin, std::vector< std::string >& labels,
               const mxgui::Color bgColor, const mxgui::Color labelColor,
               const mxgui::Font& font);

    /**
     * Constructor.
     * \param a: point corresponding to the display box's upper left corner.
     * \param width: width of the display box.
     * \param height: height of the display box.
     * \param sideMargin: distance of text from the left border of the screen.
     * \param entryMargin: distance between the the beginning of the label and
     *                     the beginning of the corresponding entry.
     * \param labels: vector containing the text for the labels.
     * \param bgColor: background color.
     * \param font: text font.
     */
    DisplayBox(const mxgui::Point a, const int width, const int height,
               const int sideMargin, const int entryMargin,
               std::vector< std::string >& labels, const mxgui::Color bgColor,
               const mxgui::Color labelColor, const mxgui::Font& font);

    /**
     * Destructor.
     */
    ~DisplayBox() { }

    /**
     * Get the coordinates of the rectangle's upper left corner.
     * \return a Point containing coordinates of the upper left corner.
     */
    mxgui::Point getUpperLeftCorner() { return a; }

    /**
     * Get the coordinates of the rectangle's bottom right corner.
     * \return a Point containing coordinates of the bottom right corner.
     */
    mxgui::Point getLowerRightCorner() { return b; }

    /**
     * Set the text of a given entry.
     * \param entry: number of the entry to be updated.
     * \param text: text to be displayed.
     * \param color: text color.
     */
    void setEntryValue(const int entry, const std::string& text,
                       const mxgui::Color color = mxgui::white);

    /**
     * Draw the text box.
     */
    void draw(mxgui::DrawingContext& dc);

private:

    mxgui::Point a;                                     // Uppper left corner.
    mxgui::Point b;                                     // Lower right corner.
    int sideMargin;                                     // Side margin.
    int entryMargin;                                    // Entry margin.
    int ySpacing;                                       // Vertical spacing between entries.
    std::vector< std::string >& labels;                 // Entry labels.
    std::vector< std::pair< std::string,
                            mxgui::Color > > entries;   // Entries.
    mxgui::Color bgColor;                               // Background color.
    const mxgui::Color labelColor;                      // Label text color.
    const mxgui::Font& font;                            // Text font.
};

#endif // DISPLAYBOX_H
