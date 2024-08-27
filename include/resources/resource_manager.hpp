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

#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <unordered_map>
#include <string>

#include "id_manager.hpp"
#include "resource.hpp"
#include "resource_handle.hpp"

namespace SBE
{
    // // ResourceManager:     Utility
    class ResourceManager
    {
        private:
            SBE::IDManager resIDMan;
            std::unordered_map<int, Resource*> intResource;
            std::unordered_map<std::string, Resource*> strResource;

        public:
            ResourceManager();

            Resource* createResource(std::string filename, bool persistence, bool preload);
            ResourceHandle* getHandle(std::string name);
            ResourceHandle* getHandle(int id);
            std::vector<int> getValidResources();
    };
};
#endif