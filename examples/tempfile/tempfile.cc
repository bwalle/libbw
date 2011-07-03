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
#include <cstring>

#include <libbw/io/tempfile.h>

#define APPNAME "tempfile"

/* ---------------------------------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
    bw::io::TempFile noDelete(APPNAME);
    std::cout << "Created " << noDelete.name()
              << " with fd " << noDelete.nativeHandle()
              << std::endl;
    write(noDelete.nativeHandle(), "Test\n", std::strlen("Test\n"));

    bw::io::TempFile deleteOnClose(APPNAME, bw::io::TempFile::DeleteOnClose);
    std::cout << "Created " << deleteOnClose.name()
              << " with fd " << deleteOnClose.nativeHandle()
              << " with flag DeleteOnClose"
              << std::endl;
    deleteOnClose.close();

    bw::io::TempFile deleteOnExit(APPNAME, bw::io::TempFile::DeleteOnExit);
    std::cout << "Created " << deleteOnExit.name()
              << " with fd " << deleteOnExit.nativeHandle()
              << " with flag DeleteOnExit"
              << std::endl;
    deleteOnExit.close();

    return EXIT_SUCCESS;
}
