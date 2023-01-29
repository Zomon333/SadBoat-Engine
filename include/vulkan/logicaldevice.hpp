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
    class LogicalDevice
    {
    private:
        Instance* host;
        PhysicalDevice* parent;

        VkPhysicalDeviceFeatures* requiredFeatures;

        VkDeviceCreateInfo* creationInfo;
        VkDevice self;
    public:
        // Constructors

        // Make a device, assume some info
        LogicalDevice(PhysicalDevice* parent, VkPhysicalDeviceFeatures* requiredFeatures=nullptr, vector<VkLayerProperties> layersToEnable=vector<VkLayerProperties>(), vector<VkExtensionProperties> extToEnable=vector<VkExtensionProperties>())
        {
            this->parent=parent;
            this->host=parent->getHost();
            this->creationInfo=new VkDeviceCreateInfo;
            this->requiredFeatures=requiredFeatures;

            // We need to initialize our LogicalDevice, so we're setting up some boilerplate structs to store the info for the constructor.
            // VkDeviceCreateInfo initInfo;
            creationInfo->sType=VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            creationInfo->pNext=nullptr;
            creationInfo->flags=0;

            auto queueFams = QueueFamilyCollection(parent);
            auto optimalFam = queueFams.getOptimal().second;

            // We need some info on the Queues we're initializing inside of it, too.
            VkDeviceQueueCreateInfo initDevQueue;
            initDevQueue.sType=VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            initDevQueue.pNext=nullptr;
            initDevQueue.flags=0;

            // Find some queue family in the collection of queue families.
            // Ideally, we'd do more work here to find the *right* queue family but for this boilerplate
            // we're just going to assume the first queue fam is fine.

            // Specifies the family of the queues we want to create
            // Set to the index in the array of queue families stored in QueueFamilyCollection
            initDevQueue.queueFamilyIndex=queueFams.getOptimal().first;

            /*
            Render categories:
                -Player
                -Projectiles
                -Enemies
                -UI
                -Background
                -Other
            */
            initDevQueue.queueCount=6;

            // An optional pointer to an array of floats representing priority of work submitted to each of the queues. These values are normalized.
            // Use this later for prioritizing rendering for high mobility objects and the player model
            // Setting this to nullptr has the device treat every queue the same
            initDevQueue.pQueuePriorities=nullptr;
            
            creationInfo->queueCreateInfoCount=6;
            VkDeviceQueueCreateInfo infos[6];
            for(int i=0; i<1; i++)
            {
                infos[i]=initDevQueue;
            }

            creationInfo->pQueueCreateInfos=infos;

            // Enable the layers named by the parameter
            creationInfo->enabledLayerCount=layersToEnable.size();
            char** layArr = new char*[layersToEnable.size()];
            for(int i = 0; i<layersToEnable.size(); i++)
            {
                layArr[i]=layersToEnable[i].layerName;
            }
            creationInfo->ppEnabledLayerNames=layArr;


            // Enable the extensions named by the parameter
            creationInfo->enabledExtensionCount=extToEnable.size();
            char** strArr = new char*[extToEnable.size()];
            for(int i = 0; i<extToEnable.size(); i++)
            {
                strArr[i]=extToEnable[i].extensionName;
            }
            creationInfo->ppEnabledExtensionNames=strArr;

            // We generally don't want this to be nullptr. This is just as a proof of concept.
            creationInfo->pEnabledFeatures=this->requiredFeatures;

            vkCreateDevice(parent->getDevice(), creationInfo, parent->getHost()->getAllocationInfo(), &self);
        }

        // Make a device, assume no info
        LogicalDevice(PhysicalDevice* parent, VkDeviceCreateInfo* creationInfo, VkPhysicalDeviceFeatures* requiredFeatures={})
        {
            this->host=parent->getHost();
            this->creationInfo=creationInfo;
            this->requiredFeatures=requiredFeatures;

            // Todo: Add code for requiredFeatures to be checked against parent's features

            vkCreateDevice(parent->getDevice(), creationInfo, host->getAllocationInfo(), &self);
        }

        // Mutators

        void update()
        {
            parent->update();
            host=parent->getHost();

        }

        void setParent(auto parent) { this->parent=parent; }
        void setHost(auto host) { this->host=host; }
        void setCreationInfo(auto creationInfo) { this->creationInfo=creationInfo; }
        void setRequiredFeats(auto requiredFeats) { this->requiredFeatures=requiredFeats; }

        // Accessors
        auto getParent() { return parent; }
        auto getHost() { return host; }
        auto getSelf() { return self; }
        auto getCreationInfo() { return creationInfo; }
        auto getRequiredFeats() { return requiredFeatures; }
    };
};
#endif