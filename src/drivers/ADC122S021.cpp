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

#include <limits>
#include <miosix.h>
#include "hwmapping.h"
#include "ADC122S021.h"

using namespace miosix;
using namespace std;

/**
 * RAII class for chip select management.
 */
class ScopedCs
{
public:

    ScopedCs()
    {
        adc::cs::low();
    }

    ~ScopedCs()
    {
        adc::cs::high();
    }
};

ADC122S021& ADC122S021::instance()
{
    static ADC122S021 adc;
    return adc;
}

ADC122S021::ADC122S021()
{
    adc::cs::mode(Mode::OUTPUT);
    adc::cs::high();

    // SPI4 is on AF5
    adc::miso::mode(Mode::ALTERNATE);
    adc::mosi::mode(Mode::ALTERNATE);
    adc::sck::mode(Mode::ALTERNATE);
    adc::miso::alternateFunction(5);
    adc::mosi::alternateFunction(5);
    adc::sck::alternateFunction(5);

    RCC->APB2ENR |= RCC_APB2ENR_SPI4EN;
    RCC_SYNC();

    SPI4->CR1 = SPI_CR1_DFF     // 16-bit transfer size
              | SPI_CR1_SSM     // Software management of nCS
              | SPI_CR1_SSI     // Force internal nCS
              | SPI_CR1_BR_2    // Fclock: 90MHz/64 = 1.4MHz
              | SPI_CR1_BR_0
              | SPI_CR1_MSTR;   // Master mode

    SPI4->CR1 |= SPI_CR1_SPE;   // Enable peripheral
}

ADC122S021::~ADC122S021()
{
    adc::cs::high();
    RCC->APB2ENR &= ~RCC_APB2ENR_SPI4EN;
}

float ADC122S021::getValue(const AdcChannel channel)
{
    // Select the ADC
    ScopedCs cs;
    delayUs(1);

    uint16_t ch = static_cast< uint16_t >(channel) << 11;
    long long t = getTick();

    // Set the channel to be sampled, incoming value is discarded.
    SPI4->DR = ch;
    while((SPI4->SR & SPI_SR_RXNE) == 0)
    {
        if((getTick() - t) > 10)
        {
            return numeric_limits< float >::signaling_NaN();
        }
    }

    // Flush data register and reset RXNE flag.
    (void) SPI4->DR;

    // Get a sample of the channel voltage.
    t = getTick();
    SPI4->DR = ch;
    while((SPI4->SR & SPI_SR_RXNE) == 0)
    {
        if((getTick() - t) > 10)
        {
            return numeric_limits< float >::signaling_NaN();
        }
    }

    uint16_t value = SPI4->DR;
    return (static_cast< float >(value) * 5.0f) / 4096.0f;
}
