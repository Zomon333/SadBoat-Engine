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

#include <mutex>

#include "utilities/logging/startup_logger.hpp"
#include "vulkan/vulkan_result_lookup.hpp"
#include "vulkan/instance.hpp"
#include "vulkan/physical_device.hpp"
#include "vulkan/queue_family.hpp"
#include "vulkan/queue_family_collection.hpp"
#include "vulkan/extension_collection.hpp"

#include "vulkan/logical_device.hpp"

namespace SBE
{
    // Constructors
    //----------------------------------

    // Make a device, assume some info
    LogicalDevice::LogicalDevice(PhysicalDevice *parent, VkPhysicalDeviceFeatures *requiredFeatures, std::vector<VkLayerProperties> layersToEnable, std::vector<VkExtensionProperties> extToEnable)
    {
        allocationCount = 0;

        // Save device data passed in by parameters
        this->host = parent->getHost();
        this->parent = parent;

        // Enable all features by default.
        this->requiredFeatures = parent->getFeatures();

        // Start querying queue families to use for queues.
        QueueFamilyCollection deviceQueueFamilies = QueueFamilyCollection(parent);
        std::vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfos;

        // Find the optimal queue family
        this->optimalFamily = new QueueFamily(deviceQueueFamilies.getOptimal().second);

        this->queueCount = (optimalFamily->getProps()->queueCount > 16) ? 16 : optimalFamily->getProps()->queueCount;

        // Generate the struct to create as many queues within the family as possible.
        deviceQueueCreateInfos.emplace_back(
            VkDeviceQueueCreateInfo{
                VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, // VkStructureType             sType;
                nullptr,                                    // const void*                 pNext;
                0,                                          // VkDeviceQueueCreateFlags    flags;
                (unsigned int)(optimalFamily->getIndex()),  // uint32_t                    queueFamilyIndex;
                this->queueCount,                           // uint32_t                    queueCount;
                nullptr                                     // const float*                pQueuePriorities;
            });

        // Query all of the possible extensions for the physical device
        ExtensionCollection deviceExtensions = ExtensionCollection(parent);
        std::vector<char *> extensionsToEnable;
        // Sort through them all and choose any that are shared between our list of requested extensions
        for (unsigned int i = 0; i < deviceExtensions.getProps().size(); i++)
        {
            for (unsigned int j = 0; j < extToEnable.size(); j++)
            {
                if ((std::string(extToEnable[j].extensionName) == std::string(deviceExtensions.getProp(i).extensionName)) && (extToEnable[j].specVersion == deviceExtensions.getProp(i).specVersion))
                {
                    extensionsToEnable.emplace_back(extToEnable[j].extensionName);
                }
            }
        }

        // Pass the info we need to create the device to the struct
        this->creationInfo = new VkDeviceCreateInfo{
            VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,          // VkStructureType                      sType;
            nullptr,                                       // const void*                          pNext;
            0, /*Reserved for future use*/                 // VkDeviceCreateFlags                  flags;
            (unsigned int)(deviceQueueCreateInfos.size()), // uint32_t                             queueCreateInfoCount;
            deviceQueueCreateInfos.data(),                 // const VkDeviceQueueCreateInfo*       pQueueCreateInfos;
            0, /*Deprecated*/                              // uint32_t                             enabledLayerCount;
            nullptr, /*Deprecated*/                        // const char* const*                   ppEnabledLayerNames;
            (unsigned int)(extensionsToEnable.size()),     // uint32_t                             enabledExtensionCount;
            extensionsToEnable.data(),                     // const char* const*                   ppEnabledExtensionNames;
            this->requiredFeatures                         // const VkPhysicalDeviceFeatures*      pEnabledFeatures;
        };

        auto result = vkCreateDevice(
            this->parent->getDevice(),
            this->creationInfo,
            this->host->getAllocationInfo(),
            &self);

        std::stringstream tmpstream;
        tmpstream << "LogicalDevice created with result: " << VkResultLookup(result);
        SBE::log->debug(tmpstream.str());
    }

    // Make a device, assume no info
    LogicalDevice::LogicalDevice(PhysicalDevice *parent, VkDeviceCreateInfo *creationInfo, VkPhysicalDeviceFeatures *requiredFeatures)
    {
        allocationCount = 0;

        this->host = parent->getHost();
        this->creationInfo = creationInfo;
        this->requiredFeatures = parent->getFeatures();

        // this->creationInfo->pQueueCreateInfos->queueCount = (this->creationInfo->pQueueCreateInfos->queueCount>16) ? 16 : this->creationInfo->pQueueCreateInfos->queueCount;
        this->queueCount = creationInfo->pQueueCreateInfos->queueCount;

        // Todo: Add code for requiredFeatures to be checked against parent's features

        auto result = vkCreateDevice(parent->getDevice(), creationInfo, host->getAllocationInfo(), &self);
        std::stringstream info;
        info << "LogicalDevice created with result: " << VkResultLookup(result);
        SBE::log->info(&info);
    }

    // Deconstructors
    //----------------------------------

    LogicalDevice::~LogicalDevice()
    {
        vkDeviceWaitIdle(self);
        vkDestroyDevice(self, host->getAllocationInfo());
    }

    // Mutators
    //----------------------------------

    void LogicalDevice::update()
    {
        parent->update();
        host = parent->getHost();
    }

    void LogicalDevice::setParent(SBE::PhysicalDevice *parent)
    {
        this->parent = parent;
    }

    void LogicalDevice::setHost(SBE::Instance *host)
    {
        this->host = host;
    }

    void LogicalDevice::setCreationInfo(VkDeviceCreateInfo *creationInfo)
    {
        this->creationInfo = creationInfo;
    }

    void LogicalDevice::setRequiredFeats(VkPhysicalDeviceFeatures *requiredFeats)
    {
        this->requiredFeatures = requiredFeats;
    }

    unsigned int LogicalDevice::incAllocs()
    {
        allocationMod.lock();
        allocationCount += 1;
        allocationMod.unlock();
        return allocationCount;
    }

    unsigned int LogicalDevice::decAllocs()
    {
        allocationMod.lock();
        allocationCount -= 1;
        allocationMod.unlock();
        return allocationCount;
    }

    // Accessors
    //----------------------------------

    SBE::PhysicalDevice *LogicalDevice::getParent()
    {
        return parent;
    }

    SBE::Instance *LogicalDevice::getHost()
    {
        return host;
    }

    VkDevice LogicalDevice::getSelf()
    {
        return self;
    }

    VkDeviceCreateInfo *LogicalDevice::getCreationInfo()
    {
        return creationInfo;
    }

    VkPhysicalDeviceFeatures *LogicalDevice::getRequiredFeats()
    {
        return requiredFeatures;
    }

    SBE::QueueFamily *LogicalDevice::getOptimalQueueFam()
    {
        return optimalFamily;
    }

    unsigned int LogicalDevice::getQueueCount()
    {
        return queueCount;
    }

    unsigned int *LogicalDevice::getQueueCountArray()
    {
        unsigned int *array = new unsigned int[queueCount];
        for (unsigned int i = 0; i < queueCount; i++)
        {
            array[i] = i;
        }
        return array;
    }
};