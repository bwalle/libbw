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
#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <limits>

#include "../noncopyable.h"
#include "../bwerror.h"

/**
 * \file semaphore.h
 * \brief Portable semaphore class
 *
 * This file contains a semaphore class that aims to provide a portable
 * interface.
 *
 * \author Bernhard Walle <bernhard@bwalle.de>
 */

#include <stdexcept>

namespace bw {
namespace thread {

struct SemaphorePrivate;

/* Semaphore {{{ */

/**
 * \class Semaphore
 * \brief Portable semaphore class
 *
 * A semaphore is a generalization of a mutex. While a mutex can only be
 * locked once, it's possible to acquire a semaphore multiple times.
 * Semaphores are typically used to protect a certain number of identical
 * resources.
 *
 * \author Bernhard Walle <bernhard@bwalle.de>
 * \ingroup thread
 */
class Semaphore : private ::bw::Noncopyable {

    public:
        /**
         * \brief Constructor
         *
         * \param[in] n the initial value of the seamphore
         */
        Semaphore(int n = 0);

        /**
         * \brief Destructor
         */
        virtual ~Semaphore();

    public:

        /**
         * \brief Wait for a given semaphore
         *
         * wait() decrements (locks) the semaphore. If the semaphore's value
         * is greater than zero, then the decrement proceeds, and the function
         * returns, immediately. If the semaphore currently has the value
         * zero, then the call blocks until either it becomes possible to
         * perform the decrement (i.e., the semaphore value rises above zero).
         *
         * \param[in] timeout if not std::numeric_limits<unsigned int>::max(),
         * \return \c false on timeout, \c true if the semaphore has been
         *         locked successfully.
         * \sa tryWait()
         * 
         * \exception Error on a system error
         */
        bool wait(unsigned int timeout = std::numeric_limits<unsigned int>::max())
        throw (Error);

        /**
         * \brief Trys to wait for a given semaphore
         *
         * Like wait() but instead of blocking the function returns with \c
         * false.
         *
         * \return \c true if the semaphore has been locked, \c false otherwise.
         * \sa wait()
         *
         * \exception Error on a system error
         */
        bool tryWait()
        throw (Error);

        /**
         * \brief Unlocks the given semaphore
         *
         * post() increments (unlocks) the semaphore. If the semaphore's value
         * consequently becomes greater than zero, then another process or
         * thread blocked in a wait() call will be woken up and proceed to
         * lock the semaphore.
         *
         * \exception Error on a system error
         */
        void post()
        throw (Error);

    private:
        SemaphorePrivate *m_d;
};

/* }}} */

} // end namespace thread
} // end namespace bw

#endif // SEMAPHORE_H
