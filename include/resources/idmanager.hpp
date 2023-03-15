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

using namespace std;

namespace SBE
{
    // IDManager:   Utility
    // Generates integer IDs for later indexing
    class IDManager
    {
    private:

        int idCount;

        unordered_map<int, bool> usedIDs;       // Store for ID usage state
        stack<int> recycledIDs;                 // Store for previously used IDs

    public:
        // Constructors
        //----------------------------------
        
        // Set idCount to zero.
        IDManager()
        {
            idCount=0;
        }

        // Mutators
        //----------------------------------

        // Associate an ID with some data.
        int allocate()
        {
            // If there is an ID to be recycled, recycle it.
            if(recycledIDs.size()>0)
            {
                // Get a free ID and set it as in use
                int id=recycledIDs.top();
                usedIDs[id]=true;
                recycledIDs.pop();

                // Associate and track the ID.
                idCount++;

                // Return the ID.
                return id;
            }

            // Otherwise, create a new ID
            idCount++;
            usedIDs[idCount]=true;
            return idCount;
        }

        // Free an ID from association with data.
        void free(int id)
        {
            // Set the id usage to false.
            usedIDs[id]=false;

            // Push ID to stack for recycling.
            recycledIDs.push(id);
        }

        // Accessors
        //----------------------------------

        // Get the number of IDs available.
        int getCount()
        {
            return idCount;
        }

        // Get whether an ID is being used or not.
        bool isUsed(int id)
        {
            return usedIDs[id];
        }
        
    };
};
#endif