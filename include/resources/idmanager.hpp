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
#ifndef IDMANAGER_H
#define IDMANAGER_H

#include "sb-engine.hpp"
#include "../primitives/range.hpp"

using namespace std;

namespace SBE
{
    // IDManager:   Utility
    // Generates integer IDs for later indexing
    class IDManager
    {
    private:
        Range idRange;

        unordered_map<int, bool> usedIDs;       // Store for ID usage state
        stack<int> recycledIDs;                 // Store unused IDs

    public:
        // Constructors
        //----------------------------------
        
        // Set idCount to zero.
        IDManager()
        {
            idRange = Range(0,1);
        }

        // Mutators
        //----------------------------------

        // Associate an ID with some data.
        int allocate()
        {
            int id=0;
            
            if(recycledIDs.size()>0)
            {
                id=recycledIDs.top();
                recycledIDs.pop();
            }
            else
            {
                idRange+=pair<double,double>(0,1);
                id=idRange.getMax();
            }

            usedIDs[id]=true;
            return id;
        }

        // Free an ID from association with data.
        void free(int id)
        {
            // Set the id usage to false.
            usedIDs[id]=false;

            recycledIDs.push(id);
        }

        // Accessors
        //----------------------------------

        // Get the number of IDs in use.
        int getCount()
        {
            return idRange.getMax()-recycledIDs.size();
        }

        // Get whether an ID is being used or not.
        bool isUsed(int id)
        {
            return usedIDs[id];
        }
        
    };
};
#endif