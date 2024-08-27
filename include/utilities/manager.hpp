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
#ifndef MANAGER_H
#define MANAGER_H

#include <unordered_map>
#include <vector>
#include <mutex>

#include "resources/id_manager.hpp"

namespace SBE
{
    template<class datum>
    class Manager
    {
    private:
        IDManager dataIDs;
        std::unordered_map<int, datum*> data;
        std::unordered_map<datum*, int> reverseData;

        std::mutex accessible;

    public:
        // Constructors
        //----------------------------------
        Manager();
        
        // Mutators
        //----------------------------------
        void freeData(datum* toFree);
        void freeData(int id);
        int allocateData(datum* toAllocate);
        
        // Accessors
        //---------------------------------- 
        std::vector<datum*> getData();
        
        datum* getData(int id);
        int getID(datum* toGet);

        std::vector<int> getUsedIDs();

        // Operators
        //----------------------------------
        datum* operator[](int rhs);

        // Destructors
        //----------------------------------
        ~Manager();
        
    };


    // Constructors
    //----------------------------------
    template<class datum>
    Manager<datum>::Manager()
    {

    }

    // Mutators
    //----------------------------------
    template<class datum>
    void Manager<datum>::freeData(datum* toFree)
    {
        accessible.lock();

        int id = reverseData[toFree];
        reverseData[toFree]=0;
        data[id]=nullptr;
        dataIDs.free(id);

        delete toFree;

        accessible.unlock();
    }

    template<class datum>
    void Manager<datum>::freeData(int id)
    {
        accessible.lock();

        datum* toFree = data[id];
        reverseData[toFree]=nullptr;
        data[id]=nullptr;
        dataIDs.free(id);
        delete toFree;

        accessible.unlock();
    }

    template<class datum>
    int Manager<datum>::allocateData(datum* toAllocate)
    {
        accessible.lock();
        int id = dataIDs.allocate();
        data[id] = toAllocate;

        accessible.unlock();
        return id;
    }

    // Accessors
    //---------------------------------- 
    template<class datum>
    std::vector<datum*> Manager<datum>::getData()
    {
        std::vector<datum*> list;
        auto usedIDs = dataIDs.getUsedIDs();
        list.resize(usedIDs.size());
        for(int i=0; i<usedIDs.size(); i++)
        {
            list[i]=data[usedIDs[i]];
        }
        return list;
    }

    template<class datum>
    datum* Manager<datum>::getData(int id)
    {
        return data[id];
    }

    template<class datum>
    int Manager<datum>::getID(datum* toGet)
    {
        return reverseData[toGet];
    }

    template<class datum>
    std::vector<int> Manager<datum>::getUsedIDs()
    {
        return dataIDs.getUsedIDs();
    }

    // Operators
    //----------------------------------
    template<class datum>
    datum* Manager<datum>::operator[](int rhs)
    {
        return data[rhs];
    }

    // Destructors
    //----------------------------------
    template<class datum>
    Manager<datum>::~Manager()
    {
        auto toDelete = getData();
        for(int i=0; i<toDelete.size(); i++)
        {
            freeData(toDelete[i]);
        }
    }
        
};
#endif