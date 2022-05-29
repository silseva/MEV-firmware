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

#include "common/ActiveObject.h"
#include "common/PidRegulator.h"
#include "drivers/ADC122S021.h"
#include "drivers/Blower.h"
#include "BellJar/BjState.h"

/**
 * Bell Jar level controller, active object.
 */
class LevelController : public ActiveObject
{
public:

    /**
     * Constructor.
     */
    LevelController();

    /**
     * Destructor.
     */
    virtual ~LevelController();

private:

    /**
     * Worker function of the sensor sampler, called by the active object
     * thread.
     */
    virtual void run() override;

    /**
     * Update measurements of process input.
     */
    void updateMeasurements();


    ADC122S021&  adc;    ///< ADC driver.
    Blower&      blower; ///< Blower controller
    PidRegulator pid;    ///< PID regulator
    CtrlMode     rMode;  ///< Current regulator's operating mode
};
