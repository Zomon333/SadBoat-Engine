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

#include "vulkan.hpp"

#include "utilities/logging/startup_logger.hpp"
#include "vulkan/vulkan_result_lookup.hpp"
#include "vulkan/buffer.hpp"
#include "vulkan/logical_device.hpp"

namespace SBE
{
    void Buffer::allocate()
    {
        // Check the memory types supported by the device
        auto mem = parent->getParent()->getMem();
        int optimalMemIndex = -1;

        // Search through them all for one that is on the GPU but CPU visible
        for (unsigned int i = 0; i < mem->memoryTypeCount; i++)
        {
            auto flags = mem->memoryTypes[i].propertyFlags;
            if (flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT && flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
            {
                // Save it
                optimalMemIndex = i;
                break;
            }
        }
        if (optimalMemIndex == -1)
        {
            // If there aren't any, then throw an exception and don't allocate.
            SBE::log->error("Required memory type not supported-- should support VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT and VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT");
            throw new std::bad_alloc();
        }

        // Verify we're even *able* to allocate more memory types.
        unsigned int allocs = parent->incAllocs();
        unsigned int maxAllocs = parent->getParent()->getProperties()->limits.maxMemoryAllocationCount;
        SBE::log->info(std::string("Allocating memory for buffer, allocation number ").append(std::to_string(allocs)).append(" / ").append(std::to_string(maxAllocs)));
        if (allocs > maxAllocs)
        {
            parent->decAllocs();
            SBE::log->error("Maximum number of resource allocations exceeded.");
            throw new std::bad_alloc();
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
        log->info(std::string("Buffer memory allocated with size of ").append(std::to_string((this->createInfo.size))).append(", resulting: ").append(SBE::VkResultLookup(result)));
        if (result != VK_SUCCESS)
            throw new std::bad_alloc();
    }

    void Buffer::allocate(VkDeviceSize bufferSize)
    {
        // Check the memory types supported by the device
        auto mem = parent->getParent()->getMem();
        int optimalMemIndex = -1;

        // Search through them all for one that is on the GPU but CPU visible
        for (unsigned int i = 0; i < mem->memoryTypeCount; i++)
        {
            auto flags = mem->memoryTypes[i].propertyFlags;
            if (flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT && flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
            {
                // Save it
                optimalMemIndex = i;
                break;
            }
        }
        if (optimalMemIndex == -1)
        {
            // If there aren't any, then throw an exception and don't allocate.
            SBE::log->error("Required memory type not supported-- should support VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT and VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT");
            throw new std::bad_alloc();
        }

        // Verify we're even *able* to allocate more memory types.
        unsigned int allocs = parent->incAllocs();
        unsigned int maxAllocs = parent->getParent()->getProperties()->limits.maxMemoryAllocationCount;
        SBE::log->info(std::string("Allocating memory for buffer, allocation number ").append(std::to_string(allocs)).append(" / ").append(std::to_string(maxAllocs)));
        if (allocs > maxAllocs)
        {
            parent->decAllocs();
            SBE::log->error("Maximum number of resource allocations exceeded.");
            throw new std::bad_alloc();
        }

        // Assign allocation info
        allocationInfo = {
            VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
            nullptr,
            bufferSize,
            (unsigned int)(optimalMemIndex) // memoryTypeIndex, determining what type of memory we need, which should be chosen from vkGetPhysicalDeviceMemoryProperties.
        };

        // Attempt to allocate, output result, throw if invalid.
        auto result = vkAllocateMemory((parent->getSelf()), &allocationInfo, (parent->getHost()->getAllocationInfo()), &internalBacking);
        SBE::log->info(std::string("Buffer memory allocated with size of ").append(std::to_string((bufferSize))).append(", resulting: ").append(VkResultLookup(result)));
        if (result != 0)
            throw new std::bad_alloc();
    }

    // Bind the internalBacking to the internalBuffer
    void Buffer::bind()
    {
        vkGetBufferMemoryRequirements(parent->getSelf(), internalBuffer, &memReqs);
        if ((memReqs.memoryTypeBits >> (allocationInfo.memoryTypeIndex - 1) & 1) != 1)
        {
            throw new std::bad_alloc();
        }

        auto result = vkBindBufferMemory(parent->getSelf(), internalBuffer, internalBacking, 0);

        std::stringstream tmpStream;
        tmpStream << "Binding buffer memory to buffer. MemoryTypeBits: " << memReqs.memoryTypeBits << ", Relevant bit: " << ((((memReqs.memoryTypeBits >> (allocationInfo.memoryTypeIndex - 1) & 1)) == 1) ? "Supported" : "Unsupported") << ", with result of: " << VkResultLookup(result);
        SBE::log->info(tmpStream.str());

        mapped = false;
    }

    // Constructors
    //----------------------------------

    // Create buffer given a parent and all the creation info
    Buffer::Buffer(LogicalDevice *parent, VkBufferCreateInfo createInfo, bool sparse)
    {
        // Assign our data
        this->parent = parent;
        this->createInfo = createInfo;
        this->sparse = sparse;

        // Check for format
        if (createInfo.usage & VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT || createInfo.usage & VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT)
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
        std::stringstream tmpstream;
        tmpstream << "Buffer created with result: " << VkResultLookup(result);
        SBE::log->info(tmpstream.str());

        if (result != 0)
            throw new std::runtime_error("Failed to create buffer.");

        // Attempt to allocate memory for the buffer.
        allocate();
        // Attempt to bind memory to the buffer.
        bind();
    }

    // Create buffer given a parent, size, and use case
    Buffer::Buffer(LogicalDevice *parent, VkDeviceSize size, VkBufferUsageFlags usage, bool sparse)
    {
        // Assign our data
        this->sparse = sparse;
        this->parent = parent;
        unsigned int accessNumber = (unsigned int)(parent->getOptimalQueueFam()->getIndex());
        this->createInfo = {
            VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO, nullptr,
            0,
            size,
            usage,
            VK_SHARING_MODE_CONCURRENT,
            1,
            &accessNumber};

        // Check for format
        if (createInfo.usage & VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT || createInfo.usage & VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT)
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
        std::stringstream tmpstream;
        tmpstream << "Buffer created with result: " << VkResultLookup(result);
        SBE::log->info(tmpstream.str());
        if (result != 0)
            throw new std::runtime_error("Failed to create buffer.");

        // Attempt to allocate memory for the buffer.
        allocate();
        // Attempt to bind memory to the buffer.
        bind();
    }

    // Create buffer given a parent, size, usage, and sharing needs.
    Buffer::Buffer(LogicalDevice *parent, VkDeviceSize size, VkBufferUsageFlags usage, std::vector<unsigned int> queueFamIndices, bool sparse)
    {
        // Assign the data we need for the buffer
        this->sparse = sparse;
        this->parent = parent;
        this->createInfo = {
            VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO, nullptr,
            0,
            size,
            usage,
            (queueFamIndices.size() > 0) ? VK_SHARING_MODE_CONCURRENT : VK_SHARING_MODE_EXCLUSIVE,
            (uint32_t)(queueFamIndices.size()), queueFamIndices.data()};

        // Create the buffer and output the result
        auto result = vkCreateBuffer(parent->getSelf(), &(this->createInfo), parent->getHost()->getAllocationInfo(), &internalBuffer);
        SBE::log->info(std::string("Buffer created with result: ").append(VkResultLookup(result)));

        // Throw an allocation exception
        if (result != 0)
            throw new std::runtime_error("Failed to create buffer.");

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

    void *Buffer::map()
    {
        // Check if memory has the VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT set
        if ((parent->getParent()->getMem()->memoryTypes[allocationInfo.memoryTypeIndex].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) == 0)
        {
            return nullptr;
        }

        if (mapped)
        {
            return nullptr;
        }
        mapped = true;

        void *mapLocation;
        vkMapMemory(parent->getSelf(), internalBacking, 0, allocationInfo.allocationSize, {}, &mapLocation);
        return mapLocation;
    }
    void *Buffer::map(std::pair<unsigned int, unsigned int> mappedRange)
    {
        if ((parent->getParent()->getMem()->memoryTypes[allocationInfo.memoryTypeIndex].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) == 0)
        {
            return nullptr;
        }

        if (mapped || mappedRange.second > allocationInfo.allocationSize)
        {
            return nullptr;
        }
        mapped = true;

        void *mapLocation;
        vkMapMemory(parent->getSelf(), internalBacking, mappedRange.first, mappedRange.second, {}, &mapLocation);
        return mapLocation;
    }

    void Buffer::unmap()
    {
        if (!mapped)
        {
            return;
        }
        mapped = false;

        vkUnmapMemory(parent->getSelf(), internalBacking);
    }

    bool Buffer::isMapped()
    {
        return mapped;
    }

    VkBuffer Buffer::getBuffer()
    {
        return this->internalBuffer;
    }

    VkDeviceMemory Buffer::getMemory()
    {
        return this->internalBacking;
    }

    SBE::LogicalDevice *Buffer::getParent()
    {
        return parent;
    }

    VkMemoryAllocateInfo Buffer::getAllocInfo()
    {
        return allocationInfo;
    }

    VkBufferCreateInfo Buffer::getCreateInfo()
    {
        return createInfo;
    }

    VkMemoryRequirements Buffer::getMemReqs()
    {
        return memReqs;
    }

    // Operators
    //----------------------------------

    // Destructors
    //----------------------------------

    Buffer::~Buffer()
    {
        // Verify that no work is pending on the buffer

        vkFreeMemory(parent->getSelf(), internalBacking, parent->getHost()->getAllocationInfo());
        parent->decAllocs();
        vkDestroyBuffer(parent->getSelf(), internalBuffer, parent->getHost()->getAllocationInfo());

        // Destroy any buffer views that have been created of this buffer.
        // vkDestroyBufferView(parent->getSelf(), VkBufferView bufferView, parent->getHost()->getAllocationInfo());

        SBE::log->info("Destroying buffer.");
    }

};