/*
 * MEV board firmware
 * Copyright (C) 2022  Silvano Seva silvano.seva@polimi.it
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <graphics/graphics.h>

template< class T >
class CfgEntry
{
public:

    /**
     *
     */
    CfgEntry(const mxgui::Point startPoint, const std::string& label,
             T& refValue) : startPoint(startPoint), label(label),
             value(refValue) { }

    /**
     *
     */
    ~CfgEntry() { }

   /**
    *
    */
    bool update(mxgui::Event& e, mxgui::DrawingContext& dc)
    {
        using namespace mxgui;

        int btnOffset = dc.getWidth()
                      - spacing
                      - btnWidth;

        mxgui::Point btnPoint(startPoint.x() + btnOffset, startPoint.y());
        mxgui::Point lblPoint(startPoint.x() + spacing,
                              startPoint.y() + (droid21.getHeight() / 2));
        mxgui::Point txtPoint(lblPoint.x() + 3*spacing, lblPoint.y());

        char text[32];
        toText(text, sizeof(text), value);

        dc.setTextColor(black, lightGrey);
        dc.write(lblPoint, label.c_str());
        dc.write(txtPoint, text);

        Button bt(btnPoint, btnWidth, btnHeight, "Set", droid21);
        bool pressed = bt.handleTouchEvent(e);
        bt.draw(dc);

        return pressed;
    }

private:

    /**
     *
     */
    inline void toText(char *text, const size_t maxText, const T& value)
    {
        snprintf(text, maxText, "%d", static_cast< unsigned int >(value));
    }

    static constexpr int spacing   = 15;
    static constexpr int btnWidth  = 50;
    static constexpr int btnHeight = 30;

    mxgui::Point      startPoint;
    mxgui::Point      textPoint;
    const std::string label;
    T&                value;
};


template<>
inline void CfgEntry< float >::toText(char *text, const size_t maxText,
                                      const float& value)
{
    snprintf(text, maxText, "%.3f", value);
}
