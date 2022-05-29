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

/**
 * This class provides an interface representing a generic actuator taking a
 * normalised floating point input from 0 (0% of actuation) to 1.0 (100% of
 * actuation).
 * An object directly instantiated from this class does nothing, except returning
 * the input values corresponding to the lower and upper actuation limits.
 */

class Actuator
{
public:

    /**
     * Constructor.
     */
     Actuator() {}

     /**
      * Destructor.
      */
     virtual ~Actuator() {}

     /**
      * This function allows to query the input value corresponding to the lower
      * actuation limit.
      *
      * @return minimum allowed value for the input.
      */
     constexpr float uMin() { return 0.0f; }

     /**
      * This function allows to query the input value corresponding to the upper
      * actuation limit.
      *
      * @return maximum allowed value for the input.
      */
     constexpr float uMax() { return 1.0f; }

     /**
      * Set a new actuation value. The allowed range for the input value is
      * [uMin uMax], every value going outside this range will be saturated to
      * one of the two limits.
      *
      * @param u: actuator value to be set.
      */
     virtual void setValue(const float u) { (void) u; }
};
