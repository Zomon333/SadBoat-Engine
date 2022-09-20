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
#ifndef TOL_H
#define TOL_H

#include <utility>
#include "range.hpp"

//Tolerance: Primitive
//Class for adding an error rate to a value
//Holds origin and error, uses Range to verify:
// (Origin-Error) <= input <= (Origin+Error)
class Tolerance
{
private:
    double error;
    double origin;
    Range buffer;

public:
    //  Constructors
    //----------------------------------

    Tolerance()
    {
        origin=0;
        error=0.000001;
        double min, max;
        min=origin-error;
        max=origin+error;
        buffer= Range(min, max);
    }
    Tolerance(double origin, double error)
    {
        this->origin=origin;
        this->error=error;
        double min, max;
        min=origin-error;
        max=origin+error;
        buffer= Range(min, max);
    }

    //  Mutators
    //----------------------------------
    
    void setError(double error2)
    {
        error=error2;
    }

    void setOrigin(double origin2)
    {
        //May need changing, tbd
        origin=origin2;
        double min, max;
        min=origin-error;
        max=origin+error;
        buffer= Range(min, max);
    }

    //  Accessors
    //----------------------------------
    
    double getError()
    {
        return error;
    }

    double getOrigin()
    {
        return origin;
    }

    //  inTolerance and op[]
    //----------------------------------    
    
    bool inTolerance(double test)
    {
        return (buffer[test] || buffer.onBounds(test));
    }

    bool operator[](double test)
    {
        return inTolerance(test);
    }
};

#endif