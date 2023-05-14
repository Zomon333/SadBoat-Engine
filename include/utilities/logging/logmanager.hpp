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
#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include "sb-engine.hpp"
#include "../include/utilities/logging/loghandle.hpp"

using namespace std;

namespace SBE
{
    class LogManager
    {
    private:
        IDManager logHandleIDs;
        unordered_map<int, LogHandle*> handles;

        string fileName;

        mutex accessible;
        ofstream logAccess;    

        Event<string,pair<LogFlags,string>>* loggingFunction;   

    public:
        // Constructors
        //----------------------------------
        LogManager(string fileName)
        {
            this->fileName=fileName;

            logAccess.open(fileName, ios::binary);

            loggingFunction = new Event<string,pair<LogFlags,string>>(
                lF(pair<LogFlags, string> logRequest)
                {
                    string toReturn = this->generateLine(logRequest);
                    (*this)<<logRequest;
                    return toReturn;
                }
            );

        }

        // Mutators
        //----------------------------------
        void freeHandle(LogHandle* toFree)
        {
            // this->freeHandle(toFree->getID());
        }
        void freeHandle(int id)
        {
            accessible.lock();
            logHandleIDs.free(id);
            delete handles[id];
            accessible.unlock();
        }

        string generateLine(pair<LogFlags, string> logRequest)
        {
            string toWrite="";
            string timeStamp="";
            string errorCode="[";

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
                    throw new exception();
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
        auto allocateHandle(unsigned int logLevel = 0b00000)
        {
            accessible.lock();
            int id = logHandleIDs.allocate();
            handles[id] = new LogHandle(loggingFunction, id, logLevel);

            accessible.unlock();
            return handles[id];
        }
        vector<LogHandle*> getHandles()
        {
            vector<LogHandle*> list;
            auto usedIDs = logHandleIDs.getUsedIDs();
            list.resize(usedIDs.size());
            for(int i=0; i<usedIDs.size(); i++)
            {
                list[i]=handles[usedIDs[i]];
            }
            return list;
        }
        
        // Operators
        //----------------------------------
        void operator<<(pair<LogFlags, string> logRequest)
        {
            accessible.lock();

            string toWrite = generateLine(logRequest);
            logAccess<<toWrite<<endl;
            cout<<endl;

            accessible.unlock();

        }

        // Destructors
        //----------------------------------
        ~LogManager()
        {
            auto toDelete = getHandles();
            for(int i=0; i<toDelete.size(); i++)
            {
                freeHandle(toDelete[i]);
            }

            logAccess.flush();
            logAccess.close();
        }
        
    };
};
#endif