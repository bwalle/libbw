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
#include <cstring>
#include <cassert>
#include <cstdio>

#include "datetime.h"
#include "bwconfig.h"

namespace bw {

/* gmtime_r() / localtime_r() {{{ */

// -------------------------------------------------------------------------------------------------
#ifndef HAVE_GMTIME_R
struct tm *gmtime_r(const time_t *timep, struct tm *result)
{
    std::assert(timep != NULL);
    std::assert(result != NULL);

    *result = *(gmtime(timep));
    return result;
}
#endif // HAVE_GMTIME_R

// -------------------------------------------------------------------------------------------------
#ifndef HAVE_LOCALTIME_R
struct tm *localtime_r(const time_t *timep, struct tm *result)
{
    std::assert(timep != NULL);
    std::assert(result != NULL);

    *result = *(localtime(timep));
    return result;
}
#endif // HAVE_LOCALTIME_R

/* }}} */
/* Datetime {{{ */

// -------------------------------------------------------------------------------------------------
Datetime::Datetime(const time_t &time)
    : m_time(time)
    , m_useUtc(false)
{
    fillTime();
}

// -------------------------------------------------------------------------------------------------
Datetime Datetime::now()
{
    return Datetime( std::time(NULL) );
}

// -------------------------------------------------------------------------------------------------
time_t Datetime::timestamp() const
{
    return m_time;
}

// -------------------------------------------------------------------------------------------------
bool Datetime::useUtc() const
{
    return m_useUtc;
}

// -------------------------------------------------------------------------------------------------
void Datetime::setUseUtc(bool use_utc)
{
    m_useUtc = use_utc;
    fillTime();
}

// -------------------------------------------------------------------------------------------------
int Datetime::day() const
{
    return m_tm.tm_mday;
}

// -------------------------------------------------------------------------------------------------
int Datetime::month() const
{
    return m_tm.tm_mon + 1;
}

// -------------------------------------------------------------------------------------------------
int Datetime::year() const
{
    return m_tm.tm_year + 1900;
}

// -------------------------------------------------------------------------------------------------
int Datetime::hour() const
{
    return m_tm.tm_hour;
}

// -------------------------------------------------------------------------------------------------
int Datetime::minute() const
{
    return m_tm.tm_min;
}

// -------------------------------------------------------------------------------------------------
int Datetime::second() const
{
    return m_tm.tm_sec;
}

// -------------------------------------------------------------------------------------------------
Datetime &Datetime::addDays(int days)
{
    return addSeconds(days * 24 * 60 * 60);
}

// -------------------------------------------------------------------------------------------------
Datetime &Datetime::addHours(int hours)
{
    return addSeconds(hours * 60 * 60);
}

// -------------------------------------------------------------------------------------------------
Datetime &Datetime::addMinutes(int minutes)
{
    return addSeconds(minutes * 60);
}

// -------------------------------------------------------------------------------------------------
Datetime &Datetime::addSeconds(int secs)
{
    m_time += secs;
    fillTime();

    return *this;
}

// -------------------------------------------------------------------------------------------------
std::string Datetime::str() const
{
    char buffer[80];
    std::sprintf(buffer, "%04d-%02d-%02d %02d:%02d:%02d",
             year(), month(), day(), hour(), minute(), second());

    return std::string(buffer);
}

// -------------------------------------------------------------------------------------------------
long long Datetime::secsTo(const Datetime &time) const
{
    double diff = difftime(time.m_time, m_time);
    return static_cast<long long>(diff);
}

// -------------------------------------------------------------------------------------------------
bool Datetime::operator==(const Datetime &other)
{
    return m_time == other.m_time;
}

// -------------------------------------------------------------------------------------------------
bool Datetime::operator!=(const Datetime &other)
{
    return m_time != other.m_time;
}

// -------------------------------------------------------------------------------------------------
bool Datetime::operator<(const Datetime &other)
{
    return m_time < other.m_time;
}

// -------------------------------------------------------------------------------------------------
bool Datetime::operator<=(const Datetime &other)
{
    return m_time <= other.m_time;
}

// -------------------------------------------------------------------------------------------------
bool Datetime::operator>(const Datetime &other)
{
    return m_time > other.m_time;
}

// -------------------------------------------------------------------------------------------------
bool Datetime::operator>=(const Datetime &other)
{
    return m_time >= other.m_time;
}

// -------------------------------------------------------------------------------------------------
void Datetime::fillTime()
{
    if (m_useUtc)
        gmtime_r(&m_time, &m_tm);
    else
        localtime_r(&m_time, &m_tm);
}

// -------------------------------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &os, const Datetime &datetime)
{
    os << datetime.str();
    return os;
}

/* }}} */

} // end namespace bw

// vim: set sw=4 ts=4 et fdm=marker:
