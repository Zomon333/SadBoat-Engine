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

//Where you want to define some function in an event, you can now just use F(params){}; like normal instead of [](params){};
#define F(a...) [](a...)

#include <functional>
#include <thread>
#include <future>

template <class Return, class ...Parameters>
class Event
{
    private:
        //Holds the function to be executed
        std::packaged_task<Return(Parameters...)> function;
        
        //Predicts the future, somewhat
        std::future<Return> future;

    public:
        //  Constructors
        //----------------------------------
        Event()
        {
            function = [](){return Return();};
        }
        Event(auto func)
        {
            function = std::packaged_task<Return(Parameters...)>(func); 
        }


        //  Accessors
        //----------------------------------

        //Wait for the result, then get it when it exists.
        Return getResult()
        {
            future.wait();
            Return returnable = future.get();
            return returnable;
        }

        //  Execution modes
        //----------------------------------

        //Launch as it's own thread
        void launch(Parameters... params)
        {
            future = function.get_future();
            std::jthread thread(std::move(function), params...);
            thread.detach();
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