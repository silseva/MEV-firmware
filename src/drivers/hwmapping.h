/*
 * MEV board firmware
 * Copyright (C) 2021 - 2024  Silvano Seva silvano.seva@polimi.it
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

#include <util/software_i2c.h>
#include <miosix.h>

#ifdef BJ_BOARD

//
// I/O mapping for Bell Jar controller board
//

namespace adc
{
    using cs   = miosix::Gpio<GPIOE_BASE, 3>;
    using sck  = miosix::Gpio<GPIOE_BASE, 2>;
    using miso = miosix::Gpio<GPIOE_BASE, 5>;
    using mosi = miosix::Gpio<GPIOE_BASE, 6>;
}

namespace i2c
{
    using sda_1 = miosix::Gpio<GPIOD_BASE, 4>;
    using scl_1 = miosix::Gpio<GPIOD_BASE, 5>;
    using i2c_1 = miosix::SoftwareI2C<sda_1, scl_1>;

    using sda_2 = miosix::Gpio<GPIOG_BASE, 2>;
    using scl_2 = miosix::Gpio<GPIOG_BASE, 3>;
    using i2c_2 = miosix::SoftwareI2C<sda_2, scl_2>;
}

namespace analog
{
    using dac_out = miosix::Gpio<GPIOA_BASE, 5>;
    using pwm_out = miosix::Gpio<GPIOC_BASE, 8>;
}

namespace hpOutputs
{
    using out_1 = miosix::Gpio<GPIOF_BASE, 6>;
    using out_2 = miosix::Gpio<GPIOB_BASE, 2>;
}

namespace rs485
{
    using txd = miosix::Gpio<GPIOC_BASE, 12>;
    using rxd = miosix::Gpio<GPIOD_BASE, 2>;
    using te  = miosix::Gpio<GPIOC_BASE, 11>;
}

#else

//
// I/O mapping for bed controller board
//

namespace adc
{
    using muxs = miosix::Gpio<GPIOC_BASE, 3>;
    using cs   = miosix::Gpio<GPIOE_BASE, 3>;
    using sck  = miosix::Gpio<GPIOE_BASE, 2>;
    using miso = miosix::Gpio<GPIOE_BASE, 5>;
    using mosi = miosix::Gpio<GPIOE_BASE, 6>;
}

namespace i2c
{
    using sda_1 = miosix::Gpio<GPIOD_BASE, 4>;
    using scl_1 = miosix::Gpio<GPIOD_BASE, 5>;
    using i2c_1 = miosix::SoftwareI2C<sda_1, scl_1>;
}

namespace hpOutputs
{
    using out_1 = miosix::Gpio<GPIOF_BASE, 6>;
    using out_2 = miosix::Gpio<GPIOB_BASE, 2>;
}

namespace rs485
{
    using txd = miosix::Gpio<GPIOC_BASE, 12>;
    using rxd = miosix::Gpio<GPIOD_BASE, 2>;
    using te  = miosix::Gpio<GPIOC_BASE, 11>;
}

#endif
