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
#include "mutex.h"

namespace bw {
namespace thread {

/* MutexLocker {{{ */

/**
 * \class MutexLocker thread/mutexlocker.h
 *
 * \brief Convencience class that simplifies locking and unlocking.
 *
 * The MutexLocker is a class that implements the C++ RAII (ressource allocation is initialisation)
 * idiom for mutexes. The mutex is locked in the constructor and unlocked in the destructor which is
 * invoked when the mutex goes out of scope.
 *
 * Intead of writing
 *
 * \code
 * void MyClass::myfunction()
 * {
 *     m_mutex.lock();
 *
 *     // do something
 *
 *     m_mutex.unlock();
 * }
 * \endcode
 *
 * we can write
 *
 * \code
 * void MyClass::myfunction()
 * {
 *     MutexLocker(&m_mutex);
 *
 *     // do something
 * }
 * \endcode
 *
 * This is shorter and less error-prone because the unlocking takes also place when an exception
 * occurs or when the function returns in the mid of the function.
 *
 * \author Bernhard Walle <bernhard@bwalle.de>
 * \ingroup thread
 */
class MutexLocker {

    public:
        /**
         * \brief Constructor
         *
         * Initializes the MutexLocker object and locks the mutex.
         *
         * \param[in] mutex the mutex that should be locked (and unlocked in the destructor).
         */
        MutexLocker(Mutex *mutex);

        /**
         * \brief Destructor
         *
         * Unlocks the mutex.
         */
        virtual ~MutexLocker();

    private:
        Mutex *m_mutex;
};

/* }}} */

} // end namespace thread
} // end namespace bw

// vim: set sw=4 ts=4 et fdm=marker:
