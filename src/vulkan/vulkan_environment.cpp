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

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include "glfw/include/GLFW/glfw3.h"

#include "utilities/logging/startup_logger.hpp"
#include "utilities/logging/log_manager.hpp"
#include "utilities/logging/log_handle.hpp"

#include "utilities/configs/config.hpp"
#include "utilities/configs/config_manager.hpp"

#include "vulkan/vulkan_environment.hpp"
#include "vulkan/vulkan_result_lookup.hpp"
#include "vulkan/instance.hpp"
#include "vulkan/extension_collection.hpp"
#include "vulkan/layer_collection.hpp"
#include "vulkan/physical_device.hpp"
#include "vulkan/physical_device_collection.hpp"
#include "vulkan/logical_device.hpp"
#include "vulkan/queue_collection.hpp"
#include "vulkan/queue.hpp"

namespace SBE
{
    std::vector<VkExtensionProperties> VulkanEnvironment::filterExtensions()
    {
        std::vector<VkExtensionProperties> toReturn;
        for (unsigned int i = 0; i < extensions->getProps().size(); i++)
        {
            if (extensions->getProps().at(i).extensionName[3] == 'K')
            {
                SBE::log->debug(std::string("Enabling extension: ").append(extensions->getProps().at(i).extensionName));
                toReturn.push_back(extensions->getProps().at(i));
            }
            else
            {
                std::stringstream toLog;
                toLog << "Not enabling optional extension: " << extensions->getProps().at(i).extensionName;
                SBE::log->debug(&toLog);
            }
        }

        return toReturn;
    }

    std::vector<VkLayerProperties> VulkanEnvironment::filterLayers()
    {
        std::vector<VkLayerProperties> toReturn;
        for (unsigned int i = 0; i < layers->getProps().size(); i++)
        {
            if (layers->getProps().at(i).layerName[3] == 'K')
            {
                SBE::log->debug(std::string("Enabling layer: ").append(layers->getProps().at(i).layerName));
                toReturn.push_back(layers->getProps().at(i));
            }
            else
            {
                std::stringstream toLog;
                toLog << "Not enabling optional layer: " << layers->getProps().at(i).layerName;
                SBE::log->debug(&toLog);
            }
        }

        return toReturn;
    }

    VulkanEnvironment::VulkanEnvironment(std::string gameName, Config* deviceConfig)
    {
        if(!deviceConfig)
        {
            SBE::ConfigManager configs;
            this->deviceConfig = configs.getConfig("./assets/config/graphicsOptions.xml");
        }
        else
        {
            this->deviceConfig = deviceConfig;
        }

        if (!glfwInit())
        {
            SBE::log->critical("Unable to initialize GLFW. Quitting program.");
            abort();
        }

        glfwSetErrorCallback([](int error, const char *description)
                             {
            std::stringstream toLog;
            toLog<<"Error: "<<description;
            SBE::log->error(&toLog); });

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        window = glfwCreateWindow(640, 480, gameName.c_str(), nullptr, nullptr);
        if (!window)
        {
            SBE::log->critical("Unable to initialize GLFW Window. Quitting program.");
            abort();
        }

        uint32_t extCount;
        VkExtensionProperties *pProperties = new VkExtensionProperties;
        SBE::log->debug(SBE::VkResultLookup(vkEnumerateInstanceExtensionProperties(
            NULL,
            &extCount,
            pProperties)));

        std::vector<VkExtensionProperties> toEnable;
        for (unsigned int i = 0; i < extCount; i++)
        {
            toEnable.push_back(pProperties[i]);
        }

        instance = new Instance(toEnable);

        devices = new PhysicalDeviceCollection(this->instance);

        preferredDevice = devices->getOptimal(this->deviceConfig);

        extensions = new ExtensionCollection(this->preferredDevice);
        layers = new LayerCollection(this->preferredDevice);

        enabledExtensions = filterExtensions();
        enabledLayers = filterLayers();

        requiredFeatures = new VkPhysicalDeviceFeatures;
        requiredFeatures->multiDrawIndirect = VK_TRUE;
        requiredFeatures->tessellationShader = VK_TRUE;
        requiredFeatures->geometryShader = VK_TRUE;

        logicalDevice = new LogicalDevice(preferredDevice, requiredFeatures, enabledLayers, enabledExtensions);

        queues = new QueueCollection(logicalDevice, logicalDevice->getOptimalQueueFam(), logicalDevice->getQueueCount());

        if (glfwCreateWindowSurface(*(instance->getInstance()), window, nullptr, &surface) != VK_SUCCESS)
        {
            SBE::log->critical("Unable to get surface for instance! Aborting.");
            glfwDestroyWindow(window);
            glfwTerminate();
            abort();
        }
        else
        {
            std::stringstream toLog;
            toLog << "Created new VkSurface through GLFW.";
            SBE::log->info(&toLog);
        }

        commandPools = new CommandPoolManager(logicalDevice);

    }

    Config *VulkanEnvironment::getDeviceConfig()
    {
        return deviceConfig;
    }

    ExtensionCollection *VulkanEnvironment::getExtensionCollection()
    {
        return extensions;
    }

    LayerCollection *VulkanEnvironment::getLayerCollection()
    {
        return layers;
    }

    std::vector<VkExtensionProperties> *VulkanEnvironment::getEnabledExtensions()
    {
        return &enabledExtensions;
    }

    std::vector<VkLayerProperties> *VulkanEnvironment::getEnabledLayers()
    {
        return &enabledLayers;
    }

    Instance *VulkanEnvironment::getInstance()
    {
        return instance;
    }

    PhysicalDeviceCollection *VulkanEnvironment::getDevices()
    {
        return devices;
    }

    PhysicalDevice *VulkanEnvironment::getPreferredDevice()
    {
        return preferredDevice;
    }

    VkPhysicalDeviceFeatures *VulkanEnvironment::getRequiredFeatures()
    {
        return requiredFeatures;
    }

    LogicalDevice *VulkanEnvironment::getLogicalDevice()
    {
        return logicalDevice;
    }

    QueueCollection *VulkanEnvironment::getQueues()
    {
        return queues;
    }

    GLFWwindow *VulkanEnvironment::getWindow()
    {
        return window;
    }

    VkSurfaceKHR *VulkanEnvironment::getSurface()
    {
        return &surface;
    }

    CommandPoolManager *VulkanEnvironment::getCommandPoolManager()
    {
        return commandPools;
    }

};