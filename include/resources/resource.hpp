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

#ifndef RESOURCE_H
#define RESOURCE_H

#include <fstream>
#include <mutex>
#include <string>
#include <unordered_map>

#include "id_manager.hpp"
#include "resource_handle.hpp"

namespace SBE
{
    class Resource
    {
    private:
        std::string filename;
        
        // Data Variables
        //----------------------------------

        std::mutex dataAccess;
        void* data;
        size_t initialSize;
        size_t dataSize;
        int dataID;
        bool persistent;

        // Handle Management
        //----------------------------------
        IDManager handleIDs;
        std::unordered_map<int, ResourceHandle*> handles;
        int handleCount;

        // Load some file into the resource's data spot
        void load(std::string filename);

        // Initialize a data store of a given size.
        void load(std::size_t dataSize);

        void unload();

    public:

        // Constructors
        //----------------------------------

        // Create a resource for this file. Optionally, load it.
        Resource(int id, std::string filename, bool persistence=false, bool preload=false);

        // Create a resource for some raw memory. Optionally, copy from an existing location.
        Resource(int id, size_t dataSize, bool persistence=false, bool copy=false, void* initData=nullptr);

        // Destructors
        //----------------------------------

        // Unload a resource and destroy the object.
        ~Resource();

        // Handle Management
        //----------------------------------

        // Create a new handle for the resource.
        ResourceHandle* allocateHandle();
        
        // Free an already made handle by ID.
        void freeHandle(int id);
        
        // Free an already made handle by reference.
        void freeHandle(ResourceHandle* toFree);
    
        // Accessors
        //----------------------------------

        // Get the resource's ID.
        int getID();
        
        // Get the name of the resource's file.
        std::string getFilename();
        
        // Get how many handles are currently in use.
        int getHandleCount();
        
        // Get whether this object should unload.
        bool getPersistence();

        // Get a handle by it's ID.
        ResourceHandle* getHandle(int id);
        
        // Get the resource's current size.
        size_t getSize();

        template<class T>
        T getData();

        // Mutators
        //----------------------------------

        // Allow or disallow the resource to remain in memory when not in use.
        void setPersistence(bool newPersistent);
    };
};
#endif