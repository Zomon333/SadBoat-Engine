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

#include <functional>
#include <thread>
#include <future>

#include <any>
#include <deque>


template <class Return, class ...Parameters>
class Event
{
    private:
        std::function<Return(Parameters...)> function;
        std::future<Return> future;

    public:
        Event()
        {
            function = [](){return Return();};
        }
        Event(auto func)
        {
            function = func;
        }

        //Launch as it's own thread
        void launch(Parameters... params)
        {
            future = std::async(std::launch::async, function, params...);
        }

        //Wait for the result, then get it when it exists.
        Return getResult()
        {
            future.wait();
            Return returnable = future.get();
            return returnable;
        }

        //Run linearly, do not multithread
        Return operator()(Parameters... params)
        {
            return function(params...);
        }
};

#endif