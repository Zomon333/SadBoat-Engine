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
#ifndef QUEUE_FAMILY_H
#define QUEUE_FAMILY_H

#include "sb-engine.hpp"
#include "./vulkan/vulkan.hpp"

using namespace std;

namespace SBE
{
    // QueueFamily: Graphics
    // Class for generalized containing of properties of a given QueueFamily
class QueueFamily
    {
    private:
        PhysicalDevice* parent;

        VkQueueFamilyProperties* selfProps;

    public:
        // Constructors 
        //----------------------------------
    
        //Construct a QueueFamily given some parent and some properties
        QueueFamily(PhysicalDevice* parent, VkQueueFamilyProperties* selfProps)
        {
            this->parent=parent;
            this->selfProps=selfProps;
        }

        // Accessors
        //----------------------------------

        // Get the QueueFamily's parent
        auto getParent() { return parent; }

        // Get the QueueFamily's properties
        auto getProps() { return selfProps; }
    };
};
#endif