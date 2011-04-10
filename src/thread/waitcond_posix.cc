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

#include <cerrno>

#include <time.h>

#include "mutex.h"
#include "waitcond.h"
#include "mutex_private_posix.h"

namespace bw {
namespace thread {

/* MutexPrivate {{{ */

struct WaitCondPrivate {
    pthread_cond_t waitcond;
};

/* }}} */
/* WaitCond {{{ */

/* ---------------------------------------------------------------------------------------------- */
WaitCond::WaitCond()
    throw (Error)
    : m_d(new WaitCondPrivate())
{
    int err = pthread_cond_init(&m_d->waitcond, NULL);
    if (err != 0)
        throw SystemError("Unable to call pthread_cond_init()", err);
}

/* ---------------------------------------------------------------------------------------------- */
WaitCond::~WaitCond()
{
    pthread_cond_destroy(&m_d->waitcond);
}

/* ---------------------------------------------------------------------------------------------- */
bool WaitCond::wait(Mutex *mutex, unsigned int timeout)
    throw (Error)
{
    bool hasTimeout = (timeout == std::numeric_limits<unsigned int>::max());
    int err;

    if (hasTimeout) {
        struct timespec abstime;

        err = clock_gettime(CLOCK_REALTIME, &abstime);
        if (err < 0)
            throw SystemError("Unable to call clock_gettime()", errno);

        abstime.tv_sec += timeout / 1000;
        if ( (abstime.tv_nsec + (timeout % 1000) * 1000000) > 1000000000 )
            abstime.tv_sec ++;
        abstime.tv_nsec += (timeout % 1000) * 1000 * 1000;

        err = pthread_cond_timedwait(&m_d->waitcond, &mutex->m_d->mutex, &abstime);
        if (err == ETIMEDOUT)
            return false;
    } else
        err = pthread_cond_wait(&m_d->waitcond, &mutex->m_d->mutex);

    if (err != 0)
        throw SystemError("Unable to call pthread_cond_wait/pthread_cond_timedwait()", err);

    return true;
}

/* ---------------------------------------------------------------------------------------------- */
void WaitCond::signal()
{
    pthread_cond_signal(&m_d->waitcond);
}

/* ---------------------------------------------------------------------------------------------- */
void WaitCond::broadcast()
{
    pthread_cond_broadcast(&m_d->waitcond);
}

/* }}} */

} // end namespace thread
} // end namespace bw
