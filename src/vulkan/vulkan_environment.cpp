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

#include <unistd.h>

#include "events/recurring_event.hpp"

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
#include "vulkan/swapchain.hpp"

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
        std::stringstream toLog;

        if(!deviceConfig)
        {
            SBE::ConfigManager configs;
            this->deviceConfig = configs.getConfig("./assets/config/graphicsOptions.xml");
        }
        else
        {
            this->deviceConfig = deviceConfig;
        }

        

        uint32_t extCount;
        SBE::log->debug(SBE::VkResultLookup(vkEnumerateInstanceExtensionProperties(
            NULL,
            &extCount,
            nullptr)));

        std::vector<VkExtensionProperties> pProperties = std::vector<VkExtensionProperties>(extCount);
        SBE::log->debug(SBE::VkResultLookup(vkEnumerateInstanceExtensionProperties(
            NULL,
            &extCount,
            pProperties.data())));

        std::vector<VkExtensionProperties> toEnable;
        std::unordered_map<char*, bool> instanceExtensions(false);

        for (unsigned int i = 0; i < extCount; i++)
        {
            toEnable.push_back(pProperties[i]);
            
            // toLog<<"Enabling instance extension: "<<pProperties[i].extensionName;
            // SBE::log->debug(&toLog);

            instanceExtensions[pProperties[i].extensionName] = true;
        }

        instance = new Instance(toEnable);

        devices = new PhysicalDeviceCollection(this->instance);

        preferredDevice = devices->getOptimal(this->deviceConfig);


        if (!glfwInit())
        {
            SBE::log->critical("Unable to initialize GLFW. Quitting program.");
            abort();
        }

        glfwSetErrorCallback([](int error, const char *description)
                             {
            std::stringstream toLog;
            toLog<<"Error: "<<description;
            SBE::log->error(&toLog); 
            });

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        int displayXPos, displayYPos;
        int displayX, displayY;
        
        if(deviceConfig)
        {
            try
            {
                SBE::log->info("Getting screen resolution from config file.");
                std::string resX = (*deviceConfig)["GraphicsOptions"][0]["Resolution"][0]["X"][0].getContents<std::string>();
                std::string resY = (*deviceConfig)["GraphicsOptions"][0]["Resolution"][0]["Y"][0].getContents<std::string>();

                displayX = std::stoi(resX);
                displayY = std::stoi(resY);
            }
            catch(const std::exception& e)
            {
                SBE::log->info("Failed to read config file. Getting screen resolution from display.");
                glfwGetMonitorWorkarea(monitor, &displayXPos, &displayYPos, &displayX, &displayY);
                std::cerr << e.what() << '\n';
            }
        }
        else
        {
            if(monitor)
            {    
                SBE::log->info("Getting screen resolution from display.");
                glfwGetMonitorWorkarea(monitor, &displayXPos, &displayYPos, &displayX, &displayY);
            }
            else
            {
                SBE::log->info("Unable to read screen resolution from display. Assuming 1920x1080.");
                displayX = 1920;
                displayY = 1080;
            }
        }
        

        window = glfwCreateWindow(displayX, displayY, gameName.c_str(), nullptr, nullptr);
        if (!window)
        {
            SBE::log->critical("Unable to initialize GLFW Window. Quitting program.");
            abort();
        }
        else
        {
            toLog<<"Created GLFW window with dimensions "<<displayX<<" x "<<displayY<<". ";
            SBE::log->debug(&toLog);
        }


        extensions = new ExtensionCollection(this->preferredDevice);
        layers = new LayerCollection(this->preferredDevice);

        enabledExtensions = filterExtensions();
        enabledLayers = filterLayers();

        requiredFeatures = new VkPhysicalDeviceFeatures;
        requiredFeatures->multiDrawIndirect = VK_TRUE;
        requiredFeatures->tessellationShader = VK_TRUE;
        requiredFeatures->geometryShader = VK_TRUE;

        logicalDevice = new LogicalDevice(preferredDevice, requiredFeatures, enabledLayers, enabledExtensions);

        // ************

        queues = new QueueCollection(logicalDevice, logicalDevice->getOptimalQueueFam(), logicalDevice->getQueueCount());

        if (glfwCreateWindowSurface(*(instance->getInstance()), window, nullptr, &surface) != VK_SUCCESS)
        {
            // What this should probably do is iterate through every LogicalDevice that can be made from the PhysicalDeviceCollection.
            // This way, if the system defaults to a non-presentable GPU it can search for a backup.
            // This really should never happen, though...

            SBE::log->critical("Unable to get surface for instance! Aborting.");
            glfwDestroyWindow(window);
            glfwTerminate();
            abort();
        }
        else
        {
            toLog << "Created new VkSurface through GLFW.";
            SBE::log->debug(&toLog);
        }

        commandPools = new CommandPoolManager(logicalDevice);

        renderPass = new Renderpass(logicalDevice, nullptr);
        
        swapchain = new Swapchain(
            preferredDevice,
            logicalDevice,
            queues,
            surface,
            renderPass->getRenderpass()
        );

// ******************************************************************************

        VkCommandBufferBeginInfo beginInfo = {
            VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            nullptr,
            0,
            nullptr
        };

        VkCommandBuffer* commandBuffer = commandPools->getPools()[0]->getBuffers()[0]->getSelf();
        toLog<<"Attempting to begin commandBuffer: "<<SBE::VkResultLookup(vkBeginCommandBuffer(*commandBuffer, &beginInfo));
        SBE::log->debug(&toLog);
        

        
        std::tuple<uint32_t, VkImage, VkImageView, VkFramebuffer> frameData = swapchain->getFramedata();

        // This NEEDS to be getSwapchainIndex() in the future.
        // getSwapchainIndex segfaults, though. FIX THIS.
        uint32_t index = std::get<0>(frameData);
        
        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = *(renderPass->getRenderpass());
        renderPassInfo.framebuffer = std::get<3>(frameData);
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = swapchain->getExtent();

        VkClearValue clearColor = { {0.0f, 0.0f, 0.0f, 1.0f} };
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(*commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        SBE::log->debug("Beginning command recording.");
        // Record draw commands here
        // malloc(1);
        // vkCmdDraw(*commandBuffer, 0, 0, 0, 0);

        SBE::log->debug("Ending command recording.");
        vkCmdEndRenderPass(*commandBuffer);


        toLog<<"Attempting to end command buffer recording: "<<SBE::VkResultLookup(vkEndCommandBuffer(*commandBuffer));
        SBE::log->debug(&toLog);

        
        VkPipelineStageFlags waitStages[] = {
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
        };


        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = commandBuffer;
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = swapchain->getSemaphore();
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.signalSemaphoreCount = 1;

        VkSemaphoreCreateInfo semInfo = {VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,nullptr,0};
        VkSemaphore renderFinished;
        toLog<<"Attempting to create new renderFinished semaphore: "<<SBE::VkResultLookup(vkCreateSemaphore(logicalDevice->getSelf(), &semInfo, nullptr, &renderFinished));
        SBE::log->debug(&toLog);

        submitInfo.pSignalSemaphores = swapchain->getSemaphore();

        int frameCount = 0;
        SBE::RecurringEvent<int> recurSum([this, &frameCount, index, submitInfo](int a)
        {
            std::stringstream toLog;
            toLog<<"Attempting to submit queue: "<<SBE::VkResultLookup(vkQueueSubmit(queues->getQueue(0)->getSelf(), 1, &submitInfo, VK_NULL_HANDLE));
            SBE::log->debug(&toLog);

            VkPresentInfoKHR presentInfo = {};
            presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
            presentInfo.waitSemaphoreCount = 1;
            presentInfo.pWaitSemaphores = swapchain->getSemaphore();
            presentInfo.swapchainCount = 1;
            presentInfo.pSwapchains = swapchain->getSwapchain();
            presentInfo.pImageIndices = &index;

            toLog<<"Attempting to present work: "<<SBE::VkResultLookup(vkQueuePresentKHR(queues->getQueue(0)->getSelf(), &presentInfo));
            SBE::log->debug(&toLog);
            
            toLog<<"Completed frame: "<<++frameCount;
            SBE::log->debug(&toLog);

            if(!glfwWindowShouldClose(window))
            {
                glfwPollEvents();
            }

            return a;
        }, std::chrono::microseconds(13000));
        recurSum.recur(0);

        std::this_thread::sleep_for(std::chrono::seconds(5));  
        recurSum.suppress();
        recurSum.end();

        glfwDestroyWindow(window);
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