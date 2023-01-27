/*
Copyright 2023 Dagan Poulin, Justice Guillory
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

#include "sb-engine.hpp"

using namespace std;

namespace SBE
{
    template<class Return, class... Params>
    class EventPool
    {
    private:
        stack<pair<tuple<Params...>,Event<Return,tuple<Params...>>*>> pool;

        RecurringEvent<int> parser;
        milliseconds period;

    public:
            // Constructors
            EventPool()
            {
                period = milliseconds(20);

                parser = RecurringEvent<int>(
                [&pool](int a)
                {
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
            EventPool(auto period)
            {
                this->period=std::chrono::duration_cast<milliseconds>(period);

                parser = RecurringEvent<int>(
                [&pool](int a)
                {
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
            auto getParser() { return parser; }
            auto getPeriod() { return period; }

            // Mutators
            void setPeriod(auto period)
            {
                this->period=std::chrono::duration_cast<milliseconds>(period);
            }
            void insert(Event<Return,tuple<Params...>>* event, Params... parameters)
            {
                pool.push(
                    pair<
                        tuple<Params...>,
                        Event<Return,tuple<Params...>>*
                    >
                    (
                        tuple<Params...>(parameters...), 
                        event
                    ));
            }
    };
};
#endif