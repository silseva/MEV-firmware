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

#ifndef BUTTON_H
#define BUTTON_H

#include <display.h>
#include <image.h>
#include <point.h>
#include <level2/input.h>

/**
 * Class for a clickable button with label.
 */
class Button
{
public:

    /**
     * Constructor.
     * \param a: point corresponding to the button's upper left corner.
     * \param b: point corresponding to the button's bottom right corner.
     * \param label: text to be displayed inside the button.
     * \param labelFont: font for the button text.
     * \param togglable: flag to specify if the button behaviour between momentary
     *                   or togglable.
     */
    Button(const mxgui::Point a, const mxgui::Point b, const std::string& label,
           const mxgui::Font labelFont, const bool togglable = false);

    /**
     * Constructor.
     * \param a: point corresponding to the button's upper left corner.
     * \param width: button width.
     * \param height: button height.
     * \param label: text to be displayed inside the button.
     * \param labelFont: font for the button text.
     * \param toggle: flag to specify if the button behaviour between momentary
     *                or togglable.
     */
    Button(const mxgui::Point a, const int width, const int height,
           const std::string& label, const mxgui::Font labelFont,
           const bool togglable = false);

    /**
     * Destructor.
     */
    ~Button();

    /**
     * Get the coordinates of the button's upper left corner.
     * \return a Point containing coordinates of the upper left corner.
     */
    mxgui::Point getUpperLeftCorner() { return a; }

    /**
     * Get the coordinates of the button's bottom right corner.
     * \return a Point containing coordinates of the bottom right corner.
     */
    mxgui::Point getBottomRightCorner() { return b; }

    /**
     * Handle a touch of the screen that could possibly result in the button
     * being pressed.
     * \param e screen event.
     * \return true if the button is pressed.
     */
    bool handleTouchEvent(mxgui::Event& e)
    {
        if((e.getEvent() == EventType::TouchDown) && within(e.getPoint(),a,b))
        {
            down = true;
            if(togglable) clicked = !clicked;
            return true;
        }

        if(e.getEvent() == EventType::TouchUp)
        {
            down = false;
        }

        return false;
    }

    /**
     * \return true if the button is clicked.
     */
    bool isClicked()
    {
        return clicked;
    }

    /**
     * Draw the button
     */
    void draw(mxgui::DrawingContext& dc);

private:

    mxgui::Point a;     // Uppper left corner of button.
    mxgui::Point b;     // Lower right corner of button.
    bool down;          // Flag to check if button is pressed.
    bool clicked;       // Flag to check if button is clicked.
    bool togglable;     // Flag to determine if button is togglable or not.
    std::string label;  // Button label.
    mxgui::Font font;   // Label font.
};

#endif // BUTTON_H
