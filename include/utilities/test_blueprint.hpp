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


//----------------------------------------------------------------
//THIS IS A BLUEPRINT TEST CAMPAIGN
//NOTHING HERE WILL RUN UNTIL YOU REMOVE THIS DEFINITION
#define TEST_BLUEPRINT
//----------------------------------------------------------------

#ifdef CONFIG_TEST
#ifndef TEST_BLUEPRINT
#define TEST_BLUEPRINT

#include "../utilities/catch.hpp"
#include "../primitives/"

using namespace std;

#define test_suite "[]"

TEST_CASE("",test_suite)
{
    //CHECK();
}
TEST_CASE("",test_suite)
{
    //CHECK();
}
TEST_CASE("",test_suite)
{
    //CHECK();
}
TEST_CASE("",test_suite)
{
    //CHECK();
}
TEST_CASE("",test_suite)
{
    //CHECK();
}

#endif
#endif