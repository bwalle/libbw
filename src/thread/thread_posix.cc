/* {{{
 * Copyright (c) 2008-2010, Bernhard Walle <bernhard@bwalle.de>
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

#include "thread.h"

namespace bw {
namespace thread {

/* ThreadPrivate {{{ */

struct ThreadPrivate {

    ThreadPrivate(Thread *th)
        : containing_thread(th) {}

    static void *run(void *cookie)
    {
        ThreadPrivate *self = static_cast<ThreadPrivate *>(cookie);

        self->containing_thread->run();
        return NULL;
    }

    pthread_t thread;
    Thread  *containing_thread;
};

/* }}} */
/* Thread {{{ */

/* ---------------------------------------------------------------------------------------------- */
Thread::Thread()
    : m_d(new ThreadPrivate(this) )
    , m_shouldStop(false)
{}

/* ---------------------------------------------------------------------------------------------- */
Thread::~Thread()
{
    delete m_d;
}

/* ---------------------------------------------------------------------------------------------- */
void Thread::start(int flags)
    throw (Error)
{
    pthread_attr_t attr;
    int err = pthread_attr_init(&attr);
    if (err != 0)
        throw SystemError("Unable to call pthread_attr_init()", err);

    if (flags & DETACHED) {
        err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        if (err != 0)
            throw SystemError("Unable to call pthread_attr_setdetachstate()", err);
    }

    err = pthread_create(&m_d->thread, NULL, ThreadPrivate::run, m_d);
    if (err != 0) {
        pthread_attr_destroy(&attr);
        throw SystemError("Unable to call pthread_create()", err);
    }

    err = pthread_attr_destroy(&attr);
    if (err != 0)
        throw SystemError("Unable to call pthread_attr_destroy()", err);
}

/* ---------------------------------------------------------------------------------------------- */
void Thread::adviseToStop()
{
    m_shouldStop = true;
}

/* ---------------------------------------------------------------------------------------------- */
void Thread::join()
    throw (Error)
{
    int err = pthread_join(m_d->thread, NULL);
    if (err != 0)
        throw SystemError("Unable to call pthread_join()", err);
}

/* }}} */

} // end namespace thread
} // end namespace bw

// vim: set sw=4 ts=4 et fdm=marker:
