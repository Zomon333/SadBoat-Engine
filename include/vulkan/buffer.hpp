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
#ifndef BUFFER_H
#define BUFFER_H

#include "sb-engine.hpp"
#include "./vulkan/vulkan.hpp"
#include "../include/vulkan/logicaldevice.hpp"

using namespace std;

namespace SBE
{
    class Buffer
    {
    private:
        LogicalDevice* parent;
        VkBufferCreateInfo createInfo;

        VkBuffer internalBuffer;

    public:
        // Constructors
        //----------------------------------
        
        // Create buffer given a parent and all the creation info
        Buffer(LogicalDevice* parent, VkBufferCreateInfo createInfo)
        {
            this->parent=parent;
            this->createInfo=createInfo;

            if(createInfo.usage & VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT || createInfo.usage & VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT)
            {
                // If we reach here, the buffer IS FORMATTED.
                // We'll need this later for when we start caring.

                // Also, we need to agree upon a standard format for texels in the engine.
                const VkFormat formatType = VK_FORMAT_UNDEFINED;

                VkFormatProperties formatProps;
                vkGetPhysicalDeviceFormatProperties((parent->getParent()->getDevice()), formatType, &formatProps);
            }

            auto result = vkCreateBuffer(parent->getSelf(), &(this->createInfo), parent->getHost()->getAllocationInfo(), &internalBuffer);
            cout<<"Buffer created with result: "<<VkResultLookup(result)<<endl;
        }

        // Create buffer given a parent, size, and use case
        Buffer(LogicalDevice* parent, VkDeviceSize size, VkBufferUsageFlags usage)
        {
            this->parent=parent;
            this->createInfo={
                VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO, nullptr,
                0,
                size,
                usage,
                VK_SHARING_MODE_EXCLUSIVE,
                0, nullptr
            };

            if(createInfo.usage & VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT || createInfo.usage & VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT)
            {
                // If we reach here, the buffer IS FORMATTED.
                // We'll need this later for when we start caring.

                // Also, we need to agree upon a standard format for texels in the engine.
                const VkFormat formatType = VK_FORMAT_UNDEFINED;

                VkFormatProperties formatProps;
                vkGetPhysicalDeviceFormatProperties((parent->getParent()->getDevice()), formatType, &formatProps);
            }

            auto result = vkCreateBuffer(parent->getSelf(), &(this->createInfo), parent->getHost()->getAllocationInfo(), &internalBuffer);
            cout<<"Buffer created with result: "<<VkResultLookup(result)<<endl;
        }

        // Create buffer given a parent, size, usage, and sharing needs.
        Buffer(LogicalDevice* parent, VkDeviceSize size, VkBufferUsageFlags usage, vector<unsigned int> queueFamIndices)
        {
            this->parent=parent;
            this->createInfo={
                VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO, nullptr,
                0,
                size,
                usage,
                (queueFamIndices.size()>0) ? VK_SHARING_MODE_CONCURRENT : VK_SHARING_MODE_EXCLUSIVE,
                (uint32_t)(queueFamIndices.size()), queueFamIndices.data()
            };

            auto result = vkCreateBuffer(parent->getSelf(), &(this->createInfo), parent->getHost()->getAllocationInfo(), &internalBuffer);
            cout<<"Buffer created with result: "<<VkResultLookup(result)<<endl;
        }

        // Mutators
        //----------------------------------

        // Accessors
        //----------------------------------

        // Operators
        //----------------------------------

        // Destructors
        //----------------------------------

        
    };
};
#endif