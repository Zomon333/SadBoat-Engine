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
#ifndef COMMANDPOOL_H
#define COMMANDPOOL_H

#include "vulkan.hpp"

#include "vulkan/command_buffer.hpp"
#include "utilities/manager.hpp"

namespace SBE
{
    class CommandPool
    {
    private:
        VkCommandPool self;
        VkCommandPoolCreateInfo *pCreateInfo;
        VkCommandBufferAllocateInfo *pAllocateInfo;

        LogicalDevice *parent;
        VkCommandBuffer *bufferBacking;
        Manager<CommandBuffer> buffers;

        QueueCollection *queues;

        void allocate(unsigned int bufferCount);

    public:
        // Constructors
        //----------------------------------
        CommandPool(LogicalDevice *parent, VkCommandPoolCreateInfo *pCreateInfo, VkCommandBufferAllocateInfo *pAllocateInfo, unsigned int bufferCount = 0);

        CommandPool(LogicalDevice *parent, bool isTransient = true, bool isResettable = true, unsigned int bufferCount = 0);

        // Mutators
        //----------------------------------
        void freeBuffer(int id);
        void freeBuffers(int *ids, int count = 1);

        // Accessors
        //----------------------------------
        VkCommandPool getSelf();
        SBE::LogicalDevice *getParent();
        SBE::QueueCollection *getQueues();

        VkCommandPoolCreateInfo *getCreateInfo();
        VkCommandBufferAllocateInfo *getAllocateInfo();

        std::vector<CommandBuffer *> getBuffers();

        // Operators
        //----------------------------------
        CommandBuffer *operator[](int rhs);

        // Destructors
        //----------------------------------
        ~CommandPool();
    };
};
#endif