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

#include "resources/id_manager.hpp"

namespace SBE
{
    // IDManager Implementation Functions
    //----------------------------------------------------------------------------------------------

    IDManager::IDManager()
    {
        maxId = 1;
    }

    // Mutators
    //----------------------------------

    int IDManager::allocate()
    {
        int id=0;
            
        if(recycledIDs.size()>0)
        {
            id=recycledIDs.top();
            recycledIDs.pop();
        }
        else
        {
            maxId += 1;
            id = maxId;
        }

        usedIDs[id]=true;
        return id;
    }

    void IDManager::free(int id)
    {
        // Set the id usage to false.
        usedIDs[id]=false;

        recycledIDs.push(id);
    }

    // Accessors
    //----------------------------------

    int IDManager::getCount()
    {
        return maxId - recycledIDs.size();
    }

    bool IDManager::isUsed(int id)
    {
        return usedIDs[id];
    }

    std::vector<int> IDManager::getUsedIDs()
    {
        std::vector<int> results;
        for(const std::pair<const int, bool>& n : this->usedIDs)
        {
            if(n.second)
            {
                results.push_back(n.first);
            } 
        }
        return results;
    }
}