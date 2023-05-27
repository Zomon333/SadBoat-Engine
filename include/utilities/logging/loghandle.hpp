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
        mutex handleUse;

        stringstream commitment;

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
        void log(string log, LogFlags level)
        {
            handleUse.lock();

            string result = logFunction->call(pair<LogFlags, string>(level, log));
            if(consoleLevel & level)
            {
                cout<<endl;
                cout<<result;
            }
            handleUse.unlock();
        }
        void debug(string log){this->log(log, DEBUG);}
        void info(string log){this->log(log, INFO);}
        void warn(string log){this->log(log, WARNING);}
        void error(string log){this->log(log, ERROR);}
        void critical(string log){this->log(log, CRITICAL);}

        void debug(stringstream* log){this->debug(log->str());}
        void info(stringstream* log){this->info(log->str());}
        void warn(stringstream* log){this->warn(log->str());}
        void error(stringstream* log){this->error(log->str());}
        void critical(stringstream* log){this->critical(log->str());}

        void commit(LogFlags level)
        {
            handleUse.lock();
            // const LogFlags level = CRITICAL;
            
            string commitString = commitment.str();
            commitment.clear();

            string result = logFunction->call(pair<LogFlags, string>(level, commitString));
            if(consoleLevel & level)
            {
                cout<<endl;
                cout<<result;
            }
            handleUse.unlock();
        }
        stringstream* getStream(){return &commitment;}

        int getID()
        {
            return id;
        }

        unsigned int getLevel(){return consoleLevel;}

        // Operators
        //----------------------------------

        void operator()(string rhs)
        {
            this->info(rhs);
        }
        LogHandle* operator<<(string rhs)
        {
            this->commitment<<rhs;
            return (this);
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