# {{{
# Copyright (c) 2009-2010, Bernhard Walle <bernhard@bwalle.de>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of the <organization> nor the
#       names of its contributors may be used to endorse or promote products
#       derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY <copyright holder> ''AS IS'' AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL <copyright holder> BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. }}}

# Readline

find_path(READLINE_INCLUDE_DIR readline/readline.h
    NO_DEFAULT_PATH
    PATHS /opt/local/include /usr/local/include /usr/include)
mark_as_advanced(READLINE_INCLUDE_DIR)

find_library(READLINE_LIBRARY readline
    HINTS /opt/local/lib /usr/lib64 /usr/lib)
mark_as_advanced(READLINE_LIBRARY)

if (READLINE_LIBRARY AND READLINE_INCLUDE_DIR)
    include_directories( ${READLINE_INCLUDE_DIR} )
    set (EXTRA_LIBS ${EXTRA_LIBS} ${READLINE_LIBRARY})
    set (HAVE_LIBREADLINE TRUE)

    if (NOT CURSES_LIBRARIES)
        find_package(Curses REQUIRED)
    endif (NOT CURSES_LIBRARIES)

    set (EXTRA_LIBS ${EXTRA_LIBS} ${CURSES_LIBRARIES})
    include_directories( ${CURSES_INCLUDE_DIR} )
endif (READLINE_LIBRARY AND READLINE_INCLUDE_DIR)

# do we have a proper getopt_long() implementation

include(CheckFunctionExists)

set(CMAKE_REQUIRED_DEFINITIONS "-D_GNU_SOURCE=1")
set(CMAKE_REQUIRED_INCLUDES "getopt.h")
check_function_exists("getopt_long" HAVE_GETOPT_LONG)
unset(CMAKE_REQUIRED_DEFINITIONS)
unset(CMAKE_REQUIRED_INCLUDES)

# check for strcasecmp() -> WIN32
check_function_exists("strcasecmp" HAVE_STRCASECMP)
# check for localtime_r() -> thread-safe version of localtime
check_function_exists("localtime_r" HAVE_LOCALTIME_R)
# check for gmtime_r() -> thread-safe version of gmtime
check_function_exists("gmtime_r" HAVE_GMTIME_R)
# check for strftime()
check_function_exists("strftime" HAVE_STRFTIME)

# check for unistd.h and syslog.h
include(CheckIncludeFile)
check_include_file("unistd.h" HAVE_UNISTD_H)
check_include_file("syslog.h" HAVE_SYSLOG)

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
# for bwconfig.h
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

set(LIBBW_SRCS
    stringutil.h
    stringutil.cc
    completion.h
    completion.cc
    bwerror.h
    optionparser.h
    optionparser.cc
    datetime.h
    datetime.cc
    exithandler.cc
)
if (CMAKE_HOST_UNIX)
    set(LIBBW_SRCS
        ${LIBBW_SRCS}
        os_posix.cc
    )
else (CMAKE_HOST_UNIX)
    set(LIBBW_SRCS
        ${LIBBW_SRCS}
        os_generic.cc
    )
endif (CMAKE_HOST_UNIX)

include(io/CMakeLists.txt)
set(LIBBW_SRCS ${LIBBW_SRCS} ${LIBBW_IO_SRCS})

include(log/CMakeLists.txt)
set(LIBBW_SRCS ${LIBBW_SRCS} ${LIBBW_LOG_SRCS})

# link our own version of getopt_long() if the system doesn't provide a
# suitable one
if (NOT HAVE_GETOPT_LONG)
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/ext/getopt")
    set(LIBBW_SRCS
        ${LIBBW_SRCS}
        ext/getopt/getopt.c
        ext/getopt/getopt.h
        ext/getopt/getopt_long.c
    )
endif (NOT HAVE_GETOPT_LONG)

add_library(
    bw
    STATIC
    ${LIBBW_SRCS}
)

target_link_libraries(
    bw
    ${EXTRA_LIBS}
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/bwconfig.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/bwconfig.h"
)

# vim: set sw=4 ts=4 et fdm=marker: