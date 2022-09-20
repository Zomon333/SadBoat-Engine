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

//RecurringEvent: Event, Derived
//A derivation of TimedEvent which keeps it's function running in a loop.
//Creates a new thread, and runs stored function with n-second sleeps between runs. Stops execution when returnable!=null.
//Returns single int for return status.
template <class... Parameters>
class RecurringEvent : public TimedEvent<int, Parameters...>, public Event<int, Parameters...>
{
private:
    //  Recursion Control Variables
    //----------------------------------

    std::promise<bool> returnable;

    //  Recursion Helper Functions
    //----------------------------------

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
    //  Constructors
    //----------------------------------

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

    //  Recursion Flow Control Functions
    //----------------------------------

    void recur(Parameters... params)
    {
        //Create an event that runs an infinite loop
        Event<int, Parameters...> loop(
            lF(Parameters... params)
            {
                //Get the eventual return of the recurringEvent
                std::future<bool> stopSignal = this->returnable.get_future();
                
                //Loop continuously
                while(true)
                {
                    //Run a same-threaded defer of the stored function, and save it's result
                    int result = this->localDefer(params...);

                    //Query if the stopSignal has been set.
                    //The offset of 500 microseconds here forces a hard cap that threads cannot recur more than 2,000 times a second.
                    std::future_status status = stopSignal.wait_until(EngineClock::now()+uFreqMax);

                    //If we got a result that the stopSignal has been set:
                    if(status==std::future_status::ready)
                    {
                        //Get it!
                        if(stopSignal.get()==true)
                        {
                            //If we're supposed to stop, return what we got.
                            return result;
                        }
                        else
                        {
                            //Otherwise...

                            //This section accounts for an error where stopSignal may be ready but assigned false.
                            //This will return -1 because we should never set stopSignal to false, only true.
                            return -1;
                        }
                    }
                }

                //If we get here then there's some issue, because this *should* be unreachable code.
                return -1;
            }
        );

        //Launch our infinite loop
        loop.launch(params...);

        //An aside to explain this function a bit more clearly:
            //defer() is a function that launches a thread, but waits a given amount of time before doing it's calculations.
            //localDefer() is similar to defer; it still waits, but it does not launch a new thread before doing so.
            //So, what does recur() do?
            //  1) Creates a new thread.
            //  2) Runs an almost-infinite loop in that thread, with a single stop condition that's only accessible from outside the thread.
            //  3) Runs the function provided to RecurringEvent with a local defer. This way:
            //      3-1) The function still runs on a schedule
            //      3-2) But the function runs synchronously on recur's thread.
            //  4) Checks at the end of every recur cycle if the future was set
            //      4-1) If it was set, return result on true.
            //      4-2) Else, return -1 to indicate error.
            //
            //So, TLDR; Recur is single threaded, but it manages itself on it's own thread that is not main().

    }

    int end()
    {
        returnable.set_value(true);
        return this->getResult();
    }

    //  Mutators
    //----------------------------------
    
    void setTime(Instant period)
    {
        this->exeTime = period;
    }
    
};

#endif