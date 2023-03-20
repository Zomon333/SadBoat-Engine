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
#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "sb-engine.hpp"

using namespace std;

namespace SBE
{
    // ResourceManager:     Utility
    // Loads resources as binary directly into RAM.
    // Implements automatic reference counting for data through ResourceHandles; an itemized tool for accessing data discretely.
    class ResourceManager
    {
    private:
        // Reference stores
        //----------------------------------
        unordered_map<int,void*> resources;
        unordered_map<int, bool> persistent;

        unordered_map<int,IDManager> handleIDs;
        unordered_map<int,unordered_map<int,ResourceHandle*>> handles;

        // Manager for IDs in use by resource manager
        IDManager idTracker;
        
    public:
        // Constructors
        //----------------------------------

        ResourceManager();

        // Allocators
        //----------------------------------

        // Load resource into resource map. Assign ID and handle.
        ResourceHandle* load(string resource, bool persistent=false)
        {
            try
            {
                // Open resource
                fstream file;
                file.open(resource, ios::in | ios::out | ios::binary | ios::ate);

                // Verify resource opened
                if(!file.is_open())
                {
                    throw new invalid_argument(
                        ((string)("Provided file did not open. File: ")).append(resource).c_str()
                    );
                } 
                
                // Find size of opened resource
                size_t size = file.tellg();
                file.close();

                // Verify resource has a size
                if(size==0)
                {
                    throw new out_of_range(
                        ((string)("Provided file has size of zero bytes. File: ")).append(resource).c_str()
                    );
                }

                // Reopen with preferred settings
                file.open(resource, ios::in | ios::out | ios::binary | ios::app);

                // Generate new ID
                int id = idTracker.allocate();
                resources[id] = malloc(size);

                // Move resource to ID.
                file.read(((char*)(resources[id])),size);

                // Set persistence
                this->persistent[id]=persistent;

                // Set Handle.
                int newID = handleIDs[id].allocate();
                ResourceHandle* tmp = new ResourceHandle(resource, resources[id], size, id, newID);
                handles[id][newID] = tmp;
                handles[id][-1] = tmp;

                return handles[id][newID];
            }
            catch(invalid_argument &e)
            {
                cout<<e.what()<<endl;
                return nullptr;
            }
            catch(out_of_range &e)
            {
                cout<<e.what()<<endl;
                return nullptr;
            }

            return nullptr;
        }

        ResourceHandle* allocateHandle(int id)
        {
            if(handles[id].size()==0) return nullptr;

            // handles[id].emplace(ResourceHandle(*(handles[id].top())));
            int newID=handleIDs[id].allocate();
            handles[id][newID] = new ResourceHandle(*(handles[id][-1]));

            return handles[id][newID];
        }

        // Deallocators
        //----------------------------------

        // Unload data and delete all handles.
        void unload(int id)
        {
            // Store where the data is / used to be
            void* data = resources[id];

            // Free the id
            idTracker.free(id);

            // Destroy all handles
            for(int i = 0; i<handles[id].size(); i++)
            {
                delete handles[id][i];
            }

            // Clear all data.
            free(data);
        }

        // Follow handle rules, free handle
        void freeHandle(int resID, int handID)
        {
            delete handles[resID][handID];

            if(handles[resID].size()==0 && persistent[resID]==false)
            {
                unload(resID);
            }
        }

        // Follow handle rules, free handle
        void freeHandle(ResourceHandle* resHandle)
        {
            this->freeHandle(resHandle->getResID(), resHandle->getHandID());
        }
    };
};
#endif