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
    // // ResourceManager:     Utility
    class ResourceManager
    {
        private:
            IDManager resIDMan;
            unordered_map<int, Resource*> intResource;
            unordered_map<string, Resource*> strResource;


        public:
            ResourceManager(){}

            Resource* createResource(string filename, bool persistence=false, bool preload=false)
            {
                int id = resIDMan.allocate();
                Resource* toCreate = new Resource(id, filename, persistence, preload);
                return toCreate;
            }

            ResourceHandle* getHandle(string name)
            {
                return strResource[name]->allocateHandle();
            }
            ResourceHandle* getHandle(int id)
            {
                return intResource[id]->allocateHandle();
            }

            auto getValidResources()
            {
                return resIDMan.getUsedIDs();
            }

    };
};
#endif