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

#ifndef RECUR_H
#define RECUR_H

#include "event.hpp"
#include "timed_event.hpp"

#ifndef T_EVENT_H
#error Missing header: timed_event.hpp is required to use recurring_event.hpp
#endif

namespace SBE
{
    template <class... Parameters>
    class RecurringEvent : public TimedEvent<int, Parameters...>
    {
    private:
        //  Recursion Control Variables
        //----------------------------------

        std::mutex accessible;
        std::promise<bool> returnable;
        std::chrono::milliseconds tOff;
        const std::chrono::milliseconds defaultFrequency = std::chrono::milliseconds(10);
        const std::chrono::milliseconds maxFrequency =  std::chrono::milliseconds(1);

        //  Recursion Helper Functions
        //----------------------------------

        int localDefer(Parameters... params);

    public:
        //  Constructors
        //----------------------------------

        RecurringEvent(auto f, auto period);
        RecurringEvent();
        RecurringEvent(Event<int, Parameters...> f, auto period);
        RecurringEvent(TimedEvent<int, Parameters...> f, auto period);

        //  Recursion Flow Control Functions
        //----------------------------------
        void recur(Parameters... params);
        void recur();
        int end();

        //  Mutators
        //----------------------------------
        
        void setFreq(auto period);

        //  Accessors
        //----------------------------------

        std::promise<bool>* getPromise();
        
    };

    // RecurringEvent Implementation Functions
    //----------------------------------------------------------------------------------------------

    // Private:

    //  Recursion Helper Functions
    //----------------------------------

    template <class... Parameters>
    int RecurringEvent<Parameters...>::localDefer(Parameters... params)
    {
        accessible.lock();
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
            //Copy the captured old function
            const std::function<int(Parameters...)> oldFunc =
                std::function<int(Parameters...)>(                       
                    static_cast<const                                       
                        std::function<int(Parameters...)>>
                            (func));

            //Get what time it is now
            auto deferredTime = std::chrono::steady_clock::now() + this->tOff;

            //Sleep until our offset is over
            std::this_thread::sleep_until(deferredTime);

            //And return the value of the *copied* function with the *normal* parameters.
            return oldFunc(params...);
        };
        accessible.unlock();
        //Call the new function
        return nFunc(params...);
    }

    // Public:
    //  Constructors
    //----------------------------------

    template <class... Parameters>
    RecurringEvent<Parameters...>::RecurringEvent(auto f, auto period)
    {
        accessible.lock();
        this->function = std::function<int(Parameters...)>(f);

        this->tOff = std::chrono::duration_cast<std::chrono::milliseconds>(period);
        accessible.unlock();
    }

    template <class... Parameters>
    RecurringEvent<Parameters...>::RecurringEvent()
    {
        accessible.lock();
        this->function = std::function<int(Parameters...)>(
            [](int a=0)
            {
                return 0;
            }
        );

        this->tOff = defaultFrequency;
        accessible.unlock();
    }

    template <class... Parameters>
    RecurringEvent<Parameters...>::RecurringEvent(Event<int, Parameters...> f, auto period)
    {
        accessible.lock();
        this->function = std::function<int(Parameters...)>(
            [f](Parameters... params)
            {
                f(params...);
                return 0;
            }
        );

        this->tOff = std::chrono::duration_cast<std::chrono::milliseconds>(period);
        accessible.unlock();
    }
    
    template <class... Parameters>
    RecurringEvent<Parameters...>::RecurringEvent(TimedEvent<int, Parameters...> f, auto period)
    {
        accessible.lock();
        this->function = std::function<int(Parameters...)>(
            [f](Parameters... params)
            {
                f(params...);
                return 0;
            }
        );

        this->tOff = std::chrono::duration_cast<std::chrono::milliseconds>(period);
        accessible.unlock();
    }
    

    //  Recursion Flow Control Functions
    //----------------------------------
    template <class... Parameters>
    void RecurringEvent<Parameters...>::recur(Parameters... params)
    {
        //Create an event that runs an infinite loop
        Event<int, Parameters...> loop(
            [this](Parameters... params)
            {
                //Get the eventual return of the recurringEvent
                std::future<bool> stopSignal = this->returnable.get_future();
                int result = 0;

                //Loop continuously
                while(stopSignal.wait_for(std::chrono::microseconds(1)) != std::future_status::ready)
                {
                    //Run a same-threaded defer of the stored function, and save it's result
                    if(this->getSuppressed()==false)
                    {
                        result = this->localDefer(params...);
                        //I may want to add an implementation that stores results in a priority_queue of time
                        //It could sort by oldest and cull the oldest results for garbage collected multithreading events
                    }

                    //Query if the stopSignal has been set.
                    //The offset of 500 microseconds here forces a hard cap that threads cannot recur more than 2,000 times a second.
                    std::future_status status = stopSignal.wait_until(std::chrono::steady_clock::now()+maxFrequency);

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
        //  2) Runs an almost-infinite loop in that thread, with a single stop condition that's only accessible from outside the thread. <<<< NOTE: You could pass the promise in through a pointer, I guess? 
        //  3) Runs the function provided to RecurringEvent with a local defer. This way:
        //      3-1) The function still runs on a schedule
        //      3-2) But the function runs synchronously on recur's thread.
        //  4) Checks at the end of every recur cycle if the future was set
        //      4-1) If it was set, return result on true.
        //      4-2) Else, return -1 to indicate error.
        //
        //So, TLDR; Recur is single threaded, but it manages itself on it's own thread that is not main().
    }
    
    template <class... Parameters>
    void RecurringEvent<Parameters...>::recur()
    {
        accessible.lock();
        this->recur(Parameters()...);
        accessible.unlock();
    }
    
    template <class... Parameters>
    int RecurringEvent<Parameters...>::end()
    {
        accessible.lock();
        returnable.set_value(true);
        accessible.unlock();
        //return this->getResult(); //Result is stored in loop. Why would I want to get the result of the RecurringEvent? It doesn't have a result!
        return 0;
    }
    

    //  Mutators
    //----------------------------------
    
    template <class... Parameters>
    void RecurringEvent<Parameters...>::setFreq(auto period)
    {
        accessible.lock();
        this->tOff = std::chrono::duration_cast<std::chrono::milliseconds>(period);
        accessible.unlock();
    }
    

    //  Accessors
    //----------------------------------

    template <class... Parameters>
    std::promise<bool>* RecurringEvent<Parameters...>::getPromise()
    {
        return &returnable;
    }
};
#endif