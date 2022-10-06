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

#ifndef TEST_T_EVENT
#define TEST_T_EVENT

#include "../utilities/catch.hpp"
#include "../sb-engine.hpp"

#include <queue>
#include <stack>

using namespace std;
using namespace SBE;

#define test_timed_event "[TimedEvent]"


TEST_CASE("TimedEvent Constructor",test_timed_event)
{
    //Call default constructor
    TimedEvent<int, int> defTest = TimedEvent<int, int>();

    REQUIRE(
        (defTest(0)==int())
    );
    REQUIRE(
        (defTest==steady_clock::time_point(milliseconds()))
    );

    //Call constructor with some uninitialized steady_clock::time_point
    TimedEvent<int, int, int> sum(
        F(int a, int b)
        {
            return a + b;
        }
    );

    CHECK
    (
        (sum(1, 1)==2)
    );
    REQUIRE
    (
        (sum==steady_clock::time_point(milliseconds()))
    );

    //Call constructor with some initialized steady_clock::time_point
    TimedEvent<int, int, int> sum2(
        F(int a, int b)
        {
            return a + b;
        },
        steady_clock::time_point(
            milliseconds(
                50
            )
        )
    );
    CHECK(sum2(1,1)==2);        //Check that sum2 returns 1+1=2
    CHECK(sum(1,1)==sum2(1,1)); //Check that sum and sum2 return the same thing

    //Call copy constructor
    TimedEvent<int, int, int> copyTest(sum2);

    CHECK
    (
        (sum2==copyTest)
    );
    CHECK
    (
        (copyTest(1,1)==2)
    );

}
TEST_CASE("Comparison Tests",test_timed_event)
{
    TimedEvent<bool, bool> echoTest(
        F(bool a)
        {
            return a;
        },
        steady_clock::time_point(
            milliseconds(
                0
            )
        )
    );

    TimedEvent<bool, bool> reverseTest(
        F(bool b)
        {
            return !b;
        },
        steady_clock::time_point(
            milliseconds(
                5
            )
        )
    );

    //Test equivalence of echoTest to Instances of Unit Time
    CHECK
    (
        (echoTest==steady_clock::time_point(milliseconds(0)))
    );
    CHECK
    (
        (echoTest!=steady_clock::time_point(milliseconds(5)))
    );

    //Test equivalence of reverseTest to Instances of Unit Time
    CHECK
    (
        (reverseTest==steady_clock::time_point(milliseconds(5)))
    );
    CHECK
    (
        (reverseTest!=steady_clock::time_point(milliseconds(0)))
    );

    //Test equivalence of echoTest and reverseTest
    CHECK
    (
        (reverseTest!=echoTest)
    );
    CHECK
    (
        !(reverseTest==echoTest)
    );

    //Test equivalence of reverseTest and reverseTest, and echoTest and echoTest
    CHECK
    (
        (echoTest==echoTest)
    );
    CHECK
    (
        (reverseTest==reverseTest)
    );

    //Test greater than functionality
    CHECK
    (
        (reverseTest>echoTest)
    );
    CHECK
    (
        !(echoTest>reverseTest)
    );

    //Test less than functionality
    CHECK
    (
        !(reverseTest<echoTest)
    );
    CHECK
    (
        (echoTest<reverseTest)
    );

    //Test less than equal to functionality
    CHECK
    (
        (echoTest<=reverseTest)
    );
    CHECK
    (
        !(reverseTest<=echoTest)
    );
    CHECK
    (
        (echoTest<=echoTest)
    );
    CHECK
    (
        (reverseTest<=reverseTest)
    );

    //Test greater than equal to functionality
    CHECK
    (
        !(echoTest>=reverseTest)
    );
    CHECK
    (
        (reverseTest>=echoTest)
    );
    CHECK
    (
        (echoTest>=echoTest)
    );
    CHECK
    (
        (reverseTest>=reverseTest)
    );


}
TEST_CASE("Deferred future test",test_timed_event)
{
    TimedEvent<int, int, int> deferredSum(
        F(int a, int b)
        {
            return a + b;
        }
    );

    steady_clock::time_point executionTime = steady_clock::now() + std::chrono::milliseconds(25);
    deferredSum.defer(executionTime, 3, 3);

    CHECK(
        (deferredSum(4, 7)==11)
    );

    deferredSum.launch(2,1);
    CHECK(
        (deferredSum.getResult()==3)
    );

    CHECK(
        (deferredSum.getResult()==6)
    );

}
TEST_CASE("Deferred timing test",test_timed_event)
{
    TimedEvent<double, int> waitress(
        F(int a)
        {
            return steady_clock::now().time_since_epoch().count();
        }
    );

    steady_clock::time_point executionTime = steady_clock::now() + std::chrono::microseconds(75);
    
    waitress.defer(executionTime, 0);
    double start = waitress(0);
    double end = waitress.getResult();

    CHECK(
        ((end-start)>0)
    );
}
TEST_CASE("Deferred contents test",test_timed_event)
{
    TimedEvent<int, int*, int*> deferredSum(
        F(int* a, int* b)
        {
            return (*a)+(*b);
        }
    );

    int a = 3;
    int b = 3;

    CHECK(
        (deferredSum(&a, &b)==6)
    );

    steady_clock::time_point exTime = steady_clock::now() + std::chrono::milliseconds(15);
    
    deferredSum.defer(exTime, &a, &b);
    a=10;

    CHECK(
        (deferredSum.getResult()==13)
    );
}
TEST_CASE("Suppression test",test_timed_event)
{
    TimedEvent<int, int> returner(
        F(int a)
        {
            return a;
        }
    );
    returner.suppress();
    CHECK
    (
        (returner.call(24768)==int())
    );
    returner.release();
    CHECK
    (
        (returner.call(24768)==24768)
    );
}
TEST_CASE("PriorityQueue Sorting Test",test_timed_event)
{
    std::priority_queue<TimedEvent<int, int>*> eventQueue;
    eventQueue.emplace(new TimedEvent<int, int>(
        F(int a)
        {
            return a;
        },
        steady_clock::now()+std::chrono::milliseconds(25)
    ));

    eventQueue.emplace(new TimedEvent<int, int>(
        F(int a)
        {
            return a+10;
        },
        steady_clock::now()+std::chrono::milliseconds(50)
    ));

    std::stack<TimedEvent<int,int>*> results;

    while(eventQueue.size()>0)
    {
        if(eventQueue.top()->operator<=(steady_clock::time_point::clock::now()))
        {
            eventQueue.top()->launch(5);
            results.emplace(eventQueue.top());
            eventQueue.pop();
        }
    }

    CHECK(
        (results.top()->getResult()==5)
    );

    results.pop();

    CHECK(
        (results.top()->getResult()==15)
    );
    
}

#endif