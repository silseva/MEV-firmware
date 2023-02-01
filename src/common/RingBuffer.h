/*
 * MEV board firmware
 * Copyright (C) 2023  Silvano Seva silvano.seva@polimi.it
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

#include <miosix.h>
#include <cstdint>

/**
 * Class implementing a dinamically allocated circular buffer with non-blocking
 * push and pop functions.
 */
template < typename T, size_t N >
class RingBuffer
{
public:

    /**
     * Constructor.
     */
    RingBuffer() : readPos(0), writePos(0), numElements(0)
    {
        data = new T[N];
    }

    /**
     * Destructor.
     */
    ~RingBuffer()
    {
        delete[] data;
    }

    /**
     * Push an element to the buffer.
     *
     * @param elem: element to be pushed.
     * @param erase: if set to true, when the buffer is full this function
     * erases the oldest element in the buffer to make room for the new one.
     * @return true if the element has been successfully pushed to the queue,
     * false if the queue is full.
     */
    bool push(const T& elem, bool erase)
    {
        miosix::Lock< miosix::Mutex > l(mutex);

        if(numElements >= N)
        {
            if(erase)
            {
                eraseElement();
            }
            else
            {
                return false;
            }
        }

        data[writePos] = elem;
        writePos = (writePos + 1) % N;
        numElements += 1;

        return true;
    }

    /**
     * Pop an element from the buffer.
     *
     * @param elem: place where to store the popped element.
     * @return true if the element has been successfully popped from the queue,
     * false if the queue is empty.
     */
    bool pop(T& elem)
    {
        miosix::Lock< miosix::Mutex > l(mutex);

        if(numElements == 0)
            return false;

        elem    = data[readPos];
        readPos = (readPos + 1) % N;
        numElements -= 1;

        return true;
    }

    /**
     * Check if the buffer is empty.
     *
     * @return true if the buffer is empty.
     */
    bool empty()
    {
        return numElements == 0;
    }

    /**
     * Check if the buffer is full.
     *
     * @return true if the buffer is full.
     */
    bool full()
    {
        return numElements >= N;
    }

    size_t porco(){ return numElements; }

    /**
     * Discard one element from the buffer's tail, creating a new empty slot.
     */
    void eraseElement()
    {
        miosix::Lock< miosix::Mutex > l(mutex);

        // Nothing to erase
        if(numElements == 0) return;

        // Chomp away one element just by advancing the read pointer.
        readPos = (readPos + 1) % N;
        numElements -= 1;
    }

private:

    size_t readPos;      ///< Read pointer.
    size_t writePos;     ///< Write pointer.
    size_t numElements;  ///< Number of elements currently present.
    T      *data;        ///< Data storage.

    miosix::Mutex mutex; ///< Mutex for concurrent access.
};
