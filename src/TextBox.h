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

#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <display.h>
#include <point.h>
#include <color.h>

/**
 * Simple class for drawing text boxes.
 */
class TextBox
{
public:

    /**
     * Constructor.
     * \param a: point corresponding to the rectangle's upper left corner.
     * \param b: point corresponding to the rectangle's bottom right corner.
     * \param bgColor: background color.
     * \param textColor: text color.
     */
    TextBox(const mxgui::Point a, const mxgui::Point b,
            const mxgui::Color bgColor, const mxgui::Color textColor,
            const mxgui::Font& font);

    /**
     * Constructor.
     * \param a: point corresponding to the rectangle's upper left corner.
     * \param width: rectangle width.
     * \param height: rectangle height.
     * \param bgColor: background color.
     * \param textColor: text color.
     */
    TextBox(const mxgui::Point a, const int width, const int height,
            const mxgui::Color bgColor, const mxgui::Color textColor,
            const mxgui::Font& font);

    /**
     * Destructor.
     */
    ~TextBox() { }

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
     * Set the text box content.
     * \param text: text to be display.
     */
    void write(const std::string& text) { this->text = text; }

    /**
     * Draw the text box.
     */
    void draw(mxgui::DrawingContext& dc);

private:

    mxgui::Point a;             // Uppper left corner of button.
    mxgui::Point b;             // Lower right corner of button.
    mxgui::Color bgColor;       // Background color.
    mxgui::Color textColor;     // Text color.
    const mxgui::Font& font;    // Text font.
    std::string text;           // Text.
};

#endif // TEXTBOX_H
