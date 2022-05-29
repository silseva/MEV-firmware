/*
 * Copyright (C) 2022 Silvano Seva silvano.seva@polimi.it
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
 *
 */

#include "PidRegulator.h"

PidRegulator::PidRegulator(const PidParameters& p) : pars(p), u(0.0f), up(0.0f),
ud(0.0f), ui(0.0f), uio(0.0f), edo(0.0f), udo(0.0f),trackingEnabled(false),
utr(0.0f)
{

}

PidRegulator::~PidRegulator()
{

}

float PidRegulator::computeAction(const float w, const float y)
{
    // Run mode: at each step a new value for up, ui and ud is computed.
    // Hold mode: up, ui and ud are not updated and keep their previous value,
    // the only thing to do is to update ud(k-1) and ui(k-1).

    // Current error
    float e = w - y;

    // Proportional
    up = pars.k*e;

    // Integrative
    ui = pars.k*(pars.Tsample/pars.Ti)*e + uio;

    // Derivative
    if((pars.Td != 0) && (pars.N != 0))
    {
        ud = pars.Td/(pars.Td+pars.N*pars.Tsample)*udo
           + ((pars.k*pars.N*pars.Td)/(pars.Td+pars.N*pars.Tsample))*(e-edo);
    }
    else
    {
        ud = 0;
    }

    if(trackingEnabled)
    {
        u = utr;
        ud = 0;
        up = 0;
    }
    else
    {
        u = up + ui + ud;
    }

    u = std::max(pars.uMin, std::min(u, pars.uMax));
    uio = u - up - ud;
    udo = ud;
    edo = e;

    return u;
}

PidParameters PidRegulator::getParameters() const
{
    return pars;
}

void PidRegulator::setParameters(const PidParameters& p)
{
    pars = p;
}

void PidRegulator::enableTracking()
{
    utr = u;
    trackingEnabled = true;
}

void PidRegulator::disableTracking()
{
    trackingEnabled = false;
    utr = 0.0f;
}

void PidRegulator::setTrackingOutput(float uref)
{
    utr = uref;
}
