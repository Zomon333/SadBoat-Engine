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

#ifndef EVENT_POOL_H
#define EVENT_POOL_H

#include "event.hpp"
#include "timed_event.hpp"
#include "recurring_event.hpp"

#ifndef RECUR_H
#error Missing header: recurring_event.hpp is required to use event_pool.hpp
#endif

namespace SBE
{
    template<class Return, class... Parameters>
    class EventPool
    {
    private:
        std::stack<std::pair<std::tuple<Parameters...>,Event<Return,std::tuple<Parameters...>>*>> pool;

        RecurringEvent<int> parser;
        std::chrono::milliseconds period;

    public:
        // Constructors
        EventPool();
        EventPool(auto period);

        // Accessors
        auto getParser();
        auto getPeriod();
        auto getPool();

        // Mutators
        void setPeriod(auto period);
        void insert(Event<Return,std::tuple<Parameters...>>* event, Parameters... parameters);
    };

    // EventPool Implementation Functions
    // <Return, Parameters...>
    //----------------------------------------------------------------------------------------------

    template<class Return, class... Parameters>
    EventPool<Return, Parameters...>::EventPool()
    {
        period = std::chrono::milliseconds(20);

        parser = RecurringEvent<int>(
            [this](int a)
            {
                auto pool = this->getPool();
                while(pool.size()>0)
                {
                    pool.top().second->call(pool.top().first);
                    pool.pop();
                }

                return 0;
            },
            period
        );

        parser.defer(0);
    }

    template<class Return, class... Parameters>
    EventPool<Return, Parameters...>::EventPool(auto period)
    {
        this->period = std::chrono::duration_cast<std::chrono::milliseconds>(period);

        parser = RecurringEvent<int>(
            [this](int a)
            {
                auto pool = this->getPool();
                while(pool.size()>0)
                {
                    pool.top().second->call(pool.top().first);
                    pool.pop();
                }

                return 0;
            },
            period
        );

        parser.defer(0);
    }

    // Accessors
    //----------------------------------

    template<class Return, class... Parameters>
    auto EventPool<Return, Parameters...>::getParser()
    {
        return parser;
    }

    template<class Return, class... Parameters>
    auto EventPool<Return, Parameters...>::getPeriod()
    {
        return period;
    }

    template<class Return, class... Parameters>
    auto EventPool<Return, Parameters...>::getPool()
    {
        return &pool;
    }

    // Mutators
    //----------------------------------

    template<class Return, class... Parameters>
    void EventPool<Return, Parameters...>::setPeriod(auto period)
    {
        this->period = std::chrono::duration_cast<std::chrono::milliseconds>(period);
    }

    template<class Return, class... Parameters>
    void EventPool<Return, Parameters...>::insert(Event<Return, std::tuple<Parameters...>>* event, Parameters... parameters)
    {
        pool.push(
            std::pair<std::tuple<Parameters...>,Event<Return,std::tuple<Parameters...>>*>
            (
                std::tuple<Parameters...>(parameters...), 
                event
            )
        );
    }
};
#endif