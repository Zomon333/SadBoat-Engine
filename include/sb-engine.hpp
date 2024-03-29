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
#include <cstring>
#include <fstream>
#include <functional>
#include <future>
#include <iostream>
#include <iomanip>
#include <mutex>
#include <semaphore>
#include <stack>
#include <string>
#include <thread>
#include <unordered_map>
#include <utility>
#include <variant>


// RapidXML Include
#include "../lib/rapidxml-1.13/rapidxml.hpp" //RapidXML is a FOSS header only library for XML reading. We use it for settings and config files.
#include "../lib/rapidxml-1.13/rapidxml_utils.hpp"
#include "../lib/rapidxml-1.13/rapidxml_print.hpp"

// LibPNG includes
#include "zlib.h"
#include "../lib/libpng/include/libpng16/png.h"
#include "../lib/libpng/include/libpng16/pngconf.h"
#include "../lib/libpng/include/libpng16/pnglibconf.h"

// Eigen include
//----------------------------------
#include "../lib/eigen/Eigen/Eigen"


//  Engine Specific Defines
//----------------------------------
#include "../include/utilities/defines.hpp"

// Event Includes
//----------------------------------
#include "../include/events/event.hpp"
#include "../include/events/timedevent.hpp"
#include "../include/events/recurringevent.hpp"
#include "../include/events/eventpool.hpp"

//  Logging Includes
//----------------------------------
#include "../include/resources/idmanager.hpp"
#include "../include/utilities/logging/logmanager.hpp"
#include "../include/utilities/logging/loghandle.hpp"
#include "../include/utilities/logging/startuplogger.hpp"

//  Configuration Includes
//----------------------------------
#include "../include/utilities/configs/confignode.hpp"
#include "../include/utilities/configs/config.hpp"
#include "../include/utilities/configs/configmanager.hpp"

//  Resource Includes
//----------------------------------
#include "../include/resources/resourcehandle.hpp"
#include "../include/resources/resource.hpp"
#include "../include/resources/resourcemanager.hpp"


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
#include "../include/utilities/manager.hpp"

//  Physics Includes
//----------------------------------



/*
---------------------------------------------------------------------------------------------------------------------
    Vulkan Includes
---------------------------------------------------------------------------------------------------------------------
*/
#ifdef INCLUDE_VULKAN
    
    #include "./vulkan/vulkan.hpp"

    // VkResult helper function
    #include "../include/vulkan/vulkanresultlookup.hpp"

    // Instances
    #include "../include/vulkan/instance.hpp"

    // Physical Devices
    #include "../include/vulkan/physicaldevice.hpp"
    #include "../include/vulkan/physicaldevicecollection.hpp"

    // Queues
    #include "../include/vulkan/queuefamily.hpp"
    #include "../include/vulkan/queuefamilycollection.hpp"

    // Layers, Extensions
    #include "../include/vulkan/layercollection.hpp"
    #include "../include/vulkan/extensioncollection.hpp"

    // Logical Devices
    #include "../include/vulkan/logicaldevice.hpp"

    // Queues Continued
    #include "../include/vulkan/queue.hpp"
    #include "../include/vulkan/queuecollection.hpp"

    // Memory models
    #include "../include/vulkan/buffer.hpp"
    #include "../include/vulkan/image.hpp"

    // Command Classes
    #include "../include/vulkan/commandbuffer.hpp"
    #include "../include/vulkan/commandpool.hpp"
    #include "../include/vulkan/commandpoolmanager.hpp"

    #include "../include/vulkan/vulkandispatchables.hpp"
#endif




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
    #include "../lib/catch.hpp"

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

    // Test Resource Loading
    #include "../include/tests/test_resourcehandle.hpp"

#endif
#endif