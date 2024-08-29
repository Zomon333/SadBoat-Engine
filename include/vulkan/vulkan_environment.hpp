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
#ifndef VK_ENV_h
#define VK_ENV_H

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include "glfw/include/GLFW/glfw3.h"

#include "utilities/logging/startup_logger.hpp"
#include "utilities/logging/log_manager.hpp"
#include "utilities/logging/log_handle.hpp"

#include "utilities/configs/config.hpp"

#include "vulkan/instance.hpp"
#include "vulkan/extension_collection.hpp"
#include "vulkan/layer_collection.hpp"
#include "vulkan/physical_device.hpp"
#include "vulkan/physical_device_collection.hpp"
#include "vulkan/logical_device.hpp"
#include "vulkan/queue_collection.hpp"
#include "vulkan/queue.hpp"
#include "vulkan/command_pool_manager.hpp"

namespace SBE
{
    class VulkanEnvironment
    {
    private:
        Config* deviceConfig;
        ExtensionCollection* extensions;
        LayerCollection* layers;

        std::vector<VkExtensionProperties> enabledExtensions;
        std::vector<VkExtensionProperties> filterExtensions();

        std::vector<VkLayerProperties> enabledLayers;
        std::vector<VkLayerProperties> filterLayers();

        Instance* instance;
        PhysicalDeviceCollection* devices;
        PhysicalDevice* preferredDevice;
        VkPhysicalDeviceFeatures* requiredFeatures;

        LogicalDevice* logicalDevice;
        QueueCollection* queues;

        GLFWwindow* window;
        VkSurfaceKHR surface;

        CommandPoolManager* commandPools;

    public:
        VulkanEnvironment(std::string gameName);

        Config* getDeviceConfig();
        ExtensionCollection* getExtensionCollection();
        LayerCollection* getLayerCollection();

        std::vector<VkExtensionProperties>* getEnabledExtensions();
        std::vector<VkLayerProperties>* getEnabledLayers();

        Instance* getInstance();
        PhysicalDeviceCollection* getDevices();
        PhysicalDevice* getPreferredDevice();
        VkPhysicalDeviceFeatures* getRequiredFeatures();

        LogicalDevice* getLogicalDevice();
        QueueCollection* getQueues();

        CommandPoolManager* getCommandPoolManager();

        GLFWwindow* getWindow();
        VkSurfaceKHR* getSurface();

    };
};
#endif