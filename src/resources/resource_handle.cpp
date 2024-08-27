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

#include "resources/resource_handle.hpp"

namespace SBE
{
    // ResourceHandle Implementation Functions
    //----------------------------------------------------------------------------------------------

    ResourceHandle::ResourceHandle(int id, std::mutex* dataAccess, int parentID, void* data, size_t dataSize)
    {
        this->resourceID=parentID;
        this->dataAccess=dataAccess;
        this->handleID=id;
        this->dataReference=data;
        this->dataSize=dataSize;
    }

    ResourceHandle::~ResourceHandle()
    {
        dataReference = nullptr;
    }

    template<class DataType = void*> DataType ResourceHandle::getData()
    {
        dataAccess->lock();
        auto toReturn = ((DataType)(dataReference));
        dataAccess->unlock();
        
        return toReturn;
    }

    int ResourceHandle::getHandleID()
    {
        return handleID;
    }

    int ResourceHandle::getResourceID()
    {
        return this->resourceID;
    }

}
