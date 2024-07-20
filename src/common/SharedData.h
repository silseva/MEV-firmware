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

#include <miosix.h>
#include <stdexcept>

/**
 * Quick and dirty implementation of a kinda-sorta llvm::expected used to manage
 * references to miosix Mutex. This allows to provide a mechanism to return a
 * boolean outcome and a valid parameter only if the outcome is positive.
 */
class ExpectedMutex
{
public:

    /**
     * Constructor, creates an object carrying a valid element.
     * @param m: reference to the mutex carried by the object being created.
     */
    ExpectedMutex(const miosix::Mutex& m) : m(m), isValid(true), checked(false)
                                            { }

    /**
     * Constructor, creates an object carrying an invalid element.
     */
    ExpectedMutex() : m(*static_cast< miosix::Mutex *>(0)), isValid(false),
                      checked(false) { }

    /**
     * Destructor.
     */
    ~ExpectedMutex() { }


    /**
     * Check if the object is carrying a valid element.
     * @return true if the carried element is valid, false otherwise.
     */
    bool valid()
    {
        checked = true;
        return isValid;
    }

    /**
     * Get the carried element.
     * A std::runtime_error error exception is thrown either if this function
     * is called before checking the validity or if the objec carries an invalid
     * element.
     * @return the carried mutex reference.
     */
    const miosix::Mutex& get() const
    {

        if(!checked) throw std::runtime_error("ExpectedMutex get() called \
                                               before checking");
        if(!isValid) throw std::runtime_error("Invalid ExpectedMutex");
        return m;
    }

    /**
     * Overloading of boolean operator, allows to check if the object is
     * carrying a valid element.
     * @return true if the carried element is valid, false otherwise.
     */
    operator bool()
    {
        checked = true;
        return isValid;
    }

private:

    const miosix::Mutex& m;
    bool isValid;
    bool checked;
};

/**
 * Generic template class to manage any kind of data shared beetween threads.
 * Provides a safe locking mechanism to ensure that only one thread at a time
 * can obtain access to the shared data.
 */
template< class T >
class SharedData
{
public:

    /**
     * Constructor.
     */
    SharedData() { }

    /**
     * Destructor.
     */
    ~SharedData() { }

    /**
     * Lock access to the shared data. In case the underlying mutex is already
     * in use, this function blocks the execution flow until the mutex is
     * acquired by the calling thread.
     * @return reference to the mutex.
     */
    miosix::Mutex& lockAccess()
    {
        mutex.lock();
        return mutex;
    }

    /**
     * Try locking access to the shared data. This function does not block the
     * execution flow even when the underlying mutex is already in use.
     * @return an object of type ExpectedMutex.
     */
    ExpectedMutex tryLockAccess()
    {
        if(!mutex.tryLock())
        {
            ExpectedMutex invalid;
            return invalid;
        }

        ExpectedMutex valid(mutex);
        return valid;
    }

    /**
     * Access shared data. Access is allowed only after the calling thread
     * locked the mutex. Throws a std::runtime_error exception if access is
     * tempted with an unlocked mutex.
     * @param m: previously locked mutex, obtained by calling lockAccess() or
     *           tryLockAccess().
     * @return reference to the managed data.
     */
    T& getData(miosix::Mutex& m)
    {
        if(m.tryLock())
        {
            m.unlock();
            throw std::runtime_error("Access to getData() with wrong mutex");
        }

        return data;
    }

    /**
     * Unlock access to the shared data. Throws a std::runtime_error exception
     * if the function is called with an already unlocked mutex.
     */
    void unlockAccess(miosix::Mutex& m)
    {
        if(m.tryLock())
        {
            throw std::runtime_error("Unlocking wrong mutex");
        }

        m.unlock();
    }

private:

    T data;
    miosix::Mutex mutex;
};
