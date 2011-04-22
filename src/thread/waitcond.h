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
#ifndef WAITCOND_H
#define WAITCOND_H

#include "../noncopyable.h"
#include "../bwerror.h"

/**
 * \file waitcond.h
 * \brief Portable wait condition class
 *
 * This file contains a wait condition class that aims to provide a portable
 * interface.
 *
 * \author Bernhard Walle <bernhard@bwalle.de>
 */

#include <stdexcept>
#include <limits>

namespace bw {
namespace thread {

struct WaitCondPrivate;
class Mutex;

/* WaitCond {{{ */

/**
 * \class WaitCond thread/waitcond.h
 * \brief Portable wait condition class
 *
 * The WaitCond class provides a condition variable for synchronizing threads.
 *
 * WaitCond allows a thread to tell other threads that some sort of condition
 * has been met. One or many threads can block waiting for a WaitCond to set a
 * condition with signal() or broadcast(). Use signal() to wake one randomly
 * selected condition or broadcast() to wake them all.
 *
 * \author Bernhard Walle <bernhard@bwalle.de>
 * \ingroup thread
 */
class WaitCond : private ::bw::Noncopyable {

    public:
        friend class WaitCondPrivate;

    public:
        /**
         * \brief Constructor
         *
         * \exception Error on a system error like if there are too much mutexes in the system.
         */
        WaitCond()
        throw (Error);

        /**
         * \brief Destructor
         */
        virtual ~WaitCond();

    public:

        /**
         * \brief * Releases the locked mutex and waits on the wait condition.
         *
         * The mutex must be initially locked by the calling thread. If mutex
         * is not in a locked state, this function returns immediately.
         *
         * The mutex will be unlocked, and the calling thread will block until
         * either of these conditions is met:
         *
         *  - Another thread signals it using signal() or broadcast(). This
         *    function will return true in this case.
         *  - time milliseconds has elapsed. If time is
         *    <tt>std::numeric_limits<unsigned int>::max()</tt> (the default),
         *    then the wait will never timeout (the event must be signalled).
         *    This function will return false if the wait timed out.
         *
         * The mutex will be returned to the same locked state. This function
         * is provided to allow the atomic transition from the locked state to
         * the wait state.
         *
         * \param[in] mutex the mutex (see the description above)
         * \param[in] timeout the timeout in milliseconds or 
         *            std::numeric_limits<unsigned int>::max() for no timeout
         *            (the default)
         * \return \c false on timeout, \c true on success.
         * \exception Error on any other error
         */
        bool wait(Mutex *mutex, unsigned int timeout = std::numeric_limits<unsigned int>::max())
        throw (Error);

        /**
         * \brief Unblocks at least one waiting thread
         *
         * The signal() function shall unblock at least one of the threads that
         * are blocked on the condition variable cond (if any threads are
         * blocked).
         *
         * If more than one thread is blocked on a condition variable, the
         * scheduling policy shall determine the order in which threads are
         * unblocked.
         */
        void signal();

        /**
         * \brief Unblocks all waiting threads
         *
         * The  broadcast() function shall unblock all threads currently
         * blocked on the specified condition variable cond.
         */
        void broadcast();

    private:
        WaitCondPrivate *m_d;
};

/* }}} */

} // end namespace thread
} // end namespace bw

#endif // WAITCOND_H
