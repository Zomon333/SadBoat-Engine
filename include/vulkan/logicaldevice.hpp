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
#ifndef LOGICAL_DEVICE_H
#define LOGICAL_DEVICE_H

#include "sb-engine.hpp"
#include "./vulkan/vulkan.hpp"

#include "./instance.hpp"
#include "./physicaldevice.hpp"


using namespace std;

namespace SBE
{
    // LogicalDevice: Graphics
    // Vulkan wrapper for the logical device used for rendering
    class LogicalDevice
    {
    private:
        Instance* host;
        PhysicalDevice* parent;

        mutex allocationMod;
        int allocationCount;

        QueueFamily* optimalFamily;

        VkPhysicalDeviceFeatures* requiredFeatures;

        VkDeviceCreateInfo* creationInfo;
        VkDevice self;

    public:
        // Constructors
        //----------------------------------

        // Make a device, assume some info
        LogicalDevice(PhysicalDevice* parent, VkPhysicalDeviceFeatures* requiredFeatures=nullptr, vector<VkLayerProperties> layersToEnable=vector<VkLayerProperties>(), vector<VkExtensionProperties> extToEnable=vector<VkExtensionProperties>())
        {
            allocationCount=0;

            // Save device data passed in by parameters
            this->host=parent->getHost();
            this->parent=parent;
            
            // Enable all features by default.
            this->requiredFeatures = parent->getFeatures();

            // Start querying queue families to use for queues.
            QueueFamilyCollection deviceQueueFamilies = QueueFamilyCollection(parent);
            vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfos;

            // Find the optimal queue family
            this->optimalFamily = new QueueFamily(deviceQueueFamilies.getOptimal().second);

            // Generate the struct to create as many queues within the family as possible.
            deviceQueueCreateInfos.emplace_back(
                VkDeviceQueueCreateInfo{
                    VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,                     // VkStructureType             sType;
                    nullptr,                                                        // const void*                 pNext;
                    0,                                                              // VkDeviceQueueCreateFlags    flags;
                    (unsigned int)(optimalFamily->getIndex()),                            // uint32_t                    queueFamilyIndex;
                    (optimalFamily->getProps()->queueCount>16) ? 16 : optimalFamily->getProps()->queueCount,                    // uint32_t                    queueCount;
                    nullptr                                                         // const float*                pQueuePriorities;
                }
            );

            
            // Query all of the possible extensions for the physical device
            ExtensionCollection deviceExtensions = ExtensionCollection(parent);
            vector<char*> extensionsToEnable;
            // Sort through them all and choose any that are shared between our list of requested extensions
            for(int i=0; i<deviceExtensions.getProps().size(); i++)
            {
                for(int j=0; j<extToEnable.size(); j++)
                {
                    if((extToEnable[j].extensionName==deviceExtensions.getProp(i).extensionName) && (extToEnable[j].specVersion==deviceExtensions.getProp(i).specVersion))
                    {
                        extensionsToEnable.emplace_back(extToEnable[j].extensionName);
                    }
                }
            }

            // Pass the info we need to create the device to the struct
            this->creationInfo = new VkDeviceCreateInfo{
                    VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,                                                   // VkStructureType                      sType;
                    nullptr,                                                                                // const void*                          pNext;
                    0, /*Reserved for future use*/                                                          // VkDeviceCreateFlags                  flags;
                    (unsigned int)(deviceQueueCreateInfos.size()),                                          // uint32_t                             queueCreateInfoCount;
                    deviceQueueCreateInfos.data(),                                                          // const VkDeviceQueueCreateInfo*       pQueueCreateInfos;
                    0, /*Deprecated*/                                                                       // uint32_t                             enabledLayerCount;
                    nullptr, /*Deprecated*/                                                                 // const char* const*                   ppEnabledLayerNames;
                    (unsigned int)(extensionsToEnable.size()),                                              // uint32_t                             enabledExtensionCount;
                    extensionsToEnable.data(),                                                              // const char* const*                   ppEnabledExtensionNames;
                    this->requiredFeatures                                                                  // const VkPhysicalDeviceFeatures*      pEnabledFeatures;
            };



            auto result = vkCreateDevice(
                this->parent->getDevice(),
                this->creationInfo,
                this->host->getAllocationInfo(),
                &self
            );
            cout<<"LogicalDevice created with result: "<<VkResultLookup(result)<<endl;
        }

        // Make a device, assume no info
        LogicalDevice(PhysicalDevice* parent, VkDeviceCreateInfo* creationInfo, VkPhysicalDeviceFeatures* requiredFeatures={})
        {
            allocationCount=0;

            this->host=parent->getHost();
            this->creationInfo=creationInfo;
            this->requiredFeatures=parent->getFeatures();

            // Todo: Add code for requiredFeatures to be checked against parent's features

            vkCreateDevice(parent->getDevice(), creationInfo, host->getAllocationInfo(), &self);
        }

        // Deconstructors
        //----------------------------------

        ~LogicalDevice()
        {
            vkDeviceWaitIdle(self);
            vkDestroyDevice(self, host->getAllocationInfo());
        }

        // Mutators
        //----------------------------------

        void update()
        {
            parent->update();
            host=parent->getHost();

        }

        void setParent(auto parent) { this->parent=parent; }
        void setHost(auto host) { this->host=host; }
        void setCreationInfo(auto creationInfo) { this->creationInfo=creationInfo; }
        void setRequiredFeats(auto requiredFeats) { this->requiredFeatures=requiredFeats; }

        int incAllocs()
        {
            allocationMod.lock();
            allocationCount+=1;
            allocationMod.unlock();
            return allocationCount;
        }
        int decAllocs()
        {
            allocationMod.lock();
            allocationCount-=1;
            allocationMod.unlock();
            return allocationCount;
        }

        // Accessors
        //----------------------------------

        auto getParent() { return parent; }
        auto getHost() { return host; }
        auto getSelf() { return self; }
        auto getCreationInfo() { return creationInfo; }
        auto getRequiredFeats() { return requiredFeatures; }
        auto getOptimalQueueFam() { return optimalFamily; }


        template <class Function>
        Function getFunc(const char* pName)
        {
            return ((Function)(vkGetDeviceProcAddr(self, pName)));
        }
    };
};
#endif