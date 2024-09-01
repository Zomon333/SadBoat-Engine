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
            SBE::log->error(&toLog); });

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        int displayXPos, displayYPos;
        int displayX, displayY;
        
        if(monitor)
        {    
            glfwGetMonitorWorkarea(monitor, &displayXPos, &displayYPos, &displayX, &displayY);
        }
        else
        {
            displayX = 640;
            displayY = 480;
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
            SBE::log->info(&toLog);
        }

        commandPools = new CommandPoolManager(logicalDevice);

        setupRenderpass();
        
        SBE::Swapchain testSwapchain = Swapchain(
            preferredDevice,
            logicalDevice,
            queues,
            surface,
            &renderPass
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
        

        
        std::tuple<uint32_t, VkImage, VkImageView, VkFramebuffer> frameData = testSwapchain.getFramedata();

        // This NEEDS to be getSwapchainIndex() in the future.
        // getSwapchainIndex segfaults, though. FIX THIS.
        uint32_t index = std::get<0>(frameData);
        
        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass;
        renderPassInfo.framebuffer = std::get<3>(frameData);
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = testSwapchain.getExtent();


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
        submitInfo.pWaitSemaphores = testSwapchain.getSemaphore();
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.signalSemaphoreCount = 1;

        VkSemaphoreCreateInfo semInfo = {VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,nullptr,0};
        VkSemaphore renderFinished;
        toLog<<"Attempting to create new renderFinished semaphore: "<<SBE::VkResultLookup(vkCreateSemaphore(logicalDevice->getSelf(), &semInfo, nullptr, &renderFinished));
        SBE::log->debug(&toLog);

        submitInfo.pSignalSemaphores = testSwapchain.getSemaphore();

        toLog<<"Attempting to submit queue: "<<SBE::VkResultLookup(vkQueueSubmit(queues->getQueue(0)->getSelf(), 1, &submitInfo, VK_NULL_HANDLE));
        SBE::log->debug(&toLog);

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = testSwapchain.getSemaphore();
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = testSwapchain.getSwapchain();
        presentInfo.pImageIndices = &index;

        toLog<<"Attempting to present work: "<<SBE::VkResultLookup(vkQueuePresentKHR(queues->getQueue(0)->getSelf(), &presentInfo));
        SBE::log->debug(&toLog);

    }

    void VulkanEnvironment::test()
    {
        // THIS IS A TEST

        std::stringstream toLog;
        VkCommandBufferBeginInfo beginInfo = {
            VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
            nullptr,
            0,
            nullptr
        };

        VkCommandBuffer* commandBuffer = commandPools->getPools()[0]->getBuffers()[0]->getSelf();
        toLog<<"Attempting to begin commandBuffer: "<<SBE::VkResultLookup(vkBeginCommandBuffer(*commandBuffer, &beginInfo));
        SBE::log->debug(&toLog);
        

        
        // This NEEDS to be getSwapchainIndex() in the future.
        // getSwapchainIndex segfaults, though. FIX THIS.
        uint32_t index = getSwapchainIndex();
        if (index >= swapchainFramebuffers.size())
        {
            SBE::log->error("Invalid swapchain framebuffer index");
            abort();
        }

        if(!renderPass || !swapchainFramebuffers[index])
        {
            SBE::log->error("Invalid render pass or framebuffer");
        }

        if (!commandBuffer || !*commandBuffer) {
            // Handle invalid command buffer
            SBE::log->error("Invalid command buffer.");
            return;
        }


        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass;
        renderPassInfo.framebuffer = this->swapchainFramebuffers[index];
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = swapchainInfo.imageExtent;



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
        submitInfo.pWaitSemaphores = &imageAvailableSemaphore;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.signalSemaphoreCount = 1;

        VkSemaphoreCreateInfo semInfo = {VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,nullptr,0};
        VkSemaphore renderFinished;
        toLog<<"Attempting to create new renderFinished semaphore: "<<SBE::VkResultLookup(vkCreateSemaphore(logicalDevice->getSelf(), &semInfo, nullptr, &renderFinished));
        SBE::log->debug(&toLog);

        submitInfo.pSignalSemaphores = &imageAvailableSemaphore;

        toLog<<"Attempting to submit queue: "<<SBE::VkResultLookup(vkQueueSubmit(queues->getQueue(0)->getSelf(), 1, &submitInfo, VK_NULL_HANDLE));
        SBE::log->debug(&toLog);

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = &imageAvailableSemaphore;
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = &swapchain;
        presentInfo.pImageIndices = &index;

        toLog<<"Attempting to present work: "<<SBE::VkResultLookup(vkQueuePresentKHR(queues->getQueue(0)->getSelf(), &presentInfo));
        SBE::log->debug(&toLog);

    }

    void VulkanEnvironment::setupSwapchain()
    {
        std::stringstream toLog;

        VkSurfaceCapabilitiesKHR surfaceCapabilities;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(preferredDevice->getDevice(), surface, &surfaceCapabilities);

        uint32_t formatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(preferredDevice->getDevice(), surface, &formatCount, nullptr);
        std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(preferredDevice->getDevice(), surface, &formatCount, surfaceFormats.data());

        uint32_t presentModeCount = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(preferredDevice->getDevice(), surface, &presentModeCount, nullptr);
        std::vector<VkPresentModeKHR> presentModes(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(preferredDevice->getDevice(), surface, &presentModeCount, presentModes.data());

        if(surfaceFormats.size() == 1 && surfaceFormats[0].format == VK_FORMAT_UNDEFINED)
        {
            // swapchainFormat = { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
            swapchainFormat.format = VK_FORMAT_B8G8R8A8_UNORM;
            swapchainFormat.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        } 
        else
        {
            bool found = false;
            for (const auto& format : surfaceFormats)
            {
                if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) 
                {
                    swapchainFormat = format;
                    found = true;
                    break;
                }
            }
            if(!found)
            {
                // 
                abort();
                // swapchainFormat = surfaceFormats[0];
            }
        }

        VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;
        for (const auto& mode : presentModes) {
            if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
                swapchainPresentMode = mode;
                break;
            }
        }

        VkExtent2D swapchainExtent = surfaceCapabilities.currentExtent;
        if (surfaceCapabilities.currentExtent.width == UINT32_MAX) 
        {
            swapchainExtent.width = std::max(surfaceCapabilities.minImageExtent.width,
                                            std::min(surfaceCapabilities.maxImageExtent.width, swapchainExtent.width));
            swapchainExtent.height = std::max(surfaceCapabilities.minImageExtent.height,
                                            std::min(surfaceCapabilities.maxImageExtent.height, swapchainExtent.height));
        }

        swapchainInfo = {};
        swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        swapchainInfo.surface = surface;
        swapchainInfo.minImageCount = surfaceCapabilities.minImageCount + 1;
        swapchainInfo.imageFormat = swapchainFormat.format;
        swapchainInfo.imageColorSpace = swapchainFormat.colorSpace;
        swapchainInfo.imageExtent = swapchainExtent;
        swapchainInfo.imageArrayLayers = 1;
        swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        // This should practically always be true, I think?
        swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

        uint32_t queueFamilyIndices[] = {(queues->getFamily()->getIndex())};

        swapchainInfo.queueFamilyIndexCount = 1;
        swapchainInfo.pQueueFamilyIndices = queueFamilyIndices;

        swapchainInfo.preTransform = surfaceCapabilities.currentTransform;
        swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        swapchainInfo.presentMode = swapchainPresentMode;
        swapchainInfo.clipped = VK_TRUE;
        swapchainInfo.oldSwapchain = VK_NULL_HANDLE;

        VkResult swapchainCreationResult = vkCreateSwapchainKHR(logicalDevice->getSelf(), &swapchainInfo, nullptr, &swapchain);

        if(swapchainCreationResult)
        {
            toLog<<"Failed to create swapchain! Error: "<<SBE::VkResultLookup(swapchainCreationResult);
            SBE::log->critical(&toLog);
        }
        else
        {
            toLog<<"Created swapchain with requested attributes. ";
            SBE::log->debug(&toLog);
        }

        uint32_t imageCount = 0;
        toLog<< "Attempting to get swapchain image count: "<<SBE::VkResultLookup(vkGetSwapchainImagesKHR(logicalDevice->getSelf(), swapchain, &imageCount, nullptr));
        SBE::log->debug(&toLog);
        toLog<<"Swapchain image count: "<<imageCount;
        SBE::log->info(&toLog);

        swapchainImages.resize(imageCount);
        toLog<< "Attempting to get swapchain images: "<<SBE::VkResultLookup(vkGetSwapchainImagesKHR(logicalDevice->getSelf(), swapchain, &imageCount, swapchainImages.data()));
        SBE::log->debug(&toLog);


        swapchainImageViews.resize(imageCount);
        for(unsigned int i = 0; i < imageCount; i++)
        {
            // &(swapchainImages[i])
            VkImageViewCreateInfo viewCreateInfo = {};
            viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewCreateInfo.image = swapchainImages[i];
            viewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            viewCreateInfo.format = swapchainFormat.format;
            viewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            viewCreateInfo.subresourceRange.baseMipLevel = 0;
            viewCreateInfo.subresourceRange.levelCount = 1;
            viewCreateInfo.subresourceRange.baseArrayLayer = 0;
            viewCreateInfo.subresourceRange.layerCount = 1;

            toLog<<"Attempting to create VkImageView for free swapchain image with index of "<<i<<". Result: "<<SBE::VkResultLookup(vkCreateImageView(logicalDevice->getSelf(), &viewCreateInfo, nullptr, &swapchainImageViews.data()[i]));
            SBE::log->debug(&toLog);
        }

        swapchainFramebuffers.resize(imageCount);
        for(unsigned int i = 0; i < imageCount; i++)
        {
            VkFramebufferCreateInfo framebufferCreateInfo = {};
            framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferCreateInfo.renderPass = renderPass;
            framebufferCreateInfo.attachmentCount = 1;
            framebufferCreateInfo.pAttachments = &swapchainImageViews.data()[i];
            framebufferCreateInfo.width = swapchainExtent.width;
            framebufferCreateInfo.height = swapchainExtent.height;
            framebufferCreateInfo.layers = 1;

            toLog<<"Attempting to create VkFramebuffer for swapchainImageView number "<<i<<": "<<SBE::VkResultLookup(vkCreateFramebuffer(logicalDevice->getSelf(), &framebufferCreateInfo, nullptr, &swapchainFramebuffers.data()[i]));
            SBE::log->debug(&toLog);
        }

        // Record commands to command buffers
        // Submit command buffers
        // Present the image

        // We'll also need to handle swapchain recreation.
    }

    void VulkanEnvironment::setupRenderpass()
    {
        std::stringstream toLog;
        // ToDo: Add renderpass creation.
        VkAttachmentDescription colorAttachment = {};
        colorAttachment.format = VK_FORMAT_B8G8R8A8_UNORM; // Example format
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; // Clear attachment at the start
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE; // Store result in memory
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE; // No stencil buffer
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED; // Layout before render pass
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // Layout after render pass

        VkAttachmentReference colorAttachmentRef = {};
        colorAttachmentRef.attachment = 0; // Index of the attachment in the array
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // Layout used in this subpass

        VkSubpassDescription subpass = {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS; // Pipeline type
        subpass.colorAttachmentCount = 1; // Number of color attachments
        subpass.pColorAttachments = &colorAttachmentRef; // Array of color attachments


        VkSubpassDependency dependency = {};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL; // Synchronization dependency with external commands
        dependency.dstSubpass = 0; // Subpass index
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT; // Stages at which operations are performed
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT; // Access types needed for the source stage
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT; // Access types needed for the destination stage

        VkAttachmentDescription attachments[] = { colorAttachment };

        VkRenderPassCreateInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1; // Number of attachments
        renderPassInfo.pAttachments = attachments;
        renderPassInfo.subpassCount = 1; // Number of subpasses
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1; // Number of dependencies
        renderPassInfo.pDependencies = &dependency;

        toLog<<"Attempting renderpass creation: "<<SBE::VkResultLookup(vkCreateRenderPass(logicalDevice->getSelf(), &renderPassInfo, nullptr, &renderPass));
        SBE::log->debug(&toLog);

    }

    uint32_t VulkanEnvironment::getSwapchainIndex()
    {
        std::stringstream toLog;
        
        uint64_t semValue = 0;
        // imageAvailableSemaphore++;
        toLog<<"vkGetSemaphoreCounterValue returns with: "<<SBE::VkResultLookup(vkGetSemaphoreCounterValue(logicalDevice->getSelf(), imageAvailableSemaphore, &semValue));
        toLog<<"\t Returned Value: "<<semValue;
        SBE::log->debug(&toLog);


        uint32_t imageIndex = 0;
        toLog<< SBE::VkResultLookup(vkAcquireNextImageKHR(logicalDevice->getSelf(), swapchain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex))<<" on acquisition of swapchain image.";
        toLog<<"\tReturned imageIndex: "<<imageIndex;
        SBE::log->info(&toLog);

        return imageIndex;
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