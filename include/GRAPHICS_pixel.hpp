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
#ifndef PIX_H
#define PIX_H

#include "GAMEPLAY_boundingbox.hpp"
#include "GRAPHICS_color.hpp"

class Pixel : public BoundingBox, public ColorClass
{
public:
    // Constructors
    //----------------------------------
    Pixel()
    {
        Pixel(1, 1, make_pair<int, int>(0, 0), ColorClass(0, 0, 0, 0));
    }
    Pixel(pair<int, int> location, BoundingBox box, ColorClass color)
    {
        int width = box.getWidth().getSpan();
        int height = box.getHeight().getSpan();
        Pixel(width, height, location, color);
    }
    Pixel(int width, int height, pair<int, int> location, ColorClass color)
    {
        BoundingBox(
            Range(
                width,
                width + location.first),
            Range(
                height,
                height + location.second));
        ColorClass(color.getR(), color.getG(), color.getB(), color.getA());
    }

    //Equivalence Operators
    //----------------------------------
    bool operator==(Pixel rhs)
    {
        if (this->getColorClass() != rhs.getColorClass())
            return false;

        if (this->getWidth() != rhs.getWidth())
            return false;

        if (this->getHeight() != rhs.getHeight())
            return false;

        return true;
    }
    bool operator!=(Pixel rhs)
    {
        if(*this == rhs)
        return false;
        return true;
    }

};

#endif