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
void Semaphore::wait()
    throw (Error)
{
    int err = sem_wait(&m_d->semaphore);
    if (err < 0)
        throw SystemError("Unable to call sem_wait()", errno);
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
