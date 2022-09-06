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
#ifndef POINT_H
#define POINT_H

#include <utility>

class Point
{
    private:
        double x;
        double y;

    public:
        //Constructors
        //----------------------------------
        Point()
        {
            this->x=0;
            this->y=0;
        }
        Point(std::pair<int, int> p)
        {
            this->x=p.first;
            this->y=p.second;
        }

        //Mutators
        //----------------------------------
        void setX(double x2)
        {
            this->x=x2;
        }
        void setY(double y2)
        {
            this->y=y2;
        }

        //Accessors
        //----------------------------------
        double getX()
        {
            return this->x;
        }
        double getY()
        {
            return this->y;
        }

        //Equivalence Operators
        //----------------------------------    
        bool operator==(Point rhs)
        {
            return (
                    rhs.getX() == this->x
                )&&(
                    rhs.getY() == this->y
                );
        }

        bool operator!=(Point rhs)
        {
            return !(*this == rhs);
        }
};

#endif