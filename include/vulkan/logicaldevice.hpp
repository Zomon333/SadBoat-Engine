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
    // LogicalDevice: Graphics
    // Vulkan wrapper for the logical device used for rendering
    class LogicalDevice
    {
    private:
        Instance* host;
        PhysicalDevice* parent;

        VkPhysicalDeviceFeatures* requiredFeatures;

        VkDeviceCreateInfo* creationInfo;
        VkDevice self;

        unordered_map<int, string> resultLookup;

    public:
        // Constructors
        //----------------------------------

        // Make a device, assume some info
        LogicalDevice(PhysicalDevice* parent, VkPhysicalDeviceFeatures* requiredFeatures=nullptr, vector<VkLayerProperties> layersToEnable=vector<VkLayerProperties>(), vector<VkExtensionProperties> extToEnable=vector<VkExtensionProperties>())
        {

            Event<void*, void*> setupLookup(lF(void* map)
            {
                #define LOOKUP(a,b) this->resultLookup[b]=a

                LOOKUP("VK_SUCCESS",0);
                LOOKUP("VK_NOT_READY",1);
                LOOKUP("VK_TIMEOUT",2);
                LOOKUP("VK_EVENT_SET",3);
                LOOKUP("VK_EVENT_RESET",4);
                LOOKUP("VK_INCOMPLETE",5);
                LOOKUP("VK_ERROR_OUT_OF_HOST_MEMORY",-1);
                LOOKUP("VK_ERROR_OUT_OF_DEVICE_MEMORY",-2);
                LOOKUP("VK_ERROR_INITIALIZATION_FAILED",-3);
                LOOKUP("VK_ERROR_DEVICE_LOST",-4);
                LOOKUP("VK_ERROR_MEMORY_MAP_FAILED",-5);
                LOOKUP("VK_ERROR_LAYER_NOT_PRESENT",-6);
                LOOKUP("VK_ERROR_EXTENSION_NOT_PRESENT",-7);
                LOOKUP("VK_ERROR_FEATURE_NOT_PRESENT",-8);
                LOOKUP("VK_ERROR_INCOMPATIBLE_DRIVER",-9);
                LOOKUP("VK_ERROR_TOO_MANY_OBJECTS",-10);
                LOOKUP("VK_ERROR_FORMAT_NOT_SUPPORTED",-11);
                LOOKUP("VK_ERROR_FRAGMENTED_POOL",-12);
                LOOKUP("VK_ERROR_UNKNOWN",-13);
                LOOKUP("VK_ERROR_OUT_OF_POOL_MEMORY",-1000069000);
                LOOKUP("VK_ERROR_INVALID_EXTERNAL_HANDLE",-1000072003);
                LOOKUP("VK_ERROR_FRAGMENTATION",-1000161000);
                LOOKUP("VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS",-1000257000);
                LOOKUP("VK_PIPELINE_COMPILE_REQUIRED",1000297000);
                LOOKUP("VK_ERROR_SURFACE_LOST_KHR",-1000000000);
                LOOKUP("VK_ERROR_NATIVE_WINDOW_IN_USE_KHR",-1000000001);
                LOOKUP("VK_SUBOPTIMAL_KHR",1000001003);
                LOOKUP("VK_ERROR_OUT_OF_DATE_KHR",-1000001004);
                LOOKUP("VK_ERROR_INCOMPATIBLE_DISPLAY_KHR",-1000003001);
                LOOKUP("VK_ERROR_VALIDATION_FAILED_EXT",-1000011001);
                LOOKUP("VK_ERROR_INVALID_SHADER_NV",-1000012000);
                LOOKUP("VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR",-1000023000);
                LOOKUP("VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR",-1000023001);
                LOOKUP("VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR",-1000023002);
                LOOKUP("VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR",-1000023003);
                LOOKUP("VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR",-1000023004);
                LOOKUP("VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR",-1000023005);
                LOOKUP("VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT",-1000158000);
                LOOKUP("VK_ERROR_NOT_PERMITTED_KHR",-1000174001);
                LOOKUP("VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT",-1000255000);
                LOOKUP("VK_THREAD_IDLE_KHR",1000268000);
                LOOKUP("VK_THREAD_DONE_KHR",1000268001);
                LOOKUP("VK_OPERATION_DEFERRED_KHR",1000268002);
                LOOKUP("VK_OPERATION_NOT_DEFERRED_KHR",1000268003);
                LOOKUP("VK_ERROR_COMPRESSION_EXHAUSTED_EXT",-1000338000);
                LOOKUP("VK_ERROR_OUT_OF_POOL_MEMORY_KHR",-1000069000);
                LOOKUP("VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR",-1000072003);
                LOOKUP("VK_ERROR_FRAGMENTATION_EXT",-1000161000);
                LOOKUP("VK_ERROR_NOT_PERMITTED_EXT",-1000174001);
                LOOKUP("VK_ERROR_INVALID_DEVICE_ADDRESS_EXT",-1000257000);
                LOOKUP("VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS_KHR",-1000257000);
                LOOKUP("VK_PIPELINE_COMPILE_REQUIRED_EXT",1000297000);
                LOOKUP("VK_ERROR_PIPELINE_COMPILE_REQUIRED_EXT",1000297000);
                LOOKUP("VK_RESULT_MAX_ENUM",0x7FFFFFFF);
                

                #undef LOOKUP(a,b)

                return map;
            });
            setupLookup(nullptr);

            // Save device data passed in by parameters
            this->host=parent->getHost();
            this->parent=parent;
            
            // Enable all features by default.
            this->requiredFeatures = parent->getFeatures();

            // Start querying queue families to use for queues.
            QueueFamilyCollection deviceQueueFamilies = QueueFamilyCollection(parent);
            vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfos;

            // Find the optimal queue family
            auto optimalFamily = deviceQueueFamilies.getOptimal();

            // Generate the struct to create as many queues within the family as possible.
            deviceQueueCreateInfos.emplace_back(
                VkDeviceQueueCreateInfo{
                    VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,                     // VkStructureType             sType;
                    nullptr,                                                        // const void*                 pNext;
                    0,                                                              // VkDeviceQueueCreateFlags    flags;
                    (unsigned int)(optimalFamily.first),                            // uint32_t                    queueFamilyIndex;
                    optimalFamily.second.getProps()->queueCount,                    // uint32_t                    queueCount;
                    nullptr                                                         // const float*                pQueuePriorities;
                }
            );

            
            // Query all of the possible extensions for the physical device
            ExtensionCollection deviceExtensions = ExtensionCollection(parent);
            vector<char*> extensionsToEnable;
            // Sort through them all and choose any that are shared between our list of requested extensions
            for(int i=0; i<deviceExtensions.getProps().size(); i++)
            {
                for(int j=0; j<extToEnable.size(); j++)
                {
                    if((extToEnable[j].extensionName==deviceExtensions.getProp(i).extensionName) && (extToEnable[j].specVersion==deviceExtensions.getProp(i).specVersion))
                    {
                        extensionsToEnable.emplace_back(extToEnable[j].extensionName);
                    }
                }
            }

            // Pass the info we need to create the device to the struct
            this->creationInfo = new VkDeviceCreateInfo{
                    VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,                                                   // VkStructureType                      sType;
                    nullptr,                                                                                // const void*                          pNext;
                    0, /*Reserved for future use*/                                                          // VkDeviceCreateFlags                  flags;
                    (unsigned int)(deviceQueueCreateInfos.size()),                                          // uint32_t                             queueCreateInfoCount;
                    deviceQueueCreateInfos.data(),                                                          // const VkDeviceQueueCreateInfo*       pQueueCreateInfos;
                    0, /*Deprecated*/                                                                       // uint32_t                             enabledLayerCount;
                    nullptr, /*Deprecated*/                                                                 // const char* const*                   ppEnabledLayerNames;
                    (unsigned int)(extensionsToEnable.size()),                                              // uint32_t                             enabledExtensionCount;
                    extensionsToEnable.data(),                                                              // const char* const*                   ppEnabledExtensionNames;
                    this->requiredFeatures                                                                  // const VkPhysicalDeviceFeatures*      pEnabledFeatures;
            };



            auto result = vkCreateDevice(
                this->parent->getDevice(),
                this->creationInfo,
                this->host->getAllocationInfo(),
                &self
            );
            cout<<"LogicalDevice created with result: "<<resultLookup[result]<<endl;
        }

        // Make a device, assume no info
        LogicalDevice(PhysicalDevice* parent, VkDeviceCreateInfo* creationInfo, VkPhysicalDeviceFeatures* requiredFeatures={})
        {
            this->host=parent->getHost();
            this->creationInfo=creationInfo;
            this->requiredFeatures=parent->getFeatures();

            // Todo: Add code for requiredFeatures to be checked against parent's features

            vkCreateDevice(parent->getDevice(), creationInfo, host->getAllocationInfo(), &self);
        }

        // Deconstructors
        //----------------------------------

        ~LogicalDevice()
        {
            vkDeviceWaitIdle(self);
            vkDestroyDevice(self, host->getAllocationInfo());
        }

        // Mutators
        //----------------------------------

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
        //----------------------------------

        auto getParent() { return parent; }
        auto getHost() { return host; }
        auto getSelf() { return self; }
        auto getCreationInfo() { return creationInfo; }
        auto getRequiredFeats() { return requiredFeatures; }

        template <class Function>
        Function getFunc(const char* pName)
        {
            return ((Function)(vkGetDeviceProcAddr(self, pName)));
        }
    };
};
#endif