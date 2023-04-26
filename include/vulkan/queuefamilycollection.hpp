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
#ifndef QUEUE_FAMILY_COLLECTION_H
#define QUEUE_FAMILY_COLLECTION_H

#include "sb-engine.hpp"
#include "./vulkan/vulkan.hpp"

using namespace std;

namespace SBE
{
    // QueueFamilyCollection: Graphics
    // Generalized container for querying and storage of QueueFamilies from a given PhysicalDevice
    class QueueFamilyCollection
    {
    private:
        PhysicalDevice* parent;

        uint32_t queueFamPropCount;
        vector<QueueFamily> selfQueueFamilies;

    public:
        //Constructors    
        //----------------------------------

        //Initialize a collection of QueueFamilies based on the parent device
        QueueFamilyCollection(PhysicalDevice* parent)
        {
            this->parent=parent;

            this->update();
        }

        //Mutators
        //----------------------------------

        //Update the collection of QueueFamilies based on the parent device
        void update()
        {
            vkGetPhysicalDeviceQueueFamilyProperties(parent->getDevice(), &queueFamPropCount, nullptr);            
            
            VkQueueFamilyProperties* tmp = new VkQueueFamilyProperties[queueFamPropCount];

            vkGetPhysicalDeviceQueueFamilyProperties(parent->getDevice(), &queueFamPropCount, tmp);

            selfQueueFamilies.clear();
            for(int i = 0; i<queueFamPropCount; i++)
            {
                selfQueueFamilies.push_back(QueueFamily(parent, &(tmp[i])));
            }
        }

        //Accessors
        //----------------------------------

        // Returns parent device
        auto getParent(){ return parent; }
        
        // Returns number of queue families
        auto getFamilyCount() { return queueFamPropCount; }
        
        // Returns queue family from backing
        auto getQueueFamily(int which) { return selfQueueFamilies[which]; }
        auto describeQueue(int which) { return selfQueueFamilies[which].getProps(); }
        
        auto getOptimal() 
        {
            return pair<int,QueueFamily>(0, selfQueueFamilies[0]);
        }

    };

};
#endif

