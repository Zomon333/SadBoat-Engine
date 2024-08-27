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

#ifndef T_EVENT_H
#define T_EVENT_H

#include "event.hpp"

#ifndef EVENT_H
#error Missing header: event.hpp is required to use timed_event.hpp
#endif

namespace SBE
{


    template <class Return, class... Parameters>
    class TimedEvent : public Event<Return, Parameters...>
    {
    private:
        std::chrono::steady_clock::time_point exeTime;
        bool suppressed = false;

        std::mutex accessible;

    public:
        //  Constructors
        //----------------------------------

        TimedEvent();
        TimedEvent(auto lFunc);
        TimedEvent(auto lFunc, std::chrono::steady_clock::time_point newExe);
        TimedEvent(TimedEvent<Return, Parameters...> &cp);

        //  Flow Control Functions
        //----------------------------------

        void suppress();
        void release();

        //  Launch Style Functions
        //----------------------------------

        void defer(std::chrono::steady_clock::time_point execution, Parameters... params);
        void defer(Parameters... params);
        Return call(Parameters... params);
        void launch(Parameters... params);

        //  Accessors
        //----------------------------------

        std::chrono::steady_clock::time_point getTime();
        bool getSuppressed();

        //  Mutators
        //----------------------------------

        void setTime(std::chrono::steady_clock::time_point nTime);
        
        //  Operators
        //----------------------------------

        Return operator()(Parameters... rhs);

        bool operator==(std::chrono::steady_clock::time_point rhs);
        bool operator==(TimedEvent<Return, Parameters...> rhs);

        bool operator!=(std::chrono::steady_clock::time_point rhs);
        bool operator!=(TimedEvent<Return, Parameters...> rhs);

        bool operator>(std::chrono::steady_clock::time_point rhs);
        bool operator>(TimedEvent<Return, Parameters...> rhs);

        bool operator<(std::chrono::steady_clock::time_point rhs);
        bool operator<(TimedEvent<Return, Parameters...> rhs);
        bool operator<=(std::chrono::steady_clock::time_point rhs);
        bool operator<=(TimedEvent<Return, Parameters...> rhs);
        bool operator>=(std::chrono::steady_clock::time_point rhs);
        bool operator>=(TimedEvent<Return, Parameters...> rhs);
    };

    // TimedEvent Implementation Functions
    //----------------------------------------------------------------------------------------------
    
    // Public:
    //  Constructors
    //----------------------------------
    
    template <class Return, class... Parameters>
    TimedEvent<Return, Parameters...>::TimedEvent() : 
        Event<Return, Parameters...>()
    {
        exeTime = std::chrono::steady_clock::time_point(std::chrono::milliseconds(0));
    }

    template <class Return, class... Parameters>
    TimedEvent<Return, Parameters...>::TimedEvent(auto lFunc) : 
        Event<Return, Parameters...>(lFunc)
    {
        exeTime = std::chrono::steady_clock::time_point(std::chrono::milliseconds(0));
    }

    template <class Return, class... Parameters>
    TimedEvent<Return, Parameters...>::TimedEvent(auto lFunc, std::chrono::steady_clock::time_point newExe) : 
        Event<Return, Parameters...>(lFunc)
    {
        exeTime = newExe;
    }

    template <class Return, class... Parameters>
    TimedEvent<Return, Parameters...>::TimedEvent(TimedEvent<Return, Parameters...> &cp) : 
        Event<Return, Parameters...>(cp.function)
    {
        exeTime = cp.getTime();
    }

    //  Flow Control Functions
    //----------------------------------

    template <class Return, class... Parameters>
    void TimedEvent<Return, Parameters...>::suppress()
    {
        accessible.lock();
        suppressed = true;
        accessible.unlock();
    }

    template <class Return, class... Parameters>
    void TimedEvent<Return, Parameters...>::release()
    {
        accessible.lock();
        suppressed = false;
        accessible.unlock();
    }

    //  Launch Style Functions
    //----------------------------------

    //Launches thread, but awaits time==now before running event.
    //Don't use this for long running conditions! It opens a watchdog thread!
    template <class Return, class... Parameters>
    void TimedEvent<Return, Parameters...>::defer(std::chrono::steady_clock::time_point execution, Parameters... params)
    {
        accessible.lock();
        //Copy TimedEvent's intended function
        const std::function<Return(Parameters...)> func = 
                std::function<Return(Parameters...)>(                       
                    static_cast<const                                       
                        std::function<Return(Parameters...)>>
                            (this->function));
        
        //Create some new function that takes the exact same parameters
        //But have it's scope include the old function and intended execution time
        auto nFunc = [this, func, execution](Parameters...params)
        {
            //But don't actually use the captured execution time!
            //If you use the actual captured execution time you may end up with deleted functions and expressions
            //It may throw errors due to that

            //So, instead;

            //Copy it into a constant.
            const std::chrono::steady_clock::time_point lExe = execution;

            //Copy the captured old function aswell
            const std::function<Return(Parameters...)> oldFunc =
                std::function<Return(Parameters...)>(                       
                    static_cast<const                                       
                        std::function<Return(Parameters...)>>
                            (func));

            //Wait until the *copied* execution time
            std::this_thread::sleep_until(lExe);

            //And return the value of the *copied* function with the *normal* parameters.
            //But only if it's not suppressed.
            return this->getSuppressed() ? Return() : oldFunc(params...);
        };

        //Set the event's function to the new deferred function
        this->function = std::function<Return(Parameters...)>(nFunc);

        //Launch a thread with the deferred function
        accessible.unlock();
        this->launch(params...);
        accessible.lock();

        //Set the event's function back to the non-deferred version
        this->function = std::function<Return(Parameters...)>(func);
        accessible.unlock();
    }

    //Launches thread, but awaits time==exeTime before running event.
    //Don't use this for long running conditions! It opens a watchdog thread!
    template <class Return, class... Parameters>
    void TimedEvent<Return, Parameters...>::defer(Parameters... params)
    {
        accessible.lock();
        //Copy TimedEvent's intended function
        const std::function<Return(Parameters...)> func = 
                std::function<Return(Parameters...)>(                       
                    static_cast<const                                       
                        std::function<Return(Parameters...)>>
                            (this->function));
        
        //Create some new function that takes the exact same parameters
        //But have it's scope include the old function and intended execution time
        auto nFunc = [this, func](Parameters...params)
        {
            //But don't actually use the captured execution time!
            //If you use the actual captured execution time you may end up with deleted functions and expressions
            //It may throw errors due to that

            //So, instead;

            //Copy it into a constant.
            const std::chrono::steady_clock::time_point lExe = this->exeTime;

            //Copy the captured old function aswell
            const std::function<Return(Parameters...)> oldFunc =
                std::function<Return(Parameters...)>(                       
                    static_cast<const                                       
                        std::function<Return(Parameters...)>>
                            (func));

            //Wait until the *copied* execution time
            std::this_thread::sleep_until(lExe);

            //And return the value of the *copied* function with the *normal* parameters.
            //But only if it's not suppressed.
            return this->getSuppressed() ? Return() : oldFunc(params...);
        };

        //Set the event's function to the new deferred function
        this->function = std::function<Return(Parameters...)>(nFunc);

        //Launch a thread with the deferred function
        accessible.unlock();
        this->launch(params...);
        accessible.lock();

        //Set the event's function back to the non-deferred version
        this->function = std::function<Return(Parameters...)>(func);
        accessible.unlock();
    }

    template <class Return, class... Parameters>
    Return TimedEvent<Return, Parameters...>::call(Parameters... params)
    {
        if(!suppressed)
        {
            return this->function(params...);
        }
        else 
        {
            return Return();
        }
    }

    template <class Return, class... Parameters>
    void TimedEvent<Return, Parameters...>::launch(Parameters... params)
    {
        accessible.lock();
        if(!suppressed)
        {    
            Event<Return, Parameters...>::launch(params...);
        }
        accessible.unlock();
    }

    //  Accessors
    //----------------------------------

    template <class Return, class... Parameters>
    std::chrono::steady_clock::time_point TimedEvent<Return, Parameters...>::getTime()
    {
        accessible.lock();
        auto time = exeTime;
        accessible.unlock();

        return time;
    }

    template <class Return, class... Parameters>
    bool TimedEvent<Return, Parameters...>::getSuppressed()
    {
        accessible.lock();
        auto sup = suppressed;
        accessible.unlock();

        return sup;
    }

    //  Mutators
    //----------------------------------

    template <class Return, class... Parameters>
    void TimedEvent<Return, Parameters...>::setTime(std::chrono::steady_clock::time_point nTime)
    {
        accessible.lock();
        exeTime=nTime;
        accessible.unlock();
    }
    
    //  Operators
    //----------------------------------

    template <class Return, class... Parameters>
    Return TimedEvent<Return, Parameters...>::operator()(Parameters... rhs)
    {
        return this->function(rhs...);
    }

    template <class Return, class... Parameters>
    bool TimedEvent<Return, Parameters...>::operator==(std::chrono::steady_clock::time_point rhs)
    {
        return this->getTime()==rhs;
    }

    template <class Return, class... Parameters>
    bool TimedEvent<Return, Parameters...>::operator==(TimedEvent<Return, Parameters...> rhs)
    {
        return (*this)==rhs.getTime();
    }

    template <class Return, class... Parameters>
    bool TimedEvent<Return, Parameters...>::operator!=(std::chrono::steady_clock::time_point rhs)
    {
        return this->getTime()!=rhs;
    }

    template <class Return, class... Parameters>
    bool TimedEvent<Return, Parameters...>::operator!=(TimedEvent<Return, Parameters...> rhs)
    {
        return (*this)!=rhs.getTime();
    }

    template <class Return, class... Parameters>
    bool TimedEvent<Return, Parameters...>::operator>(std::chrono::steady_clock::time_point rhs)
    {
        return this->getTime()>rhs;
    }

    template <class Return, class... Parameters>
    bool TimedEvent<Return, Parameters...>::operator>(TimedEvent<Return, Parameters...> rhs)
    {
        return (*this)>rhs.getTime();
    }

    template <class Return, class... Parameters>
    bool TimedEvent<Return, Parameters...>::operator<(std::chrono::steady_clock::time_point rhs)
    {
        return this->getTime()<rhs;
    }

    template <class Return, class... Parameters>
    bool TimedEvent<Return, Parameters...>::operator<(TimedEvent<Return, Parameters...> rhs)
    {
        return (*this)<rhs.getTime();
    }

    template <class Return, class... Parameters>
    bool TimedEvent<Return, Parameters...>::operator<=(std::chrono::steady_clock::time_point rhs)
    {
        return !((*this)>rhs);
    }

    template <class Return, class... Parameters>
    bool TimedEvent<Return, Parameters...>::operator<=(TimedEvent<Return, Parameters...> rhs)
    {
        return !((*this)>rhs);
    }

    template <class Return, class... Parameters>
    bool TimedEvent<Return, Parameters...>::operator>=(std::chrono::steady_clock::time_point rhs)
    {
        return !((*this)<rhs);
    }

    template <class Return, class... Parameters>
    bool TimedEvent<Return, Parameters...>::operator>=(TimedEvent<Return, Parameters...> rhs)
    {
        return !((*this)<rhs);
    }
};
#endif