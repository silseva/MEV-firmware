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

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <display.h>
#include <point.h>
#include <color.h>

/**
 * Simple class for drawing rectangles.
 */
class Rectangle
{
public:

    /**
     * Constructor.
     * \param a: point corresponding to the rectangle's upper left corner.
     * \param b: point corresponding to the rectangle's bottom right corner.
     * \param color: border color.
     */
    Rectangle(const mxgui::Point a, const mxgui::Point b,
              const mxgui::Color color);

    /**
     * Constructor.
     * \param a: point corresponding to the rectangle's upper left corner.
     * \param width: rectangle width.
     * \param height: rectangle height.
     * \param color: border color.
     */
    Rectangle(const mxgui::Point a, const int width, const int height,
              const mxgui::Color color);

    /**
     * Destructor.
     */
    virtual ~Rectangle() { }

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
     * Draw the button
     */
    virtual void draw(mxgui::DrawingContext& dc);

protected:

    mxgui::Point a;         // Uppper left corner of button.
    mxgui::Point b;         // Lower right corner of button.
    mxgui::Color border;    // Color of borders.
};

/**
 * Simple class for drawing rectangles filled with a given color.
 */
class FilledRectangle : public Rectangle
{
public:

    /**
     * Constructor.
     * \param a: point corresponding to the rectangle's upper left corner.
     * \param b: point corresponding to the rectangle's bottom right corner.
     * \param lineColor: border color.
     * \param fillColor: fill color.
     */
    FilledRectangle(const mxgui::Point a, const mxgui::Point b,
                    const mxgui::Color lineColor, const mxgui::Color fillColor);

    /**
     * Constructor.
     * \param a: point corresponding to the rectangle's upper left corner.
     * \param width: rectangle width.
     * \param height: rectangle height.
     * \param lineColor: border color.
     * \param fillColor: fill color.
     */
    FilledRectangle(const mxgui::Point a, const int width, const int height,
                    const mxgui::Color lineColor, const mxgui::Color fillColor);

    /**
     * Destructor.
     */
    virtual ~FilledRectangle() { }

    /**
     * Draw the button
     */
    virtual void draw(mxgui::DrawingContext& dc);

private:

    mxgui::Color fill;  // Fill color.
};

/**
 * Simple class for drawing fillable rectangles with shadowed border.
 */
class ShadowRectangle : public Rectangle
{
public:

    /**
     * Constructor.
     * \param a: point corresponding to the rectangle's upper left corner.
     * \param b: point corresponding to the rectangle's bottom right corner.
     * \param colors: border colors, first element of the std::pair is the
     *                "main" color for the border, while the other one is used
     *                to draw the shadow.
     */
    ShadowRectangle(const mxgui::Point a, const mxgui::Point b,
                    const std::pair< mxgui::Color, mxgui::Color > colors);

    /**
     * Constructor.
     * \param a: point corresponding to the rectangle's upper left corner.
     * \param width: rectangle width.
     * \param height: rectangle height.
     * \param colors: border colors, first element of the std::pair is the
     *                "main" color for the border, while the other one is used
     *                to draw the shadow.
     */
    ShadowRectangle(const mxgui::Point a, const int width, const int height,
                    const std::pair< mxgui::Color, mxgui::Color > colors);

    /**
     * Destructor.
     */
    virtual ~ShadowRectangle() { }

    /**
     * Fill the rectangle with a given color.
     * \param fillColor: filling color.
     */
    void fill(const mxgui::Color fillColor)
    {
        filled = true;
        this->fillColor = fillColor;
    }

    /**
     * Draw the button
     */
    virtual void draw(mxgui::DrawingContext& dc);

protected:

    const std::pair< mxgui::Color, mxgui::Color > colors;    // Border colors
    bool filled;                                             // Flag for filling
    mxgui::Color fillColor;                                  // Fill color
};

#endif // RECTANGLE_H
