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
#ifndef PHYSICALDEVICE_H
#define PHYSICALDEVICE_H

#include "sb-engine.hpp"
#include "./vulkan/vulkan.hpp"

using namespace std;

namespace SBE
{
    class PhysicalDevice
    {
    private:
        // What are we?
        VkPhysicalDevice self; 

        // What can we do?
        VkPhysicalDeviceProperties selfProps;
        VkPhysicalDeviceFeatures selfFeats;

        // Where do we do it?
        VkPhysicalDeviceMemoryProperties selfMem;

        // How do we do it?
        uint32_t queueFamPropCount;
        vector<VkQueueFamilyProperties*> selfQueueFamilyProperties;

    public:

        //Constructors
        //----------------------------------
        PhysicalDevice(VkPhysicalDevice nSelf)
        {
            self=nSelf;
            this->update();
        }

        PhysicalDevice(VkPhysicalDevice nSelf, VkPhysicalDeviceProperties nProps)
        {
            self=nSelf;
            selfProps=nProps;
            this->update();
        }

        //Mutators
        //----------------------------------
        void update()
        {
            vkGetPhysicalDeviceProperties(self, &selfProps);
            vkGetPhysicalDeviceFeatures(self, &selfFeats);
            vkGetPhysicalDeviceMemoryProperties(self, &selfMem);

            vkGetPhysicalDeviceQueueFamilyProperties(self, &queueFamPropCount, nullptr);
            selfQueueFamilyProperties.resize(queueFamPropCount);
            vkGetPhysicalDeviceQueueFamilyProperties(self, &queueFamPropCount, *(selfQueueFamilyProperties.data()));
        }

        //Accessors
        //----------------------------------
        auto getDevice() { return self; }
        auto getProperties() { return selfProps; }
        auto getFeatures() {  return selfFeats; }
        auto getMem() {  return selfMem; }
        auto getQueueFamPropCount() {  return queueFamPropCount; }
        auto getQueueFamilyProps() {  return selfQueueFamilyProperties; }

    };
};
#endif