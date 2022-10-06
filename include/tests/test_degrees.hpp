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
#ifndef TEST_DEGREES
#define TEST_DEGREES

#include "../utilities/catch.hpp"
#include "../primitives/angles/degrees.hpp"

using namespace std;
using namespace SBE;

#define degrees_suite "[degrees]"

TEST_CASE("Degrees constructor test",degrees_suite)
{
    Degrees deg = Degrees();
    REQUIRE(deg.get()==0);

    deg = Degrees(58);
    CHECK(deg.get()==58);

    deg = Degrees(365);
    CHECK(deg.get()==5);

    deg = Degrees(-47);
    CHECK(deg.get()==313);
}
TEST_CASE("Degrees get and set test",degrees_suite)
{
    Degrees deg = Degrees(55);
    REQUIRE(deg.get()==55);

    deg.setDegrees(60);
    CHECK(deg.get()==60);
}
TEST_CASE("Degrees operator+ and operator-",degrees_suite)
{
    Degrees deg = Degrees(55);
    REQUIRE((deg+5).get()==60);
    REQUIRE((deg-5).get()==50);
    REQUIRE((deg+Degrees(15)).get()==70);
    REQUIRE((deg-Degrees(15)).get()==40);
}
TEST_CASE("Degrees operator+= and operator-=",degrees_suite)
{
    Degrees deg = Degrees(25);
    deg+=5;
    REQUIRE(deg.get()==30);

    deg-=30;
    REQUIRE(deg.get()==0);

    deg+=Degrees(15);
    REQUIRE(deg.get()==15);

    deg-=Degrees(5);
    REQUIRE(deg.get()==10);

}
TEST_CASE("Degrees operator* and operator/",degrees_suite)
{
    Degrees deg = 30;
    REQUIRE(((deg*3)==90));
    REQUIRE(((deg/3)==10));
}
TEST_CASE("Degrees operator*= and operator /=",degrees_suite)
{
    Degrees deg = 30;

    deg*=3;
    REQUIRE((deg==90));

    deg/=10;
    REQUIRE((deg==9));
}

#endif
#endif