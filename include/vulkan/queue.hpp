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
#ifndef QUEUE_H
#define QUEUE_H

#include "sb-engine.hpp"

#include "./logicaldevice.hpp"
#include "./queuefamily.hpp"

using namespace std;

namespace SBE
{
    class Queue
    {
    private:
        LogicalDevice* parent;
        QueueFamily* family;
        int queueNumber;

        VkQueue self;
        mutex queueSync;

        LogHandle* log;

    public:
        // Constructors
        //----------------------------------
        Queue(LogicalDevice* parent, QueueFamily* family, int queueNumber)
        {
            this->parent=parent;
            this->family=family;
            this->queueNumber=queueNumber;

            this->log = logger->allocateHandle(0b11111);

            vkGetDeviceQueue(parent->getSelf(), family->getIndex(), queueNumber, &self);
        }

        // Mutators
        //----------------------------------

        // Do something here
        VkResult submitWork()
        {
            queueSync.lock();
            // Do our something!

            queueSync.unlock();

            // Temporary pending actual code here
            return VK_SUCCESS;
        }
        bool isBusy()
        {
            return false;
        }

        // Accessors
        //----------------------------------
        auto getParent(){return parent;}
        auto getFamily(){return family;}
        auto getQueueNumber(){return queueNumber;}

        auto getSelf(){return self;}

        // Operators
        //----------------------------------

        // Destructors
        //----------------------------------
        ~Queue()
        {
            // Ensure the queue has no work to be done

            // Free the log handle
            logger->freeHandle(this->log->getID());
            this->log=nullptr;
        }
        
    };
};
#endif