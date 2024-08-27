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

#ifndef IDMANAGER_H
#define IDMANAGER_H

#include <stack>
#include <unordered_map>
#include <vector>

namespace SBE
{
    // IDManager:   Utility
    // Generates integer IDs for later indexing
    class IDManager
    {
    private:
        int maxId;

        std::unordered_map<int, bool> usedIDs;
        std::stack<int> recycledIDs;

    public:
        // Constructors
        //----------------------------------

        IDManager();

        // Mutators
        //----------------------------------

        int allocate();
        void free(int id);

        // Accessors
        //----------------------------------

        int getCount();
        bool isUsed(int id);
        std::vector<int> getUsedIDs();
    };
};
#endif