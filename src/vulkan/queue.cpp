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

#include <mutex>

#include "utilities/logging/startup_logger.hpp"
#include "vulkan/logical_device.hpp"
#include "vulkan/queue_family.hpp"
#include "vulkan/queue.hpp"

namespace SBE
{
    // Constructors
    //----------------------------------
    Queue::Queue(LogicalDevice *parent, QueueFamily *family, int queueNumber)
    {
        this->parent = parent;
        this->family = family;
        this->queueNumber = queueNumber;

        this->log = logger->allocateHandle(0b11111);

        vkGetDeviceQueue(parent->getSelf(), family->getIndex(), queueNumber, &self);
    }

    // Mutators
    //----------------------------------

    // Do something here
    VkResult Queue::submitWork()
    {
        queueSync.lock();
        // Do our something!

        queueSync.unlock();

        // Temporary pending actual code here
        return VK_SUCCESS;
    }

    bool Queue::isBusy()
    {
        return false;
    }

    // Accessors
    //----------------------------------
    LogicalDevice *Queue::getParent()
    {
        return parent;
    }

    QueueFamily *Queue::getFamily()
    {
        return family;
    }

    int Queue::getQueueNumber()
    {
        return queueNumber;
    }

    VkQueue Queue::getSelf()
    {
        return self;
    }

    // Operators
    //----------------------------------

    // Destructors
    //----------------------------------
    Queue::~Queue()
    {
        // Ensure the queue has no work to be done

        // Free the log handle
        logger->freeHandle(this->log->getID());
        this->log = nullptr;
    }

};