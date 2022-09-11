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

template <class Return, class ...Parameters>
class Event
{
    private:
        // This line of code was previously used. It's become defunct as it can't easily be passed to a std::jthread.
        // std::function<Return(Parameters...)> function;

        std::packaged_task<Return(Parameters...)> function;
        std::future<Return> future;

    public:
        Event()
        {
            function = [](){return Return();};
        }
        Event(auto func)
        {
            // This line of code was for when function was an std::function.
            // function = func;

            function = std::packaged_task<Return(Parameters...)>(func); 
        }

        //Launch as it's own thread
        void launch(Parameters... params)
        {
            // This line of code also works and is somewhat thread-optimized. JThread is probably faster though, and auto joins anyways.
            //  future = std::async(std::launch::async, function, params...);

            future = function.get_future();
            std::jthread thread(std::move(function), params...);
            thread.detach();
        }

        //Wait for the result, then get it when it exists.
        Return getResult()
        {
            future.wait();
            Return returnable = future.get();
            return returnable;
        }

        //Creates new thread, does not detach.
        Return operator()(Parameters... params)
        {
            future = function.get_future();
            std::jthread thread(std::move(function), params...);
            return getResult();
        }
};

#endif