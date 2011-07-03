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
#include <cstdlib>

#include <unistd.h>
#include <sys/wait.h>

#include "os.h"

namespace bw {

/* ---------------------------------------------------------------------------------------------- */
int daemonize()
{
    return daemon(false, false);
}

/* ---------------------------------------------------------------------------------------------- */
int system(const std::string &process, const std::vector<std::string> &args)
{
    pid_t child = fork();
    if (child == 0) {
        const char *argVector[args.size() + 1];

        for (size_t i = 0; i < args.size(); ++i)
            argVector[i] = args[i].c_str();
        argVector[args.size()] = NULL;

        int ret = execvp(process.c_str(), const_cast<char **>(argVector));
        // exit the child
        if (ret < 0)
            std::exit(-1);
    } else if (child > 0) {
        int rc;
        int ret = waitpid(child, &rc, 0);
        if (ret < 0)
            return -1;
        else
            return WEXITSTATUS(rc);
    }
    return -1;
}

} // end namespace bw

// vim: set sw=4 ts=4 et fdm=marker:
