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

#ifndef EVENT_H
#define EVENT_H

#include "function.hpp"
#include "idman.hpp"

using namespace std;

template <class Return, class... ParamPack>
class Event : private Function<Return, ParamPack>
{
private:
    int id;
    bool multiThreaded = false;
    bool joinable = false;

    Return returnable;

public:
    static IdManager eventIDManager;

    //Constructors
    //----------------------------------
    Event()
    {
        id = eventIDManager.allocate();
        this->storedFunction = 0;
    }

    Event(Function<Return, ParamPack> toUse)
    {
        id = eventIDManager.allocate();
        this->storedFunction = toUse;
    }

    //Accessors
    //----------------------------------
    bool getMultiThreaded()
    {
        return this->multiThreaded;
    }
    bool getJoinable()
    {
        return this->joinable;
    }

    //Mutators
    //----------------------------------
    void setFunction(Function<Return, ParamPack> toSet)
    {
        this->storedFunction = toSet.storedFunction;
    }
    void setMultiThreaded(bool multiThread)
    {
        this->multiThreaded = multiThread;
    }


    
    // Function execution schemes
    //----------------------------------
    Return call(ParamPack parameters)
    {
        joinable = false;
        Return temp = this->storedFunction(parameters);
        joinable = true;
        return temp;
    }

    void store(ParamPack parameters)
    {
        joinable = false;
        returnable = call(parameters);
        joinable = true;
    }

    
};

#endif