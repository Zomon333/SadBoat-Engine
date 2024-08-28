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
#ifndef EXTENSION_COLLECTION_H
#define EXTENSION_COLLECTION_H

#include "./vulkan/vulkan.hpp"
#include "../include/vulkan/physical_device.hpp"

namespace SBE
{
    // ExtensionCollection: Graphics
    // Generalized container for querying and selecting the extensions available on the given PhysicalDevice
    class ExtensionCollection
    {
    private:
        VkResult result;
        PhysicalDevice *parent;

        unsigned int count;
        std::vector<VkExtensionProperties> properties;

    public:
        // Constructors
        ExtensionCollection(PhysicalDevice *parent);

        // Accessors
        std::vector<VkExtensionProperties> getProps();
        VkExtensionProperties getProp(int which);

        // Mutators
        void update();
    };
};
#endif