/* {{{
 * Copyright (c) 2011, Bernhard Walle <bernhard@bwalle.de>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY <copyright holder> ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. }}}
 */
#ifndef MUTEX_H
#define MUTEX_H

#include "../noncopyable.h"
#include "../bwerror.h"

/**
 * \file mutex.h
 * \brief Portable mutex class
 *
 * This file contains a mutex class that aims to provide a portable
 * interface.
 *
 * \author Bernhard Walle <bernhard@bwalle.de>
 */

#include <stdexcept>

namespace bw {
namespace thread {

struct MutexPrivate;
class WaitCond;

/* Mutex {{{ */

/**
 * \class Mutex
 * \brief Portable mutex class
 *
 * A simple mutex to be used to protect data. Only one thread of execution can
 * lock a mutex. If the mutex is already locked, the process waits until the
 * other thread releases the mutex.
 *
 * The mutex can be only used in the same application and is not suitable so
 * synchronize different processes.
 *
 * \author Bernhard Walle <bernhard@bwalle.de>
 * \ingroup thread
 */
class Mutex : private ::bw::Noncopyable {

    public:
        friend class WaitCond;

    public:
        /**
         * \brief Constructor
         *
         * \exception Error on a system error like if there are too much mutexes in the system.
         */
        Mutex()
        throw (Error);

        /**
         * \brief Destructor
         */
        virtual ~Mutex();

    public:

        /**
         * \brief Locks the current mutex
         *
         * If the mutex is already locked, the calling thread will block until
         * the mutex becomes available.
         * 
         * \sa tryLock()
         * \exception Error if a deadlock would occur if the thread blocked
         *            waiting for mutex.
         */
        void lock()
        throw (Error);

        /**
         * \brief Trys to lock the current mutex
         *
         * The tryLock() function locks mutex. If the mutex is already locked,
         * tryLock() will not block waiting for the mutex, but will return \c false.
         *
         * \return \c true if the mutex has been locked successfully, \c false
         *         if the mutex is already locked
         * \sa lock()
         *
         * \exception on a system error
         */
        bool tryLock()
        throw (Error);

        /**
         * \brief Unlocks the current mutex
         *
         * If the current thread holds the lock on mutex, then the unlock()
         * function unlocks mutex.
         *
         * Calling unlock() with a mutex that the calling thread does not hold
         * will result in undefined behavior.
         *
         * \exception Error if the mutex isn't locked
         */
        void unlock()
        throw (Error);



    private:
        MutexPrivate *m_d;
};

/* }}} */

} // end namespace thread
} // end namespace bw

#endif // MUTEX_H
