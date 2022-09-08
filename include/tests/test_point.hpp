/*
Copyright 2022 Dagan Poulin, Justice Guillory
   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
       http://www.apache.org/licenses/LICENSE-2.0
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#ifndef TEST_POINT
#define TEST_POINT

#include "../utilities/catch.hpp"
#include "../primitives/point.hpp"

using namespace std;

TEST_CASE("Constructor test","[point]")
{
    for(int i = -10; i++; i<11)
    {
        REQUIRE(
            Point(std::pair<double, double>(i, i)).getX()==i
        );
        REQUIRE(
            Point(std::pair<double, double>(i, i)).getY()==i
        );
    }
}
TEST_CASE("Getter & Setter test.","[point]")
{
    REQUIRE(Point(std::pair<double, double>(5, 10)).getX()==5);
    REQUIRE(Point(std::pair<double, double>(5, 10)).getY()==10);

    Point testPoint = Point(std::pair<double, double>(5, 10));
    for(int i = -5; i<6; i++)
    {
        testPoint.setX(i);
        REQUIRE(testPoint.getX()==i);

        testPoint.setY(i);
        REQUIRE(testPoint.getY()==i);
    }

}
TEST_CASE("Distance test.","[point]")
{
    Point origin = Point(std::pair<double, double>(0,0));
    Point ones = Point(std::pair<double, double>(1,1));

    REQUIRE(origin.distance(ones)==sqrt(2));

}
TEST_CASE("Equivalence, non-equivalence test","[point]")
{
    for(int i = 0; i<4; i++)
    {
        REQUIRE(Point(std::pair<double, double>(i, i))==Point(std::pair<double,double>(i,i)));
    }
}
TEST_CASE("+, += test","[point]")
{
    Point origin = Point(std::pair<double, double>(0,0));
    Point ones = Point(std::pair<double, double>(1,1));

    REQUIRE((origin+ones)==ones);
    origin+=ones;
    REQUIRE(origin==ones);
}
TEST_CASE("-, -= test","[point]")
{
    Point origin = Point(std::pair<double, double>(0,0));
    Point ones = Point(std::pair<double, double>(1,1));

    REQUIRE((ones+ones)==origin);
    ones-=ones;
    REQUIRE(origin==ones);
}
TEST_CASE("*, *= test","[point]")
{
    Point ones = Point(std::pair<double, double>(1, 1));
    REQUIRE((ones*3).getX()==3);
    ones*=3;
    REQUIRE(ones.getX()==3);
}
TEST_CASE("/, /= test","[point]")
{
    Point threes = Point(std::pair<double, double>(1, 1));
    REQUIRE((threes/3).getX()==1);
    threes/=3;
    REQUIRE((threes).getX()==1);
}
#endif
