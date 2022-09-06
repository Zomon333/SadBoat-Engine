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
#include <variant>
#include "../primitives/angles.hpp"
#include "../primitives/point.hpp"


class UVector
{
    private:
        Point origin = Point();
        //Radians angle = Radians();
        AngleUnion angle;
    
    public:
        //Constructors
        //----------------------------------
        UVector();
        UVector(Degrees deg, Point origin=Point())
        {
            angle = Radians(deg);
            this->origin=origin;
        }
        UVector(Radians rad, Point origin=Point())
        {
            angle = Radians(rad);
            this->origin=origin;
        }


        //Accessors
        //----------------------------------
        AngleUnion getAngleUnion()
        {
            return angle;
        }

        
        const Point getOrigin()
        {
            return origin;
        }

        const double getMagnitude()
        {
            return 1;
        }

        //Mutators
        //----------------------------------
        void setAngle(std::variant<Radians, Degrees> angle2)
        {
            try
            {
                angle.set(std::get<Radians>(angle2));
            }
            catch(const std::exception& e)
            {
                angle.set(std::get<Degrees>(angle2));
            }
        }
        void setOrigin(Point origin2)
        {
            origin=origin2;
        }
        
        /*//Vector sum
        UVector operator+(UVector rhs)
        {

        }

        //Difference of vector
        UVector operator-(UVector rhs)
        {

        }

        //Multiply by scalar
        UVector operator*(double rhs)
        {   

        }

        //Divide by scalar
        UVector operator/(double rhs)
        {

        }

        //Negative of vector
        UVector operator!()
        {

        }*/

};

#endif