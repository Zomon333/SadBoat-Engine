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
#ifndef SPRITE_H
#define SPRITE_H

#include "GRAPHICS_pixel.hpp"
#include <vector>
#include <string>
#include <utility>

using namespace std;

class Sprite
{
private:
    vector<vector<Pixel>> spriteContents;

public:
    // Constructors
    //----------------------------------
    Sprite();
    Sprite(vector<vector<Pixel>> spriteContents);
    Sprite(string fileLocation);
    Sprite(string resourceName);

    // Accessors
    //----------------------------------
    Pixel getPixel(pair<int, int> coordinates);
    Pixel getPixel(int x, int y);

    void display();

    // Mutators
    //----------------------------------
    void setPixel(Pixel newPixel, pair<int, int> coordinates);
    void setPixel(Pixel newPixel, int x, int y);
    
};

#endif