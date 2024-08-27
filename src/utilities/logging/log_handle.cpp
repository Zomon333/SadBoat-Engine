/*
Copyright 2024 Dagan Poulin, Justice Guillory
   Licensed under the Apache License, Version 2.0 (the "License")
    {
        
    }
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
       http://www.apache.org/licenses/LICENSE-2.0
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "events/event.hpp"
#include "utilities/logging/log_handle.hpp"
#include "utilities/logging/log_manager.hpp"

namespace SBE
{
    // Constructors
    //----------------------------------
    LogHandle::LogHandle(SBE::Event<std::string,std::pair<LogFlags,std::string>>* logFunction, int id, unsigned int consoleLevel=0b00000)
    {
        this->logFunction=logFunction;
        this->id=id;
        this->consoleLevel = consoleLevel;
    }

    // Mutators
    //----------------------------------

    void LogHandle::enableFlag(LogFlags flag)
    {
        consoleLevel = (consoleLevel | flag);
    }
    void LogHandle::disableFlag(LogFlags flag)
    {
        consoleLevel = (consoleLevel ^ flag);
    }

    void LogHandle::enableAllFlags()
    {
        consoleLevel = 0b11111;
    }
    void LogHandle::disableAllFlags()
    {
        consoleLevel = 0b00000;
    }

    void LogHandle::setConsoleLevel(unsigned int newLevel)
    {
        this->consoleLevel = newLevel;
    }

    // Accessors
    //----------------------------------
    void LogHandle::log(std::string log, LogFlags level)
    {
        handleUse.lock();

        std::string result = logFunction->call(std::pair<LogFlags, std::string>(level, log));
        if(consoleLevel & level)
        {
            std::cout<<std::endl;
            std::cout<<result;
        }
        handleUse.unlock();
    }
    void LogHandle::debug(std::string log)
    {
        this->log(log, DEBUG);
    }
    void LogHandle::info(std::string log)
    {
        this->log(log, INFO);
    }
    void LogHandle::warn(std::string log)
    {
        this->log(log, WARNING);
    }
    void LogHandle::error(std::string log)
    {
        this->log(log, ERROR);
    }
    void LogHandle::critical(std::string log)
    {
        this->log(log, CRITICAL);
    }

    void LogHandle::debug(std::stringstream* log)
    {
        this->debug(log->str()); 
        log->str("");
    }
    void LogHandle::info(std::stringstream* log)
    {
        this->info(log->str()); 
        log->str("");
    }
    void LogHandle::warn(std::stringstream* log)
    {
        this->warn(log->str()); 
        log->str("");
    }
    void LogHandle::error(std::stringstream* log)
    {
        this->error(log->str()); 
        log->str("");
    }
    void LogHandle::critical(std::stringstream* log)
    {
        this->critical(log->str()); 
        log->str("");
    }

    void LogHandle::commit(LogFlags level)
    {
        handleUse.lock();
        // const LogFlags level = CRITICAL;
        
        std::string commitString = commitment.str();
        commitment.str("");

        std::string result = logFunction->call(std::pair<LogFlags, std::string>(level, commitString));
        if(consoleLevel & level)
        {
            std::cout<<std::endl;
            std::cout<<result;
        }
        handleUse.unlock();
    }
    std::stringstream* LogHandle::getStream()
    {
        return &commitment;
    }

    int LogHandle::getID()
    {
        return id;
    }

    unsigned int LogHandle::getLevel()
    {
        return consoleLevel;
    }

    // Operators
    //----------------------------------

    void LogHandle::operator()(std::string rhs)
    {
        this->info(rhs);
    }
    LogHandle* LogHandle::operator<<(std::string rhs)
    {
        this->commitment<<rhs;
        return (this);
    }

    // Destructors
    //----------------------------------
    LogHandle::~LogHandle()
    {
        delete logFunction;
    }
}

    