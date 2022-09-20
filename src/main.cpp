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

---------------------------------------------------------------------------------------------------------------------
SadBoat Engine:
    The SadBoat Engine is an open-source, free to use game engine being designed in C++.
    Most of it's functionality is defined in it's header files, making it a mostly header only engine.
    Consequently, you do not need this main.cpp, specifically, to run your game. It is, however, recommended.
    Opting to not use this main.cpp file means you may need to reimplement the code found herein. This is up to your discretion.

Disclaimer:
    We are not responsible for what you do with our code. You can use it as intended, or make your own use of it.
    If you decide to use it within the parameters we've defined, our documentation exists to help you!
    If something is not clear in our documentation, you can email contact@sadboat.com and we'll try and clarify.
    If your usage is outside of the scope of our documentation, we may not be able to provide you any helpful results.

---------------------------------------------------------------------------------------------------------------------
*/

//  Engine specific defines
//----------------------------------

//Shorthand lambda function body
#define F(a...) [](a...)
//Shorthand lambda function for internal captures
#define lF(a...) [this](a...)

//Steady clock for timekeeping.
#define EngineClock std::chrono::steady_clock

//Microseconds-- engine unit time.
#define uTime std::chrono::milliseconds

//100 HZ -- engine unit frequency.
#define uFreq uTime(10)

//2 kHZ -- maximum engine frequency
#define uFreqMax std::chrono::microseconds(500)

//pow(2, -47)-- engine unit tolerance. Arbitrarily small.
#define uTol pow(2,-32)

//An instant in time-- a microsecond by the engine's clock.
#define Instant EngineClock::time_point



// STD Includes
//----------------------------------
#include <iostream>
#include <string>
#include <variant>
#include <deque>
#include <future>
#include <thread>
#include <any>



// Primitive Includes
//----------------------------------
#include "../include/primitives/range.hpp"
#include "../include/primitives/tolerance.hpp"
#include "../include/primitives/point.hpp"

// Angles
#include "../include/primitives/angles/angle.hpp"
#include "../include/primitives/angles/degrees.hpp"
#include "../include/primitives/angles/radians.hpp"

// Event Includes
//----------------------------------
#include "../include/events/event.hpp"
#include "../include/events/timedevent.hpp"
#include "../include/events/recurringevent.hpp"


//  Utility Includes
//----------------------------------
#include "../include/utilities/vargroup.hpp"

/*
---------------------------------------------------------------------------------------------------------------------
This definition is used to determine whether or not we want to include our OpenGL classes for compilation.
This exists in case you're trying to compile but don't have the libraries set up to link.
This allows you to still write code; it just won't be included in compilation.
---------------------------------------------------------------------------------------------------------------------
*/
//#define OPENGL_LINKED
#ifdef OPENGL_LINKED
    #include "../include/utilities/opengl/vao.hpp"
    #include "../include/utilities/opengl/uniform.hpp"
    #include "../include/utilities/opengl/shader.hpp"
    #include "../include/utilities/opengl/shaderprogram.hpp"
    #include "../include/utilities/opengl/interfaceblock.hpp"
#endif

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

    // Test primitives
    //----------------------------------
    #include "../include/tests/test_point.hpp"
    #include "../include/tests/test_range.hpp"
    #include "../include/tests/test_tolerance.hpp"

    // Angle tests
    #include "../include/tests/test_degrees.hpp"
    #include "../include/tests/test_radians.hpp"

    // Event tests
    #include "../include/tests/test_events.hpp"
    #include "../include/tests/test_timedevent.hpp"


#endif

using namespace std;

//----------------------------------
//  main(int argc, char* argv[]) function:
//
//      Entry point for program. Manages testing runtimes, and launches other session managers.
//      Return point for all session managers, processes engine shutdown.
//
//----------------------------------
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

    Event<void, int> test(F(int a){});

    return 0;
}
