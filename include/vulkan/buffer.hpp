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

        VkMemoryAllocateInfo allocationInfo;
        VkMemoryRequirements memReqs;

        VkDeviceMemory internalBacking;
        VkBuffer internalBuffer;

        bool mapped;

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
                log->error("Required memory type not supported-- should support VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT and VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT");
                throw new bad_alloc();
            }

            // Verify we're even *able* to allocate more memory types.
            auto allocs = parent->incAllocs();
            auto maxAllocs = parent->getParent()->getProperties()->limits.maxMemoryAllocationCount;
            log->debug(string("Allocating memory for buffer, allocation number ").append(to_string(allocs)).append(" / ").append(to_string(maxAllocs)));
            if(allocs>maxAllocs)
            {
                parent->decAllocs();
                log->error("Maximum number of resource allocations exceeded.");
                throw new bad_alloc();
            }

            // Assign allocation info
            allocationInfo = {
                VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
                nullptr,
                this->createInfo.size,
                (unsigned int)(optimalMemIndex) // memoryTypeIndex, determining what type of memory we need, which should be chosen from vkGetPhysicalDeviceMemoryProperties.
            };

            // Attempt to allocate, output result, throw if invalid.
            auto result = vkAllocateMemory((parent->getSelf()), &allocationInfo, (parent->getHost()->getAllocationInfo()), &internalBacking);
            log->debug(string("Buffer memory allocated with size of ").append(to_string((this->createInfo.size))).append(", resulting: ").append(VkResultLookup(result)));
            if(result!=0) throw new bad_alloc();
        }

        // Bind the internalBacking to the internalBuffer
        void bind()
        {
            vkGetBufferMemoryRequirements(parent->getSelf(), internalBuffer, &memReqs);
            if((memReqs.memoryTypeBits>>(allocationInfo.memoryTypeIndex-1) & 1)!=1)
            {
                throw new bad_alloc();
            }

            auto result = vkBindBufferMemory(parent->getSelf(), internalBuffer, internalBacking, 0);

            stringstream tmpStream;
            tmpStream<<"Binding buffer memory to buffer. MemoryTypeBits: "<<memReqs.memoryTypeBits<<", Relevant bit: "<< ((((memReqs.memoryTypeBits>>(allocationInfo.memoryTypeIndex-1) & 1))==1) ? "Supported" : "Unsupported") <<", with result of: "<<VkResultLookup(result);
            log->debug(tmpStream.str());

            mapped=false;
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
            stringstream tmpstream;
            tmpstream<<"Buffer created with result: "<<VkResultLookup(result);
            log->debug(tmpstream.str());

            if(result!=0) throw new runtime_error("Failed to create buffer.");

            // Attempt to allocate memory for the buffer.
            allocate();
            // Attempt to bind memory to the buffer.
            bind();
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
            stringstream tmpstream;
            tmpstream<<"Buffer created with result: "<<VkResultLookup(result);
            log->debug(tmpstream.str());
            if(result!=0) throw new runtime_error("Failed to create buffer.");

            // Attempt to allocate memory for the buffer.
            allocate();
            // Attempt to bind memory to the buffer.
            bind();
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
            log->debug(string("Buffer created with result: ").append(VkResultLookup(result)));

            // Throw an allocation exception
            if(result!=0) throw new runtime_error("Failed to create buffer.");

            // Attempt to allocate memory for the buffer.
            allocate();     
            // Attempt to bind memory to the buffer.
            bind();
        }

        // Mutators
        //----------------------------------
        
        /*
            No mutators currently exist for the buffer as we do not know how we're going to use it. Just that it exists.
        */

        // Accessors
        //----------------------------------

        void* map()
        {
            // Check if memory has the VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT set
            if((parent->getParent()->getMem()->memoryTypes[allocationInfo.memoryTypeIndex].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)==0)
            {
                return nullptr;
            }
            
            if(mapped) 
            {
                return nullptr;
            }
            mapped=true;

            void* mapLocation;
            vkMapMemory(parent->getSelf(), internalBacking, 0, allocationInfo.allocationSize, {}, &mapLocation);
            return mapLocation;
        }
        void* map(Range mappedRange)
        {
            if((parent->getParent()->getMem()->memoryTypes[allocationInfo.memoryTypeIndex].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)==0)
            {
                return nullptr;
            }

            if(mapped || mappedRange.getMax()>allocationInfo.allocationSize) 
            {
                return nullptr;
            }
            mapped=true;

            void* mapLocation;
            vkMapMemory(parent->getSelf(), internalBacking, mappedRange.getMin(), mappedRange.getMax(), {}, &mapLocation);
            return mapLocation;
        }

        void unmap()
        {
            if(!mapped)
            {
                return;
            }
            mapped=false;

            vkUnmapMemory(parent->getSelf(), internalBacking);
        }

        bool isMapped() { return mapped; }

        auto getBuffer() { return this->internalBuffer; }        
        auto getMemory() { return this->internalBacking; }
        auto getParent() { return parent; }
        auto getAllocInfo() { return allocationInfo; }
        auto getCreateInfo() { return createInfo; }
        auto getMemReqs() { return memReqs; }

        // Operators
        //----------------------------------

        // Destructors
        //----------------------------------

        ~Buffer()
        {
            // Verify that no work is pending on the buffer
            
            vkFreeMemory(parent->getSelf(), internalBacking, parent->getHost()->getAllocationInfo());
            parent->decAllocs();
            vkDestroyBuffer(parent->getSelf(), internalBuffer, parent->getHost()->getAllocationInfo());

            // Destroy any buffer views that have been created of this buffer.
            // vkDestroyBufferView(parent->getSelf(), VkBufferView bufferView, parent->getHost()->getAllocationInfo());

            log->debug("Destroying buffer.");
        }
        
    };
};
#endif