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

// Utility Includes
//----------------------------------
#include "SYSTEMS_range.hpp"
#include "SYSTEMS_idman.hpp"
#include "EVENTS_function.hpp"
#include "EVENTS_event.hpp"

// OpenGL Includes
//----------------------------------

// OpenAL Includes
//----------------------------------

// Physics Related Includes
//----------------------------------
#include "GAMEPLAY_boundingbox.hpp"
#include "GAMEPLAY_hitbox.hpp"
#include "GAMEPLAY_hitmesh.hpp"

// Graphics Framework Includes
//----------------------------------
#include "GRAPHICS_color.hpp"
#include "GRAPHICS_pixel.hpp"

// ID Declaration Includes
//----------------------------------
#include "SYSTEMS_iddeclares.hpp"

using namespace std;

int main()
{
    cout << "Hello world! \n";

    auto testLocation = make_pair<int, int>(0, 0);

    ColorClass testColor = ColorClass(0, 255, 0, 0);
    Range testRange = Range(0, 100);
    BoundingBox testBox = BoundingBox(Range(0, 100), Range(0, 100));
    Hitbox testHitbox = Hitbox(Range(0, 100), Range(0, 100), testLocation, 0);
    Pixel testPixel = Pixel(testLocation, testBox, testColor);
    IdManager testManager = IdManager();
   
   //try using https://en.cppreference.com/w/cpp/utility/variant for events & objects

    return 0;
}
