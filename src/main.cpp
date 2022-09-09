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

// STD Includes
//----------------------------------
#include <iostream>
#include <queue>
#include <vector>
#include <utility>
#include <algorithm>
#include <fstream>
#include <unordered_map>
#include <deque>
#include <string>
#include <cmath>
#include <thread>

#include "../include/primitives/range.hpp"
#include "../include/primitives/tolerance.hpp"
#include "../include/primitives/angles.hpp"
#include "../include/primitives/point.hpp"

#define CATCH_CONFIG_RUNNER
#include "../include/utilities/catch.hpp"

#include "../include/tests/test_range.hpp"
#include "../include/tests/test_tolerance.hpp"
#include "../include/tests/test_radians.hpp"
#include "../include/tests/test_point.hpp"

using namespace std;

int main(int argc, char* argv[])
{
    int results = 0;
    results = Catch::Session().run(argc, argv);

    //If we ran the catch session, the results variable may change
    //So if it is anything other than it's default value, it indicates a failed test.
    //So, if a test fails, then don't launch the rest of the game.
    if(results!=0)
    {
        return results;
    }
    



    return 0;
}
