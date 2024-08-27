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

#ifndef LOGHANDLE_H
#define LOGHANDLE_H

#include "events/event.hpp"
#include "utilities/defines.hpp"

#include <string>
#include <iomanip>


namespace SBE
{
    class LogHandle
    {
    private:
        SBE::Event<std::string,std::pair<SBE::LogFlags,std::string>>* logFunction;
        int id;
        std::mutex handleUse;

        std::stringstream commitment;

        unsigned int consoleLevel = 0b00000;

    public:
        // Constructors
        //----------------------------------
        LogHandle(SBE::Event<std::string,std::pair<SBE::LogFlags,std::string>>* logFunction, int id, unsigned int consoleLevel);

        // Mutators
        //----------------------------------

        void enableFlag(SBE::LogFlags flag);
        void disableFlag(SBE::LogFlags flag);

        void enableAllFlags();
        void disableAllFlags();

        void setConsoleLevel(unsigned int newLevel);

        // Accessors
        //----------------------------------
        void log(std::string log, SBE::LogFlags level);
        void debug(std::string log);
        void info(std::string log);
        void warn(std::string log);
        void error(std::string log);
        void critical(std::string log);

        void debug(std::stringstream* log);
        void info(std::stringstream* log);
        void warn(std::stringstream* log);
        void error(std::stringstream* log);
        void critical(std::stringstream* log);

        void commit(SBE::LogFlags level);
        std::stringstream* getStream();

        int getID();

        unsigned int getLevel();

        // Operators
        //----------------------------------

        void operator()(std::string rhs);
        SBE::LogHandle* operator<<(std::string rhs);

        // Destructors
        //----------------------------------
        ~LogHandle();
    };

};
#endif