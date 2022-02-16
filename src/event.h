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

#include <chrono>
#include <queue>
#include <thread>
using namespace std;


template<class R, class P> class Event
{
    private:
        static priority_queue<pair<chrono::milliseconds, Event*>,vector<pair<chrono::milliseconds, Event*>>,greater<pair<chrono::milliseconds, Event*>>> eventScheduler;

        R (*eventFunction)(P parameter);
        
        P parameters;
        R returnable;

        bool multiThreaded;


    public:
        Event() //Creates event with return of R and parameters of P at NOW
        {
            Event<R, P>* toPass;
        }



};









#endif