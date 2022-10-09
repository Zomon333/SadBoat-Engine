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
    Consequently, you do not need our main.cpp, specifically, to run your game. It is, however, recommended.
    Opting to not use our main.cpp file means you may need to reimplement the code. This is up to your discretion.

Disclaimer:
    We are not responsible for what you do with our code. You can use it as intended, or make your own use of it.
    If you decide to use it within the parameters we've defined, our documentation exists to help you!
    If something is not clear in our documentation, you can email contact@sadboat.com and we'll try and clarify.
    If your usage is outside of the scope of our documentation, we may not be able to provide you any helpful results.

---------------------------------------------------------------------------------------------------------------------
*/

/*
    This file contains all the engine specific includes.
    You can include this file and your main should run fine. You can use these features as needed.
*/

#ifndef ENGINE_INCLUDES
#define ENGINE_INCLUDES

// STD Includes
//----------------------------------

#include <algorithm>
#include <any>
#include <chrono>
#include <cmath>
#include <fstream>
#include <functional>
#include <future>
#include <iostream>
#include <iomanip>
#include <stack>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>


//  Engine Specific Defines
//----------------------------------
#include "../include/utilities/defines.hpp"

// Event Includes
//----------------------------------
#include "../include/events/event.hpp"
#include "../include/events/timedevent.hpp"
#include "../include/events/recurringevent.hpp"


// Primitive Includes
//----------------------------------
#include "../include/primitives/range.hpp"
#include "../include/primitives/tolerance.hpp"
#include "../include/primitives/point.hpp"

// Angles
#include "../include/primitives/angles/angle.hpp"
#include "../include/primitives/angles/degrees.hpp"
#include "../include/primitives/angles/radians.hpp"

//  Utility Includes
//----------------------------------
#include "../include/utilities/vargroup.hpp"

//  Physics Includes
//----------------------------------
#include "../include/vectors/vector.hpp"

/*
---------------------------------------------------------------------------------------------------------------------
    Vulkan Includes
---------------------------------------------------------------------------------------------------------------------
*/
#include "./vulkan/vulkan.hpp"


//RapidXML is a FOSS header only library for XML reading. We use it for settings and config files.
#include "utilities/rapidxml-1.13/rapidxml.hpp"

#include "utilities/rapidxml-1.13/rapidxml_print.hpp"
#include "utilities/rapidxml-1.13/rapidxml_utils.hpp"
//#include "utilities/rapidxml-1.13/rapidxml_iterators.hpp"


/*
---------------------------------------------------------------------------------------------------------------------
This definition is used to determine whether or not we want to include our test profiles in our compile.
This is controlled through the makefile; 
-run "make test" to enable CATCH tests
-run "make prod" to disable CATCH tests
---------------------------------------------------------------------------------------------------------------------
*/

//#define CONFIG_TEST
//#define CONFIG_PROD

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
    #include "../include/tests/test_recurringevent.hpp"


#endif
#endif