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

class range
{
private:
    int min, max;

public:
    range()
    {
        range(0, 0);
    }
    range(int min, int max)
    {
        setRange(min, max);
    }

    void setRange(pair<int, int> toSet)
    {
        this->min = toSet.first;
        this->max = toSet.second;
    }
    void setRange(int min, int max)
    {
        this->min = min;
        this->max = max;
    }

    bool inRange(int toCheck)
    {
        return (toCheck>min && toCheck<max);
    }
    bool onBounds(int toCheck)
    {
        return (toCheck==min || toCheck==max);
    }

};

#endif