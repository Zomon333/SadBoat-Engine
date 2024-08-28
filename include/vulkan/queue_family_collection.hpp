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
#ifndef QUEUE_FAMILY_COLLECTION_H
#define QUEUE_FAMILY_COLLECTION_H

#include "./vulkan/vulkan.hpp"

namespace SBE
{
    // QueueFamilyCollection: Graphics
    // Generalized container for querying and storage of QueueFamilies from a given PhysicalDevice
    class QueueFamilyCollection
    {
    private:
        PhysicalDevice *parent;

        uint32_t queueFamPropCount;
        std::vector<QueueFamily> selfQueueFamilies;

    public:
        // Constructors
        //----------------------------------

        // Initialize a collection of QueueFamilies based on the parent device
        QueueFamilyCollection(PhysicalDevice *parent);

        // Mutators
        //----------------------------------

        // Update the collection of QueueFamilies based on the parent device
        void update();

        // Accessors
        //----------------------------------

        // Returns parent device
        PhysicalDevice *getParent();

        // Returns number of queue families
        unsigned int getFamilyCount();

        // Returns queue family from backing
        QueueFamily getQueueFamily(int which);

        VkQueueFamilyProperties *describeQueue(int which);

        std::pair<int, QueueFamily> getOptimal();
    };
};
#endif
