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

#ifndef KEYPAD_H
#define KEYPAD_H

#include <display.h>
#include <point.h>
#include <color.h>
#include <cstdint>
#include <Button.h>
#include <TextBox.h>
#include <Rectangle.h>

/**
 * Class for drawing and managing an input keypad.
 */
class Keypad
{
public:

    /**
     * Constructor.
     * \param p: point corresponding to the keypad's upper left corner.
     */
    Keypad (const mxgui::Point p);

    /**
     * Constructor.
     * \param x: x coordinate of keypad's upper left corner.
     * \param y: y coordinate of keypad's upper left corner.
     */
    Keypad (const unsigned int x, const unsigned int y);

    /**
     * Destructor.
     */
    ~Keypad() { }

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
     * Draw the keypad.
     */
    void draw(mxgui::DrawingContext& dc);

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
     * Clear the keypad, making it ready to accept a new user input.
     */
    void clear();

    /**
     * Retrieve the user-inserted number. Value is valid only after the "OK"
     * button has been pressed.
     * \return the number inserted or NaN if the value is not valid.
     */
    float getNumber();

private:

    mxgui::Point startPoint;    // Keypad starting point, its upper left corner.
    char input[17];             // Array for user input.
    uint8_t inputPos;           // Current position of user input inside the array.
    bool okPressed;             // Flag for "OK" button pressed.
    bool dotPressed;            // Flag for dot button pressed.
    FilledRectangle rect;       // Background rectangle.
    TextBox textBox;            // Text box for user input visualisation
    Button  *keyboard[12];      // Button matrix

    static constexpr int btnWidth      = 60;
    static constexpr int btnHeight     = 30;
    static constexpr int btnSpacing    = 10;
    static constexpr int textBoxWidth  = 2*btnSpacing + 3*btnWidth;
    static constexpr int totalWidth    = 4*btnSpacing + 3*btnWidth;
    static constexpr int totalHeigth   = 6*btnSpacing + 5*btnHeight;

    const std::string labels[12] = { "1", "2", "3",
                                     "4", "5", "6",
                                     "7", "8", "9",
                                     "0", ".", "OK"};
};

#endif // KEYPAD_H
