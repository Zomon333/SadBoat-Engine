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
#ifndef VULKAN_DISPATCHABLES_H
#define VULKAN_DISPATCHABLES_H

#include "sb-engine.hpp"

using namespace std;

namespace SBE
{
    struct VulkanDispatchables
    {
        Instance* vulkanInstance;
        PhysicalDeviceCollection* vulkanDevices;
        PhysicalDevice* preferredDevice;

        VkPhysicalDeviceFeatures* requiredFeatures;

        LogicalDevice* vulkanLogicalDevice;

        Event<VulkanDispatchables*, VulkanDispatchables*> setup = Event<VulkanDispatchables*, VulkanDispatchables*>(
            F(VulkanDispatchables* toInit)
            {
                toInit->vulkanInstance = new Instance;

                toInit->vulkanDevices = new PhysicalDeviceCollection(toInit->vulkanInstance);
                toInit->preferredDevice = toInit->vulkanDevices->getOptimal();

                toInit->requiredFeatures = new VkPhysicalDeviceFeatures;
                toInit->requiredFeatures->multiDrawIndirect=VK_TRUE;
                toInit->requiredFeatures->tessellationShader=VK_TRUE;
                toInit->requiredFeatures->geometryShader=VK_TRUE;

                toInit->vulkanLogicalDevice = new LogicalDevice(toInit->preferredDevice, toInit->requiredFeatures);

                return toInit;
            }
        );
    };
};
#endif

