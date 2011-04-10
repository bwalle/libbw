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

#include "mutex.h"

namespace bw {
namespace thread {

/* MutexPrivate {{{ */

struct MutexPrivate {
    pthread_mutex_t mutex;
};

/* }}} */
/* Mutex {{{ */

/* ---------------------------------------------------------------------------------------------- */
Mutex::Mutex()
    throw (Error)
    : m_d(new MutexPrivate())
{
    int err = pthread_mutex_init(&m_d->mutex, NULL);
    if (err != 0)
        throw SystemError("Unable to call pthread_mutex_init()", err);
}

/* ---------------------------------------------------------------------------------------------- */
Mutex::~Mutex()
{
    pthread_mutex_destroy(&m_d->mutex);
}

/* ---------------------------------------------------------------------------------------------- */
void Mutex::lock()
    throw (Error)
{
    int err = pthread_mutex_lock(&m_d->mutex);
    if (err != 0)
        throw SystemError("Unable to call pthread_mutex_lock()", err);
}

/* ---------------------------------------------------------------------------------------------- */
bool Mutex::tryLock()
    throw (Error)
{
    int err = pthread_mutex_trylock(&m_d->mutex);
    if (err != 0)
        if (err == EBUSY)
            return false;
        else
            throw SystemError("Unable to call pthread_mutex_trylock()", err);
    else
        return true;
}

/* ---------------------------------------------------------------------------------------------- */
void Mutex::unlock()
    throw (Error)
{
    int err = pthread_mutex_unlock(&m_d->mutex);
    if (err != 0)
        throw SystemError("Unable to call pthread_mutex_unlock()", err);
}

/* }}} */

} // end namespace thread
} // end namespace bw