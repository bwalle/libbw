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

#include <unistd.h>

#include <libbw/datetime.h>

/* ---------------------------------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
    bw::Datetime time = bw::Datetime::now();
    std::cout << "Current time (localtime): " << time << std::endl;
    time.setUseUtc(true);
    std::cout << "Current time (UTC): " << time << std::endl;
    time.setUseUtc(true);
    sleep(1);
    bw::Datetime time2 = bw::Datetime::now();
    std::cout << "Current time: " << time2 << std::endl;
    std::cout << "Difference: " << time.secsTo(time2) << " s" << std::endl;

    std::cout << "One day later: " << time2.addDays(1) << std::endl;

    time = bw::Datetime(2011, bw::Datetime::January, 5, 18, 30, 0, false);
    std::cout << "2011-01-05 18:30 is " << time << std::endl;
    time.setUseUtc(true);
    std::cout << "2011-01-05 18:30 is UTC " << time << std::endl;

    time = bw::Datetime(2011, bw::Datetime::January, 5, 18, 30, 0, true);
    std::cout << "2011-01-05 18:30 UTC is " << time << std::endl;

    time = bw::Datetime(2011, bw::Datetime::July, 5, 18, 30, 0, true);
    std::cout << "2011-07-05 18:30 UTC is " << time << std::endl;

    std::cout << "Format: " << time.strftime("%d. %B %Y %H:%M:%S") << std::endl;

    time = bw::Datetime::strptime("2012-02-03 18:35:36", "%Y-%m-%d %H:%M:%S");
    std::cout << "Parsed time: " << time << std::endl;

    return EXIT_SUCCESS;
}
