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
#ifndef UVECTOR_H
#define UVECTOR_H

#include <utility>
#include "../primitives/angles.hpp"
#include "../primitives/point.hpp"

template <class Unit>
class UVector
{
    private:
        Point origin = Point();
        Unit angle = Unit();

    public:
        //Constructors
        //----------------------------------
        UVector();
        UVector(Degrees deg, Point origin=Point())
        {
            angle = Unit(deg);
            this->origin=origin;
        }
        UVector(Radians rad, Point origin=Point())
        {
            angle = Unit(rad);
            this->origin=origin;
        }


        //Accessors
        //----------------------------------
        Unit getAngle()
        {
            return angle;
        }
        
        const Point getOrigin()
        {
            return origin;
        }

        const int getMagnitude()
        {
            return 1;
        }

        //Mutators
        //----------------------------------
        void setAngle(Unit angle2)
        {
            angle=angle2;
        }
        
        //Vector sum
        UVector operator+(UVector rhs)
        {
            return UVector((this->angle)+rhs.getAngle());
        }

        //Difference of vector
        UVector operator-(UVector rhs)
        {
            return UVector((this->angle)-rhs.getAngle());
        }

        //Multiply by scalar
        UVector operator*(double rhs)
        {
            return UVector((this->getMagnitude())*rhs);
        }

        //Divide by scalar
        UVector operator/(double rhs)
        {
            return UVector((this->getMagnitude())*rhs);
        }

        //Negative of vector
        UVector operator!()
        {
            return UVector((this->getAngle())*-1)
        }

};

#endif