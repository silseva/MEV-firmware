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

#pragma once

#include <algorithm>

/**
 * This class acts as a container for PID tuning parameters, comprised output
 * saturation and sample time.
 */
struct PidParameters
{
    /**
     * @param k: proportional term.
     * @param Ti: integral time.
     * @param Td: derivative time.
     * @param N: filter coefficient.
     * @param uMin: minimum allowed value for the controlled variable.
     * @param uMax: maximum allowed value for the controlled variable.
     * @param Tsample: sampling period of the discretisation used to obtain the
     * discrete-time transfer function of the regulator.
     */
    PidParameters(float k, float Ti, float Td, float N, float uMin, float uMax,
                  float Tsample) : k(k), Ti(Ti), Td(Td), N(N), uMin(uMin),
                  uMax(uMax), Tsample(Tsample) {}
    float k;
    float Ti;
    float Td;
    float N;
    float uMin;
    float uMax;
    float Tsample;
};


/**
 * Discrete-time PID regulator.
 */
class PidController
{
public:

    /**
     * Constructor.
     * @param p: regulator parameters
     */
    PidController(const PidParameters& p);

    /**
     * Destructor
     */
    ~PidController();

    /**
     * Set regulator's reference to @param w
     */
    void setReference(const float w);

    /**
     * Compute one control action, i.e. perform one step of a periodic
     * discrete-time regulator.
     * @param y: actual value of the process' output, computation of the error
     * with respect to the reference is done internally.
     * @return newly computed control action u
     */
    float computeAction(const float y);

    /**
     * Get actual regulator's tuning parameters.
     * @return a PidParameters object containing the current tuning.
     */
    PidParameters getParameters() const;

    /**
     * Set new regulator's tuning parameters.
     * @param p: regulator parameters
     */
    void setParameters(const PidParameters& p);

    /**
     * Enable tracking mode.
     */
    void enableTracking(float uref);

    /**
     * Disable tracking mode.
     */
    void disableTracking();

private:

    // regulator's parameters
    PidParameters pars;

    // Controller internal state
    float w;
    float u;
    float up;
    float ud;
    float ui;
    float uio;
    float edo;
    float udo;

    // Tracking
    bool trackingEnabled;
    float utr;
};

#endif // PIDCONTROLLER_H
