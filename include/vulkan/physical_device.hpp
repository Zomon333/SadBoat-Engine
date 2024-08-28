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
#ifndef PHYSICALDEVICE_H
#define PHYSICALDEVICE_H

#include "./vulkan/vulkan.hpp"

namespace SBE
{
    // PhysicalDevice: Graphics
    // Wrapper object for enumerated physical devices found by the Vulkan instance
    class PhysicalDevice
    {
    private:
        // Where did we come from?
        Instance *host;

        // What are we?
        VkPhysicalDevice self;

        // What can we do?
        VkPhysicalDeviceProperties selfProps;
        VkPhysicalDeviceFeatures selfFeats;

        // Where do we do it?
        VkPhysicalDeviceMemoryProperties selfMem;

        // How do we do it?
        // QueueFamilyCollection* queueFamilies;

    public:
        // Constructors
        //----------------------------------
        PhysicalDevice(VkPhysicalDevice nSelf, Instance *host);

        PhysicalDevice(VkPhysicalDevice nSelf, VkPhysicalDeviceProperties nProps, Instance *host);

        // Mutators
        //----------------------------------
        void update();

        // Accessors
        //----------------------------------
        VkPhysicalDevice getDevice();
        VkPhysicalDeviceProperties *getProperties();
        VkPhysicalDeviceFeatures *getFeatures();
        VkPhysicalDeviceMemoryProperties *getMem();

        Instance *getHost();

        // auto getQueueFamPropCount() {  return queueFamPropCount; }
        // auto getQueueFamilyProps() {  return selfQueueFamilyProperties; }
    };
};
#endif