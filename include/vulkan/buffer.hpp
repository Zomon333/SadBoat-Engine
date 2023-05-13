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

        VkDeviceMemory internalBacking;
        VkBuffer internalBuffer;

        // Allocate memory into internalBacking
        void allocate()
        {
            // Check the memory types supported by the device
            auto mem = parent->getParent()->getMem();
            int optimalMemIndex = -1;

            // Search through them all for one that is on the GPU but CPU visible
            for(int i=0; i<mem->memoryTypeCount; i++)
            {
                auto flags = mem->memoryTypes[i].propertyFlags;
                if(flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT && flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
                {
                    // Save it
                    optimalMemIndex = i;
                    break;
                }
            }
            if(optimalMemIndex==-1)
            {
                // If there aren't any, then throw an exception and don't allocate.
                throw new bad_alloc();
            }

            // Verify we're even *able* to allocate more memory types.
            auto allocs = parent->incAllocs();
            auto maxAllocs = parent->getParent()->getProperties()->limits.maxMemoryAllocationCount;
            cout<<"Allocating memory for buffer, allocation number "<<allocs<<" / "<<maxAllocs<<endl;
            if(allocs>maxAllocs)
            {
                parent->decAllocs();
                throw new bad_alloc();
            }

            // Assign allocation info
            VkMemoryAllocateInfo allocation = {
                VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
                nullptr,
                this->createInfo.size,
                optimalMemIndex // memoryTypeIndex, determining what type of memory we need, which should be chosen from vkGetPhysicalDeviceMemoryProperties.
            };

            // Attempt to allocate, output result, throw if invalid.
            auto result = vkAllocateMemory((parent->getSelf()), &allocation, (parent->getHost()->getAllocationInfo()), &internalBacking);
            cout<<"Buffer memory allocated with size of "<<this->createInfo.size<<", resulting: "<<VkResultLookup(result)<<endl;
            if(result!=0) throw new bad_alloc();
        }

    public:
        // Constructors
        //----------------------------------
        
        // Create buffer given a parent and all the creation info
        Buffer(LogicalDevice* parent, VkBufferCreateInfo createInfo)
        {
            // Assign our data
            this->parent=parent;
            this->createInfo=createInfo;

            // Check for format
            if(createInfo.usage & VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT || createInfo.usage & VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT)
            {
                // If we reach here, the buffer IS FORMATTED.
                // We'll need this later for when we start caring.

                // Also, we need to agree upon a standard format for texels in the engine.
                const VkFormat formatType = VK_FORMAT_UNDEFINED;

                VkFormatProperties formatProps;
                vkGetPhysicalDeviceFormatProperties((parent->getParent()->getDevice()), formatType, &formatProps);
            }

            // Attempt to create buffer, output result, throw if invalid.
            auto result = vkCreateBuffer(parent->getSelf(), &(this->createInfo), parent->getHost()->getAllocationInfo(), &internalBuffer);
            cout<<"Buffer created with result: "<<VkResultLookup(result)<<endl;
            if(result!=0) throw new runtime_error("Failed to create buffer.");

            // Attempt to allocate memory for the buffer.
            allocate();
        }

        // Create buffer given a parent, size, and use case
        Buffer(LogicalDevice* parent, VkDeviceSize size, VkBufferUsageFlags usage)
        {
            // Assign our data
            this->parent=parent;
            this->createInfo={
                VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO, nullptr,
                0,
                size,
                usage,
                VK_SHARING_MODE_EXCLUSIVE,
                0, nullptr
            };

            // Check for format
            if(createInfo.usage & VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT || createInfo.usage & VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT)
            {
                // If we reach here, the buffer IS FORMATTED.
                // We'll need this later for when we start caring.

                // Also, we need to agree upon a standard format for texels in the engine.
                const VkFormat formatType = VK_FORMAT_UNDEFINED;

                VkFormatProperties formatProps;
                vkGetPhysicalDeviceFormatProperties((parent->getParent()->getDevice()), formatType, &formatProps);
            }

            // Attempt to create buffer, output result, throw if invalid.
            auto result = vkCreateBuffer(parent->getSelf(), &(this->createInfo), parent->getHost()->getAllocationInfo(), &internalBuffer);
            cout<<"Buffer created with result: "<<VkResultLookup(result)<<endl;
            if(result!=0) throw new runtime_error("Failed to create buffer.");

            // Attempt to allocate memory for the buffer.
            allocate();
        }

        // Create buffer given a parent, size, usage, and sharing needs.
        Buffer(LogicalDevice* parent, VkDeviceSize size, VkBufferUsageFlags usage, vector<unsigned int> queueFamIndices)
        {
            // Assign the data we need for the buffer
            this->parent=parent;
            this->createInfo={
                VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO, nullptr,
                0,
                size,
                usage,
                (queueFamIndices.size()>0) ? VK_SHARING_MODE_CONCURRENT : VK_SHARING_MODE_EXCLUSIVE,
                (uint32_t)(queueFamIndices.size()), queueFamIndices.data()
            };

            // Create the buffer and output the result
            auto result = vkCreateBuffer(parent->getSelf(), &(this->createInfo), parent->getHost()->getAllocationInfo(), &internalBuffer);
            cout<<"Buffer created with result: "<<VkResultLookup(result)<<endl;

            // Throw an allocation exception
            if(result!=0) throw new runtime_error("Failed to create buffer.");

            // Attempt to allocate memory for the buffer.
            allocate();     
        }

        // Mutators
        //----------------------------------

        // Accessors
        //----------------------------------

        // Operators
        //----------------------------------

        // Destructors
        //----------------------------------

        ~Buffer()
        {
            parent->decAllocs();
        }
        
    };
};
#endif