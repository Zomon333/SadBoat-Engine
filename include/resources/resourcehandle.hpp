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
#ifndef RESOURCEHANDLE_H
#define RESOURCEHANDLE_H

#include "sb-engine.hpp"

using namespace std;

namespace SBE
{
    // ResourceHandle:     Utility
    // A wrapper for abstract data access. Ensures data access follows uniform routes and management schemes.
    // Directly ties into Resource for automated reference counting for memory management.
    class ResourceHandle
    {
    private:
        // Data reference & size
        binary_semaphore* dataAccess;
        void* dataReference;
        size_t dataSize;

        // ResourceHandle ID
        int handleID;
        int resourceID;
    public:
        // Constructors
        //----------------------------------

        // Create a handle given some id, data, and size.
        ResourceHandle(int id, binary_semaphore* dataAccess, int parentID, void* data, size_t dataSize)
        {
            this->resourceID=parentID;
            this->dataAccess=dataAccess;
            this->handleID=id;
            this->dataReference=data;
            this->dataSize=dataSize;
        }

        ~ResourceHandle()
        {
            // Explicitly do not delete dataReference!
            // Just set it to nullptr.
            dataReference = nullptr;
        }

        // Accessors
        //----------------------------------

        // Get the data. Optionally, provide some data type.
        template<class type=void*>
        type getData()
        {
            dataAccess->acquire();
            auto toReturn = ((type)(dataReference));
            dataAccess->release();
            
            return toReturn;
        }

        // Get the ID of the ResourceHandle.
        int getHandleID()
        {
            return handleID;
        }

        // Get the ID of the Resource.
        int getResourceID()
        {
            return this->resourceID;
        }
    };
};
#endif