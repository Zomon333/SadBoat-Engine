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
#ifndef T_EVENT_H
#define T_EVENT_H

#include <functional>
#include <stack>
#include <thread>
#include <future>
#include <chrono>
#include "event.hpp"



template <class Return, class... Parameters>
class TimedEvent : public Event<Return, Parameters...>
{
private:
    Instant exeTime;
    bool suppressed = false;

public:
    TimedEvent() 
    : Event<Return, Parameters...>()
    {
        exeTime = Instant(uTime(0));
    }

    TimedEvent(auto lFunc)
    : Event<Return, Parameters...>(lFunc)
    {
        exeTime = Instant(uTime(0));
    }

    TimedEvent(auto lFunc, Instant newExe)
    : Event<Return, Parameters...>(lFunc)
    {
        exeTime = newExe;
    }

    TimedEvent(TimedEvent<Return, Parameters...> &cp)
    : Event<Return, Parameters...>(cp.function)
    {
        exeTime = cp.getTime();
    }

    //Do not launch event on time
    void suppress()
    {
        suppressed = true;
    }

    //Undo suppress
    void release()
    {
        suppressed = false;
    }


    //Launches thread, but awaits time==now before running event.
    //Don't use this for long running conditions! It opens a watchdog thread!
    void defer(Instant execution, Parameters... params)
    {
        auto f = lF(Instant execution, std::promise<Return>* toReturn, Parameters... params)
        {
           std::this_thread::sleep_until(execution);
           toReturn->set_value(this->call(params...));
        };

        std::promise<Return> deferredResult;
        

        std::jthread thread(
            std::move(f),
            execution,
            &deferredResult,
            params...
        );

        thread.detach();
        this->callStack.emplace(deferredResult.get_future());
    }

    Return call(Parameters... params)
    {
        if(!suppressed)
            return this->function(params...);
        else return Return();
    }

    void launch(Parameters... params)
    {
        if(!suppressed)
        {    
            Event<Return, Parameters...>::launch(params...);
        }
    }

    Instant getTime()
    {
        return exeTime;
    }
    void setTime(Instant nTime)
    {
        exeTime=nTime;
    }
    
    Return operator()(Parameters... rhs)
    {
       return this->function(rhs...);
    }

    bool operator==(Instant rhs)
    {
        return this->getTime()==rhs;
    }
    bool operator==(TimedEvent<Return, Parameters...> rhs)
    {
        return (*this)==rhs.getTime();
    }

    bool operator!=(Instant rhs)
    {
        return this->getTime()!=rhs;
    }
    bool operator!=(TimedEvent<Return, Parameters...> rhs)
    {
        return (*this)!=rhs.getTime();
    }

    bool operator>(Instant rhs)
    {
        return this->getTime()>rhs;
    }
    bool operator>(TimedEvent<Return, Parameters...> rhs)
    {
        return (*this)>rhs.getTime();
    }

    bool operator<(Instant rhs)
    {
        return this->getTime()<rhs;
    }
    bool operator<(TimedEvent<Return, Parameters...> rhs)
    {
        return (*this)<rhs.getTime();
    }

    bool operator<=(Instant rhs)
    {
        return !((*this)>rhs);
    }
    bool operator<=(TimedEvent<Return, Parameters...> rhs)
    {
        return !((*this)>rhs);
    }

    bool operator>=(Instant rhs)
    {
        return !((*this)<rhs);
    }
    bool operator>=(TimedEvent<Return, Parameters...> rhs)
    {
        return !((*this)<rhs);
    }
};

#endif