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
#include "Button.h"

using namespace mxgui;

Button::Button(const Point a, const Point b, const std::string& label,
               const Font labelFont, const bool togglable) : a(a), b(b),
               down(false), clicked(false), togglable(togglable), label(label),
               font(labelFont){ }

Button::Button(const mxgui::Point a, const int width, const int height,
               const std::string& label, const mxgui::Font labelFont,
               const bool togglable) : a(a), b(a.x() + width, a.y() + height),
               down(false), clicked(false), togglable(togglable), label(label),
               font(labelFont){ }

Button::~Button()
{

}

void Button::draw(mxgui::DrawingContext& dc)
{
    static const unsigned short tlp[]=
    {
        61276,46420,31661,48565,33741,46452,31661,46452,57050
    };
    const Image tl(3,3,tlp); //Button top left
    static const unsigned short trp[]=
    {
        31661,46420,61276,57050,31628,48565,48499,42161,31661
    };
    const Image tr(3,3,trp); //Button top right
    static const unsigned short blp[]=
    {
        31661,57050,48499,48565,33741,42225,61276,46420,31661
    };
    const Image bl(3,3,blp); //Button bottom left
    static const unsigned short brp[]=
    {
        48499,42161,31661,42225,33741,48565,31661,46420,61276
    };
    const Image br(3,3,brp); //Button bottom right
    static const unsigned short topColors[]={31628,59131,48499};
    static const unsigned short botColors[]={48499,48499,31628};
    //Draw button corners
    dc.drawImage(a,tl);
    dc.drawImage(Point(b.x()-2,a.y()),tr);
    dc.drawImage(Point(a.x(),b.y()-2),bl);
    dc.drawImage(Point(b.x()-2,b.y()-2),br);
    //Draw button surrounding lines
    for(int i=0;i<3;i++)
        dc.line(Point(a.x()+3,a.y()+i),Point(b.x()-2,a.y()+i),topColors[i]);
    for(int i=0;i<3;i++)
        dc.line(Point(a.x()+i,a.y()+3),Point(a.x()+i,b.y()-3),topColors[i]);
    for(int i=0;i<3;i++)
        dc.line(Point(b.x()-i,a.y()+3),Point(b.x()-i,b.y()-3),botColors[3-i]);
    for(int i=0;i<3;i++)
        dc.line(Point(a.x()+3,b.y()-i),Point(b.x()-2,b.y()-i),botColors[3-i]);
    Point aa(a.x()+3,a.y()+3);
    Point bb(b.x()-3,b.y()-3);
    if(clicked || down)
    {
        dc.clear(aa,bb,darkGrey);
        dc.setTextColor(black,darkGrey);
    }
    else
    {
        dc.clear(aa,bb,lightGrey);
        dc.setTextColor(black,lightGrey);
    }
    dc.setFont(font);
    int len=font.calculateLength(label.c_str());
    int dx=bb.x()-aa.x();
    int yy=aa.y()+(bb.y()-aa.y()-font.getHeight())/2;
    if(len<=dx) dc.write(Point(aa.x()+(dx-len)/2,yy),label.c_str());
}

