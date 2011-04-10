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
#include <semaphore.h>
#include <sys/time.h>

#include "semaphore.h"

namespace bw {
namespace thread {

/* SemaphorePrivate {{{ */

struct SemaphorePrivate {
    sem_t semaphore;
};

/* }}} */
/* Semaphore {{{ */

/* ---------------------------------------------------------------------------------------------- */
Semaphore::Semaphore(int n)
    : m_d(new SemaphorePrivate())
{
    sem_init(&m_d->semaphore, 0, n);
}

/* ---------------------------------------------------------------------------------------------- */
Semaphore::~Semaphore()
{
    sem_destroy(&m_d->semaphore);
}

/* ---------------------------------------------------------------------------------------------- */
bool Semaphore::wait(unsigned int timeout)
    throw (Error)
{
    bool hasTimeout = (timeout == std::numeric_limits<unsigned int>::max());
    int err;

    if (hasTimeout) {
        struct timespec abstime;
        struct timeval tv;

        /* 
         * clock_gettime() would provide struct timespec directly, but 
         * isn't present on Mac OS
         */

        err = gettimeofday(&tv, NULL);
        if (err < 0)
            throw SystemError("Unable to call gettimeofday()", errno);

        abstime.tv_sec = tv.tv_sec;
        abstime.tv_nsec = tv.tv_usec * 1000;

        abstime.tv_sec += timeout / 1000;
        if ( (abstime.tv_nsec + (timeout % 1000) * 1000000) > 1000000000 )
            abstime.tv_sec ++;
        abstime.tv_nsec += (timeout % 1000) * 1000 * 1000;

        err = sem_timedwait(&m_d->semaphore, &abstime);
        if (err == ETIMEDOUT)
            return false;
    } else
        err = sem_wait(&m_d->semaphore);

    if (err < 0)
        throw SystemError("Unable to call sem_wait()/sem_timedwait()", errno);

    return true;
}

/* ---------------------------------------------------------------------------------------------- */
bool Semaphore::tryWait()
    throw (Error)
{
    int err = sem_trywait(&m_d->semaphore);
    if (err < 0)
        if (errno == EAGAIN)
            return false;
        else
            throw SystemError("Unable to call sem_trywait()", errno);
    else
        return true;
}

/* ---------------------------------------------------------------------------------------------- */
void Semaphore::post()
    throw (Error)
{
    int err = sem_post(&m_d->semaphore);
    if (err < 0)
        throw SystemError("Unable to call sem_post()", errno);
}

/* }}} */

} // end namespace thread
} // end namespace bw
