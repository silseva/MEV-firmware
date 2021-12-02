/*
 * MEV board firmware
 * Copyright (C) 2021  Silvano Seva silvano.seva@polimi.it
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

#include <cstdint>
#include "miosix.h"

/**
 * Driver for FSP2000 differential pressure sensor.
 */
template< class T >
class FSP2000
{
public:

    /**
     * Default constructor
     */
    FSP2000() { }

    /**
     * Destructor.
     */
    ~FSP2000() { }

    /**
     * Read the current differential pressure measured by the sensor, in Pa.
     *
     * @return differential pressure in Pa.
     */
    float getPressure()
    {
        T::sendStart();
        T::send(address);
        T::send(0x00);
        T::send(0x43);

        T::sendRepeatedStart();
        T::send(address | 0x01);
        miosix::delayUs(100);
        int32_t press = T::recvWithAck();
        press = (press << 8) | T::recvWithAck();
        press = (press << 8) | T::recvWithAck();
        press = (press << 8) | T::recvWithNack();

        T::sendStop();

        return static_cast< float >(press)/1000.0f;
    }

private:

    static constexpr uint8_t address = 0x02;
};
