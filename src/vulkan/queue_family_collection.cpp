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
#include "vulkan/physical_device.hpp"
#include "vulkan/queue_family.hpp"
#include "vulkan/queue_family_collection.hpp"

namespace SBE
{
    // Constructors
    //----------------------------------

    // Initialize a collection of QueueFamilies based on the parent device
    QueueFamilyCollection::QueueFamilyCollection(PhysicalDevice *parent)
    {
        this->parent = parent;

        this->update();
    }

    // Mutators
    //----------------------------------

    // Update the collection of QueueFamilies based on the parent device
    void QueueFamilyCollection::update()
    {
        vkGetPhysicalDeviceQueueFamilyProperties(parent->getDevice(), &queueFamPropCount, nullptr);

        VkQueueFamilyProperties *tmp = new VkQueueFamilyProperties[queueFamPropCount];

        vkGetPhysicalDeviceQueueFamilyProperties(parent->getDevice(), &queueFamPropCount, tmp);

        selfQueueFamilies.clear();
        for (unsigned int i = 0; i < queueFamPropCount; i++)
        {
            selfQueueFamilies.push_back(QueueFamily(parent, (unsigned int)(i), &(tmp[i])));
        }
    }

    // Accessors
    //----------------------------------

    // Returns parent device
    PhysicalDevice *QueueFamilyCollection::getParent()
    {
        return parent;
    }

    // Returns number of queue families
    unsigned int QueueFamilyCollection::getFamilyCount()
    {
        return queueFamPropCount;
    }

    // Returns queue family from backing
    QueueFamily QueueFamilyCollection::getQueueFamily(int which)
    {
        return selfQueueFamilies[which];
    }

    VkQueueFamilyProperties *QueueFamilyCollection::describeQueue(int which)
    {
        return selfQueueFamilies[which].getProps();
    }

    std::pair<int, QueueFamily> QueueFamilyCollection::getOptimal()
    {
        std::pair<int, QueueFamily> result(0, selfQueueFamilies[0]);

        for (unsigned int i = 0; i < selfQueueFamilies.size(); i++)
        {
            if (
                (selfQueueFamilies[i].getProps()->queueFlags & VK_QUEUE_GRAPHICS_BIT) &&
                (selfQueueFamilies[i].getProps()->queueFlags & VK_QUEUE_TRANSFER_BIT) &&
                (selfQueueFamilies[i].getProps()->queueFlags & VK_QUEUE_COMPUTE_BIT) &&
                (selfQueueFamilies[i].getProps()->queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) &&
                (selfQueueFamilies[i].getProps()->queueCount >= result.second.getProps()->queueCount))
            {
                result = std::pair<int, QueueFamily>(i, selfQueueFamilies[i]);
            }
        }

        std::stringstream tmpstream;
        tmpstream << "Choosing queueFamily number " << result.first << ", which supports ";
        tmpstream << result.second.getProps()->queueCount << " queues.";
        SBE::log->debug(&tmpstream);

        tmpstream<<"QueueFamily Capabilities: ";
        tmpstream<<"VK_QUEUE_GRAPHICS_BIT: "<<(((result.second.getProps()->queueFlags & VK_QUEUE_GRAPHICS_BIT) > 0) ? "TRUE" : "FALSE")<<", ";
        tmpstream<<"VK_QUEUE_COMPUTE_BIT: "<<(((result.second.getProps()->queueFlags & VK_QUEUE_COMPUTE_BIT) > 0) ? "TRUE" : "FALSE")<<", ";
        tmpstream<<"VK_QUEUE_TRANSFER_BIT: "<<(((result.second.getProps()->queueFlags & VK_QUEUE_TRANSFER_BIT) > 0) ? "TRUE" : "FALSE")<<", ";
        tmpstream<<"VK_QUEUE_SPARSE_BINDING_BIT: "<<(((result.second.getProps()->queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) > 0) ? "TRUE" : "FALSE")<<", ";
        tmpstream<<"VK_QUEUE_PROTECTED_BIT: "<<(((result.second.getProps()->queueFlags & VK_QUEUE_PROTECTED_BIT) > 0) ? "TRUE" : "FALSE");
        SBE::log->debug(&tmpstream);

        return result;
    }
};
