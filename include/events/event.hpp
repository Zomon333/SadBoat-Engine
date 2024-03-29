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

#include "utilities/defines.hpp"
#include "sb-engine.hpp"

namespace SBE
{
    //Event: Events
    //Base class for the event system. 
    //Allows passage of both lambda functions and std::function for both single and multi threaded use.
    //
    //Template takes any return and any parameters, allowing flexibility in your events.
    //Cannot be initialized with void for return or parameters. Use int and return error results instead.
    //
    //Returns asynchronously and stores results in an internal call stack.
    template <class Return, class ...Parameters>
    class Event
    {
        protected:
            //Holds the function to be executed
            std::function<Return(Parameters...)> function;// = std::function<int(int)>([](int a){return a;});

            std::mutex accessible;            

            //Holds the concactenated function
            std::vector<std::function<Return(Parameters...)>*> concFuncs;

            //Predicts the future, somewhat
            std::stack<std::future<Return>> callStack;
            
            std::stack<std::jthread::id> idStack;
            std::stack<std::jthread*> threadStack;

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
            Event(std::function<Return(Parameters...)> func) : Event()
            {
                function = func;
            }
            Event(auto func) : Event()
            {
                function = std::function<Return(Parameters...)>(func);
            }
            Event(const Event &copiedEvent) : Event()
            {
                this->function = std::function<Return(Parameters...)>(static_cast<const std::function<Return(Parameters...)>>(copiedEvent.function));
            }

            ~Event()
            {
                while(concFuncs.size()>0)
                {
                    delete concFuncs.back();
                    concFuncs.pop_back();
                }
            }

            //  Accessors
            //----------------------------------

            //Wait for the result, then get it when it exists.
            Return getResult()
            {
                accessible.lock();

                //If there are no results to get, give a default answer.
                if(callStack.size()==0)
                {
                    return Return();
                }
                callStack.top().wait();
                Return result = callStack.top().get();

                callStack.pop();
                idStack.pop();
                threadStack.pop();

                accessible.unlock();

                return result;
            }

            std::jthread::id getID()
            {
                accessible.lock();
                auto id = idStack.top();
                accessible.unlock();

                return id;
            }

            std::jthread* getThread()
            {
                accessible.lock();
                auto thread = threadStack.top();
                accessible.unlock();

                return thread;
            }

            //  Execution modes
            //----------------------------------

            //Launch as it's own thread
            void launch(Parameters... params)
            {
                accessible.lock();

                //Get the function as a task
                auto task = copyPackage();

                //Get the future of the task
                callStack.emplace(task.get_future());

                //Create a new thread and move the lambda function to it
                std::jthread thread(std::move(task), params...);

                idStack.emplace(thread.get_id());
                threadStack.push(&thread);

                //Be free, my child!
                thread.detach();

                accessible.unlock();
            }

            //Do not create a new thread; run synchronously.
            Return operator()(Parameters... params)
            {
                return function(params...);
            }
            Return call(Parameters... params)
            {
                return function(params...);
            }

            //  Operators
            //----------------------------------

            //Equivalence operator
            //Copies the function of the given event into this event. Does not copy the call stack.
            void operator=(Event& rhs)
            {
                accessible.lock();
                this->function = std::function<Return(Parameters...)>(static_cast<const std::function<Return(Parameters...)>>(rhs.function));
                accessible.unlock();
            }

            //
            //  Combines the two added events into a new event, where the 2nd operand is called as an event with the return of the 1st event.
            //  C = A + B  --->  C(params) = B(  A(params)  );
            //  
            //  Because B is called with A's return, B must accept A's return as a parameter.
            //
            Event<Return, Parameters...> operator+(Event<Return, Return> &rhs)
            {
                accessible.lock();
                Event<Return, Parameters...> temp = Event<Return, Parameters...>(
                    [this, &rhs](Parameters... params)
                    {
                        this->launch(params...);
                        return rhs(this->getResult());
                    }
                );
                accessible.unlock();
                return temp;
            }

            // 
            // Combines two explicitly given events into a new event, where the return of the 2nd operand is the parameter to the 1st operand
            // 
            // Because B is called with A's return, B must accept A's return as a parameter.
            // Unlike the other addition operator, B may return a different value than A.
            // 
            template<class Intermediary=Return>
            Event<Return, Parameters...> combine(Event<Return, Intermediary> &second, Event<Intermediary, Parameters...> &first)
            {
                accessible.lock();
                Event<Return, Parameters...> tmpEvent(
                    [&first, &second](Parameters... params)
                    {
                        first.launch(params...);
                        second.launch(first.getResult());
                        return second.getResult();
                    }
                );
                accessible.unlock();
                return tmpEvent;
            }

            //
            //  Performs event addition as described above. Sets the current event to the sum of the two events.
            //
            //  A += B  --->    A = A + B   --->    A(params) = B( A(params) );
            //
            void operator+=(Event<Return, Return> &rhs)
            {
                accessible.lock();
                //Make a new blank function
                std::function<Return(Parameters...)>* old = new std::function<Return(Parameters...)>([](Parameters... params){return Return();});
                
                //Take the current function out
                old->swap(this->function);

                //Make sure we still know where old is
                this->concFuncs.push_back(old);

                //Set the stored function to the old function + RHS's function by reference
                this->function = std::function<Return(Parameters...)>(
                    [old, &rhs](Parameters... params)
                    {
                        return rhs(old->operator()(params...));
                    }
                );
                accessible.unlock();
            }

            

            //
            //  Performs event multiplication with an integer. Multiplication is repeated addition.
            //  Multiplying by 0 returns an event which returns it's parameters. Multiplying by 1 returns the original event.
            //  Returns the result of i-many additive cycles, creating a "looping" event.
            //
            //  A = B * 1   --->    A(params) = B(params);
            //  A = B * 2   --->    A(params) = B( B(params) );
            //  A = B * 3   --->    A(params) = B( B( B(params) ) );
            //  A = B * n   --->    A(params) = B( B(... params));
            //
            Event<Return, Return> operator*(int i)
            {
                accessible.lock();
                Event<Return, Return> returnable(
                    [this, i](Return start)
                    {
                        Return temp = this->operator()(start);
                        for(int j = i-1; j>0; j--)
                        {
                            temp = this->operator()(temp);
                        }
                        return temp;
                    }
                );
                accessible.unlock();
                return returnable;
            }

            

            //  
            //  Performs a multiplication *and* an equivalence operator.
            //  -Multiplication is repeated addition of itself to itself.
            //  -Equivalence updates itself to it's new multiplied value once the operation is done.
            //
            void operator*=(int i)
            {                          
                accessible.lock();
                //Make a new blank function
                std::function<Return(Parameters...)>* old = new std::function<Return(Parameters...)>([](Parameters... params){return Return();});

                //Take the current function out
                old->swap(this->function);

                //Do the manipulation
                Event<Return, Return> toUse(
                    [old, i](Return a)
                    {
                        Return results = old->operator()(a);
                        for(int j = i-1; j>0; j--)
                        {
                            results = old->operator()(results);
                        }
                        return results;
                    }
                );

                //Make sure we still know where old is
                this->concFuncs.push_back(old);

                //Set the stored function to the old function many times
                this->function = toUse.function;
                accessible.unlock();
            }

    };

    /*
        Different template specializations for different states of void for parameters and returns.
        These aren't fully tested yet and may not be fully functional.
    */

    template<class Return>
    class Event<Return, void> : Event<Return, int>
    {
        Event(auto func)
        {
            Event<Return, int> EventB([&func](int a){
                return func();
            });
            ((Event<Return, int>*)(this))->operator=(EventB);
        }

        Return call()
        {
            return ((Event<Return, int>*)(this))->call(0);
        }
        void launch()
        {
            ((Event<Return, int>*)(this))->launch(0);
        }
    };

    template<class ...Parameters>
    class Event<void, Parameters...> : Event<int, Parameters...>
    {
        public:
        Event(auto func)
        {
            Event<int, Parameters...> EventB([&func](Parameters... a){
                func(a...);
                return 0;
            });
            ((Event<int, Parameters...>*)(this))->operator=(EventB);
        }
        void call(Parameters... params)
        {
            ((Event<int, Parameters...>(this)))->call(params...);
        }
        void launch(Parameters... params)
        {
            ((Event<int, Parameters...>)(this))->launch(params...);
        }
    };

    template<>
    class Event<void, void> : Event<int, int>
    {
        public:

        Event(auto func)
        {
            Event<int, int> EventB([&func](int a){
                func();
                return a;
            });
            ((Event<int, int>*)(this))->operator=(EventB);
        }

        void call()
        {
            ((Event<int, int>*)(this))->call(0);
        }

        void launch()
        {
            ((Event<int, int>*)(this))->launch(0);
        }

    };
};
#endif
