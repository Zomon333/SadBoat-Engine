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

#include "primitives/range.hpp"

#define CATCH_CONFIG_RUNNER
#include "utilities/catch.hpp"

#include "tests/test_range.hpp"

using namespace std;

int main(int argc, char* argv[])
{
    int result = Catch::Session().run( argc, argv );
    if(result!=0)
    {  
        return -1;
    }
    


    return 0;
}
