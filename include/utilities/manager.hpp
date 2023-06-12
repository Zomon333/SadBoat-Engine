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
#ifndef MANAGER_H
#define MANAGER_H

#include "sb-engine.hpp"

using namespace std;

namespace SBE
{
    template<class datum>
    class Manager
    {
    private:
        IDManager dataIDs;
        unordered_map<int, datum*> data;
        unordered_map<datum*, int> reverseData;

        mutex accessible;

    public:
        // Constructors
        //----------------------------------
        Manager(){}
        
        // Mutators
        //----------------------------------
        void freeData(datum* toFree)
        {
            accessible.lock();

            int id = reverseData[toFree];
            reverseData[toFree]=0;
            data[id]=nullptr;
            dataIDs.free(id);

            delete toFree;

            accessible.unlock();
        }
        void freeData(int id)
        {
            accessible.lock();

            datum* toFree = data[id];
            reverseData[toFree]=nullptr;
            data[id]=nullptr;
            dataIDs.free(id);
            delete toFree;

            accessible.unlock();
        }

        // Accessors
        //----------------------------------
        int allocateData(datum* toAllocate)
        {
            accessible.lock();
            int id = dataIDs.allocate();
            data[id] = toAllocate;

            accessible.unlock();
            return id;
        }
        vector<datum*> getData()
        {
            vector<datum*> list;
            auto usedIDs = dataIDs.getUsedIDs();
            list.resize(usedIDs.size());
            for(int i=0; i<usedIDs.size(); i++)
            {
                list[i]=data[usedIDs[i]];
            }
            return list;
        }
        
        datum* getData(int id)
        {
            return data[id];
        }
        int getID(datum* toGet)
        {
            return reverseData[toGet];
        }

        // Operators
        //----------------------------------
        datum* operator[](int rhs)
        {
            return data[rhs];
        }

        // Destructors
        //----------------------------------
        ~Manager()
        {
            auto toDelete = getData();
            for(int i=0; i<toDelete.size(); i++)
            {
                freeData(toDelete[i]);
            }
        }
        
    };
};
#endif