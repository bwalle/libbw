/*
 * Copyright (c) 2008-2009, Bernhard Walle <bernhard@bwalle.de>
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
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <string>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <string.h>

using std::string;
using std::vector;
using std::malloc;

/* ---------------------------------------------------------------------------------------------- */
string strip(string a, const string &chars_to_strip)
{
    if (a.length() == 0)
        return a;

    a.erase(0, a.find_first_not_of(chars_to_strip.c_str(), 0));
    a.erase(a.find_last_not_of(chars_to_strip.c_str())+1);

    return a;
}

/* ---------------------------------------------------------------------------------------------- */
string stripl(string a)
{
    if (a.length() == 0)
        return a;

    a.erase(0, a.find_first_not_of("\n \t", 0));

    return a;
}

/* ---------------------------------------------------------------------------------------------- */
string stripr(string a)
{
    if (a.length() == 0)
        return a;

    a.erase(a.find_last_not_of("\n \t")+1);

    return a;
}

/* ---------------------------------------------------------------------------------------------- */
bool startsWith(const string &str, const string &start, bool casesensitive)
{
    size_t len = start.size();
    if (str.size() < len) {
        return false;
    }

    if (casesensitive) {
        return str.substr(0, len) == start;
    } else {
        return strcasecmp(str.substr(0, len).c_str(), start.c_str()) == 0;
    }
}

/* ---------------------------------------------------------------------------------------------- */
string getRest(const string &str, const string &prefix)
{
    return str.substr(prefix.size(), str.size() - prefix.size());
}

/* ---------------------------------------------------------------------------------------------- */
char **stringvector_to_array(const vector<string> &vec)
{
    if (vec.size() == 0)
        return NULL;

    /* use malloc to be able to free the result with C free() */
    char **ret = (char **)malloc(sizeof(char *) * (vec.size()+1) );

    char **cur = ret;
    for (vector<string>::const_iterator it = vec.begin(); it != vec.end(); ++it)
        *cur++ = strdup((*it).c_str());
    *cur = NULL;

    return ret;
}

/* ---------------------------------------------------------------------------------------------- */
vector<string> stringsplit(const string &str, const string &pattern)
{
    vector<string> retval;
    string s = str;

    string::size_type pos;
    while ((pos = s.find(pattern)) != string::npos) {
        string item = s.substr(0, pos);
        retval.push_back(item);

        s = s.substr(pos + pattern.length());
    }

    if (s != "") {
        retval.push_back(s);
    }

    return retval;
}

// :tabSize=4:indentSize=4:noTabs=true:mode=c++:folding=explicit:collapseFolds=1:maxLineLen=100:
