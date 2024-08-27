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
#include "resources/resource.hpp"
#include <string.h>

namespace SBE
{
    // Resource Implementation Functions
    //----------------------------------------------------------------------------------------------

    // Load some file into the resource's data spot
    void Resource::load(std::string filename)
    {
        if(handleCount>0) 
        {
            return;
        }
        try
        {
            dataAccess.lock();

            // Open resource
            std::fstream file;
            file.open(filename, std::ios_base::in | std::ios_base::binary | std::ios_base::ate);

            // Verify resource opened
            if(!file.is_open() || file.bad())
            {
                SBE::log->error(((std::string)("Provided file did not open. File: ")).append(filename).c_str());
                throw new std::invalid_argument(
                    ((std::string)("Provided file did not open. File: ")).append(filename).c_str()
                );
            } 
            
            
            // Find size of opened resource
            dataSize = file.tellg();
            
            if(initialSize==0)
            {
                initialSize=dataSize;
            }

            file.close();

            // Verify resource has a size
            if(dataSize==0)
            {
                SBE::log->error(((std::string)("Provided file has size of zero bytes. File: ")).append(filename).c_str());
                throw new std::out_of_range(
                    ((std::string)("Provided file has size of zero bytes. File: ")).append(filename).c_str()
                );
            }

            // Dedicate data
            data = malloc(dataSize);
            

            // Reopen with preferred settings
            file.open(filename, std::ios_base::in | std::ios_base::binary);

            // Save data.
            file.read(((char*)(data)),dataSize);
            
            file.close();
            
            dataAccess.unlock();
        }
        catch(std::invalid_argument* e)
        {
            SBE::log->error(e->what());
        }
        catch(std::out_of_range* e)
        {
            SBE::log->error(e->what());
        }

        SBE::log->debug(std::string("Loaded file ").append(filename).append(" into RAM."));
    }

    // Initialize a data store of a given size.
    void Resource::load(std::size_t dataSize)
    {
        try
        {
            if(dataSize==0)
            {
                throw new std::invalid_argument("Data cannot have no size. Refusing to store uninitialized data.");
            }

            dataAccess.lock();

            this->data = malloc(dataSize);
            this->dataSize=dataSize;
            this->initialSize=dataSize;
            
            dataAccess.unlock();
        }
        catch(std::invalid_argument* e)
        {
            SBE::log->error(e->what());
        }

        SBE::log->debug(std::string("Loaded arbitrary RAM into memory of size ").append(std::to_string(dataSize)));
    }

    void Resource::unload()
    {
        if( handleCount>0 )
        {
            for(auto i : handles)
            {
                if(i.second !=nullptr) { delete i.second; }
            }
        }

        if(initialSize==0 || dataSize==0)
        {
            return;
        }


        if(data!=nullptr)
        {
            free(data);
        }

        dataSize=0;
    }

    // Constructors
    //----------------------------------

    // Create a resource for this file. Optionally, load it.
    Resource::Resource(int id, std::string filename, bool persistence, bool preload)
    {
        this->dataID=id;
        this->filename=filename;
        this->persistent=persistence;

        this->dataSize=0;
        this->initialSize=0;
        this->handleCount=0;

        if(preload)
        {
            load(filename);
        }
    }

    // Create a resource for some raw memory. Optionally, copy from an existing location.
    Resource::Resource(int id, size_t dataSize, bool persistence, bool copy, void* initData)
    {
        this->dataID=id;
        this->persistent=persistence;

        this->dataSize=dataSize;
        this->initialSize=dataSize;
        this->handleCount=0;

        load(dataSize);
        if(copy)
        {
            try
            {
                if(initData==nullptr)
                {
                    throw new std::invalid_argument("initData was nullptr with copy setting. Refusing to copy invalid data range.");
                }

                memcpy(this->data, initData, dataSize);
            }
            catch(std::invalid_argument* e)
            {
                SBE::log->error(e->what());
            }
        }
    }

    // Destructors
    //----------------------------------

    // Unload a resource and destroy the object.
    Resource::~Resource()
    {
        unload();
    }

    // Handle Management
    //----------------------------------

    // Create a new handle for the resource.
    ResourceHandle* Resource::allocateHandle()
    {
        if(data==nullptr || handleCount==0)
        {
            load(filename);
        }

        handleCount++;
        int id = handleIDs.allocate();
        handles[id] = new ResourceHandle(id, &dataAccess, this->dataID, this->data, this->dataSize);

        return handles[id];
    }

    // Free an already made handle by ID.
    void Resource::freeHandle(int id)
    {
        handleIDs.free(id);
        handleCount--;
        
        delete handles[id];

        if(handleCount<=0 && !persistent)
        {
            unload();
        }
    }

    // Free an already made handle by reference.
    void Resource::freeHandle(ResourceHandle* toFree)
    {
        this->freeHandle(toFree->getHandleID());
    }

    // Accessors
    //----------------------------------

    // Get the resource's ID.
    int Resource::getID()
    {
        return this->dataID;
    }

    // Get the name of the resource's file.
    std::string Resource::getFilename()
    {
        return this->filename;
    }

    // Get how many handles are currently in use.
    int Resource::getHandleCount()
    {
        return this->handleCount;
    }

    // Get whether this object should unload.
    bool Resource::getPersistence()
    {
        return this->persistent;
    }

    // Get a handle by it's ID.
    ResourceHandle* Resource::getHandle(int id)
    {
        if(!handleIDs.isUsed(id))
        {
            return nullptr;
        }
        return handles[id];
    }

    // Get the resource's current size.
    size_t Resource::getSize()
    {
        return this->dataSize;
    }

    template<class T>
    T Resource::getData()
    {
        return ((T)(this->data));
    }

    // Mutators
    //----------------------------------

    // Allow or disallow the resource to remain in memory when not in use.
    void Resource::setPersistence(bool newPersistent)
    {
        this->persistent=newPersistent;
    }
}
