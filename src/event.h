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

#include "globals.h"
#include <chrono>
#include <queue>
#include <stack>
#include <thread>

using namespace std;


enum timeType
{
    STATIC,
    DYNAMIC,
    INSTANT
};

template<class Return, class Parameter> class Event
{
    private:

    typedef pair<chrono::milliseconds, Event*> eventPairing;
    typedef Return (*function)(Parameter);

        static priority_queue<
            eventPairing,
            vector<eventPairing>,
            greater<eventPairing>
        >   eventScheduler;

    function eventFunction;

    Parameter parameters;
    Return returnable;

    bool multiThreaded;

    public:

    Event()
    {
        //Do nothing with default constructor
    }

    Event(Return toReturn, Parameter toParameters, function eventFunction, bool multiThreaded)
    {
        this->eventFunction = eventFunction;
        parameters = toParameters;
        returnable = toReturn;
        this->multiThreaded = multiThreaded;
    }

    Event(Return toReturn, Parameter toParameters, function eventFunction, bool multiThreaded, chrono::milliseconds time, timeType type)
    {
        Event(toReturn, toParameters, eventFunction, multiThreaded);
        queueEvent(time, type);
    }

    void queueEvent(chrono::milliseconds time, timeType type)
    {
        if(type==INSTANT)
        {
            eventScheduler.push(make_pair(
                chrono::system_clock::now(),
                this
                ));
        }
        if(type==DYNAMIC)
        {
            eventScheduler.push(make_pair(
                (
                    chrono::system_clock::now()+time
                ),
                this
                ));
        }
        if(type==STATIC)
        {
            eventScheduler.push(make_pair(
                time,
                this
            ));
        }
    }

    void runEvent()
    {
        if(multiThreaded)
        {
            callStack.emplace(
                thread(eventFunction, parameters)
            );
        }
        else
        {
            returnable = eventFunction(parameters);
        }
    }


};

/*template<class R, class P> class Event
{
    private:
        static priority_queue<pair<chrono::milliseconds, Event*>,vector<pair<chrono::milliseconds, Event*>>,greater<pair<chrono::milliseconds, Event*>>> eventScheduler;

        R (*eventFunction)(P parameter);
        
        P parameters;
        R returnable;

        bool multiThreaded;


    public:
        static stack<thread*> openThreads;

        Event() //Creates event with return of R and parameters of P at NOW
        {
            Event<R, P>* toPass;
        }



};*/



#endif