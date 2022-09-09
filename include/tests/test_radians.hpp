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
#ifndef TEST_RADIANS
#define TEST_RADIANS

#include "../utilities/catch.hpp"
#include "../primitives/angles/radians.hpp"

using namespace std;

TEST_CASE("Radians constructor & get test","[radians]")
{
    CHECK(Radians(0, 1).get()==0);
    CHECK(Radians(1, 1).get()==1);
    CHECK(Radians(3, 1).get()==3);
    CHECK(Radians(6, 1).get()==6);
    CHECK(Radians(2, M_PI).get()==0);

    CHECK(Radians(1, M_PI).get()==M_PI);
    CHECK(Radians(0.5, M_PI).get()==M_PI_2);
}

TEST_CASE("Radian pi parse test","[radians]")
{
    CHECK(Radians(1,M_PI).get()==M_PI);
}

TEST_CASE("Radians operator+ test","[radians]")
{
    Radians A = Radians(2, 1);
    Radians B = Radians(3, 1);

    Radians AB = A + B;

    CHECK(AB.get()==5);

    A = Radians(1, M_PI);
    A = A + M_PI;

    CHECK(A.get()==(0));
}

TEST_CASE("Radians operator- test","[radians]")
{
    Radians A = Radians(6, 1);
    Radians B = Radians(5, 1);

    Radians AB = A - B;

    CHECK(AB.get()==1);

    A=Radians(1,M_PI);
    A = A - M_PI;

    CHECK(A.get()==0);
}


TEST_CASE("Radians operator+= test","[radians]")
{
    Radians A = Radians(5, 1);
    A+=Radians(1, 1);

    CHECK(A.get()==6);

    A=Radians(1,M_PI);
    A+=M_PI;

    CHECK(A.get()==0);
}

TEST_CASE("Radians operator-= test","[radians]")
{
    Radians A = Radians(6, 1);
    A-=Radians(5, 1);

    CHECK(A.get()==1);

    A=Radians(1,M_PI);
    A-=M_PI;

    CHECK(A.get()==0);
}

TEST_CASE("Radians operator* test","[radians]")
{
    Radians A = Radians(3, 1);
    A = A * 2;

    CHECK(A.get()==6);
}

TEST_CASE("Radians operator*= test","[radians]")
{
    Radians A = Radians(3, 1);
    A*=2;

    CHECK(A.get()==6);
}

TEST_CASE("Radians operator/ test","[radians]")
{
    Radians A = Radians(6,1);
    A = A / 2;

    CHECK(A.get()==3);
}

TEST_CASE("Radians operator/= test","[radians]")
{
    Radians A = Radians(5,1);
    A/=2;

    CHECK(A.get()==2.5);
}

#endif
