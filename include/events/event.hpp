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
#include <stack>
#include <thread>
#include <future>

template <class Return, class ...Parameters>
class Event
{
    private:
        //Holds the function to be executed
        std::function<Return(Parameters...)> function;// = std::function<int(int)>([](int a){return a;});
        
        //Predicts the future, somewhat
        std::stack<std::future<Return>> callStack;

    public:
        //  Constructors
        //----------------------------------
        Event()
        {
            function = [](Parameters...){return Return();};
        }
        Event(auto func)
        {
            function = std::function<Return(Parameters...)>(func);
        }


        //  Accessors
        //----------------------------------

        //Wait for the result, then get it when it exists.
        Return getResult()
        {
            //If there are no results to get, give a default answer.
            if(callStack.size()==0)
            {
                return Return();
            }
            callStack.top().wait();
            Return result = callStack.top().get();
            callStack.pop();
            return result;
        }

        //  Execution modes
        //----------------------------------

        //Launch as it's own thread
        void launch(Parameters... params)
        {
            //Create a new function that is a copy of the stored function
            //  - Doing so requires casting the other function to be a constant,
            //    as that's the only copy constructor for the function class
            std::function<Return(Parameters...)> tempFunc(static_cast<const std::function<Return(Parameters...)>>(function));

            //Create a packaged task using the temp function
            //  - This moves the referenced lambda function into the task, because tasks are move only.
            std::packaged_task<Return(Parameters...)> task = std::packaged_task<Return(Parameters...)>(tempFunc);

            //Get the future of the task
            //  - This makes a pointer to the return statement and lets us reference it later.
            //  - I've added an implementation that makes a call stack, allowing us to call an event numerous times and get the results repeatedly.
            callStack.emplace(task.get_future());

            //Create a new thread and move the lambda function to it
            //  - This unassigns the function from the packaged task, but the packaged task
            //    was a copy of a function constant anyways
            std::jthread thread(std::move(task), params...);

            //Be free, my child!
            thread.detach();
        }

        //Do not create a new thread; run synchronously.
        Return operator()(Parameters... params)
        {
            return function(params...);
        }
};

#endif