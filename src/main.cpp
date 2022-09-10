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

#include "../include/primitives/range.hpp"
#include "../include/primitives/tolerance.hpp"
#include "../include/primitives/angles/angle.hpp"
#include "../include/primitives/angles/degrees.hpp"
#include "../include/primitives/angles/radians.hpp"
#include "../include/primitives/point.hpp"

/*
---------------------------------------------------------------------------------------------------------------------
This definition is used to determine whether or not we want to include our test profiles in our compile.
Set to #define CONFIG_TEST to include and run test profiles, set to CONFIG_PROD for a production release
If CONFIG_TEST is not defined, this will default to CONFIG_PROD
---------------------------------------------------------------------------------------------------------------------
*/
#define CONFIG_TEST

#ifndef CONFIG_TEST
    #define CONFIG_PROD
#endif

#ifdef CONFIG_TEST

    #define CATCH_CONFIG_RUNNER
    #include "../include/utilities/catch.hpp"

    #include "../include/tests/test_range.hpp"
    #include "../include/tests/test_tolerance.hpp"
    #include "../include/tests/test_radians.hpp"
    #include "../include/tests/test_point.hpp"
    #include "../include/tests/test_degrees.hpp"

#endif

using namespace std;

int main(int argc, char* argv[])
{
    int results = 0;

    #ifdef CONFIG_TEST
        results = Catch::Session().run(argc, argv);
    #endif

    //If we ran the catch session, the results variable may change, indicating a failed test.
    //Do not launch the game if the tests fail.
    if(results!=0)
    {
        return results;
    }
    



    return 0;
}
