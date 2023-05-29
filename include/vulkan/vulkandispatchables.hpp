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
#ifndef VULKAN_DISPATCHABLES_H
#define VULKAN_DISPATCHABLES_H

#include "sb-engine.hpp"

using namespace std;

namespace SBE
{
    // VulkanDispatchables: Graphics
    // Struct for the creation of the Vulkan graphics environment and relevant coinciding parts
    struct VulkanDispatchables
    {
        // Our wrapper for the Vulkan instance
        // 
        Instance* vulkanInstance=nullptr;
        
        // Config settings for device
        Config* deviceConfig=nullptr;

        // Our wrapper for all Vulkan enabled devices
        // Used to determine the optimal device, but also backup devices
        PhysicalDeviceCollection* vulkanDevices=nullptr;
        ExtensionCollection* vulkanExtensions=nullptr;

        Event<vector<VkExtensionProperties>*, vector<VkExtensionProperties>*>* enabledExtFilter=nullptr;
        vector<VkExtensionProperties> enabledExtensions;

        // Our wrapper for the physical Vulkan device
        // This is the preferred device as determined by our device collection
        PhysicalDevice* preferredDevice=nullptr;

        LayerCollection* vulkanLayers=nullptr;
        Event<vector<VkLayerProperties>*, vector<VkLayerProperties>*>* enabledLayerFilter=nullptr;
        vector<VkLayerProperties> enabledLayers;

        // The struct requesting specific device features
        // Will request these be enabled in the LogicalDevice
        VkPhysicalDeviceFeatures* requiredFeatures=nullptr;

        // Our wrapper for the Vulkan logical device
        // Used for interfacing with the actual device, queues, etc.
        LogicalDevice* vulkanLogicalDevice=nullptr;


        VkBufferCreateInfo* bufferInfo=nullptr;
        Buffer* buffer=nullptr;

        QueueCollection* queues=nullptr;

        // The event to set up all of our Vulkan environment.
        // Useful so the Vulkan environment can be set up in basically two lines of code, while also being multithreaded.
        Event<VulkanDispatchables*, VulkanDispatchables*> setup = Event<VulkanDispatchables*, VulkanDispatchables*>(
            lF(VulkanDispatchables* toInit)
            {
                // Setup the instance
                toInit->vulkanInstance = new Instance;

                // Find what devices exist in it, and get the optimal device
                toInit->vulkanDevices = new PhysicalDeviceCollection(toInit->vulkanInstance);
                toInit->preferredDevice = toInit->vulkanDevices->getOptimal(this->deviceConfig);
                
                // Find the extensions and layers available on the optimal device
                toInit->vulkanExtensions = new ExtensionCollection(toInit->preferredDevice);
                toInit->vulkanLayers = new LayerCollection(toInit->preferredDevice);

                // Save the available extensions and layers for parsing
                toInit->enabledExtensions = toInit->vulkanExtensions->getProps();
                toInit->enabledLayers = toInit->vulkanLayers->getProps();

                toInit->enabledExtFilter = (toInit->enabledExtFilter==nullptr) ? new Event<vector<VkExtensionProperties>*,vector<VkExtensionProperties>*>(
                    F(vector<VkExtensionProperties>* toParse)
                    {
                        vector<VkExtensionProperties> toReturn;
                        for(int i=0; i<toParse->size(); i++)
                        {
                            if(toParse->at(i).extensionName[3]=='K')
                            {
                                SBE::log->debug(string("Enabling extension: ").append(toParse->at(i).extensionName));
                                toReturn.push_back(toParse->at(i));
                            }
                            // else
                            // {
                            //     // SBE::log->debug(string("Not enabling optional extension: ").append(toParse->at(i).extensionName));
                            // }
                        }

                        (*toParse) = toReturn;

                        return toParse;
                    }
                ) : toInit->enabledExtFilter;

                toInit->enabledLayerFilter = (toInit->enabledLayerFilter==nullptr) ? new Event<vector<VkLayerProperties>*,vector<VkLayerProperties>*>(
                    F(vector<VkLayerProperties>* toParse)
                    {
                        // for(int i=0; i<toParse->size(); i++)
                        // {
                        //     // SBE::log->debug(string("Layer exists: ").append(toParse->at(i).layerName));
                        // }

                        toParse->clear();
                        return toParse;
                    }
                ) : toInit->enabledLayerFilter;


                // Use the events in the struct to parse
                toInit->enabledExtFilter->call(&(toInit->enabledExtensions));
                toInit->enabledLayerFilter->call(&(toInit->enabledLayers));

                // Finish setup
                toInit->requiredFeatures = new VkPhysicalDeviceFeatures;
                toInit->requiredFeatures->multiDrawIndirect=VK_TRUE;
                toInit->requiredFeatures->tessellationShader=VK_TRUE;
                toInit->requiredFeatures->geometryShader=VK_TRUE;

                toInit->vulkanLogicalDevice = new LogicalDevice(toInit->preferredDevice, toInit->requiredFeatures, toInit->enabledLayers, toInit->enabledExtensions);

                toInit->bufferInfo = new VkBufferCreateInfo{
                    VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO, nullptr,
                    0,
                    1920*1080*4,
                    VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
                    VK_SHARING_MODE_EXCLUSIVE,
                    0, nullptr
                };
                toInit->buffer = new Buffer(toInit->vulkanLogicalDevice, *bufferInfo);

                toInit->queues = new QueueCollection(toInit->vulkanLogicalDevice, toInit->vulkanLogicalDevice->getOptimalQueueFam(), toInit->vulkanLogicalDevice->getQueueCount());
                

                VkImageCreateInfo tmp = 
                {
                    VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,    //  sType
                    nullptr,                                //  pNext
                    0,                                      //  flags
                    VK_IMAGE_TYPE_2D,                       //  imageType
                    VK_FORMAT_R8G8B8A8_UNORM,               //  format
                    { 1024, 1024, 1 },                      //  extent
                    10,                                     //  mipLevels
                    1,                                      //  arrayLayers
                    VK_SAMPLE_COUNT_1_BIT,                  //  samples
                    VK_IMAGE_TILING_OPTIMAL,                //  tiling
                    VK_IMAGE_USAGE_SAMPLED_BIT,             //  usage
                    VK_SHARING_MODE_EXCLUSIVE,              //  sharingMode
                    0,                                      //  queueFamilyIndexCount
                    nullptr,                                //  pQueueFamilyIndices
                    VK_IMAGE_LAYOUT_UNDEFINED               //  initialLayout
                };
                Image* testImage = new Image(toInit->vulkanLogicalDevice, &tmp);

                return toInit;
            }
        );
    };
};
#endif

