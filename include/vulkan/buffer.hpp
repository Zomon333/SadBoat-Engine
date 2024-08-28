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
#ifndef BUFFER_H
#define BUFFER_H

#include "./vulkan/vulkan.hpp"
#include "vulkan/logical_device.hpp"

namespace SBE
{
    class Buffer
    {
    private:
        LogicalDevice *parent;
        VkBufferCreateInfo createInfo;

        VkMemoryAllocateInfo allocationInfo;
        VkMemoryRequirements memReqs;

        VkDeviceMemory internalBacking;
        VkBuffer internalBuffer;

        bool sparse;
        bool mapped;

        // Allocate memory into internalBacking
        void allocate();

        void allocate(VkDeviceSize bufferSize);

        // Bind the internalBacking to the internalBuffer
        void bind();

    public:
        // Constructors
        //----------------------------------

        // Create buffer given a parent and all the creation info
        Buffer(LogicalDevice *parent, VkBufferCreateInfo createInfo, bool sparse = false);

        // Create buffer given a parent, size, and use case
        Buffer(LogicalDevice *parent, VkDeviceSize size, VkBufferUsageFlags usage, bool sparse = false);

        // Create buffer given a parent, size, usage, and sharing needs.
        Buffer(LogicalDevice *parent, VkDeviceSize size, VkBufferUsageFlags usage, std::vector<unsigned int> queueFamIndices, bool sparse = false);

        // Mutators
        //----------------------------------

        /*
            No mutators currently exist for the buffer as we do not know how we're going to use it. Just that it exists.
        */

        // Accessors
        //----------------------------------

        void *map();
        void *map(std::pair<int, int> mappedRange);

        void unmap();

        bool isMapped();

        VkBuffer getBuffer();
        VkDeviceMemory getMemory();
        SBE::LogicalDevice *getParent();
        VkMemoryAllocateInfo getAllocInfo();
        VkBufferCreateInfo getCreateInfo();
        VkMemoryRequirements getMemReqs();

        // Operators
        //----------------------------------

        // Destructors
        //----------------------------------

        ~Buffer();
    };
};
#endif