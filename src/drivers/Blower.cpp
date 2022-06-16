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

#include <algorithm>
#include <miosix.h>
#include "hwmapping.h"
#include "Blower.h"

using namespace miosix;

Blower& Blower::instance()
{
    static Blower blower;
    return blower;
}

Blower::Blower()
{
    // AF2 is TIM3_CH3
    analog::pwm_out::mode(Mode::ALTERNATE);
    analog::pwm_out::alternateFunction(2);

    RCC->DCKCFGR |= RCC_DCKCFGR_TIMPRE;    // Clock timer at 180MHz
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    RCC_SYNC();

    /*
     * Timer clock = 180MHz, 10 bit resolution, PWM frequency = 25kHz
     */

    TIM3->ARR   = 0x03FF;
    TIM3->PSC   = 6;
    TIM3->CNT   = 0;
    TIM3->CCR3  = 0;
    TIM3->CCMR2 = TIM_CCMR2_OC3M_2  // CH3 in PWM mode 1, preload enabled
                | TIM_CCMR2_OC3M_1
                | TIM_CCMR2_OC3PE;
    TIM3->CCER  = TIM_CCER_CC3E;
    TIM3->EGR   = TIM_EGR_UG;       // Update registers
    TIM3->CR1   = TIM_CR1_ARPE
                | TIM_CR1_CEN;      // Start timer
}

Blower::~Blower()
{
    TIM3->CR1 = 0;
    RCC->APB1ENR &= ~RCC_APB1ENR_TIM3EN;

    analog::pwm_out::mode(Mode::OUTPUT);
    analog::pwm_out::low();
}

void Blower::setValue(const float u)
{
    // 10-bit DAC, maximum value is 1023
    float dacValue = std::max(std::min(u, uMax()), uMin()) * 1023.0f;

    // The PWM output stage is inverting: invert PWM polarity to compensate this.
    TIM3->CCR3 = 0x3FF - static_cast< uint16_t >(dacValue);
}
