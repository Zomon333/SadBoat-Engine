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
#ifndef RANGE_H
#define RANGE_H

namespace SBE
{
    //Range:    Primitive
    //Class for storing min/max as double
    //Overloaded for boolean operations using [ ]
    class Range
    {
    private:
        double min, max;

    public:
        //Constructors
        //----------------------------------
        Range()
        {
            setRange(0,0);
        }
        Range(double min, double max) : Range()
        {
            setRange(min,max);
        }

        //Mutators
        //----------------------------------    
        void setRange(std::pair<double, double> toSet)
        {
            this->min = toSet.first;
            this->max = toSet.second;
        }
        void setRange(double min, double max)
        {
            this->min = min;
            this->max = max;
        }

        //Accessor
        //----------------------------------
        double getSpan()
        {
            return (max - min);
        }    
        double getMin() { return min; }
        double getMax() { return max; }

        //Collision Logic
        //----------------------------------
        bool inRange(double toCheck)
        {
            return (toCheck>min && toCheck<max);
        }
        bool onBounds(double toCheck)
        {
            return (toCheck==min || toCheck==max);
        }
        bool outBounds(double toCheck)
        {
            return (!inRange(toCheck) && !onBounds(toCheck));
        }


        //Arithmetic Operators
        //----------------------------------    
        Range operator+(pair<double, double> rhs)
        {
            return Range(this->getMin()+rhs.first, this->getMax()+rhs.second);
        }
        Range operator-(pair<double, double> rhs)
        {
            return Range(this->getMin()-rhs.first, this->getMax()-rhs.second);   
        }
        void operator+=(pair<double, double> rhs)
        {
            this->min+=rhs.first;
            this->max+=rhs.second;
        }
        void operator-=(pair<double, double> rhs)
        {
            this->min-=rhs.first;
            this->min-=rhs.second;
        }

        //Equivalence Operators
        //----------------------------------    
        bool operator==(Range rhs)
        {
            //Add tolerance
            if(this->min == rhs.min && this-> max == rhs.max)
            return true;
            return false;
        }

        bool operator!=(Range rhs)
        {
            if(*this == rhs)
            return false;
            return true;

        }

        bool operator[](double rhs)
        {
            return (inRange(rhs) || onBounds(rhs));
        }

    };
};
#endif