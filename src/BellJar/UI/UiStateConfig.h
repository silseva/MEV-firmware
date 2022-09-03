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

#include <graphics/graphics.h>
#include <common/Fsm.h>
#include <vector>

class BjFsmData;

class CfgEntry
{
public:

    /**
     *
     */
    CfgEntry(const mxgui::Point startPoint, const std::string& label,
             float& refValue);

    /**
     *
     */
    ~CfgEntry();

    /**
     *
     */
    bool update(mxgui::Event& e, mxgui::DrawingContext& dc);

private:

    static constexpr int spacing   = 15;
    static constexpr int btnWidth  = 50;
    static constexpr int btnHeight = 30;

    mxgui::Point      startPoint;
    mxgui::Point      textPoint;
    const std::string label;
    float&            value;
};

/**
 * FSM state for Bell-Jar controller UI: configurations page.
 */
class BjConfig : public FsmState
{
public:

    /**
     * Constructor.
     * @param fsm: pointer to FSM data structure.
     */
    BjConfig(BjFsmData* fsm);

    /**
     * Destructor.
     */
    virtual ~BjConfig();

    /**
     * Function to be called on state enter.
     */
    virtual void enter() override;

    /**
     * State update function, to be alled periodically.
     * @return pointer to next state or nullptr if no state transition is
     * required.
     */
    virtual FsmState *update();

    /**
     * Function to be called on state exit.
     */
    virtual void leave() override;

private:

    static constexpr unsigned int spacing   = 10;
    static constexpr unsigned int btnSpace  = 20;
    static constexpr unsigned int btnWidth  = 30;
    static constexpr unsigned int btnHeight = 30;

    int valueToChange;
    std::vector< std::unique_ptr< CfgEntry > > entries;
    std::unique_ptr< Button > ret;
    std::unique_ptr< Button > max;
    std::unique_ptr< Button > zero;

    BjFsmData* fsm;
};