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
#include <iostream>
#include <cstdlib>
#include <csignal>

#include "serialfile.h"

/* ---------------------------------------------------------------------------------------------- */
void dummy_signalhandler(int signal)
{
    (void)signal;

    exit(EXIT_FAILURE);
}

/* ---------------------------------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <port> <baudrate>" << std::endl;
        return EXIT_FAILURE;
    }

    /*
     * this signal handlers are necessary because atexit()-registered functions are only
     * called when the process exits with exit(2).
     */
    std::signal(SIGTERM, dummy_signalhandler);
    std::signal(SIGINT, dummy_signalhandler);

    std::string device = argv[1];
    int baudrate = std::atoi(argv[2]);

    bw::SerialFile serialFile(argv[1]);
    if (!serialFile.openPort()) {
        std::cerr << "Failed to open serial device '" << serialFile << "': "
                  << serialFile.getLastError() << std::endl;
        return EXIT_FAILURE;
    }

    if (!serialFile.reconfigure(baudrate, bw::SerialFile::FC_NONE)) {
        std::cerr << "Failed to set baudrate for serial device '" << serialFile << "': "
                  << serialFile.getLastError() << std::endl;
        return EXIT_FAILURE;
    }

    while (true) {
        std::string s;
        serialFile >> s;
        std::cout << s << std::flush;
    }

    return EXIT_SUCCESS;
}
