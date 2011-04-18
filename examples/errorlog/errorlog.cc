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
#include <iostream>
#include <cstdlib>

#include <errorlog.h>

/* ---------------------------------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: errorlog <filename | 'syslog'>" << std::endl;
        return EXIT_FAILURE;
    }

    if (std::strcmp(argv[1], "syslog") == 0)
        bw::Errorlog::configure(bw::Errorlog::LM_SYSLOG, "errorlog");
    else
        bw::Errorlog::configure(bw::Errorlog::LM_FILE, argv[1]);

    BW_ERROR(bw::Errorlog::LS_ALERT, "Alert message: %d", 5);
    BW_ERROR_EMERG("Emerg message: %d", 5);
    BW_ERROR_ALERT("Alert message: %d", 5);
    BW_ERROR_CRIT("Critical message: %d", 5);
    BW_ERROR_WARNING("Emerg message: %d", 5);
    BW_ERROR_EMERG("Emerg message: %d", 5);

    return EXIT_SUCCESS;
}
