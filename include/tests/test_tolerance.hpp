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

#ifndef TEST_TOLERANCE
#define TEST_TOLERANCE

#include "../utilities/catch.hpp"
#include "../primitives/tolerance.hpp"

using namespace std;

#define tolerance_suite "[tolerance]"

TEST_CASE("Constructor",tolerance_suite)
{
    Tolerance test = Tolerance();
    REQUIRE(sizeof(test)>0);

    test = Tolerance(0, 5);
    CHECK(test.getError()==5);
    CHECK(test.getOrigin()==0);
}

TEST_CASE("Set and Get Error",tolerance_suite)
{
    Tolerance test = Tolerance(0, 5);
    REQUIRE(test.getError()==5);

    test.setError(3);
    CHECK(test.getError()==3);
}
TEST_CASE("Set and Get Origin",tolerance_suite)
{
    Tolerance test = Tolerance(0, 5);
    REQUIRE(test.getOrigin()==0);

    test.setOrigin(5);
    CHECK(test.getOrigin()==5);
    CHECK(test[6]);
}
TEST_CASE("inTolerance and operator[] test",tolerance_suite)
{
    Tolerance test = Tolerance(3,0.5);
    REQUIRE(test.inTolerance(3.25));
    CHECK(test[2.75]);
}

#endif
