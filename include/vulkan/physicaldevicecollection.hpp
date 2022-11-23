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
#ifndef PDC_H
#define PDC_H

#include "sb-engine.hpp"
#include "vulkan/instance.hpp"
#include "vulkan/physicaldevice.hpp"
#include "./vulkan/vulkan.hpp"

using namespace std;

namespace SBE
{
    class PhysicalDeviceCollection
    {
    private:
        Instance* host;

        uint32_t numDevices;
        vector<PhysicalDevice*> devices;

    public:
        //Constructors
        //----------------------------------

        PhysicalDeviceCollection(Instance* newHost)
        {
            //Set the instance in this collection to the instance passed by the parameter
            this->host=newHost;

            //Get how many devices are in this instance
            updateCount();

            //Get current devices, place into devices array
            updateDevices();

        }

        //Mutators
        //----------------------------------

        //Gets how many devices are in this instance, and stores in numDevices. Also returns numDevices.
        uint32_t updateCount()
        {
            auto result = vkEnumeratePhysicalDevices(*(host->getInstance()),&numDevices,nullptr);
            if(result!=VK_SUCCESS)
            {
                throw new runtime_error("Vulkan failed to query number of devices from Vulkan Instance");
            }

            return numDevices;
        }

        //Gets devices currently in the instance, and stores in devices. Also returns devices.
        vector<PhysicalDevice*> updateDevices()
        {
            devices.resize(numDevices);

            vector<VkPhysicalDevice> tmp;
            tmp.resize(numDevices);

            vector<VkPhysicalDeviceProperties> tmpProps;
            tmpProps.resize(numDevices);
            
            auto result = vkEnumeratePhysicalDevices(*(host->getInstance()), &numDevices, &tmp[0]);
            if(result!=VK_SUCCESS)
            {
                throw new runtime_error("Vulkan failed to query which devices exist within the provided instance.");
            }

            for(int i = 0; i<tmp.size(); i++)
            {
                vkGetPhysicalDeviceProperties(tmp[i],&tmpProps[i]);
                devices[i]=new PhysicalDevice(tmp[i],tmpProps[i]);
            }

            return devices;
        }

        //Accessors
        //----------------------------------

        Instance* getHost() { return host; }
        uint32_t getNumDevices() { return numDevices; }
        vector<PhysicalDevice*> getDevices() { return devices; }

    };
};
#endif