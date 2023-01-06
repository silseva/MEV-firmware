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

#include <graphics/graphics.h>
#include <common/Fsm.h>
#include <vector>

class BedFsmData;

/**
 * FSM state for Bell-Jar controller UI: main view.
 */
class BedSetupPage : public FsmState
{
public:

    /**
     * Constructor.
     * @param fsm: pointer to FSM data structure.
     */
    BedSetupPage(BedFsmData* fsm);

    /**
     * Destructor.
     */
    virtual ~BedSetupPage();

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

    static constexpr unsigned int leftMargin = 10;
    static constexpr unsigned int topMargin  = 30;
    static constexpr unsigned int btnSpace   = 10;
    static constexpr unsigned int btnHeight  = 40;
    static constexpr unsigned int sbHeight   = 130;
    static constexpr mxgui::Color sbBgColor  = mxgui::white;
    static constexpr mxgui::Color sbLabColor = mxgui::black;
    static constexpr mxgui::Color uiBkgColor = mxgui::grey;

    std::unique_ptr< Button > setTin;
    std::unique_ptr< Button > setIE;
    std::unique_ptr< Button > setFs;
    std::unique_ptr< Button > ret;

    BedFsmData* fsm;
};
