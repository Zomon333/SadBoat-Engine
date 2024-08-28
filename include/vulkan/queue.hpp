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
#ifndef QUEUE_H
#define QUEUE_H

#include "vulkan/logical_device.hpp"
#include "vulkan/queue_family.hpp"

namespace SBE
{
    class Queue
    {
    private:
        LogicalDevice *parent;
        QueueFamily *family;
        int queueNumber;

        VkQueue self;
        std::mutex queueSync;

    public:
        LogHandle *log;

        // Constructors
        //----------------------------------
        Queue(LogicalDevice *parent, QueueFamily *family, int queueNumber);

        // Mutators
        //----------------------------------

        // Do something here
        VkResult submitWork();

        bool isBusy();

        // Accessors
        //----------------------------------
        LogicalDevice *getParent();
        QueueFamily *getFamily();
        int getQueueNumber();

        VkQueue getSelf();

        // Operators
        //----------------------------------

        // Destructors
        //----------------------------------
        ~Queue();
    };
};
#endif