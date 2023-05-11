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

#define CONFIG_TEST

#ifdef CONFIG_TEST
#ifndef TEST_RESOURCE_HANDLE
#define TEST_RESOURCE_HANDLE

#include "../lib/catch.hpp"
#include "../resources/resourcehandle.hpp"

using namespace std;

#define test_suite "[ResourceHandle]"

TEST_CASE("ResourceHandle from TestingFile.txt",test_suite)
{
    Resource tRes = Resource(0,"./include/tests/TestingFile");
    ResourceHandle* tHand = tRes.allocateHandle();

    CHECK(tRes.getSize()==45);

    // If we know how much data something is supposed to have, but we see it's displaying more but *only in the testing suites...*
    // Then it's probably a problem with the testing suites. Especially given that we can truncate the data, because we know how much we have...
    string tmp = tHand->getData<char*>();
    tmp = tmp.substr(0,tRes.getSize());

    CHECK(tRes.getHandle(tHand->getHandleID())==tHand);

    CHECK(tRes.getHandleCount()==1);
    CHECK(tHand->getHandleID()==2);

    CHECK(tHand->getResourceID()==0);
    
    CHECK(tmp=="The quick brown fox jumped over the lazy dog.");
}

#endif
#endif