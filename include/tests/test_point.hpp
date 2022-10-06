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
#ifndef TEST_POINT
#define TEST_POINT

#include "../utilities/catch.hpp"
#include "../primitives/point.hpp"

using namespace std;
using namespace SBE;

TEST_CASE("Constructor test","[point]")
{
    for(int i = -10; i++; i<11)
    {
        CHECK(
            Point(std::tuple<double, double, double>(i, i, i)).getX()==i
        );
        CHECK(
            Point(std::tuple<double, double, double>(i, i, i)).getY()==i
        );
        CHECK(
            Point(std::tuple<double, double, double>(i, i, i)).getZ()==i
        );
    }
}
TEST_CASE("Getter & Setter test.","[point]")
{
    CHECK(Point(std::tuple<double, double, double>(5, 10, 0)).getX()==5);
    CHECK(Point(std::tuple<double, double, double>(5, 10, 0)).getY()==10);
    CHECK(Point(std::tuple<double, double, double>(5, 10, 0)).getZ()==0);

    Point testPoint = Point(std::tuple<double, double, double>(5, 10, 0));
    for(int i = -5; i<6; i++)
    {
        testPoint.setX(i);
        CHECK(testPoint.getX()==i);

        testPoint.setY(i);
        CHECK(testPoint.getY()==i);

        testPoint.setZ(i);
        CHECK(testPoint.getZ()==i);
    }

}
TEST_CASE("Distance test.","[point]")
{
    Point origin = Point(std::tuple<double, double, double>(0,0,0));
    Point ones = Point(std::tuple<double, double, double>(1,1,1));

    CHECK(origin.distance(ones)==sqrt(3));

}
TEST_CASE("Equivalence, non-equivalence test","[point]")
{
    for(int i = 0; i<4; i++)
    {
        Point a = Point(std::tuple<double, double, double>(i, i, i));
        Point b = Point(std::tuple<double, double, double>(i, i, i));

        bool equal = a==b;
        CHECK(equal);
        //CHECK(Point(std::pair<double, double>(i, i))==Point(std::pair<double,double>(i,i)));
    }
}
TEST_CASE("+, += test","[point]")
{
    Point origin = Point(std::tuple<double, double, double>(0,0,0));
    Point ones = Point(std::tuple<double, double, double>(1,1,0));

    Point sum = origin + ones;
    bool equal = sum==ones;
    CHECK(equal);
    origin+=ones;
    equal = origin==ones;
    CHECK(equal);
}
TEST_CASE("-, -= test","[point]")
{
    Point origin = Point(std::tuple<double, double, double>(0,0,0));
    Point ones = Point(std::tuple<double, double, double>(1,1,1));

    Point nones = ones - ones;
    bool diff = nones==origin;
    CHECK(diff);

    ones-=ones;
    diff = ones==origin;
    CHECK(diff);
}
TEST_CASE("*, *= test","[point]")
{
    Point ones = Point(std::tuple<double, double, double>(1, 1, 1));

    Point threes = ones * 3;
    bool success = threes.getX()==3;

    CHECK((
        (ones*3).getX()==3
    ));

    ones*=3;
    CHECK((
        ones==threes
    ));
}
TEST_CASE("/, /= test","[point]")
{
    Point threes = Point(std::tuple<double, double, double>(3, 3, 3));
    Point thirds = threes/3;
    
    CHECK((
        (threes/3).getX()==1
    ));
    
    threes/=3;
    CHECK((
        threes==thirds
    ));
}
#endif
#endif