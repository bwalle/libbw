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
#ifndef THREAD_H
#define THREAD_H

#include "../noncopyable.h"
#include "../bwerror.h"

/**
 * \file thread.h
 * \brief Portable thread class
 *
 * This file contains a thread class that aims to provide a portable
 * interface.
 *
 * \author Bernhard Walle <bernhard@bwalle.de>
 */

#include <stdexcept>

namespace bw {
namespace thread {

struct ThreadPrivate;

/* Thread {{{ */

/**
 * \class Thread thread/thread.h
 * \brief Portable thread class
 *
 * This thread class follows the Java and Qt approach to implement a thread:
 * One derives a custom class from bw::util::Thread and reimplement the
 * run() method. For example:
 *
 * \include thread/thread.cc
 *
 * \author Bernhard Walle <bernhard@bwalle.de>
 * \ingroup thread
 */
class Thread : private ::bw::Noncopyable {

    public:
        friend class ThreadPrivate;

    protected:
        /**
         * \brief Constructor
         */
        Thread();

        /**
         * \brief Destructor
         */
        virtual ~Thread();

    public:
        /**
         * \brief Starts the thread
         *
         * This executes the run() method in another, newly created thread.
         *
         * \exception bw::Error if starting the thread failed.
         */
        void start()
        throw (Error);

        /**
         * \brief Asks the thread to stop
         *
         * The thread's run() method should check for the "stop" flag
         * from time to time in its execution loop. This flag sets the
         * loop, but it doen't kill the thread.
         */
        void adviseToStop();

        /**
         * \brief Waits for the thread to finish
         *
         * If the thread has already finished, the function just returns.
         * Otherwise it waits until the thread has finished.
         *
         * Note that the join() method \b has to be called to cleanup
         * system resources. Without calling join(), a zombie will exist
         * in the system until the thread has been joined.
         *
         * \exception bw::Error if waiting for the thread failed.
         */
        void join()
        throw (Error);

    protected:

        /**
         * \brief Main function of the thread
         *
         * This function has to be re-implemented by the real thread
         * implementation. Because the function should not be called from
         * outside (that's the purpose of the start() method), the
         * function is protected.
         *
         * The implementation should check for the stop flag by calling
         * shouldStop() from time to time.
         */
        virtual void run()
        throw () = 0;

        /**
         * \brief Checks if the stop flag is set
         *
         * This function should be called by run() from time to time.
         *
         * \return \c true if the thread should stop, \c false otherwise.
         * \see adviseToStop()
         */
        bool shouldStop() const;

    private:
        ThreadPrivate *m_d;
        bool m_shouldStop;
};

/* }}} */

} // end namespace thread
} // end namespace bw

#endif // BWERROR_H

// vim: set sw=4 ts=4 et fdm=marker:
