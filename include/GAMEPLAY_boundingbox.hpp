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
#ifndef BBOX_H
#define BBOX_H

#include <utility>
#include "SYSTEMS_range.hpp"

using namespace std;

class BoundingBox
{
private:
    Range width, height;

public:
    // Constructors
    //----------------------------------
    BoundingBox()
    {
        width = Range(0, 1);
        height = Range(0, 1);
    }

    BoundingBox(int a, int b, int c, int d)
    {
        this->width = Range(a, b);
        this->height = Range(c, d);
    }

    BoundingBox(pair<int, int> a, pair<int, int> b)
    {
        this->width = Range(a.first, a.second);
        this->height = Range(b.first, b.second);
    }

    BoundingBox(Range a, Range b)
    {
        this->width = a;
        this->height = b;
    }

    // Accessors
    //----------------------------------
    Range getWidth()
    {
        return width;
    }

    Range getHeight()
    {
        return height;
    }

    // Mutators
    //----------------------------------
    void setWidth(Range newWidth)
    {
        width = newWidth;
    }

    void setHeight(Range newHeight)
    {
        height = newHeight;
    }

    // Boundary Test Functions
    //----------------------------------
    bool inBox(pair<int, int> toCheck)
    {
        return (width.inRange(toCheck.first) && height.inRange(toCheck.second));
    }

    bool inBox(int x, int y)
    {
        return (width.inRange(x) && height.inRange(y));
    }

    bool onBox(pair<int, int> toCheck)
    {
        return (width.onBounds(toCheck.first) && height.onBounds(toCheck.second));
    }

    bool onBox(int x, int y)
    {
        return (width.onBounds(x) && height.onBounds(y));
    }
};

#endif