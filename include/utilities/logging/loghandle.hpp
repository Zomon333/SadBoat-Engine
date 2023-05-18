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
#ifndef LOGHANDLE_H
#define LOGHANDLE_H

#include "sb-engine.hpp"
#include "../include/utilities/logging/logmanager.hpp"

using namespace std;

namespace SBE
{
    class LogHandle
    {
    private:
        Event<string,pair<LogFlags,string>>* logFunction;
        int id;

        unsigned int consoleLevel = 0b00000;

    public:
        // Constructors
        //----------------------------------
        LogHandle(Event<string,pair<LogFlags,string>>* logFunction, int id, unsigned int consoleLevel=0b00000)
        {
            this->logFunction=logFunction;
            this->id=id;
            this->consoleLevel = consoleLevel;
        }
        // Mutators
        //----------------------------------

        void enableFlag(LogFlags flag)
        {
            consoleLevel = (consoleLevel | flag);
        }
        void disableFlag(LogFlags flag)
        {
            consoleLevel = (consoleLevel ^ flag);
        }

        void enableAllFlags()
        {
            consoleLevel = 0b11111;
        }
        void disableAllFlags()
        {
            consoleLevel = 0b00000;
        }

        void setConsoleLevel(unsigned int newLevel)
        {
            this->consoleLevel = newLevel;
        }

        // Accessors
        //----------------------------------
        void debug(string log)
        {
            const LogFlags level = DEBUG;

            string result = logFunction->call(pair<LogFlags, string>(level, log));
            if(consoleLevel & level)
            {
                cout<<endl;
                cout<<result;
            }
        }

        void info(string log)
        {
            const LogFlags level = INFO;
            
            string result = logFunction->call(pair<LogFlags, string>(level, log));
            if(consoleLevel & level)
            {
                cout<<endl;
                cout<<result;
            }
        }
        
        void warn(string log)
        {
            const LogFlags level = WARNING;
            
            string result = logFunction->call(pair<LogFlags, string>(level, log));
            if(consoleLevel & level)
            {
                cout<<endl;
                cout<<result;
            }
        }
        
        void error(string log)
        {
            const LogFlags level = ERROR;
            
            string result = logFunction->call(pair<LogFlags, string>(level, log));
            if(consoleLevel & level)
            {
                cout<<endl;
                cout<<result;
            }
        }
        
        void critical(string log)
        {
            const LogFlags level = CRITICAL;
            
            string result = logFunction->call(pair<LogFlags, string>(level, log));
            if(consoleLevel & level)
            {
                cout<<endl;
                cout<<result;
            }
        }

        int getID()
        {
            return id;
        }

        unsigned int getLevel()
        {
            return consoleLevel;
        }

        // Operators
        //----------------------------------

        void operator()(string rhs)
        {
            this->info(rhs);
        }

        // Destructors
        //----------------------------------
        ~LogHandle()
        {
            delete logFunction;
        }
    };

};
#endif