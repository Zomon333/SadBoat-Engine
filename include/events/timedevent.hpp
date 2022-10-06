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

#include <chrono>

using namespace std;
using namespace std::chrono;

namespace SBE{
    //TimedEvent: Events, Derived
    //Child class of Base Class Event.
    //Includes chrono time point for time-specific usage.
    //Has suppress and release functionality to cancel events before their time.
    //Has defer to run with respect to time.
    template <class Return, class... Parameters>
    class TimedEvent : public Event<Return, Parameters...>
    {
    private:
        steady_clock::time_point exeTime;
        bool suppressed = false;

    public:
        //  Constructors
        //----------------------------------

        TimedEvent() 
        : Event<Return, Parameters...>()
        {
            exeTime = steady_clock::time_point(milliseconds(0));
        }

        TimedEvent(auto lFunc)
        : Event<Return, Parameters...>(lFunc)
        {
            exeTime = steady_clock::time_point(milliseconds(0));
        }

        TimedEvent(auto lFunc, steady_clock::time_point newExe)
        : Event<Return, Parameters...>(lFunc)
        {
            exeTime = newExe;
        }

        TimedEvent(TimedEvent<Return, Parameters...> &cp)
        : Event<Return, Parameters...>(cp.function)
        {
            exeTime = cp.getTime();
        }

        //  Flow Control Functions
        //----------------------------------

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

        //  Launch Style Functions
        //----------------------------------

        //Launches thread, but awaits time==now before running event.
        //Don't use this for long running conditions! It opens a watchdog thread!
        void defer(steady_clock::time_point execution, Parameters... params)
        {
            //Copy TimedEvent's intended function
            const std::function<Return(Parameters...)> func = 
                    std::function<Return(Parameters...)>(                       
                        static_cast<const                                       
                            std::function<Return(Parameters...)>>
                                (this->function));
            
            //Create some new function that takes the exact same parameters
            //But have it's scope include the old function and intended execution time
            auto nFunc = [func, execution](Parameters...params)
            {
                //But don't actually use the captured execution time!
                //If you use the actual captured execution time you may end up with deleted functions and expressions
                //It may throw errors due to that

                //So, instead;

                //Copy it into a constant.
                const steady_clock::time_point lExe = execution;

                //Copy the captured old function aswell
                const std::function<Return(Parameters...)> oldFunc =
                    std::function<Return(Parameters...)>(                       
                        static_cast<const                                       
                            std::function<Return(Parameters...)>>
                                (func));

                //Wait until the *copied* execution time
                std::this_thread::sleep_until(lExe);

                //And return the value of the *copied* function with the *normal* parameters.
                return oldFunc(params...);
            };

            //Set the event's function to the new deferred function
            this->function = std::function<Return(Parameters...)>(nFunc);

            //Launch a thread with the deferred function
            this->launch(params...);

            //Set the event's function back to the non-deferred version
            this->function = std::function<Return(Parameters...)>(func);
        }

        //Launches thread, but awaits time==exeTime before running event.
        //Don't use this for long running conditions! It opens a watchdog thread!
        void defer(Parameters... params)
        {
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
                const steady_clock::time_point lExe = this->exeTime;

                //Copy the captured old function aswell
                const std::function<Return(Parameters...)> oldFunc =
                    std::function<Return(Parameters...)>(                       
                        static_cast<const                                       
                            std::function<Return(Parameters...)>>
                                (func));

                //Wait until the *copied* execution time
                std::this_thread::sleep_until(lExe);

                //And return the value of the *copied* function with the *normal* parameters.
                return oldFunc(params...);
            };

            //Set the event's function to the new deferred function
            this->function = std::function<Return(Parameters...)>(nFunc);

            //Launch a thread with the deferred function
            this->launch(params...);

            //Set the event's function back to the non-deferred version
            this->function = std::function<Return(Parameters...)>(func);
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

        //  Accessors
        //----------------------------------

        steady_clock::time_point getTime()
        {
            return exeTime;
        }

        bool getSuppressed()
        {
            return suppressed;
        }

        //  Mutators
        //----------------------------------

        void setTime(steady_clock::time_point nTime)
        {
            exeTime=nTime;
        }
        
        //  Operators
        //----------------------------------

        Return operator()(Parameters... rhs)
        {
        return this->function(rhs...);
        }

        bool operator==(steady_clock::time_point rhs)
        {
            return this->getTime()==rhs;
        }
        bool operator==(TimedEvent<Return, Parameters...> rhs)
        {
            return (*this)==rhs.getTime();
        }

        bool operator!=(steady_clock::time_point rhs)
        {
            return this->getTime()!=rhs;
        }
        bool operator!=(TimedEvent<Return, Parameters...> rhs)
        {
            return (*this)!=rhs.getTime();
        }

        bool operator>(steady_clock::time_point rhs)
        {
            return this->getTime()>rhs;
        }
        bool operator>(TimedEvent<Return, Parameters...> rhs)
        {
            return (*this)>rhs.getTime();
        }

        bool operator<(steady_clock::time_point rhs)
        {
            return this->getTime()<rhs;
        }
        bool operator<(TimedEvent<Return, Parameters...> rhs)
        {
            return (*this)<rhs.getTime();
        }

        bool operator<=(steady_clock::time_point rhs)
        {
            return !((*this)>rhs);
        }
        bool operator<=(TimedEvent<Return, Parameters...> rhs)
        {
            return !((*this)>rhs);
        }

        bool operator>=(steady_clock::time_point rhs)
        {
            return !((*this)<rhs);
        }
        bool operator>=(TimedEvent<Return, Parameters...> rhs)
        {
            return !((*this)<rhs);
        }
    };
};
#endif