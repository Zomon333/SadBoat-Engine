/*
Copyright 2024 Dagan Poulin, Justice Guillory
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

#include <any>
#include <functional>
#include <future>
#include <mutex>
#include <stack>
#include <thread>

namespace SBE
{

    template <class Return, class ...Parameters>
    class Event
    {
        protected:
            std::function<Return(Parameters...)> function;
            std::mutex accessible;            
            std::vector<std::function<Return(Parameters...)>*> concFuncs;
            std::stack<std::future<Return>> callStack;
            std::stack<std::jthread::id> idStack;
            std::stack<std::jthread*> threadStack;
            
            std::packaged_task<Return(Parameters...)> copyPackage();

        public:
            //  Constructors
            //----------------------------------
            Event();
            Event(std::function<Return(Parameters...)> func);
            Event(const Event &copiedEvent);

            // Auto keywords cannot always be resolved at compile-time. If we always
            // pre-compile the event module, this will result in linker errors due to
            // unresolved template references. The ability to have flexible template
            // references is fundamental to the Event system. Therefore, all auto
            // declarations must be defined in the header file to allow for compile-time
            // flexibility.
            Event(auto func)
            {
                function = std::function<Return(Parameters...)>(func);
            }

            ~Event();

            //  Accessors
            //----------------------------------

            Return getResult();
            std::thread::id getID();
            std::jthread* getThread();

            //  Execution modes
            //----------------------------------

            void launch(Parameters... params);
            Return operator()(Parameters... params);
            Return call(Parameters... params);

            //  Operators
            //----------------------------------

            void operator=(Event& rhs);
            Event<Return, Parameters...> operator+(Event<Return, Return> &rhs);
            template<class Intermediary=Return>
            Event<Return, Parameters...> combine(Event<Return, Intermediary> &second, Event<Intermediary, Parameters...> &first);
            void operator+=(Event<Return, Return> &rhs);
            Event<Return, Return> operator*(int i);
            void operator*=(int i);

    };

    /*
        Different template specializations for different states of void for parameters and returns.
        These aren't fully tested yet and may not be fully functional.
    */

    template<class Return>
    class Event<Return, void> : Event<Return, int>
    {
        public:
        // Auto keywords cannot always be resolved at compile-time. If we always
        // pre-compile the event module, this will result in linker errors due to
        // unresolved template references. The ability to have flexible template
        // references is fundamental to the Event system. Therefore, all auto
        // declarations must be defined in the header file to allow for compile-time
        // flexibility.
        Event(auto  func)
        {
            Event<Return, int> EventB([&func](int a){
                return func();
            });
            ((Event<Return, int>*)(this))->operator=(EventB);
        }
        Return call();
        void launch();
    };

    template<class ...Parameters>
    class Event<void, Parameters...> : Event<int, Parameters...>
    {
        public:
        // Auto keywords cannot always be resolved at compile-time. If we always
        // pre-compile the event module, this will result in linker errors due to
        // unresolved template references. The ability to have flexible template
        // references is fundamental to the Event system. Therefore, all auto
        // declarations must be defined in the header file to allow for compile-time
        // flexibility.
        Event(auto func)
        {
            Event<int, Parameters...> EventB([&func](Parameters... a){
                func(a...);
                return 0;
            });
            ((Event<int, Parameters...>*)(this))->operator=(EventB);
        }
        void call(Parameters... params);
        void launch(Parameters... params);
    };

    template<>
    class Event<void, void> : Event<int, int>
    {
        public:
        // Auto keywords cannot always be resolved at compile-time. If we always
        // pre-compile the event module, this will result in linker errors due to
        // unresolved template references. The ability to have flexible template
        // references is fundamental to the Event system. Therefore, all auto
        // declarations must be defined in the header file to allow for compile-time
        // flexibility.
        Event(auto func)
        {
            Event<int, int> EventB([&func](int a){
                func();
                return a;
            });
            ((Event<int, int>*)(this))->operator=(EventB);
        }
        void call();
        void launch();
    };

    // Event Implementation Functions
    // <Return, Parameters...>
    //----------------------------------------------------------------------------------------------
    
    // Private:
    template<class Return, class ...Parameters>
    std::packaged_task<Return(Parameters...)> Event<Return, Parameters...>::copyPackage()
    {
        return  std::packaged_task<Return(Parameters...)>(                                  //Packaged tasks can only *move* data
                                std::function<Return(Parameters...)>(                       //So give it a function
                                    static_cast<const                                       //Which was copy constructed
                                        std::function<Return(Parameters...)>>(function)));  //From another function
    }

    // Public:
    // Constructors
    //----------------------------------
    template<class Return, class ...Parameters>
    Event<Return, Parameters...>::Event()
    {
        function = [](Parameters...){return Return();};
    }

    template<class Return, class ...Parameters>
    Event<Return, Parameters...>::Event(std::function<Return(Parameters...)> func) : 
        Event()
    {
        function = func;
    }

    template<class Return, class ...Parameters>
    Event<Return, Parameters...>::Event(const Event &copiedEvent) : 
        Event()
    {
        this->function = std::function<Return(Parameters...)>(static_cast<const std::function<Return(Parameters...)>>(copiedEvent.function));
    }

    template<class Return, class ...Parameters>
    Event<Return, Parameters...>::~Event()
    {
        while(concFuncs.size()>0)
        {
            delete concFuncs.back();
            concFuncs.pop_back();
        }
    }

    // Accessors
    //----------------------------------
    template<class Return, class ...Parameters>
    Return Event<Return, Parameters...>::getResult()
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

    template<class Return, class ...Parameters>
    std::thread::id Event<Return, Parameters...>::getID()
    {
        accessible.lock();
        auto id = idStack.top();
        accessible.unlock();

        return id;
    }

    template<class Return, class ...Parameters>
    std::jthread* Event<Return, Parameters...>::getThread()
    {
        accessible.lock();
        auto thread = threadStack.top();
        accessible.unlock();

        return thread;
    }

    // Execution modes
    //----------------------------------

    //Launch as it's own thread
    template<class Return, class ...Parameters>
    void Event<Return, Parameters...>::launch(Parameters... params)
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
    template<class Return, class ...Parameters>
    Return Event<Return, Parameters...>::operator()(Parameters... params)
    {
        return function(params...);
    }

    template<class Return, class ...Parameters>
    Return Event<Return, Parameters...>::call(Parameters... params)
    {
        return function(params...);
    }

    // Operators
    //----------------------------------

    //Equivalence operator
    //Copies the function of the given event into this event. Does not copy the call stack.
    template<class Return, class ...Parameters>
    void Event<Return, Parameters...>::operator=(Event& rhs)
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
    template<class Return, class ...Parameters>
    Event<Return, Parameters...> Event<Return, Parameters...>::operator+(Event<Return, Return> &rhs)
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
    template<class Return, class ...Parameters>
    template<class Intermediary>
    Event<Return, Parameters...> Event<Return, Parameters...>::combine(Event<Return, Intermediary> &second, Event<Intermediary, Parameters...> &first)
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
    template<class Return, class ...Parameters>
    void Event<Return, Parameters...>::operator+=(Event<Return, Return> &rhs)
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
    template<class Return, class ...Parameters>
    Event<Return, Return> Event<Return, Parameters...>::operator*(int i)
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
    template<class Return, class ...Parameters>
    void Event<Return, Parameters...>::operator*=(int i)
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

    template<class Return>
    Return Event<Return, void>::call()
    {
        return ((Event<Return, int>*)(this))->call(0);
    }

    template<class Return>
    void Event<Return, void>::launch()
    {
        ((Event<Return, int>*)(this))->launch(0);
    }

    template<class ...Parameters>
    void Event<void, Parameters...>::call(Parameters... params)
    {
        ((Event<int, Parameters...>(this)))->call(params...);
    }

    template<class ...Parameters>
    void Event<void, Parameters...>::launch(Parameters... params)
    {
        ((Event<int, Parameters...>)(this))->launch(params...);
    }
};
#endif
