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

#include "./vulkan/vulkan.hpp"
#include "vulkan/extension_collection.hpp"
#include "vulkan/physical_device.hpp"

namespace SBE
{
    // Constructors
    ExtensionCollection::ExtensionCollection(PhysicalDevice *parent)
    {
        this->parent = parent;
        update();
    }

    // Accessors
    std::vector<VkExtensionProperties> ExtensionCollection::getProps()
    {
        return properties;
    }

    VkExtensionProperties ExtensionCollection::getProp(int which)
    {
        return properties[which];
    }

    // Mutators
    void ExtensionCollection::update()
    {
        result = vkEnumerateDeviceExtensionProperties(parent->getDevice(), nullptr, &count, nullptr);
        properties.resize(count);
        result = vkEnumerateDeviceExtensionProperties(parent->getDevice(), nullptr, &count, (properties.data()));
    }
};