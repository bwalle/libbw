/* {{{
 * Copyright (c) 2007-2010, Bernhard Walle <bernhard@bwalle.de>
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
#ifndef LIBBW_FILEUTILS_H_
#define LIBBW_FILEUTILS_H_

#include <string>
#include <stdint.h>

#include "bwerror.h"

namespace bw {

/* FileUtils {{{ */

/**
 * \class FileUtils fileutils.h libbw/fileutils.h
 * \brief Contains file system utility functions
 *
 * This class contains static utility functions such as receiving statistics information about
 * a file or creating directories. It will not contain functions to read or write to the file.
 *
 * \author Bernhard Walle <bernhard@bwalle.de>
 * \ingroup os
 */
class FileUtils {

    public:
        /**
         * \brief Returns the size of \p filename
         *
         * On platforms that have large file support, the function supports retrieving the size
         * of files larger than 2 GB on 32 bit platforms.
         *
         * \param[in] filename the name of the file, either absolute or relative to the current
         *            working directory
         * \return the size of the file in bytes which can be also 0
         * \exception Error if the size of the file cannot be determined for example because the
         *            file doesn't exist
         */
        static int64_t size(const std::string &filename)
        throw (Error);

        /**
         * \brief Checks if the \p filename exists
         *
         * The function doesn't check if it's a real file, a directory, a character device or
         * something else. It just says that there exists an object called \p filename in the
         * file system.
         *
         * \param[in] filename the name of the file, either absolute or relative to the current
         *            working directory
         * \return \c true if the file exists, \c false otherwise
         */
        static bool exists(const std::string &filename);

        /**
         * \brief Checks if \p dirname exists and is a directory
         *
         * \param[in] dirname the name of the directory
         * \return \c true if \p dirname exists and is a directory, \c false if it exists
         *         but is not a directory
         * \exception Error if information about \p dirname cannot be retrieved, for example
         *            because \p dirname doesn't exist or because permission is not granted
         */
        static bool isDirectory(const std::string &dirname)
        throw (Error);

        /**
         * Creates a new directory.
         *
         * \warning While the function works on Win32, it accepts only path names separated
         *          by slashes, not backslashes.
         *
         * \param[in] dir the name of the directory that should be created
         * \param[in] recursive \b true if the behaviour of <tt>mkdir -p</tt> should be copied,
         *            \c false otherwise.
         *
         * \throw Error on any error
         */
        static void mkdir(const std::string &dir, bool recursive=false)
        throw (Error);
};

/* }}} */

} // end namespace bw

#endif // LIBBW_FILEUTILS_H_
