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

#ifndef RECUR_EVENT_TEST
#define RECUR_EVENT_TEST

#include "sb-engine.hpp"
#include "../utilities/catch.hpp"

using namespace std;
using namespace SBE;

#define test_recur "[Recurring Event Test]"

TEST_CASE("Recurring Event Accumulator Test",test_recur)
{
    int a = 0;
    int b = 0;
    int c = 0;

    RecurringEvent<int*, int*, int*> testEvent(
        F(int* a, int* b, int* c)
        {
            (*c) = (*a) + (*b);
            return 0;
        },
        milliseconds(10)
    );

    testEvent.recur(&a, &b, &c);

    steady_clock::time_point exTime = steady_clock::now() + milliseconds(11);
    std::this_thread::sleep_until(exTime);

    for(int i = 0; i<100; i++)
    {
        a = i;
        b = (100 - i);

        exTime = steady_clock::now() + milliseconds(11);
        std::this_thread::sleep_until(exTime);

        CHECK(c==100);
    }

    testEvent.suppress();
    std::this_thread::sleep_until((steady_clock::now() + milliseconds(11)));

    a = 0;
    b = 0;
    CHECK(c==100);

    testEvent.release();
    std::this_thread::sleep_until((steady_clock::now() + milliseconds(15)));

    CHECK(c==0);

    testEvent.end();
    std::this_thread::sleep_until((steady_clock::now() + milliseconds(11)));

    a=20;
    b=20;
    CHECK(c==0);

}
TEST_CASE("Recurring Event Frequency Change Test",test_recur)
{
    int a = 0;
    RecurringEvent<int*> freqTest(
        F(int* a)
        {
            (*a) = (*a) + 1;
            return 0;
        },
        milliseconds(10)
    );

    freqTest.recur(&a);
    std::this_thread::sleep_until((steady_clock::now() + std::chrono::seconds(1)));


    int rOneRes = a;
    freqTest.suppress();
    std::this_thread::sleep_until((steady_clock::now() + milliseconds(10)));

    a = 0;
    freqTest.setFreq(milliseconds(1));

    std::this_thread::sleep_until((steady_clock::now() + milliseconds(10)));

    freqTest.release();
    std::this_thread::sleep_until((steady_clock::now() + std::chrono::seconds(1)));

    int rTwoRes = a;

    freqTest.end();

    CHECK(rTwoRes > rOneRes);

}


#endif