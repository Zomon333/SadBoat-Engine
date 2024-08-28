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
#ifndef INSTANCE_H
#define INSTANCE_H

#include "./vulkan/vulkan.hpp"
#include <vector>

namespace SBE
{
    // Instance: Graphics
    // Helper class so it's easier to instantiate a Vulkan Instance
    class Instance
    {
    private:
        VkResult creationResult;

        VkInstance *vulkanInstance;
        VkInstanceCreateInfo *creationInfo;
        VkAllocationCallbacks *allocationInfo;

    public:
        // Constructors
        //----------------------------------
        Instance(std::vector<VkExtensionProperties> extToEnable = std::vector<VkExtensionProperties>());

        Instance(VkInstanceCreateInfo creationInfo, VkAllocationCallbacks allocationInfo);

        Instance(VkInstanceCreateInfo *creationInfo, VkAllocationCallbacks allocationInfo);

        Instance(VkInstanceCreateInfo creationInfo, VkAllocationCallbacks *allocationInfo);

        Instance(VkInstanceCreateInfo *creationInfo, VkAllocationCallbacks *allocationInfo);

        // Destructors
        //----------------------------------
        ~Instance();

        // Accessors
        //----------------------------------
        VkInstance *getInstance();

        VkInstanceCreateInfo *getCreationInfo();

        VkAllocationCallbacks *getAllocationInfo();

        VkResult getResult();

        template <class Function = PFN_vkVoidFunction>
        Function getFunc(const char *pName)
        {
            return ((Function)(vkGetInstanceProcAddr(*(this->getInstance()), pName)));
        }

        // Mutators
        //----------------------------------
        void setInstance(VkInstance *newInstance);

        void setCreationInfo(VkInstanceCreateInfo *newInfo);

        void setAllocationInfo(VkAllocationCallbacks *newAllocators);
    };
};
#endif