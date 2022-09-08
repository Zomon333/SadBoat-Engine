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
#ifndef TEST_RADIANS
#define TEST_RADIANS

#include "../utilities/catch.hpp"
#include "../primitives/angles.hpp"

using namespace std;

TEST_CASE("Radians constructor & get test","[radians]")
{
    REQUIRE(Radians(0, 1).get()==0);
    REQUIRE(Radians(1, 1).get()==1);
    REQUIRE(Radians(3, 1).get()==3);
    REQUIRE(Radians(6, 2).get()==12);
    REQUIRE(Radians(14, 8).get()==112);

    REQUIRE(Radians(1, M_PI).get()==M_PI);
    REQUIRE(Radians(0.5, M_PI).get()==M_PI_2);
}

TEST_CASE("Radian pi parse test","[radians]")
{
    REQUIRE(Radians(1,M_PI).get()==M_PI);
}

TEST_CASE("Radians operator+ test","[radians]")
{
    Radians A = Radians(5, 1);
    Radians B = Radians(7, 1);

    Radians AB = A + B;

    REQUIRE(AB.get()==12);

    A = Radians(1, M_PI);
    A = A + M_PI;

    REQUIRE(A.get()==(2*M_PI));
}

TEST_CASE("Radians operator- test","[radians]")
{
    Radians A = Radians(15, 1);
    Radians B = Radians(5, 1);

    Radians AB = A - B;

    REQUIRE(AB.get()==10);

    A=Radians(2,M_PI);
    A = A - M_PI;

    REQUIRE(A.get()==M_PI);
}


TEST_CASE("Radians operator+= test","[radians]")
{
    Radians A = Radians(5, 1);
    A+=Radians(5, 1);

    REQUIRE(A.get()==10);

    A=Radians(1,M_PI);
    A+=M_PI;

    REQUIRE(A.get()==(2*M_PI));
}

TEST_CASE("Radians operator-= test","[radians]")
{
    Radians A = Radians(15, 1);
    A-=Radians(5, 1);

    REQUIRE(A.get()==10);

    A=Radians(2,M_PI);
    A-=M_PI;

    REQUIRE(A.get()==M_PI);
}

TEST_CASE("Radians operator* test","[radians]")
{
    Radians A = Radians(5, 1);
    A = A * 2;

    REQUIRE(A.get()==10);
}

TEST_CASE("Radians operator*= test","[radians]")
{
    Radians A = Radians(5, 1);
    A*=2;

    REQUIRE(A.get()==10);
}

TEST_CASE("Radians operator/ test","[radians]")
{
    Radians A = Radians(10,1);
    A = A / 2;

    REQUIRE(A.get()==5);
}

TEST_CASE("Radians operator/= test","[radians]")
{
    Radians A = Radians(10,1);
    A/=2;

    REQUIRE(A.get()==5);
}

#endif