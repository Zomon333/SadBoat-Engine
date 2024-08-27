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

#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <iostream>
#include <fstream>

#include "events/event.hpp"
#include "resources/id_manager.hpp"
#include "utilities/defines.hpp"
#include "utilities/logging/log_handle.hpp"

namespace SBE
{
    class LogManager
    {
    private:
        IDManager logHandleIDs;
        std::unordered_map<int, LogHandle*> handles;

        std::string fileName;

        std::mutex accessible;
        std::ofstream logAccess;    

        Event<std::string,std::pair<LogFlags,std::string>>* loggingFunction;   

    public:
        // Constructors
        //----------------------------------
        LogManager(std::string fileName);

        // Mutators
        //----------------------------------
        void freeHandle(LogHandle* toFree);
        void freeHandle(int id);

        std::string generateLine(std::pair<LogFlags, std::string> logRequest);

        // Accessors
        //----------------------------------
        LogHandle* allocateHandle(unsigned int logLevel);
        
        std::vector<LogHandle*> getHandles();
        // Operators
        //----------------------------------
        void operator<<(std::pair<LogFlags, std::string> logRequest);

        // Destructors
        //----------------------------------
        ~LogManager();
        
    };
};


#endif