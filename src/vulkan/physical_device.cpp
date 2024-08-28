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

#include "vulkan/instance.hpp"
#include "vulkan/physical_device.hpp"

namespace SBE
{
    // Constructors
    //----------------------------------
    PhysicalDevice::PhysicalDevice(VkPhysicalDevice nSelf, Instance *host)
    {
        this->host = host;
        self = nSelf;
        // queueFamilies = new QueueFamilyCollection(this);

        this->update();
    }

    PhysicalDevice::PhysicalDevice(VkPhysicalDevice nSelf, VkPhysicalDeviceProperties nProps, Instance *host)
    {
        this->host = host;
        self = nSelf;
        selfProps = nProps;
        // queueFamilies = new QueueFamilyCollection(this);

        this->update();
    }

    // Mutators
    //----------------------------------
    void PhysicalDevice::update()
    {
        vkGetPhysicalDeviceProperties(self, &selfProps);
        vkGetPhysicalDeviceFeatures(self, &selfFeats);
        vkGetPhysicalDeviceMemoryProperties(self, &selfMem);

        // queueFamilies->update();
    }

    // Accessors
    //----------------------------------
    VkPhysicalDevice PhysicalDevice::getDevice()
    {
        return self;
    }

    VkPhysicalDeviceProperties *PhysicalDevice::getProperties()
    {
        return &selfProps;
    }

    VkPhysicalDeviceFeatures *PhysicalDevice::getFeatures()
    {
        return &selfFeats;
    }

    VkPhysicalDeviceMemoryProperties *PhysicalDevice::getMem()
    {
        return &selfMem;
    }

    Instance *PhysicalDevice::getHost()
    {
        return host;
    }
};