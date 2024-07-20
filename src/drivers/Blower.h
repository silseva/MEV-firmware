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

#include "Actuator.h"

/**
 * This class provides a driver for the Delta FC241B07-L0E blower control board,
 * implementing the interface provided by the Actuator class.
 * Output is on PC8 GPIO and PWM signal is generated using TIM3 peripheral.
 */

class Blower : public Actuator
{
public:

   /**
    * Singleton instance getter.
    *
    * @return reference to the singleton instance of this class.
    */
    static Blower& instance();

    /**
     * Destructor.
     */
    virtual ~Blower();

    /**
     * Set a new actuation value. The allowed range for the input value is
     * [uMin uMax], every value going outside this range will be saturated to
     * one of the two limits.
     *
     * @param u: actuator value to be set.
     */
     virtual void setValue(const float u) override;

private:

    /**
     * Singleton class, constructor is private.
     */
    Blower();
};
