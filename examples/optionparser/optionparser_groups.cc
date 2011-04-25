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
#include <csignal>

#include <optionparser.h>

const std::string PACKAGE_STRING("program");
const std::string PACKAGE_VERSION("0.1.0");

/* ---------------------------------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
    bw::OptionGroup generalGroup("General options");
    generalGroup.addOption("help", 'h', bw::OT_FLAG, "Shows this help output");
    generalGroup.addOption("version", 'v', bw::OT_FLAG, "Prints the program version");

    bw::OptionGroup debugGroup("Debug options");
    debugGroup.addOption("debug", 'D', bw::OT_FLAG, "Enable debugging output");
    debugGroup.addOption("debug-file", 'd', bw::OT_STRING, "Redirect debugging output to a file");

    bw::OptionParser op;
    op.addOptions(generalGroup);
    op.addOptions(debugGroup);

    // do the parsing
    bool ret = op.parse(argc, argv);
    if (!ret)
        return EXIT_FAILURE;

    //
    // general options
    //

    // help
    if (op.getValue("help").getFlag()) {
        op.printHelp(std::cerr, PACKAGE_STRING + " " + PACKAGE_VERSION);
        return EXIT_SUCCESS;
    }

    // version
    if (op.getValue("version").getType() != bw::OT_INVALID) {
        std::cout << PACKAGE_STRING + " " + PACKAGE_VERSION << std::endl;
        return EXIT_SUCCESS;
    }

    //
    // debug options
    //

    // debug
    if (op.getValue("debug").getFlag())
        std::cout << "Debug enabled." << std::endl;

    // debug-file
    if (op.getValue("debug-file").getType() != bw::OT_INVALID)
        std::cout << "Redirection of debug output to file: "
                  << op.getValue("debug-file").getString() << std::endl;

    std::vector<std::string> args = op.getArgs();

    if (!args.empty()) {
        std::cout << "Arguments: " << std::endl;
        for (int i = 0; i < args.size(); ++i)
            std::cout << i << ":\t" << args[i] << std::endl;
    }

    return EXIT_SUCCESS;
}
