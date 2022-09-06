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

#include <utility>

class Range
{
private:
    double min, max;

public:
    //Constructors
    //----------------------------------
    Range()
    {
        Range(0, 0);
    }
    Range(double min, double max)
    {
        setRange(min, max);
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
    int getSpan()
    {
        return (max - min);
    }    

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

    //Equivalence Operators
    //----------------------------------    
    bool operator==(Range rhs)
    {
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

};

#endif