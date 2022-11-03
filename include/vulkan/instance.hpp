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
    class Instance
    {
    private:
        VkInstance vulkanInstance;
        VkInstanceCreateInfo creationInfo;
        VkAllocationCallbacks allocationInfo;

    public:
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

            vkCreateInstance(&creationInfo, &allocationInfo, &vulkanInstance);
        }

        Instance(VkInstanceCreateInfo creationInfo, VkAllocationCallbacks allocationInfo)
        {
            this->creationInfo=creationInfo;
            this->allocationInfo=allocationInfo;

            vkCreateInstance(&(this->creationInfo),&(this->allocationInfo),&vulkanInstance);
        }
        
    };
};
#endif