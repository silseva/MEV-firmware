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

#include <algorithm>

/**
 * This class acts as a container for PID tuning parameters, comprised output
 * saturation and sample time.
 */
struct PidParameters
{
    /**
     * Default constructor, initialise all fields to zero.
     */
    PidParameters() : k(0.0f), Ti(0.0f), Td(0.0f), N(0.0f), uMin(0.0f),
                      uMax(0.0f), Tsample(0.0f) {}

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
class PidRegulator
{
public:

    /**
     * Constructor.
     * @param p: regulator parameters
     */
    PidRegulator(const PidParameters& p);

    /**
     * Destructor
     */
    ~PidRegulator();

    /**
     * Compute one control action, i.e. perform one step of a periodic
     * discrete-time regulator.
     * @param w: regulator set point.
     * @param y: actual value of the process' output, computation of the error
     * with respect to the reference is done internally.
     * @return newly computed control action u
     */
    float computeAction(const float w, const float y);

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
     * To avoid bumps, when tracking is enabled the regulator output keeps the
     * last value computed before switching.
     */
    void enableTracking();

    /**
     * Disable tracking mode.
     */
    void disableTracking();

    /**
     * Set a value for the output whe regulator is in tracking mode.
     */
    void setTrackingOutput(float uref);

private:

    // regulator's parameters
    PidParameters pars;

    // Controller internal state
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
