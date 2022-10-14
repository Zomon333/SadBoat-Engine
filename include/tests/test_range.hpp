/*
Copyright 2022 Dagan Poulin, Justice Guillory
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
       http://www.apache.org/licenses/LICENSE-2.0
   Unless CHECKd by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#ifdef CONFIG_TEST
#ifndef TEST_RANGE
#define TEST_RANGE

#include "../lib/catch.hpp"
#include "../primitives/range.hpp"

using namespace std;
using namespace SBE;

TEST_CASE("inRange test for Range","[range]")
{
    Range testRange = Range(0, 10);

    for(int i = 1; i<10; i++)
    {
        CHECK(testRange.inRange(i));
    }

    for(int i = -1; i>-10; i--)
    {
        CHECK(!testRange.inRange(i));
        CHECK(!testRange.inRange((20+i)));
    }
}

TEST_CASE("onBouds test for Range","[range]")
{
    Range testRange = Range(0, 10);

    CHECK( testRange.onBounds(0) );
    CHECK( testRange.onBounds(10) );

    CHECK( !testRange.onBounds(11) );
    CHECK( !testRange.onBounds(-1) );
    CHECK( !testRange.onBounds(5) );
}

TEST_CASE("outBounds test for Range", "[range]")
{
    Range testRange = Range(0, 10);

    CHECK( testRange.outBounds(11) );
    CHECK( testRange.outBounds(-1) );

    CHECK( !testRange.onBounds(3) );
    CHECK( !testRange.onBounds(7) );
}

TEST_CASE("operator[] test for Range", "[range]")
{
    Range testRange = Range(0, 10);

    for(int i = 1; i<10; i++)
    {
        CHECK( testRange[i] );
    }
}

#endif
#endif