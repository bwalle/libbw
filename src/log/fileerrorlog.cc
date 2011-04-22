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
#include <ctime>
#include <iostream>
#include <cstring>

#include "fileerrorlog.h"

namespace bw {

/* FileErrorLog {{{ */

/* ---------------------------------------------------------------------------------------------- */
FileErrorlog::FileErrorlog(const char *filename)
    : m_file(NULL)
{
    if (!filename)
        m_file = stderr;
    if (std::strcmp(filename, "stderr") == 0)
        m_file = stderr;
    else if (std::strcmp(filename, "stdout") == 0)
        m_file = stdout;
    else {
        m_file = std::fopen(filename, "a");
        if (!m_file) {
            std::cerr << "Warning: Unable to open '" << filename << "' for writing." << std::endl;
            m_file = stderr;
        }
    }
}

/* ---------------------------------------------------------------------------------------------- */
FileErrorlog::~FileErrorlog()
{
    if (m_closeInDtor)
        std::fclose(m_file);
}

/* ---------------------------------------------------------------------------------------------- */
void FileErrorlog::vlog(Errorlog::Level level, const char *msg, std::va_list args)
{
    fprintf(m_file, "%s [%-10.10s] ", timestamp().c_str(), levelToString(level));
    vfprintf(m_file, msg, args);
    fprintf(m_file, "\n");
    fflush(m_file);
}

/* ---------------------------------------------------------------------------------------------- */
std::string FileErrorlog::timestamp() const
{
    char buffer[80];
    struct tm *t;
    time_t now;

    std::time(&now);
    t = std::localtime(&now);
    snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d %02d:%02d:%02d",
             1900+t->tm_year, t->tm_mon+1, t->tm_mday,
             t->tm_hour, t->tm_min, t->tm_sec);

    return std::string(buffer);
}

/* }}} */

} // end namespace bw

// vim: set sw=4 ts=4 et fdm=marker:
