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

        std::packaged_task<Return(Parameters...)> package()
        {
                return  std::packaged_task<Return(Parameters...)>(                      //Packaged tasks can only *move* data
                            std::function<Return(Parameters...)>(                       //So give it a function
                                static_cast<const                                       //Which was copy constructed
                                    std::function<Return(Parameters...)>>(function)));  //From another function
        }                                                                            //So the task is a free floating copy constucted task

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

        std::future<Return> getTopFuture()
        {
            return callStack.top();
        }

        //  Execution modes
        //----------------------------------

        //Launch as it's own thread
        void launch(Parameters... params)
        {
            //Get the function as a task
            auto task = package();

            //Get the future of the task
            callStack.emplace(task.get_future());

            //Create a new thread and move the lambda function to it
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