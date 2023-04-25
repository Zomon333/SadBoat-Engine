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
    // PhysicalDeviceCollection: Graphics
    // Generalized container for the querying and generation of PhysicalDevices using a given instance
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
                devices[i]=new PhysicalDevice(tmp[i],tmpProps[i], host);
            }

            return devices;
        }

        //Accessors
        //----------------------------------

        Instance* getHost() { return host; }
        uint32_t getNumDevices() { return numDevices; }
        vector<PhysicalDevice*> getDevices() { return devices; }

        PhysicalDevice* getOptimal(Config* deviceConfig=nullptr) 
        { 

            // noConfig finds what it believes to be the best device given the slim amount of criteria we have access to
            // - It first breaks down all of the devices into categories
            // - It continues to sort the categories by the most to least dedicated GPU
            // - It calculates from the order of most to least dedicated GPU, which one has the most VRAM
            // - It returns the most dedicated GPU with the most VRAM. This is likely the best GPU.
            Event<PhysicalDevice*, int> noConfig(
                lF(int a)
                {
                    // No config provided, so...
                    // 1) Intelligently choose best device as backup, save to config
                    
                    // Sort existing devices by category.
                    unordered_map<VkPhysicalDeviceType,vector<PhysicalDevice*>> deviceMap;
                    for(int i=0; i<this->devices.size(); i++)
                    {
                        deviceMap[this->devices[i]->getProperties()->deviceType].push_back(this->devices[i]);
                    }
                    
                    // sortMem returns the device with the most VRAM.
                    Event<PhysicalDevice*, VkPhysicalDeviceType> sortMem(
                        [&deviceMap](VkPhysicalDeviceType typeToCheck)
                        {
                            if(deviceMap[typeToCheck].size()<=0)
                            {
                                return ((PhysicalDevice*)(nullptr));
                            }

                            int highestHeap = 0;
                            int heapIndex = 0;
                            for(int i=0; i<deviceMap[typeToCheck].size(); i++)
                            {
                                if(deviceMap[typeToCheck][i]->getMem()->memoryHeapCount>highestHeap)
                                {
                                    highestHeap = deviceMap[typeToCheck][i]->getMem()->memoryHeapCount;
                                    heapIndex = i;
                                }
                            }
                            
                            return deviceMap[typeToCheck][heapIndex];
                        }
                    );
                    
                    PhysicalDevice* result;
                    vector<VkPhysicalDeviceType> priorities = {
                        VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU,
                        VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU,
                        VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU,
                        VK_PHYSICAL_DEVICE_TYPE_CPU
                    };

                    do
                    {
                        result = (sortMem(priorities.back())!=nullptr) ? sortMem(priorities.back()) : result;
                        priorities.pop_back();
                    }
                    while(priorities.size()>0);
                    
                    if(result!=nullptr)
                    {
                        cout<<"Preferred device"<<": "<<result->getProperties()->deviceName<<endl;
                        return result;
                    }        

                    // 2) If no device to choose, choose a-th device
                    return this->devices[a];
                }
            );

            // Do we have a config to check?
            if(deviceConfig==nullptr)
            {
                return noConfig(0);
            }
            else
            {
                // Verify the device is listed in the config
                string deviceName = "";
                
                deviceName = string((*deviceConfig)["GraphicsOptions"][0]["PreferredDevice"][0]["DeviceName"][0].getContents());

                // Check to see if the device we're looking for by name exists. If yes, use!
                for(int i=0; i<devices.size(); i++)
                {
                    if(string(devices[i]->getProperties()->deviceName)==string(deviceName))
                    {
                        return devices[i];
                    }
                }

                // If the device we're looking for doesn't exist by name, use no config provided settings...
                return noConfig(0);
            }
        }

    };
};
#endif