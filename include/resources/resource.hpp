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
#ifndef RESOURCE_H
#define RESOURCE_H

#include "sb-engine.hpp"
#include "./resourcehandle.hpp"

using namespace std;

namespace SBE
{
    // Resource:    Utility
    // Loads a single file into RAM. Keeps track of ResourceHandles.
    // Auto deletes resource when unneeded if not persistent.
    class Resource
    {
    private:
        // Resource location and name
        string filename;
        
        // Data Variables
        //----------------------------------

        binary_semaphore dataAccess{1};

        void* data;
        size_t initialSize;
        size_t dataSize;
        int dataID;

        // Do we want to keep this loaded no matter what?
        // Note: We may want to replace this with some form of enum or bitwise flag later
        bool persistent;

        // Handle Management
        //----------------------------------
        IDManager handleIDs;
        unordered_map<int, ResourceHandle*> handles;
        int handleCount;

        // Load some file into the resource's data spot
        void load(string filename)
        {
            if(handleCount>0) return;
            try
            {
                dataAccess.acquire();

                // Open resource
                fstream file;
                file.open(filename, ios_base::in | ios_base::binary | ios_base::ate);

                // Verify resource opened
                if(!file.is_open() || file.bad())
                {
                    throw new invalid_argument(
                        ((string)("Provided file did not open. File: ")).append(filename).c_str()
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
                    throw new out_of_range(
                        ((string)("Provided file has size of zero bytes. File: ")).append(filename).c_str()
                    );
                }

                // Dedicate data
                data = malloc(dataSize);
                

                // Reopen with preferred settings
                file.open(filename, ios_base::in | ios_base::binary);

                // Save data.
                file.read(((char*)(data)),dataSize);
                
                // cout<<"Loading file "<<filename<<" into memory at location "<<data<<" with data size "<<dataSize<<" bytes."<<endl;
                file.close();
                
                dataAccess.release();
            }
            catch(invalid_argument* e)
            {
                cout<<e->what()<<endl;
            }
            catch(out_of_range* e)
            {
                cout<<e->what()<<endl;
            }
        }
    
        void unload()
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

            // cout<<"Clearing data. Data at "<<data<<" with size "<<dataSize<<" will be deleted."<<endl;

            if(data!=nullptr)
            {
                free(data);
            }


            dataSize=0;
        }

    public:

        // Constructors
        //----------------------------------

        // Create a resource for this file. Optionally, load it.
        Resource(int id, string filename, bool persistence=false, bool preload=false)
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

        // Destructors
        //----------------------------------

        // Unload a resource and destroy the object.
        ~Resource()
        {
            unload();
        }

        // Handle Management
        //----------------------------------

        // Create a new handle for the resource.
        ResourceHandle* allocateHandle()
        {
            if(data==nullptr || handleCount==0)
            {
                load(filename);
            }

            handleCount++;
            int id = handleIDs.allocate();
            handles[id]=new ResourceHandle(id, &(this->dataAccess), this->dataID, this->data, this->dataSize);

            return handles[id];
        }
        
        // Free an already made handle by ID.
        void freeHandle(int id)
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
        void freeHandle(ResourceHandle* toFree)
        {
            this->freeHandle(toFree->getHandleID());
        }
    
        // Accessors
        //----------------------------------

        // Get the resource's ID.
        int getID()
        {
            return this->dataID;
        }
        
        // Get the name of the resource's file.
        string getFilename()
        {
            return this->filename;
        }
        
        // Get how many handles are currently in use.
        int getHandleCount()
        {
            return this->handleCount;
        }
        
        // Get whether this object should unload.
        bool getPersistence()
        {
            return this->persistent;
        }

        // Get a handle by it's ID.
        ResourceHandle* getHandle(int id)
        {
            if(!handleIDs.isUsed(id))
            {
                return nullptr;
            }
            return handles[id];
        }
        
        // Get the resource's current size.
        size_t getSize()
        {
            return this->dataSize;
        }

        // Mutators
        //----------------------------------

        // Allow or disallow the resource to remain in memory when not in use.
        void setPersistence(bool newPersistent)
        {
            this->persistent=newPersistent;
        }
    };
};
#endif