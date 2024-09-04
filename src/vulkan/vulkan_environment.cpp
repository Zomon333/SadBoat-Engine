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
#include <signal.h>
#include <pthread.h>
#include <thread>

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
                SBE::log->debug(std::string("Enabling extension: ").append(extensions->getProp(i).extensionName));
                toReturn.push_back(extensions->getProp(i));
            }
            else
            {
                std::stringstream toLog;
                toLog << "Not enabling optional extension: " << extensions->getProp(i).extensionName;
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

    //----------------------------------
    // Default Constructor for VulkanEnvironment:
    // Sets up the essential components for rendering in Vulkan
    //----------------------------------
    VulkanEnvironment::VulkanEnvironment(std::string gameName, Config *deviceConfig)
    {
        // Generic stringstream to make logging easier
        std::stringstream toLog;
        std::stringstream *msg = &toLog;

        // Check if the windowing environment has been initialized before.
        //
        // Due to GLFW restrictions, we're only able to assign a single set of handlers
        // for what could possibly be several windows and contexts. This restriction
        // is not something that Vulkan itself has-- which is problematic, because now
        // we need to write a wrapper to force Vulkan to comply with GLFW's single-context
        // mindset.
        if (SBE::windowEnvironment != nullptr)
        {
            SBE::log->warn("Vulkan environment has already been initialized.");
            throw std::runtime_error("VulkanEnvironment already initialized.");
        }
        else
        {
            SBE::log->debug("Beginning first time startup of Vulkan Environment.");
        }

        // Config loading
        //----------------------------------
        // * Check if a config was provided to the constructor
        //      * If we don't have a config, load the config from the default path.
        // * If a config is present, just use that one
        if (!deviceConfig)
        {
            SBE::ConfigManager configs;
            this->deviceConfig = configs.getConfig("./assets/config/graphicsOptions.xml");
        }
        else
        {
            this->deviceConfig = deviceConfig;
        }

        // Vulkan Instance Extension management
        //----------------------------------
        // Query the NUMBER of extensions available to the vulkan instance
        uint32_t extCount;
        toLog << "Querying number of available instance extensions. Result: ";
        toLog << SBE::VkResultLookup(vkEnumerateInstanceExtensionProperties(
            NULL,
            &extCount,
            nullptr));
        toLog << "\t Quantity: " << extCount;
        SBE::log->debug(msg);

        // Query WHICH extensions are available to the vulkan instance
        std::vector<VkExtensionProperties> pProperties = std::vector<VkExtensionProperties>(extCount);
        toLog << "Querying list of available instance extensions. Result: ";
        toLog << SBE::VkResultLookup(vkEnumerateInstanceExtensionProperties(
            NULL,
            &extCount,
            pProperties.data()));
        SBE::log->debug(msg);

        // Sort through the vulkan instance extensions and enable them all
        std::vector<VkExtensionProperties> toEnable;
        std::unordered_map<char *, bool> instanceExtensions(false);

        SBE::log->disableFlag(DEBUG);
        for (unsigned int i = 0; i < extCount; i++)
        {
            toEnable.push_back(pProperties[i]);

            toLog << "Enabling instance extension: " << pProperties[i].extensionName;
            SBE::log->debug(&toLog);

            instanceExtensions[pProperties[i].extensionName] = true;
        }
        SBE::log->enableFlag(DEBUG);

        // Vulkan Initialization
        //----------------------------------
        // * Create a new Instance, enabling the extensions we just queried
        // * Create a new PhysicalDeviceCollection, using the instance
        // * Get the preferred PhysicalDevice from the PhysicalDeviceCollection
        instance = new Instance(toEnable);
        devices = new PhysicalDeviceCollection(this->instance);
        preferredDevice = devices->getOptimal(this->deviceConfig);

        // Windowing Environment Setup
        //----------------------------------
        // * We're using GLFW for our windowing. Set up the essential GLFW systems.
        // * Set up error callbacks
        // * Tell GLFW we're not using OpenGL
        // * Query the window size, create the window
        //      * Get the surface from the created window

        // Initialize the GLFW subsystem
        if (!glfwInit())
        {
            // If the GLFW subsystem doesn't initialize then we can't create our window, and
            // essentially can't run the engine. So, abort if that happens!
            SBE::log->critical("Unable to initialize GLFW. Quitting program.");
            abort();
        }

        // Any other non-critical GLFW error should be logged.
        glfwSetErrorCallback([](int error, const char *description)
                             {
            std::stringstream toLog;
            toLog<<"Error: "<<description;
            SBE::log->error(&toLog); });

        // Tell GLFW we're not going to use OpenGL and we'll be manually presenting to the surface
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        // Get the currently active monitor from GLFW.
        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        int displayXPos, displayYPos;
        int displayX, displayY;

        // Resolution Determination:
        //  * Check if a config file was provided to the VulkanEnvironment
        //      * Try and get the resolution from the config file
        //          * If failure, try and get resolution from monitor
        //              * If failure, assume resolution
        //  * If no config file was provided:
        //      * Try and get resolution from monitor
        //          * If failure, assume resolution
        if (deviceConfig)
        {
            try
            {
                // Default case: A config file was provided, and contains the information we need.
                // Query the config file for the resolution
                SBE::log->info("Getting screen resolution from config file.");
                std::string resX = (*deviceConfig)["GraphicsOptions"][0]["Resolution"][0]["X"][0].getContents<std::string>();
                std::string resY = (*deviceConfig)["GraphicsOptions"][0]["Resolution"][0]["Y"][0].getContents<std::string>();

                displayX = std::stoi(resX);
                displayY = std::stoi(resY);
            }
            catch (const std::exception &e)
            {
                try
                {
                    // Fallback #1: Cannot read config file.
                    // Query the monitor for the resolution
                    SBE::log->error("Failed to read config file. Getting screen resolution from display.");
                    glfwGetMonitorWorkarea(monitor, &displayXPos, &displayYPos, &displayX, &displayY);
                    std::cerr << e.what() << '\n';
                }
                catch (const std::exception &e2)
                {
                    // Fallback #2: Cannot query monitor for resolution.
                    // Assume resolution.
                    SBE::log->error("Failed to get screen resolution from display. Assuming 1920x1080.");
                    std::cerr << e2.what() << '\n';
                    displayX = 1920;
                    displayY = 1080;
                }
            }
        }
        else
        {
            if (monitor)
            {
                try
                {
                    // Unconfigured case, Fallback #1: Config not provided.
                    // Query the monitor for the resolution.
                    SBE::log->info("Getting screen resolution from display.");
                    glfwGetMonitorWorkarea(monitor, &displayXPos, &displayYPos, &displayX, &displayY);
                }
                catch (const std::exception &e)
                {
                    // Fallback #2: Config not provided. Cannot query monitor for resolution.
                    // Assume resolution.
                    SBE::log->error("Failed to get screen resolution from display. Assuming 1920x1080.");
                    std::cerr << e.what() << '\n';
                    displayX = 1920;
                    displayY = 1080;
                }
            }
            else
            {
                // Fallback #2: Config file not provided. Monitor not queryable.
                // Assume resolution.
                SBE::log->warn("Unable to read screen resolution from display. Assuming 1920x1080.");
                displayX = 1920;
                displayY = 1080;
            }
        }

        // Window creation:
        //  * Attempt to create the window with the name of the game, and the resolution determined earlier
        //      * Abort on failure
        //  * Once window is created, handle surface creation
        //  * Set window resize handler so that we can keep our swapchain set up correctly
        window = glfwCreateWindow(displayX, displayY, gameName.c_str(), nullptr, nullptr);
        if (!window)
        {
            // If we don't have a window, then we cannot launch the engine properly
            // So, log the error, and abort.
            SBE::log->critical("Unable to initialize GLFW Window. Quitting program.");
            abort();
        }
        else
        {
            // But if we do have a window, we're fine to log its creation and continue.
            toLog << "Created GLFW window with dimensions " << displayX << " x " << displayY << ". ";
            SBE::log->debug(&toLog);
        }

        // Attempt to create new presentation surface for previously created window
        if (glfwCreateWindowSurface(*(instance->getInstance()), window, nullptr, &surface) != VK_SUCCESS)
        {
            // ERROR: Unable to get surface for window
            // Destroy the window, terminate the program

            // If we hit this point, there's no fallback to resolve the issue. The VkInstance simply cannot create a window.
            // There's no information we've provided that we could change to remedy this.
            // (Except maybe mandatory VkInstance Extensions, but that's writing a fallback case for those-- not here.)

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

        // When the window resizes, call the resize handler for this object.
        // This way, we can access private member data in a function that wouldn't
        // ordinarily be able to capture any. This lets us adjust things like swapchain
        // size in a context-dependent manner.
        glfwSetWindowSizeCallback(window,
                                  [](GLFWwindow *window, int width, int height)
                                  {
                                      SBE::windowEnvironment->handleResize(window, width, height);
                                  });

        // Device Extension & Layer Management
        //----------------------------------
        // * Set up ExtensionCollection & LayerCollection
        //      * Filter both to provide list of items to enable
        // * Set up new LogicalDevice
        //      * Provide minimum required features for device

        // Set up ExtensionCollection and LayerCollection
        extensions = new ExtensionCollection(this->preferredDevice);
        layers = new LayerCollection(this->preferredDevice);

        // Filter all the PhysicalDevice Extensions/Layers we want to enable
        enabledExtensions = filterExtensions();
        enabledLayers = filterLayers();

        // Figure out what we need for device capabilities
        requiredFeatures = new VkPhysicalDeviceFeatures;
        requiredFeatures->pipelineStatisticsQuery = VK_TRUE;
        requiredFeatures->multiViewport = VK_TRUE;
        requiredFeatures->multiDrawIndirect = VK_TRUE;
        requiredFeatures->tessellationShader = VK_TRUE;
        requiredFeatures->geometryShader = VK_TRUE;

        // Vulkan Rendering Setup
        //----------------------------------
        //  * Set up LogicalDevice from the preferred physical device, enabling layers, extensions, and features previously chosen.
        //      * Set up Queues from LogicalDevice
        //      * Set up CommandPool(s) from LogicalDevice
        //  * Set up Renderpass & Swapchain from LogicalDevice
        //      * Renderpass does not have an attached graphics pipeline yet (2024-09-04 01:04:35)
        //      * Swapchain does not support resizing yet (2024-09-04 01:04:37)
        //  * A single command buffer is chosen from the first command pool.
        //      * THIS IS FOR DEMONSTRATIVE PURPOSES ONLY. REWRITE THIS. (2024-09-04 01:05:45)
        //  * The rendering process then begins in earnest;
        //      * The frame data is requested from the swapchain
        //      * The renderpass info is created
        //      * The commands are recorded
        //      * The submission information is set up
        //  * An event is launched which synchronizes the submission of the command buffer to the
        //      queue and the presentation of the queue's work to the screen.
        //  * The engine waits infinitely, rendering sweet nothings, and performing no other tasks.
        //      * Fix this once rendering is in a MVP state. (2024-09-04 01:08:55)

        // Create LogicalDevice
        logicalDevice = new LogicalDevice(preferredDevice, requiredFeatures, enabledLayers, enabledExtensions);

        // Create Queues
        queues = new QueueCollection(logicalDevice, logicalDevice->getOptimalQueueFam(), logicalDevice->getQueueCount());

        // Create CommandBuffers
        commandPools = new CommandPoolManager(logicalDevice);

        // Create Renderpass (no attached pipeline yet)
        renderPass = new Renderpass(logicalDevice, nullptr);

        // Create swapchain (no resizing yet)
        swapchain = new Swapchain(preferredDevice, logicalDevice, queues, surface, renderPass->getRenderpass());

        VkCommandBufferBeginInfo beginInfo = {
            VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            nullptr,
            0,
            nullptr};

        // Get a command buffer for recording.
        // (Eventually, get a preferred command buffer instead of just a random one.)
        VkCommandBuffer *commandBuffer = commandPools->getPools()[0]->getBuffers()[0]->getSelf();
        toLog << "Attempting to begin commandBuffer: " << SBE::VkResultLookup(vkBeginCommandBuffer(*commandBuffer, &beginInfo));
        SBE::log->debug(&toLog);

        // Get the data we should present
        std::tuple<uint32_t, VkImage, VkImageView, VkFramebuffer> frameData = swapchain->getFramedata();
        uint32_t index = std::get<0>(frameData);

        // Format the initial renderpass
        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = *(renderPass->getRenderpass());
        renderPassInfo.framebuffer = std::get<3>(frameData);
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapchain->getExtent();

        VkClearValue clearColor = {{0.0f, 0.0f, 0.0f, 1.0f}};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        // Record the renderpass
        vkCmdBeginRenderPass(*commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        SBE::log->debug("Beginning command recording.");
            // Record draw commands here
            // malloc(1);
            // vkCmdDraw(*commandBuffer, 0, 0, 0, 0);

        SBE::log->debug("Ending command recording.");
        vkCmdEndRenderPass(*commandBuffer);

        toLog << "Attempting to end command buffer recording: " << SBE::VkResultLookup(vkEndCommandBuffer(*commandBuffer));
        SBE::log->debug(&toLog);

        VkPipelineStageFlags waitStages[] = {
            VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

        // Set up the submission info
        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = commandBuffer;
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = swapchain->getSemaphore();
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.signalSemaphoreCount = 1;

        // Synchronize our rendering
        VkSemaphoreCreateInfo semInfo = {VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO, nullptr, 0};
        VkSemaphore renderFinished;
        toLog << "Attempting to create new renderFinished semaphore: " << SBE::VkResultLookup(vkCreateSemaphore(logicalDevice->getSelf(), &semInfo, nullptr, &renderFinished));
        SBE::log->debug(&toLog);

        submitInfo.pSignalSemaphores = swapchain->getSemaphore();

        // Launch the rendering thread
        int frameCount = 0;
        auto renderLog = SBE::logger->allocateHandle(0b11111);
        renderLoop = new SBE::RecurringEvent<int>([this, &renderLog, &frameCount, index, submitInfo](int a)
                                                  {
            if(swapchain->getSwapchain() == VK_NULL_HANDLE)
            {
                return a;
            }
            if(this->getResizing()->try_lock())
            {      
                std::stringstream toLog;
                SBE::Queue* freeQueue = this->queues->getFree();

                VkResult result = vkQueueSubmit(freeQueue->getSelf(), 1, &submitInfo, *(freeQueue->getFence()));
                toLog<<"Attempting to submit queue #"<<freeQueue->getQueueNumber()<<": "<<SBE::VkResultLookup(result);
                renderLog->debug(&toLog);

                VkPresentInfoKHR presentInfo = {};
                presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
                presentInfo.waitSemaphoreCount = 1;
                presentInfo.pWaitSemaphores = swapchain->getSemaphore();
                presentInfo.swapchainCount = 1;
                presentInfo.pSwapchains = swapchain->getSwapchain();
                presentInfo.pImageIndices = &index;

                result = vkQueuePresentKHR(freeQueue->getSelf(), &presentInfo);
                

                toLog<<"Attempting to present work: "<<SBE::VkResultLookup(result);
                renderLog->debug(&toLog);

                if(result == VK_ERROR_OUT_OF_DATE_KHR)
                {
                    SBE::log->error("Swapchain is out of date and must be resized!");
                    std::cout<<""<<std::endl;
                    this->renderLoop->end();
                    return a;    
                }

                toLog<<"Completed frame: "<<++frameCount;
                renderLog->debug(&toLog);

                if(!glfwWindowShouldClose(window))
                {
                    glfwPollEvents();
                }

                // This is typically bad practice. We need to fix our fences in getFree to avoid using this.
                // vkQueueWaitIdle(freeQueue->getSelf());
                // vkResetFences(this->getLogicalDevice()->getSelf(), 1, freeQueue->getFence());
                this->getResizing()->unlock();
                return a;
            }
            else
            {
                this->getResizing()->lock();
                this->getResizing()->unlock();
                return a;
            } }, std::chrono::microseconds(13000));
        renderLoop->recur(0);

        // Wait forever
        while (true);
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

    std::mutex *VulkanEnvironment::getResizing()
    {
        return &(this->resizingWindow);
    }

    void VulkanEnvironment::handleResize(GLFWwindow *window, int width, int height)
    {
        // Implement this!
    }

};