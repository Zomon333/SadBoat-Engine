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

#include <vector>

#include "utilities/logging/startup_logger.hpp"
#include "vulkan/instance.hpp"

namespace SBE
{
    // Constructors
    //----------------------------------
    Instance::Instance(std::vector<VkExtensionProperties> extToEnable)
    {
        creationInfo = new VkInstanceCreateInfo;

        // Tells Vulkan what type of Struct this is
        creationInfo->sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

        // Allows for a linked list of structs to be passed in case we need to extend the parameter list
        // We do not need to extend the parameter list, so it is nullptr
        creationInfo->pNext = nullptr;

        // Reserved for future use
        creationInfo->flags = 0;

        // Another struct describing our application. Optional.
        VkApplicationInfo *appInfo = new VkApplicationInfo();
        appInfo->sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; // What type of struct is this?
        appInfo->pNext = nullptr;                            // Do we need any extra parameters? (No)
        appInfo->pApplicationName = "SadBoat Engine";        // What is the name of the application?
        appInfo->applicationVersion = 0;                     // What is the version of the application?
        appInfo->pEngineName = "SadBoat Engine";             // What is the name of the engine?
        appInfo->engineVersion = 29;                         // What is the version of the engine?
        appInfo->apiVersion = VK_MAKE_VERSION(1, 0, 0);      // What version of Vulkan are we using?*/

        // Pointer to another struct describing our application. Optional, but highly recommended!
        creationInfo->pApplicationInfo = appInfo;

        // Number of instance layers we would like to enable
        creationInfo->enabledLayerCount = 0;
        // Names of instance layers we'll be enabling
        creationInfo->ppEnabledLayerNames = nullptr;

        creationInfo->enabledExtensionCount = extToEnable.size();
        char **strArr = new char *[extToEnable.size()];
        for (unsigned int i = 0; i < extToEnable.size(); i++)
        {
            strArr[i] = extToEnable[i].extensionName;
        }
        creationInfo->ppEnabledExtensionNames = strArr;

        /*
        allocationInfo is a pointer to a struct of memory-management callbacks used by our program.
        By setting it to nullptr, Vulkan will use it's own internal memory manager instead of our own.
        More info on memory management in Vulkan is in Chapter 2. For 0.02.9V, we are using the default memory model.
        */
        this->allocationInfo = nullptr;

        // allocationInfo->pfnAllocation=;
        // allocationInfo->pfnFree=;
        // allocationInfo->pfnInternalAllocation=;
        // allocationInfo->pfnInternalFree=;
        // allocationInfo->pfnReallocation=;
        // allocationInfo->pUserData=;

        vulkanInstance = new VkInstance;

        creationResult = vkCreateInstance(creationInfo, allocationInfo, vulkanInstance);

        if (creationResult != VK_SUCCESS)
        {
            SBE::log->critical("Vulkan instance did not initialize.");
            throw new std::runtime_error("Vulkan Instance did not initialize.");
        }
        else
        {
            SBE::log->info("Created Vulkan Instance.");
        }
        this->allocationInfo = nullptr;
    }

    Instance::Instance(VkInstanceCreateInfo creationInfo, VkAllocationCallbacks allocationInfo)
    {
        this->creationInfo = new VkInstanceCreateInfo();
        this->allocationInfo = new VkAllocationCallbacks();

        *(this->creationInfo) = creationInfo;
        *(this->allocationInfo) = allocationInfo;

        vulkanInstance = new VkInstance;
        creationResult = vkCreateInstance(this->creationInfo, this->allocationInfo, vulkanInstance);

        if (creationResult != VK_SUCCESS)
        {
            SBE::log->critical("Vulkan instance did not initialize.");
            throw new std::runtime_error("Vulkan Instance did not initialize.");
        }
        else
        {
            SBE::log->info("Created Vulkan Instance.");
        }
        // this->allocationInfo=nullptr;
    }

    Instance::Instance(VkInstanceCreateInfo *creationInfo, VkAllocationCallbacks allocationInfo)
    {
        this->creationInfo = creationInfo;
        this->allocationInfo = new VkAllocationCallbacks();
        *(this->allocationInfo) = allocationInfo;

        vulkanInstance = new VkInstance;
        creationResult = vkCreateInstance(this->creationInfo, this->allocationInfo, vulkanInstance);

        if (creationResult != VK_SUCCESS)
        {
            SBE::log->critical("Vulkan instance did not initialize.");
            throw new std::runtime_error("Vulkan Instance did not initialize.");
        }
        else
        {
            SBE::log->info("Created Vulkan Instance.");
        }
        // this->allocationInfo=nullptr;
    }

    Instance::Instance(VkInstanceCreateInfo creationInfo, VkAllocationCallbacks *allocationInfo)
    {
        this->creationInfo = new VkInstanceCreateInfo();
        *(this->creationInfo) = creationInfo;
        this->allocationInfo = allocationInfo;

        vulkanInstance = new VkInstance;
        creationResult = vkCreateInstance(this->creationInfo, this->allocationInfo, vulkanInstance);

        if (creationResult != VK_SUCCESS)
        {
            SBE::log->critical("Vulkan instance did not initialize.");
            throw new std::runtime_error("Vulkan Instance did not initialize.");
        }
        else
        {
            SBE::log->info("Created Vulkan Instance.");
        }
        // this->allocationInfo=nullptr;
    }

    Instance::Instance(VkInstanceCreateInfo *creationInfo, VkAllocationCallbacks *allocationInfo)
    {
        this->creationInfo = creationInfo;
        this->allocationInfo = allocationInfo;

        vulkanInstance = new VkInstance;
        creationResult = vkCreateInstance(this->creationInfo, this->allocationInfo, vulkanInstance);

        if (creationResult != VK_SUCCESS)
        {
            SBE::log->critical("Vulkan instance did not initialize.");
            throw new std::runtime_error("Vulkan Instance did not initialize.");
        }
    }

    // Destructors
    //----------------------------------
    Instance::~Instance()
    {
        vkDestroyInstance(*vulkanInstance, allocationInfo);

        delete creationInfo;
        delete allocationInfo;
    }

    // Accessors
    //----------------------------------
    VkInstance *Instance::getInstance()
    {
        return this->vulkanInstance;
    }

    VkInstanceCreateInfo *Instance::getCreationInfo()
    {
        return this->creationInfo;
    }

    VkAllocationCallbacks *Instance::getAllocationInfo()
    {
        return this->allocationInfo;
        // return nullptr;
    }

    VkResult Instance::getResult()
    {
        return creationResult;
    }

    // Mutators
    //----------------------------------
    void Instance::setInstance(VkInstance *newInstance)
    {
        this->vulkanInstance = newInstance;
    }

    void Instance::setCreationInfo(VkInstanceCreateInfo *newInfo)
    {
        this->creationInfo = newInfo;
    }

    void Instance::setAllocationInfo(VkAllocationCallbacks *newAllocators)
    {
        this->allocationInfo = newAllocators;
    }
};