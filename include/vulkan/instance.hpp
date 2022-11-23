/*
Copyright 2022 Dagan Poulin, Justice Guillory
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
#ifndef INSTANCE_H
#define INSTANCE_H

#include "sb-engine.hpp"
#include "./vulkan/vulkan.hpp"

using namespace std;

namespace SBE
{
    //Instance: Graphics
    //Helper class so it's easier to instantiate a Vulkan Instance
    //
    class Instance
    {
    private:
        VkResult creationResult;

        VkInstance vulkanInstance;
        VkInstanceCreateInfo* creationInfo;
        VkAllocationCallbacks* allocationInfo;

    public:
        //Constructors
        //----------------------------------
        Instance()
        {
            //creationInfo.sType=;
            //creationInfo.pNext=;
            //creationInfo.flags=;
            //creationInfo.pApplicationInfo=;
            //creationInfo.enabledLayerCount=;
            //creationInfo.ppEnabledLayerNames=;
            //creationInfo.enabledExtensionCount=;
            //creationInfo.ppEnabledExtensionNames=;

            //allocationInfo.pfnAllocation=;
            //allocationInfo.pfnFree=;
            //allocationInfo.pfnInternalAllocation=;
            //allocationInfo.pfnInternalFree=;
            //allocationInfo.pfnReallocation=;
            //allocationInfo.pUserData=;

            creationResult = vkCreateInstance(creationInfo, allocationInfo, &vulkanInstance);
        }

        Instance(VkInstanceCreateInfo creationInfo, VkAllocationCallbacks allocationInfo)
        {
            *(this->creationInfo)=creationInfo;
            *(this->allocationInfo)=allocationInfo;

            creationResult = vkCreateInstance(this->creationInfo, this->allocationInfo, &vulkanInstance);
        }

        Instance(VkInstanceCreateInfo* creationInfo, VkAllocationCallbacks allocationInfo)
        {
            this->creationInfo=creationInfo;
            *(this->allocationInfo)=allocationInfo;

            creationResult = vkCreateInstance(this->creationInfo, this->allocationInfo, &vulkanInstance);
        }

        Instance(VkInstanceCreateInfo creationInfo, VkAllocationCallbacks* allocationInfo)
        {
            *(this->creationInfo)=creationInfo;
            this->allocationInfo=allocationInfo;

            creationResult = vkCreateInstance(this->creationInfo, this->allocationInfo, &vulkanInstance);
        }

        Instance(VkInstanceCreateInfo* creationInfo, VkAllocationCallbacks* allocationInfo)
        {
            this->creationInfo=creationInfo;
            this->allocationInfo=allocationInfo;

            creationResult = vkCreateInstance(this->creationInfo, this->allocationInfo, &vulkanInstance);
        }

        //Destructors
        //----------------------------------
        ~Instance()
        {
            delete creationInfo;
            delete allocationInfo;

            /*
                Todo: Learn how to destroy a Vulkan Instance and put it here
            */
        }

        //Accessors
        //----------------------------------
        VkInstance* getInstance()
        {
            return &(this->vulkanInstance);
        }

        VkInstanceCreateInfo* getCreationInfo()
        {
            return this->creationInfo;
        }

        VkAllocationCallbacks* getAllocationInfo()
        {
            return this->allocationInfo;
        }

        VkResult getResult()
        {
            return creationResult;
        }

        //Mutators
        //----------------------------------
        void setInstance(VkInstance newInstance)
        {
            this->vulkanInstance=newInstance;
        }
        
        void setCreationInfo(VkInstanceCreateInfo* newInfo)
        {
            this->creationInfo=newInfo;
        }
       
        void setAllocationInfo(VkAllocationCallbacks* newAllocators)
        {
            this->allocationInfo=newAllocators;
        }
       
       
    };
};
#endif