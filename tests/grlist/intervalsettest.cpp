/*  GRAPHITE2 LICENSING

    Copyright 2010, SIL International
    All rights reserved.

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation; either version 2.1 of License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should also have received a copy of the GNU Lesser General Public
    License along with this library in the file named "LICENSE".
    If not, write to the Free Software Foundation, 51 Franklin Street, 
    Suite 500, Boston, MA 02110-1335, USA or visit their web page on the 
    internet at http://www.fsf.org/licenses/lgpl.html.

Alternatively, the contents of this file may be used under the terms of the
Mozilla Public License (http://mozilla.org/MPL) or the GNU General Public
License, as published by the Free Software Foundation, either version 2
of the License or (at your option) any later version.
*/
#include "inc/Intervals.h"
#include <utility>
#include <vector>
#include <cstdio>
#include <cstddef>
#include <cassert>

namespace gr2 = graphite2;

typedef std::pair<float, float> fpair;
typedef std::vector<fpair> fvector;
typedef fvector::iterator ifvector;

void printRanges(const char *pref, gr2::IntervalSet &is)
{
    printf ("%s: [ ", pref);
    for (gr2::IntervalSet::ivtpair s = is.begin(), end = is.end(); s != end; ++s)
        printf("(%.1f, %.1f) ", s->first, s->second);
    printf ("]");
}

int doTest(const char *pref, gr2::IntervalSet &is, fvector &fv)
{
    bool pass = true;
    printRanges(pref, is);
    gr2::IntervalSet::ivtpair s = is.begin(), e = is.end();
    ifvector bs = fv.begin(), be = fv.end();
    for ( ; s != e && bs != be; ++s, ++bs)
    {
        if (*s != *bs)
        {
            pass = false;
            break;
        }
    }
    if (bs != be || s != e)
        pass = false;
    if (pass)
    {
        printf ("\t- pass\n");
        return 0;
    }
    else
    {
        printf ("\t- *** FAIL ***\n");
        return 1;
    }
}

int main(int /*argc*/, char ** /*argv*/)
{
    int res = 0;
    fvector base;
    gr2::IntervalSet test, test2, testl;

    test.clear();
    base.clear();
    test.add(fpair(10., 100.));
    base.push_back(fpair(10., 100.));
    res += doTest("Init test", test, base);

    test.remove(fpair(30., 50.));
    base.clear();
    base.push_back(fpair(10., 30.));
    base.push_back(fpair(50., 100.));
    res += doTest("remove(30,50)", test, base);

    test.remove(fpair(45., 60.));
    base.back() = fpair(60., 100.);
    res += doTest("remove(45,60)", test, base);

    test.remove(fpair(80., 90.));
    base.clear();
    base.push_back(fpair(10., 30.));
    base.push_back(fpair(60., 80.));
    base.push_back(fpair(90., 100.));
    res += doTest("remove(80,90)", test, base);

    test2.clear();
    test2.add(fpair(10., 100.));
    test2.remove(fpair(20., 40.));
    test2.remove(fpair(45., 50.));
    test2.remove(fpair(55., 85.));
    base.clear();
    base.push_back(fpair(10., 20.));
    base.push_back(fpair(40., 45.));
    base.push_back(fpair(50., 55.));
    base.push_back(fpair(85., 100.));
    res += doTest("Init test2", test2, base);
    base.clear();
    base.push_back(fpair(20., 30.));
    base.push_back(fpair(60., 80));
    test.remove(test2);
    res += doTest("test - test2", test, base);

    test.clear();
    test.add(fpair(10., 85.));
    test.remove(fpair(30., 40.));
    base.clear();
    base.push_back(fpair(10., 30.));
    base.push_back(fpair(40., 85.));
    res += doTest("Init test3", test, base);
    test2.clear();
    test2.add(fpair(20., 90.));
    test2.remove(fpair(50., 60.));
    test2.remove(fpair(70., 80.));
    base.clear();
    base.push_back(fpair(20., 50.));
    base.push_back(fpair(60., 70.));
    base.push_back(fpair(80., 90.));
    res += doTest("Init test4", test2, base);
    test.remove(test2);
    base.clear();
    base.push_back(fpair(10., 20.));
    base.push_back(fpair(50., 60.));
    base.push_back(fpair(70., 80.));
    res += doTest("test3 - test4", test, base);
    testl = test.locate(fpair(30., 35.));
    base.clear();
    base.push_back(fpair(-20., -15.));
    base.push_back(fpair(20., 25.));
    base.push_back(fpair(40., 45.));
    res += doTest("locate(30,35)", testl, base);

    test.clear();
    test.remove(fpair(10., 20.));
    base.clear();
    res += doTest("Removing from empty", test, base);

    test.clear();
    test.add(fpair(10., 50.));
    test.add(fpair(20., 30.));
    test.add(fpair(40., 60.));
    test.add(fpair(60., 80.));
    test.add(fpair(90., 90.));
    test.add(fpair(80., 100.));
    base.clear();
    base.push_back(fpair(10., 100.));
    res += doTest("Slow add 10-100", test, base);

    return res;
}

