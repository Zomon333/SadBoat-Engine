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
#include <vector>
#include <stack>

#include "../events/event.hpp"

using namespace std;

#define event_suite "[events]"

TEST_CASE("Event constructor test",event_suite)
{
    Event<int> e(
        F(){
            return 1;
        }
    );

    CHECK(e()==1);
}

TEST_CASE("Event copy constructor test",event_suite)
{
    Event<int> e(
        F(){
            return 1;
        }
    );

    CHECK(e()==1);

    Event<int> f = Event<int>(e);
    
    CHECK(f()==1);
}

TEST_CASE("Event linear run test",event_suite)
{
    Event<int, int, int> sum(
        F(int a, int b)
        {
            return a + b;
        }
    );

    CHECK(sum(3, 3)==6);
}

TEST_CASE("Event concurrent run test.",event_suite)
{
    Event<int, int, int> sum(
        F(int a, int b)
        {
            return a + b;
        }
    );
    sum.launch(3, 3);
    int answer = sum.getResult();

    CHECK(answer==6);
}

TEST_CASE("Event search test",event_suite)
{
    vector<int> s = { 0, 3, 2, 4, 7, 6, 5, 1};
    int g = 7;
    
    Event<int, int, vector<int>> search(
        F(int g, vector<int> s){
            for(int i = 0; i<s.size(); i++)
            {
                if(s[i]==g)
                return i;
            }
            return -1;
        }
    );

    search.launch(g, s);
    int answer = search.getResult();
    
    CHECK(answer==4);
}

TEST_CASE("Threaded call stack test",event_suite)
{
    Event<int, int, int> sum(
        F(int a, int b)
        {
            return a + b;
        }
    );

    std::stack<int> results;
    for(int i = 0; i<10; i++)
    {
        results.emplace(i+i);
        sum.launch(i, i);
    }
    for(int i = 0; i<10; i++)
    {
        CHECK( sum.getResult()==results.top() );
        results.pop();
    }
}

#endif
#endif