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

#include <drivers/display_stm32.h>
#include <miosix.h>
#include <memory>
#include "ActiveObject.h"

class FsmInfo;

/**
 * Base class providing an interface for a generic FSM state. When implementing
 * an FSM, all the classes defining states derive from this class.
 */
class FsmState
{
public:

    /**
     * Constructor.
     */
    FsmState() { }

    /**
     * Destructor.
     */
    virtual ~FsmState() { }

    /**
     * Function called when the FSM enters this state.
     */
    virtual void enter() = 0;

    /**
     * Function called at each FSM update step, on the currently active state.
     * @return a pointer to the next state, if a transition occurs.
     */
    virtual FsmState *update() = 0;

    /**
     * Function called when the FSM leaves this state.
     */
    virtual void leave() = 0;
};

/**
 * Active object class which runs a generic FSM, whose states are defined by
 * class FsmState.
 */
class Fsm : public ActiveObject
{
public:

    /**
     * Constructor.
     * @param initState: pointer to the initial state.
     * @param timeStep: time step for update rate, in milliseconds.
     */
    Fsm(FsmState *initState, unsigned int timeStep = 0) : curState(initState),
                                                          delay(timeStep)
    {
        curState->enter();
    }

    /**
     * Destructor.
     */
    virtual ~Fsm() { }

private:

    /**
     * Function called by the active object thread, contains the FSM update
     * logic.
     */
    virtual void run() override
    {
        while(!should_stop)
        {
            if(curState == nullptr)
            {
                should_stop = true;
                break;
            }

            FsmState *nxtState = curState->update();
            #ifdef BJ_BOARD
            DisplayStm32::instance().render();
            #endif
            if(nxtState != nullptr)
            {
                curState->leave();
                curState = nxtState;
                curState->enter();
            }

            miosix::Thread::sleep(delay);
        }
    }

    FsmState *curState;    ///< Pointer to current FSM state.
    unsigned int delay;    ///< Delay to fix the update rate, in milliseconds.
};
