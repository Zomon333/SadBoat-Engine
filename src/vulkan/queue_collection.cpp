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
#include "vulkan/logical_device.hpp"
#include "vulkan/queue.hpp"
#include "vulkan/queue_family.hpp"
#include "vulkan/queue_collection.hpp"

namespace SBE
{

    // Constructors
    //----------------------------------
    QueueCollection::QueueCollection(LogicalDevice *parent, QueueFamily *queueFamily, int queueCount)
    {
        this->parent = parent;
        this->queueFamily = queueFamily;
        this->queueCount = queueCount;

        this->log = logger->allocateHandle(0b11111);

        queues.resize(queueCount);

        for (int i = 0; i < queueCount; i++)
        {
            // Construct a Queue
            queues[i] = new Queue(parent, queueFamily, i);
        }

        std::stringstream result;
        result << "Initializing QueueCollection with a total of " << std::to_string(queueCount) << " queues.";
        this->log->debug(&result);
    }

    // Mutators
    //----------------------------------

    // Accessors
    //----------------------------------
    LogicalDevice *QueueCollection::getParent()
    {
        return parent;
    }

    QueueFamily *QueueCollection::getFamily()
    {
        return queueFamily;
    }

    int QueueCollection::getCount()
    {
        return queueCount;
    }

    std::vector<Queue *> QueueCollection::getQueues()
    {
        return queues;
    }

    Queue *QueueCollection::getQueue(int i)
    {
        return queues.at(i);
    }

    // Operators
    //----------------------------------
    Queue *QueueCollection::operator[](int rhs)
    {
        return queues.at(rhs);
    }

};