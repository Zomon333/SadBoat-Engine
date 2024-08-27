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

#ifndef RESOURCEHANDLE_H
#define RESOURCEHANDLE_H

#include "utilities/logging/startup_logger.hpp"
#include <mutex>

namespace SBE
{
    class ResourceHandle
    {
    private:
        // Data reference & size
        std::mutex* dataAccess;
        void* dataReference;
        size_t dataSize;

        // ResourceHandle ID
        int handleID;
        int resourceID;
    public:
        // Constructors
        //----------------------------------

        // Create a handle given some id, data, and size.
        ResourceHandle(int id, std::mutex* dataAccess, int parentID, void* data, size_t dataSize);
        ~ResourceHandle();

        // Accessors
        //----------------------------------

        template<class DataType> DataType getData();
        int getHandleID();
        int getResourceID();
    };
};
#endif