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
    // Directly ties into ResourceManager for automated reference counting for memory management.
    class ResourceHandle
    {
    private:
        string path;

        int id;

        void* data;
        size_t dataSize;

    public:
        // Constructors
        //----------------------------------

        ResourceHandle(string path, void* data, size_t size, int id)
        {
            this->path=path;
            this->id=id;
            this->dataSize=size;

            this->data=data;
        }

        // Destructors
        //----------------------------------

        ~ResourceHandle()
        {
            // Just set data to nullptr to ensure data persistence. 
            // Actual data deletion is handled by ResourceManager using an ARC.
            data=nullptr;
        }

        // Accessors
        //----------------------------------

        // Returns the data casted to the provided type.
        template<class dataType>
        dataType getData()
        {
            return (*(dataType*)(data));
        }

        // Returns the data casted to the provided pointer.
        template<class dataType>
        dataType* getDataPtr()
        {
            return (dataType*)(data);
        }

        // Returns the size of the data.
        size_t getDataSize()
        {
            return dataSize;
        }

        // Returns the data's ID.
        int getID()
        {
            return id;
        }

        // Returns the data's path.
        string getPath()
        {
            return path;
        }
    };
};
#endif