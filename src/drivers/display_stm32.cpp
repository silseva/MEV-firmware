/***************************************************************************
 *   Copyright (C) 2014 by Terraneo Federico                               *
 *                 2021 - 2024 by Silvano Seva                             *
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

#include "display_stm32.h"
#include <miosix.h>
#include <cstdarg>

using namespace std;
using namespace miosix;
using namespace mxgui;

//Control interface
typedef Gpio<GPIOF_BASE, 7> scl; //SPI SCK
typedef Gpio<GPIOF_BASE, 9> sda; //SPI MOSI
typedef Gpio<GPIOC_BASE, 2> csx; //SPI CS
typedef Gpio<GPIOD_BASE,13> dcx; //Data/command
typedef Gpio<GPIOD_BASE,12> rdx; //Used only un parallel mode
typedef Gpio<GPIOD_BASE,11> te;  //Tearing effect output from display, unused

/**
 * Send and receive a byte through SPI5
 * \param c byte to send
 * \return byte received
 */
static unsigned char spi5sendRev(unsigned char c=0)
{
    SPI5->DR=c;
    while((SPI5->SR & SPI_SR_RXNE)==0) ;
    return SPI5->DR;
}

/**
 * Send a command to the ILI9341 display controller
 * \param cmd command
 * \param len length of the (optional) argument, or 0 for commands without
 * arguments.
 */
static void sendCmd(unsigned char cmd, int len, ...)
{
    dcx::low();
    csx::low();
    spi5sendRev(cmd);
    csx::high();
    delayUs(1);
    dcx::high();
    va_list arg;
    va_start(arg,len);
    for(int i=0;i<len;i++)
    {
        csx::low();
        spi5sendRev(va_arg(arg,int));
        csx::high();
        delayUs(1);
    }
    va_end(arg);
}

void mxgui::registerDisplayHook(DisplayManager& dm)
{
    dm.registerDisplay(&DisplayStm32::instance());
}

//
// class DisplayStm32
//
const short int DisplayStm32::width;
const short int DisplayStm32::height;

DisplayStm32& DisplayStm32::instance()
{
    static DisplayStm32 instance;
    return instance;
}

void DisplayStm32::doTurnOn()
{
    LTDC->GCR |= LTDC_GCR_LTDCEN;
    Thread::sleep(40);
    sendCmd(0x29,0); //LCD_DISPLAY_ON
}

void DisplayStm32::doTurnOff()
{
    sendCmd(0x28,0); //LCD_DISPLAY_OFF
    LTDC->GCR &=~ LTDC_GCR_LTDCEN;
}

void DisplayStm32::doSetBrightness(int brt) {}

pair<short int, short int> DisplayStm32::doGetSize() const
{
    return make_pair(height,width);
}

void DisplayStm32::write(Point p, const char *text)
{
    font.draw(*this,textColor,p,text);
}

void DisplayStm32::clippedWrite(Point p, Point a, Point b, const char *text)
{
    font.clippedDraw(*this,textColor,p,a,b,text);
}

void DisplayStm32::clear(Color color)
{
    clear(Point(0,0),Point(width-1,height-1),color);
}

void DisplayStm32::clear(Point p1, Point p2, Color color)
{
    if(p1.x()<0 || p2.x()<p1.x() || p2.x()>=width
     ||p1.y()<0 || p2.y()<p1.y() || p2.y()>=height) return;
    if((color & 0xff)==(color>>8))
    {
        //Can use memset
        if(p1.x()==0 && p2.x()==width-1)
        {
            //Can merge lines
            memset(framebuffer1+p1.y()*width,color,(p2.y()-p1.y()+1)*width*bpp);
        } else {
            //Can't merge lines
            Color *ptr=framebuffer1+p1.x()+width*p1.y();
            short len=p2.x()-p1.x()+1;
            for(short i=p1.y();i<=p2.y();i++)
            {
                memset(ptr,color,len*bpp);
                ptr+=width;
            }
        }
    } else {
        //Can't use memset
        if(p1.x()==0 && p2.x()==width-1)
        {
            //Can merge lines
            Color *ptr=framebuffer1+p1.y()*width;
            int numPixels=(p2.y()-p1.y()+1)*width;
            //This loop is worth unrolling
            for(int i=0;i<numPixels/4;i++)
            {
                *ptr++=color;
                *ptr++=color;
                *ptr++=color;
                *ptr++=color;
            }
            for(int i=0;i<(numPixels & 3);i++) *ptr++=color;
        } else {
            //Can't merge lines
            Color *ptr=framebuffer1+p1.x()+width*p1.y();
            short len=p2.x()-p1.x()+1;
            for(short i=p1.y();i<=p2.y();i++)
            {
                for(short j=0;j<len;j++) *ptr++=color;
                ptr+=width-len;
            }
        }
    }
}

void DisplayStm32::beginPixel() {}

void DisplayStm32::setPixel(Point p, Color color)
{
    int offset=p.x()+p.y()*width;
    if(offset<0 || offset>=numPixels) return;
    *(framebuffer1+offset)=color;
}

void DisplayStm32::line(Point a, Point b, Color color)
{
    //Horizontal line speed optimization
    if(a.y()==b.y())
    {
        short minx=min(a.x(),b.x());
        short maxx=max(a.x(),b.x());
        if(minx<0 || maxx>=width || a.y()<0 || a.y()>=height) return;
        Color *ptr=framebuffer1+minx+width*a.y();
        for(short i=minx;i<=maxx;i++) *ptr++=color;
        return;
    }
    //Vertical line speed optimization
    if(a.x()==b.x())
    {
        short miny=min(a.y(),b.y());
        short maxy=max(a.y(),b.y());
        if(a.x()<0 || a.x()>=width || miny<0 || maxy>=height) return;
        Color *ptr=framebuffer1+a.x()+width*miny;
        for(short i=miny;i<=maxy;i++)
        {
            *ptr=color;
            ptr+=width;
        }
        return;
    }
    //General case
    Line::draw(*this,a,b,color);
}

void DisplayStm32::scanLine(Point p, const Color *colors, unsigned short length)
{
    if(p.x()<0 || static_cast<int>(p.x())+static_cast<int>(length)>width
        ||p.y()<0 || p.y()>=height) return;
    Color *ptr=framebuffer1+p.x()+p.y()*width;
    memcpy(ptr,colors,length*bpp);
}

Color *DisplayStm32::getScanLineBuffer()
{
    return buffer;
}

void DisplayStm32::scanLineBuffer(Point p, unsigned short length)
{
    int offset=p.x()+p.y()*width;
    if(offset<0 || offset>=numPixels) return;
    memcpy(framebuffer1+offset,buffer,length*bpp);
}

void DisplayStm32::drawImage(Point p, const ImageBase& img)
{
    short int xEnd=p.x()+img.getWidth()-1;
    short int yEnd=p.y()+img.getHeight()-1;
    if(p.x()<0 || p.y()<0 || xEnd<p.x() || yEnd<p.y()
        ||xEnd >= width || yEnd >= height) return;

//    const unsigned short *imgData=img.getData();
//    if(imgData!=0)
//    {
//        //TODO Optimized version for in-memory images
//    } else
    img.draw(*this,p);
}

void DisplayStm32::clippedDrawImage(Point p, Point a, Point b, const ImageBase& img)
{
//    if(img.getData()==0)
//    {
    img.clippedDraw(*this,p,a,b);
    // render();
    return;
//    } //else optimized version for memory-loaded images
//        //TODO: optimize
//    }
}


void DisplayStm32::drawRectangle(Point a, Point b, Color c)
{
    line(a,Point(b.x(),a.y()),c);
    line(Point(b.x(),a.y()),b,c);
    line(b,Point(a.x(),b.y()),c);
    line(Point(a.x(),b.y()),a,c);
}

DisplayStm32::pixel_iterator DisplayStm32::begin(Point p1, Point p2,
        IteratorDirection d)
{
    bool fail=false;
    if(p1.x()<0 || p1.y()<0 || p2.x()<0 || p2.y()<0) fail=true;
    if(p1.x()>=width || p1.y()>=height || p2.x()>=width || p2.y()>=height) fail=true;
    if(p2.x()<p1.x() || p2.y()<p1.y()) fail=true;
    if(fail)
    {
        //Return invalid (dummy) iterators
        this->last=pixel_iterator();
        return this->last;
    }

    //Set the last iterator to a suitable one-past-the last value
    if(d==DR) this->last=pixel_iterator(Point(p2.x()+1,p1.y()),p2,d,this);
    else this->last=pixel_iterator(Point(p1.x(),p2.y()+1),p2,d,this);

    return pixel_iterator(p1,p2,d,this);
}

DisplayStm32::~DisplayStm32() {}

DisplayStm32::DisplayStm32()
    : framebuffer1(new Color[width*height]),//reinterpret_cast<unsigned short*>(0xd0600000)),
      buffer(framebuffer1+numPixels)
{
    {
        FastInterruptDisableLock dLock;
        //PLLSAI runs @ 192MHz, both Q and R outputs are divided by 4 so 48MHz
        RCC->PLLSAICFGR=4<<28 | 4<<24 | 192<<6;
        //PLLSAI R output divided by 8 resulting in a 6MHz LTDC clock
        RCC->DCKCFGR=2<<16;
        RCC->CR |= RCC_CR_PLLSAION;
    }

    while((RCC->CR & RCC_CR_PLLSAIRDY)==0) ;

    {
        FastInterruptDisableLock dLock;

        scl::mode(Mode::ALTERNATE);    scl::alternateFunction(5); //SPI5
        sda::mode(Mode::ALTERNATE);    sda::alternateFunction(5);
        csx::mode(Mode::OUTPUT);       csx::high();
        dcx::mode(Mode::OUTPUT);
        rdx::mode(Mode::OUTPUT);       rdx::high(); //Original fw seems to leave it low

        RCC->APB2ENR |= RCC_APB2ENR_LTDCEN | RCC_APB2ENR_SPI5EN;
        RCC->AHB1ENR |= RCC_AHB1ENR_DMA2EN;

        RCC_SYNC();
    }

    SPI5->CR1=SPI_CR1_SSM   //Sowtware CS
            | SPI_CR1_SSI   //Software CS high
            | SPI_CR1_SPE   //SPI enabled
            | (1<<3)        //Divide input clock by 4: 84/4=21MHz
            | SPI_CR1_MSTR; //Master mode
    Thread::sleep(1);

    //
    // ILI9341 power up sequence -- begin
    //
    sendCmd(0xcb,5,0x39,0x2c,0x00,0x34,0x02); //LCD_POWERA
    sendCmd(0xcf,3,0x00,0xc1,0x30);           //LCD_POWERB
    sendCmd(0xe8,3,0x85,0x00,0x78);           //LCD_DTCA
    sendCmd(0xea,2,0x00,0x00);                //LCD_DTCB
    sendCmd(0xed,4,0x64,0x03,0x12,0x81);      //LCD_POWER_SEQ
    sendCmd(0xf7,1,0x20);                     //LCD_PRC
    sendCmd(0xc0,1,0x23);                     //LCD_POWER1
    sendCmd(0xc1,1,0x10);                     //LCD_POWER2
    sendCmd(0xc5,2,0x3e,0x28);                //LCD_VCOM1
    sendCmd(0xc7,1,0x87);                     //LCD_VCOM2
    sendCmd(0x36,1,0x28);                     //LCD_MAC
    sendCmd(0x3A,1,0x55);                     //PIXEL_FORMAT
    sendCmd(0xb6,3,0x08,0x82,0x27);           //LCD_DFC
    sendCmd(0xf2,1,0x00);                     //LCD_3GAMMA_EN
    sendCmd(0x2a,4,0x00,0x00,0x00,0xef);      //LCD_COLUMN_ADDR
    sendCmd(0x2b,4,0x00,0x00,0x01,0x3f);      //LCD_PAGE_ADDR
    sendCmd(0x26,1,0x01);                     //LCD_GAMMA
    sendCmd(0xe0,15,0x0f,0x29,0x24,0x0c,0x0e,0x09,0x4e,0x78,0x3c,0x09,0x13,
            0x05,0x17,0x11,0x00);             //LCD_PGAMMA
    sendCmd(0xe1,15,0x00,0x16,0x1b,0x04,0x11,0x07,0x31,0x33,0x42,0x05,0x0c,
            0x0a,0x28,0x2f,0x0f);             //LCD_NGAMMA

    Thread::sleep(200);
    sendCmd(0x11,0);                          //LCD_SLEEP_OUT
    Thread::sleep(200);
    sendCmd(0x29,0);                          //LCD_DISPLAY_ON
    sendCmd(0x2c,0);                          //LCD_GRAM
    //
    // ILI9341 power up sequence -- end
    //

    memset(framebuffer1,0,height*width*bpp);

    setFont(droid11);
    setTextColor(make_pair(Color(0xffff),Color(0x0000)));
    clear(black);
}

void DisplayStm32::render()
{
    sendCmd(0x2b,4,0x00,0x00,0x00,0xef);
    sendCmd(0x2a,4,0x00,0x00,0x01,0x3f);
    sendCmd(0x2c,0);

    SPI5->CR1 &= ~SPI_CR1_SPE;
    SPI5->CR1 |= SPI_CR1_DFF;
    SPI5->CR1 |= SPI_CR1_SPE;

    csx::low();
    dcx::high();

    for(unsigned int i = 0; i < numPixels; i++)
    {
        SPI5->DR = framebuffer1[i];
        while((SPI5->SR & SPI_SR_RXNE)==0) ;
        (void) SPI5->DR;
    }

    csx::high();

    SPI5->CR1 &= ~SPI_CR1_SPE;
    SPI5->CR1 &= ~SPI_CR1_DFF;
    SPI5->CR1 |= SPI_CR1_SPE;
}

Color DisplayStm32::pixel_iterator::dummy;
