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

#ifndef CONFIRM_BOX_H
#define CONFIRM_BOX_H

#include <display.h>
#include <point.h>
#include <cstdint>
#include <level2/input.h>
#include "Button.h"

/**
 * Class for drawing and managing a warning pop-up box with confirmation
 * buttons.
 */
class ConfirmBox
{
public:

    /**
     * Constructor.
     * \param p: point corresponding to the keypad's upper left corner.
     */
    ConfirmBox (const mxgui::Point p);

    /**
     * Constructor.
     * \param x: x coordinate of keypad's upper left corner.
     * \param y: y coordinate of keypad's upper left corner.
     */
    ConfirmBox (const unsigned int x, const unsigned int y);

    /**
     * Destructor.
     */
    ~ConfirmBox() { }

    /**
     * Get the coordinates of the keypad's upper left corner.
     * \return a Point containing the coordinates of the upper left corner.
     */
    mxgui::Point getUpperLeftCorner() { return startPoint; }

    /**
     * Get the coordinates of the keypad's lower right corner.
     * \return a Point containing the coordinates of the lower right corner.
     */
    mxgui::Point getLowerRightCorner()
    {
        return mxgui::Point(startPoint.x() + totalWidth,
                            startPoint.y() +  totalHeigth);
    }

    /**
     * Get keypad's total width.
     * \return keypad width.
     */
    static int getWidth() { return totalWidth; }

    /**
     * Get keypad's total height.
     * \return keypad height.
     */
    static int getHeight() { return totalHeigth; }

    /**
     * Draw the keypad.
     */
    void draw(mxgui::DrawingContext& dc, const std::string& line);

    /**
     * Handle a keyboard event, updating the inserted number and redrawing the
     * parts subjected to changes.
     * \param e: incoming keyboard event.
     * \param dc: drawing context for redraw.
     * \return true if the "OK" button has been pressed, meaning that the user
     *         terminated inputting the number.
     */
    bool handleEvent(mxgui::Event& e, mxgui::DrawingContext& dc);

    /**
     * Obtain the current confirmation status.
     * \return true if user pressed the "yes" button, false otherwhise.
     */
    bool confirmed() { return yesPressed; }

    /**
     * Clear the internal state, making confirmed() to return again false until
     * the "yes" button is not pressed again.
     */
    void clear() { yesPressed = false; }

private:

    mxgui::Point startPoint;    // Keypad starting point, its upper left corner.
    Button btnYes;              // "yes" button.
    Button btnNo;               // "no" button.
    bool yesPressed;            // Flag to determine if any the "yes" button was pressed.

    static constexpr int totalWidth  = 220;    // Box with
    static constexpr int totalHeigth = 110;    // Box height
    static constexpr int iconSize    = 50;     // Icon size, square icon
    static constexpr int spacing     = 10;     // Spacing between elements
    static constexpr int btnWidth    = 60;     // Button width
    static constexpr int btnHeight   = 30;     // Button height
    static constexpr int btnY        = 2*spacing + iconSize;
    static constexpr int btnYes_xOff = totalWidth - 2*(spacing + btnWidth);
    static constexpr int btnNo_xOff  = totalWidth - spacing - btnWidth;
};

#endif // CONFIRM_BOX_H
