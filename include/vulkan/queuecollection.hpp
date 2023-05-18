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
#ifndef QUEUE_COLLECTION_H
#define QUEUE_COLLECTION_H

#include "sb-engine.hpp"
#include "./vulkan/vulkan.hpp"

#include "./logicaldevice.hpp"
#include "./queue.hpp"
#include "./queuefamily.hpp"

using namespace std;

namespace SBE
{
    class QueueCollection
    {
    private:
        LogicalDevice* parent;
        QueueFamily* queueFamily;
        int queueCount;

        LogHandle* log;

        vector<Queue*> queues;

    public:
        // Constructors
        //----------------------------------
        QueueCollection(LogicalDevice* parent, QueueFamily* queueFamily, int queueCount)
        {
            this->parent=parent;
            this->queueFamily=queueFamily;
            this->queueCount=queueCount;

            this->log = logger->allocateHandle(0b11111);

            queues.resize(queueCount);

            for(int i=0; i<queueCount; i++)
            {
                // Construct a Queue
                queues[i] = new Queue(parent, queueFamily, i);
            }

            this->log->debug(string("Initializing QueueCollection with a total of ").append(to_string(queueCount)).append(" queues."));
        }


        // Mutators
        //----------------------------------


        // Accessors
        //----------------------------------
        auto getParent(){return parent;}
        auto getFamily(){return queueFamily;}
        auto getCount(){return queueCount;}

        auto getQueues(){return queues;}
        auto getQueue(int i){return queues.at(i);}



        // Operators
        //----------------------------------
        Queue* operator[](int rhs)
        {
            return queues.at(rhs);
        }

        // Destructors
        //----------------------------------

        
    };
};
#endif