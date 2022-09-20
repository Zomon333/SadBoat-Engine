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
#ifndef RECUR_H
#define RECUR_H

#include "timedevent.hpp"

using namespace std;

template <class... Parameters>
class RecurringEvent : public TimedEvent<int, Parameters...>, public Event<int, Parameters...>
{
private:

    std::promise<bool> returnable;

    int localDefer(Parameters... params)
    {
        //Copy the intended recursion
        const std::function<int(Parameters...)> func = 
                std::function<int(Parameters...)>(                       
                    static_cast<const                                       
                        std::function<int(Parameters...)>>
                            (this->function));
        
        //Create some new recursion that takes the same parameters
        //But have it's scope include the old function and intended execution time
        auto nFunc = [this, func](Parameters...params)
        {
            //But don't actually use the captured execution time!
            //If you use the actual captured execution time you may end up with deleted functions and expressions
            //It may throw errors due to that

            //So, instead;

            //Copy it into a constant.
            const Instant lExe = this->exeTime;

            //Copy the captured old function aswell
            const std::function<int(Parameters...)> oldFunc =
                std::function<int(Parameters...)>(                       
                    static_cast<const                                       
                        std::function<int(Parameters...)>>
                            (func));

            //Wait until the *copied* execution time
            std::this_thread::sleep_until(lExe);

            //And return the value of the *copied* function with the *normal* parameters.
            return oldFunc(params...);
        };

        //Call the new function
        return nFunc(params...);
    }

public:
    RecurringEvent() : TimedEvent<int, Parameters...>( 
        F(Parameters... params)
        {
            return 1;
        }, 
        uFreq
        ){};

    RecurringEvent(auto f, Instant period) : TimedEvent<int, Parameters...>(f, period){};

    RecurringEvent(Event<int, Parameters...> f, Instant period) : RecurringEvent<Parameters...>(
        [f](Parameters... params)
        {
            return f(params...);
        },
        period
        ){};

    RecurringEvent(TimedEvent<int, Parameters...> f, Instant period) : TimedEvent<int, Parameters...>(
        [f](Parameters... params)
        {
            return f(params...);
        },
        period
    ){};

    void recur(Parameters... params)
    {
        Event<int, Parameters...> loop(
            lF(Parameters... params)
            {
                std::future<bool> stopSignal = this->returnable.get_future();
                while(true)
                {
                    int result = this->localDefer(params...);
                    std::future_status status = stopSignal.wait_until(EngineClock::now()+std::chrono::microseconds(25));

                    if(status==std::future_status::ready)
                    {
                        if(stopSignal.get()==true)
                        {
                            return result;
                        }
                        else
                        {
                            //This section accounts for an error where stopSignal may be ready but assigned false.
                            //This will return -1 because we should never set stopSignal to false, only true.
                            return -1;
                        }
                    }
                }
                return -1;
            }
        );

        loop.launch(params...);

    }

    int end()
    {
        returnable.set_value(true);
        return this->getResult();
    }

    void setTime(Instant period)
    {
        this->exeTime = period;
    }
    
};

#endif