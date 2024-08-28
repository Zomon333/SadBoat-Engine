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
#ifndef LOGICAL_DEVICE_H
#define LOGICAL_DEVICE_H


#include "./vulkan/vulkan.hpp"



#include "vulkan/instance.hpp"
#include "vulkan/physical_device.hpp"
#include "vulkan/queue_family.hpp"
#include "vulkan/queue_family_collection.hpp"
#include "vulkan/extension_collection.hpp"

namespace SBE
{
    // LogicalDevice: Graphics
    // Vulkan wrapper for the logical device used for rendering
    class LogicalDevice
    {
    private:
        Instance *host;
        PhysicalDevice *parent;

        std::mutex allocationMod;
        unsigned int allocationCount;

        QueueFamily *optimalFamily;
        unsigned int queueCount;

        VkPhysicalDeviceFeatures *requiredFeatures;

        VkDeviceCreateInfo *creationInfo;
        VkDevice self;

    public:
        // Constructors
        //----------------------------------

        // Make a device, assume some info
        LogicalDevice(PhysicalDevice *parent, VkPhysicalDeviceFeatures *requiredFeatures = nullptr, std::vector<VkLayerProperties> layersToEnable = std::vector<VkLayerProperties>(), std::vector<VkExtensionProperties> extToEnable = std::vector<VkExtensionProperties>());

        // Make a device, assume no info
        LogicalDevice(PhysicalDevice *parent, VkDeviceCreateInfo *creationInfo, VkPhysicalDeviceFeatures *requiredFeatures = {});

        // Deconstructors
        //----------------------------------

        ~LogicalDevice();

        // Mutators
        //----------------------------------

        void update();

        void setParent(PhysicalDevice *parent);
        void setHost(Instance *host);
        void setCreationInfo(VkDeviceCreateInfo *creationInfo);
        void setRequiredFeats(VkPhysicalDeviceFeatures *requiredFeats);

        unsigned int incAllocs();
        unsigned int decAllocs();

        // Accessors
        //----------------------------------

        SBE::PhysicalDevice *getParent();
        SBE::Instance *getHost();
        VkDevice getSelf();
        VkDeviceCreateInfo *getCreationInfo();
        VkPhysicalDeviceFeatures *getRequiredFeats();
        SBE::QueueFamily *getOptimalQueueFam();
        unsigned int getQueueCount();
        unsigned int *getQueueCountArray();

        template <class Function>
        Function getFunc(const char *pName)
        {
            return ((Function)(vkGetDeviceProcAddr(self, pName)));
        }
    };
};
#endif