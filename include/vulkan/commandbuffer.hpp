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
#ifndef COMMANDBUFFER_H
#define COMMANDBUFFER_H

#include "sb-engine.hpp"
#include "./queuecollection.hpp"

using namespace std;

namespace SBE
{
    class CommandBuffer
    {
    private:
        LogicalDevice* parent;

        QueueCollection* queues;

        VkCommandBuffer self;
    public:
        // Constructors
        //----------------------------------
        CommandBuffer(LogicalDevice* parent, QueueCollection* queues, VkCommandBuffer self)
        {
            this->parent=parent;
            this->queues=queues;
            this->self=self;
        }
        // Mutators
        //----------------------------------

        // Accessors
        //----------------------------------

        // Operators
        //----------------------------------

        // Destructors
        //----------------------------------

        
    };
};
#endif