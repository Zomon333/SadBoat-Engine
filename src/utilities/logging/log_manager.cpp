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
#include "utilities/logging/log_manager.hpp"

namespace SBE
{
    // Constructors
    //----------------------------------
    LogManager::LogManager(std::string fileName)
    {
        this->fileName=fileName;

        logAccess.open(fileName, std::ios::binary);

        loggingFunction = new Event<std::string, std::pair<LogFlags, std::string>>( 
            [this](std::pair<LogFlags, std::string> logRequest)
            {
                std::string toReturn = this->generateLine(logRequest);
                (*this)<<logRequest;
                return toReturn;
            }
        );
    }

    // Mutators
    //----------------------------------
    void LogManager::freeHandle(LogHandle* toFree)
    {
        this->freeHandle(toFree->getID());
    }
    void LogManager::freeHandle(int id)
    {
        accessible.lock();
        logHandleIDs.free(id);
        delete handles[id];
        accessible.unlock();
    }

    std::string LogManager::generateLine(std::pair<LogFlags, std::string> logRequest)
    {
        std::string toWrite="";
        std::string timeStamp="";
        std::string errorCode="[";

        switch(logRequest.first)
        {
            case DEBUG:
                errorCode+="DEBUG]";
                break;
            case INFO:
                errorCode+="INFO]";
                break;
            case WARNING:
                errorCode+="WARNING]";
                break;
            case ERROR:
                errorCode+="ERROR]";
                break;
            case CRITICAL:
                errorCode+="CRITICAL]";
                break;
            default:
                throw new std::exception();
        }

        timeStamp+="[";

        auto now = std::chrono::system_clock::now();
        time_t convertedNow = std::chrono::system_clock::to_time_t(now);

        timeStamp+=ctime(&convertedNow);
        timeStamp+="]";

        toWrite=timeStamp.substr(0,timeStamp.size()-2);
        toWrite+="] ";
        toWrite+=errorCode;
        toWrite+=" ";
        toWrite+=logRequest.second;

        return toWrite;
    }

    // Accessors
    //----------------------------------
    LogHandle* LogManager::allocateHandle(unsigned int logLevel = 0b00000)
    {
        accessible.lock();
        int id = logHandleIDs.allocate();
        handles[id] = new LogHandle(loggingFunction, id, logLevel);

        accessible.unlock();
        return handles[id];
    }

    std::vector<LogHandle*> LogManager::getHandles()
    {
        std::vector<LogHandle*> list;
        auto usedIDs = logHandleIDs.getUsedIDs();
        list.resize(usedIDs.size());
        for(unsigned int i=0; i<usedIDs.size(); i++)
        {
            list[i]=handles[usedIDs[i]];
        }
        return list;
    }
    // Operators
    //----------------------------------
    void LogManager::operator<<(std::pair<LogFlags, std::string> logRequest)
    {
        accessible.lock();

        std::string toWrite = generateLine(logRequest);
        logAccess<<toWrite<<std::endl;

        accessible.unlock();
    }

    // Destructors
    //----------------------------------
    LogManager::~LogManager()
    {
        auto toDelete = getHandles();
        for(unsigned int i=0; i<toDelete.size(); i++)
        {
            freeHandle(toDelete[i]);
        }

        logAccess.flush();
        logAccess.close();
    }

}