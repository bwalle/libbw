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
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <cstdio>
#include <fstream>
#include <cerrno>
#include <cstring>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

#include "serialfile.h"
#include "serialfile_private_posix.h"

namespace bw {
namespace io {

/* Module-static stuff (lock file deletion) {{{ */

/// Set to @c true after deleteFilesAtExit() has been registered as delete
/// handler.
static bool s_delete_handler_registered = false;

/// Vector of files which should be deleted at exit.
static std::vector<std::string> s_files_to_delete;

/**
 * @brief Helper function which deletes a file
 *
 * This helper function just converts @p fileName to a const char * string and then calls
 * std::remove().
 *
 * @param[in] fileName the file name to delete
 */
static void delete_file(const std::string &file)
{
    std::remove(file.c_str());
}

/**
 * @brief Deletes the lock file at exit
 *
 * This function should be registered as exit handler. It deletes all files which have been
 * added to the module-global s_filesToDelete vector. After the delete handler has been registered,
 * set s_deleteHandlerRegistered to true.
 */
static void delete_files_at_exit()
{
    std::for_each(
        s_files_to_delete.begin(),
        s_files_to_delete.end(),
        delete_file
    );
}

/**
 * @brief Computes the name of the lock file from the portName
 *
 * @param[in] portName the full path to the port that should be opened, e.g.
 *            /dev/ttyS0.
 * @return the full path to the lock file name and an empty string if
 *         no lock file should be created for @p portName
 */
static std::string computeLockFileName(const std::string &portName)
{
    std::string absolutePort = ::realpath(portName.c_str(), NULL);
    std::string ret;

    if (absolutePort.size() > 5 && absolutePort.substr(0, 5) == "/dev/")
        ret = "/var/lock/LCK.." + absolutePort.substr(5, absolutePort.size());

    return ret;
}

/* }}} */
/* SerialFile {{{ */

/* ---------------------------------------------------------------------------------------------- */
bool SerialFile::createLock()
{
    if (!s_delete_handler_registered) {
        std::atexit(delete_files_at_exit);
        s_delete_handler_registered = true;
    }

    std::string lockfile = computeLockFileName(d->fileName);

    // if we don't need locking, everything is sane
    if (lockfile.empty())
        return true;

    // if the lock file is present, we failed to lock
    if (::access(lockfile.c_str(), F_OK) == 0)
        return false;

    // the race condition here is not important because the lock file is just
    // there to warn the user that another process is using the port, it's not
    // about security

    // now we can create the lock
    std::ofstream lock(lockfile.c_str());
    if (lock)
        lock << ::getpid() << std::endl;
    s_files_to_delete.push_back(lockfile);
}

/* ---------------------------------------------------------------------------------------------- */
void SerialFile::removeLock()
{
    std::string lockfile = computeLockFileName(d->fileName);

    if (lockfile.empty())
        return;

    std::remove(lockfile.c_str());

    std::vector<std::string>::iterator result = std::find(
        s_files_to_delete.begin(),
        s_files_to_delete.end(),
        lockfile);
    if (result != s_files_to_delete.end())
        s_files_to_delete.erase(result);
}

/* }}} */

} // end namespace io
} // end namespace bw
