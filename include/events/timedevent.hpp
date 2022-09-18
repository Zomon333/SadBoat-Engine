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
    std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> exeTime;
    bool suppressed = false;

    void insertDeferred(std::future<Return> entry)
    {
        static_cast<Event<Return, Parameters...>*>(this).callStack.push(entry);
    }

public:
    TimedEvent()
    {
        this->Event();
    }

    TimedEvent(std::function<Return(Parameters...)> lFunc)
    {
        exeTime = std::chrono::steady_clock().now();
        this->Event(lFunc);
    }
    TimedEvent(std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> newExe, std::function<Return(Parameters...)> lFunc)
    {
        exeTime = newExe;
        this->Event(lFunc);
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

    //Awaits condition is true, then launches.
    //Don't use this for long running conditions! It opens a watchdog thread!
    void defer(std::future<bool> condition, Parameters... params)
    {
        //Create a new event
        //Event takes 3 parameters: The old event, a condition, and the old event's parameters
        Event<Return, std::future<bool>, TimedEvent<Return, Parameters...>*, Parameters...> watchdog(        

            //This event will operate like a watchdog.
            F(std::future<bool> condition, TimedEvent<Return, Parameters...>* context, Parameters... params)
            {

                //When the new event is called:
                //Wait for the condition to resolve
                condition.wait();

                //If the condition is true,
                if(condition.get()==true)
                {
                    //Run the stored event with the parameters given
                    //Return it's value
                    return context(params...);                                                                 
                }

                //If the condition wasn't true, just make a new value.
                return Return();
            }
        );

        //Launch our watchdog thread
        //Have it watch a given condition, and when that condition is true, run *this* event.
        watchdog.launch(condition, this, params...);

        //While the watchdog is running, get the future on top of it's callStack
        //This future should either be the return of the watchdog event, which will either be Return() or the return of *this* event.
        insertDeferred(watchdog.getTopFuture());
    }


    //Awaits condition is true, then launches.
    //Don't use this for long running conditions! It opens a watchdog thread!
    void defer(bool* condition, Parameters... params)
    {
        //Create a new event
        //Event takes 3 parameters: The old event, a condition, and the old event's parameters
        Event<Return, bool*, TimedEvent<Return, Parameters...>*, Parameters...> watchdog(        

            //This event will operate like a watchdog.
            F(bool* condition, TimedEvent<Return, Parameters...>* context, Parameters... params)
            {

                //When the new event is called:
                //Wait for the condition to become true
                while((*condition)==false);

                //Then run context's thread here
                return context(params...);                                                                 
            }
        );

        //Launch our watchdog thread
        //Have it watch a given condition, and when that condition is true, run *this* event.
        watchdog.launch(condition, this, params...);

        //While the watchdog is running, get the future on top of it's callStack
        //This future should either be the return of the watchdog event, which will either be Return() or the return of *this* event.
        insertDeferred(watchdog.getTopFuture());
    }

    Return call(Parameters... params)
    {
        if(!suppressed)
            return (*this)(params...);
        else return Return();
    }

    void launch(Parameters... params)
    {
        if(!suppressed)
        {    
            static_cast<Event<Return, Parameters...>*>(this).launch(params...);
        }
    }

    std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> getTime()
    {
        return exeTime;
    }
    void setTime(std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> nTime)
    {
        exeTime=nTime;
    }


    void operator=(std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> rhs)
    {
        exeTime = rhs;
    }
    TimedEvent<Return, Parameters...> operator+(std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> rhs)
    {
        TimedEvent<Return, Parameters...> tempEvent(*this);
        tempEvent.setTime(getTime()+rhs);
        return tempEvent;
    }
    TimedEvent<Return, Parameters...> operator-(std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> rhs)
    {
        TimedEvent<Return, Parameters...> tempEvent(*this);
        tempEvent.setTime(getTime()-rhs);
        return tempEvent;
    }
    TimedEvent<Return, Parameters...> operator*(std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> rhs)
    {
        TimedEvent<Return, Parameters...> tempEvent(*this);
        tempEvent.setTime(getTime()*rhs);
        return tempEvent;
    }
    TimedEvent<Return, Parameters...> operator/(std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> rhs)
    {
        TimedEvent<Return, Parameters...> tempEvent(*this);
        tempEvent.setTime(getTime()/rhs);
        return tempEvent;
    }


    bool operator>(std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> rhs)
    {
        return (exeTime.time_since_epoch()>rhs.time_since_epoch());
    }
    bool operator<(std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> rhs)
    {
        return (exeTime.time_since_epoch()<rhs.time_since_epoch());
    }
    bool operator==(std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> rhs)
    {
        return (exeTime.time_since_epoch()==rhs.time_since_epoch());
    }
    bool operator!=(std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> rhs)
    {
        return !((*this)==rhs);
    }
    bool operator>=(std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> rhs)
    {
        return (((*this)>rhs)||((*this)==rhs));
    }
    bool operator<=(std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> rhs)
    {
        return (((*this)<rhs)||((*this)==rhs));
    }


    bool operator>(TimedEvent<Return, Parameters...> rhs)
    {
        return (*this)>rhs.getTime();
    }
    bool operator<(TimedEvent<Return, Parameters...> rhs)
    {
        return (*this)<rhs.getTime();
    }
    bool operator==(TimedEvent<Return, Parameters...> rhs)
    {
        return (*this)==rhs.getTime();
    }
    bool operator!=(TimedEvent<Return, Parameters...> rhs)
    {
        return (*this)!=rhs.getTime();
    }
    bool operator>=(TimedEvent<Return, Parameters...> rhs)
    {
        return (*this)>=rhs.getTime();
    }
    bool operator<=(TimedEvent<Return, Parameters...> rhs)
    {
        return (*this)<=rhs.getTime();
    }
};

#endif