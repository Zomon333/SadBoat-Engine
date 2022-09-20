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
#include <stack>
#include <thread>
#include <future>

//Event: Events
//Base class for the event system. 
//Allows passage of both lambda functions and std::function for both single and multi threaded use.
//
//Template takes any return and any parameters, allowing flexibility in your events.
//
//Returns asynchronously and stores results in an internal call stack.
template <class Return, class ...Parameters>
class Event
{
    protected:
        //Holds the function to be executed
        std::function<Return(Parameters...)> function;// = std::function<int(int)>([](int a){return a;});
        
        //Predicts the future, somewhat
        std::stack<std::future<Return>> callStack;

        std::packaged_task<Return(Parameters...)> copyPackage()
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
        Event(std::function<Return(Parameters...)> func)
        {
            function = func;
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
            //Get the function as a task
            auto task = copyPackage();

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

//Alternate implementation for if we want to return void with parameters
template <class... Parameters>
class Event<void, Parameters...> : public Event<int, Parameters...>
{
    public:
        Event()
        {
            this->function = [](Parameters... params){return 0;};
        }
        Event(std::function<void(Parameters...)> func)
        {
            this->function = [func](Parameters... params){ func(params...); return 0;};
        }
        Event(auto func)
        {
            this->function = std::function<int(Parameters...)>([func](Parameters... params)
            {
                func(params...);
                return 0;
            });
        }

        //This is the same as a normal event's launch except that it doesn't add to the call stack.
        void launch(Parameters... params)
        {
            //Get the function as a task
            auto task = this->copyPackage();

            //Create a new thread and move the lambda function to it
            std::jthread thread(std::move(task), params...);

            //Be free, my child!
            thread.detach();
        }

        void getResult()
        {
            return;
        }

        void operator()(Parameters... params)
        {
            this->function(params...);
        }

};

//Alternate implementation for if we want to return void without parameters
template <>
class Event<void, void> : public Event<int, int>
{
    public:
        Event()
        {
            this->function = [](int a=0){return 0;};
        }
        Event(std::function<void(void)> func)
        {
            this->function = [func](int a=0){ func(); return 0;};
        }
        Event(auto func)
        {
            this->function = std::function<int(int)>([func](int a = 0)
            {
                func();
                return 0;
            });
        }

        //This is the same as a normal event's launch except that it doesn't add to the call stack and takes no parameters.
        void launch()
        {
            //Get the function as a task
            auto task = this->copyPackage();

            //Create a new thread and move the lambda function to it
            std::jthread thread(std::move(task),0);

            //Be free, my child!
            thread.detach();
        }

        void getResult()
        {
            return;
        }

        void operator()()
        {
            this->function(0);
        }

};

//Alternate implementation for if we want to return some value without parameters.
//Doesn't sound super useful, unless you capture something external.
template <class Return>
class Event<Return, void> : public Event<Return, int>
{
    public:
        Event()
        {
            this->function = [](int a=0){return Return();};
        }
        Event(std::function<void(void)> func)
        {
            this->function = [func](int a=0){ return func();};
        }
        Event(auto func)
        {
            this->function = std::function<int(int)>([func](int a = 0)
            {
                return func();
            });
        }

        //This is the same as a normal event's launch except that it takes no parameters
        void launch()
        {
            //Get the function as a task
            auto task = this->copyPackage();

            this->callStack.emplace(task.get_future());

            //Create a new thread and move the lambda function to it
            std::jthread thread(std::move(task),0);

            //Be free, my child!
            thread.detach();
        }

        Return operator()()
        {
            return this->function(0);
        }

};

#endif