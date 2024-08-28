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

#include "utilities/logging/startup_logger.hpp"
#include "vulkan/vulkan_result_lookup.hpp"
#include "vulkan/command_buffer.hpp"
#include "vulkan/command_pool.hpp"

namespace SBE
{
    void CommandPool::allocate(unsigned int bufferCount)
    {
        bufferBacking = new VkCommandBuffer[bufferCount];
        auto result = vkAllocateCommandBuffers(parent->getSelf(), pAllocateInfo, bufferBacking);
        std::stringstream info;
        info << "Creating " << bufferCount << " CommandBuffers with result of " << VkResultLookup(result) << ". ";
        SBE::log->info(&info);

        queues = new QueueCollection(parent, parent->getOptimalQueueFam(), parent->getOptimalQueueFam()->getProps()->queueCount);
        for (int i = 0; i < parent->getOptimalQueueFam()->getProps()->queueCount; i++)
        {
            buffers.allocateData(new CommandBuffer(parent, queues, bufferBacking[i]));
        }
    }

    // Constructors
    //----------------------------------
    CommandPool::CommandPool(LogicalDevice *parent, VkCommandPoolCreateInfo *pCreateInfo, VkCommandBufferAllocateInfo *pAllocateInfo, unsigned int bufferCount = 0)
    {
        this->parent = parent;
        this->pCreateInfo = pCreateInfo;
        this->pAllocateInfo = pAllocateInfo;

        if (!parent)
            throw new std::exception();
        if (!pCreateInfo)
            throw new std::exception();
        if (!pAllocateInfo)
            throw new std::exception();

        if (bufferCount == 0)
        {
            bufferCount = parent->getOptimalQueueFam()->getProps()->queueCount;
        }

        VkResult result = vkCreateCommandPool(parent->getSelf(), pCreateInfo, parent->getHost()->getAllocationInfo(), &self);

        std::stringstream info;
        info << "Creating CommandPool with result of " << VkResultLookup(result) << ". ";
        info << "Transient: " << ((pCreateInfo->flags & VK_COMMAND_POOL_CREATE_TRANSIENT_BIT) ? "True" : "False");
        info << " ";
        info << "Resettable: " << ((pCreateInfo->flags & VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT) ? "True" : "False");
        info << " ";
        SBE::log->info(&info);

        allocate(bufferCount);
    }

    CommandPool::CommandPool(LogicalDevice *parent, bool isTransient = true, bool isResettable = true, unsigned int bufferCount = 0)
    {
        this->parent = parent;

        this->pCreateInfo = new VkCommandPoolCreateInfo{
            VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
            nullptr,
            ((isTransient) ? VK_COMMAND_POOL_CREATE_TRANSIENT_BIT : (unsigned int)(0)) | ((isResettable) ? VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT : (unsigned int)(0)),
            parent->getOptimalQueueFam()->getIndex()};
        if (bufferCount == 0)
        {
            bufferCount = parent->getOptimalQueueFam()->getProps()->queueCount;
        }

        VkResult result = vkCreateCommandPool(parent->getSelf(), pCreateInfo, parent->getHost()->getAllocationInfo(), &self);

        std::stringstream info;
        info << "Creating CommandPool with result of " << VkResultLookup(result) << ". ";
        info << "Transient: " << ((pCreateInfo->flags & VK_COMMAND_POOL_CREATE_TRANSIENT_BIT) ? "True" : "False");
        info << " ";
        info << "Resettable: " << ((pCreateInfo->flags & VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT) ? "True" : "False");
        info << " ";
        SBE::log->debug(&info);

        this->pAllocateInfo = new VkCommandBufferAllocateInfo{
            VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
            nullptr,
            self,
            VK_COMMAND_BUFFER_LEVEL_PRIMARY,
            bufferCount};

        allocate(bufferCount);
    }

    // Mutators
    //----------------------------------
    void CommandPool::freeBuffer(int id)
    {
        vkFreeCommandBuffers(parent->getSelf(), self, 1, buffers[id]->getSelf());
    }
    void CommandPool::freeBuffers(int *ids, int count = 1)
    {
        for (int i = 0; i < count; i++)
        {
            this->freeBuffer(ids[i]);
        }
    }

    // Accessors
    //----------------------------------
    VkCommandPool CommandPool::getSelf()
    {
        return self;
    }

    SBE::LogicalDevice *CommandPool::getParent()
    {
        return parent;
    }

    SBE::QueueCollection *CommandPool::getQueues()
    {
        return queues;
    }

    VkCommandPoolCreateInfo *CommandPool::getCreateInfo()
    {
        return pCreateInfo;
    }

    VkCommandBufferAllocateInfo *CommandPool::getAllocateInfo()
    {
        return pAllocateInfo;
    }

    std::vector<CommandBuffer *> CommandPool::getBuffers()
    {
        return buffers.getData();
    }

    // Operators
    //----------------------------------
    CommandBuffer *CommandPool::operator[](int rhs)
    {
        return (buffers[rhs]);
    }

    // Destructors
    //----------------------------------
    CommandPool::~CommandPool()
    {
        vkDestroyCommandPool(parent->getSelf(), self, parent->getHost()->getAllocationInfo());
    }

};