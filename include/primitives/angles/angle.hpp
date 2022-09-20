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
#ifndef ANGLE_H
#define ANGLE_H

#include <utility>
#include <cmath>

//  Abstract class for inheritance reasons
//  Does nothing.
//----------------------------------
class Angle
{
    public:
        enum ATYPE
        {
            RADIANS,
            DEGREES
        };
        
        ATYPE getUnit() { return unit; }
        void setUnit(ATYPE unit2) { unit=unit2; }
        virtual double get() = 0;

    private:
        ATYPE unit;
};

#endif