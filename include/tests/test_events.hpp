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
#ifndef TEST_EVENTS
#define TEST_EVENTS

#include "../utilities/catch.hpp"

#include <iostream>
#include <future>
#include <thread>

#include "../events/event.hpp"

using namespace std;

#define event_suite "[events]"

TEST_CASE("Event test",event_suite)
{

}
TEST_CASE("",event_suite)
{
    //CHECK();
}
TEST_CASE("",event_suite)
{
    //CHECK();
}
TEST_CASE("",event_suite)
{
    //CHECK();
}
TEST_CASE("",event_suite)
{
    //CHECK();
}

#endif
#endif