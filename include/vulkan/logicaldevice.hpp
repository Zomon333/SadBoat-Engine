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
#ifndef LOGICAL_DEVICE_H
#define LOGICAL_DEVICE_H

#include "sb-engine.hpp"
#include "./vulkan/vulkan.hpp"

#include "./instance.hpp"
#include "./physicaldevice.hpp"


using namespace std;

namespace SBE
{
    class LogicalDevice
    {
    private:
        Instance* host;
        PhysicalDevice* parent;

        VkDeviceCreateInfo* creationInfo;
        VkDevice self;
    public:
        // Constructors
        LogicalDevice(PhysicalDevice* parent, VkDeviceCreateInfo* creationInfo)
        {
            this->host=parent->getHost();
            this->creationInfo=creationInfo;
            vkCreateDevice(parent->getDevice(), creationInfo, host->getAllocationInfo(), &self);
        }
        // Mutators

        // Accessors
    };
};
#endif