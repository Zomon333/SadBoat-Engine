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
#ifndef T_EVENT_H
#define T_EVENT_H

#include "EVENTS_event.hpp"
#include <chrono>

using namespace std;

class TimedEvent : public Event
{
    private:
    chrono::steady_clock::time_point time;

    void insert();

    public:
    // Constructors
    //----------------------------------
    RecurringEvent();
    RecurringEvent(Function toCall, bool multiThreaded = false, chrono::steady_clock::time_point time = chrono::steady_clock::now() );

    // Function execution schemes
    //----------------------------------
    auto call(any* parameters);
    auto operator()(any *params);
    void store(any* parameters);
};

#endif