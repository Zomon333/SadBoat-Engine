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

#include <vector>

#include "vulkan.hpp"

#include "utilities/logging/startup_logger.hpp"

#include "vulkan/swapchain.hpp"
#include "vulkan/instance.hpp"
#include "vulkan/extension_collection.hpp"
#include "vulkan/layer_collection.hpp"
#include "vulkan/physical_device.hpp"
#include "vulkan/physical_device_collection.hpp"
#include "vulkan/logical_device.hpp"
#include "vulkan/queue_collection.hpp"
#include "vulkan/queue.hpp"
#include "vulkan/command_pool_manager.hpp"
#include "vulkan/vulkan_result_lookup.hpp"

namespace SBE
{
    Swapchain::Swapchain(SBE::PhysicalDevice* physicalDevice, SBE::LogicalDevice* logicalDevice, QueueCollection* queues, VkSurfaceKHR surface, VkRenderPass* renderPass)
    {
        // Update the class's internal data
        this->parent = physicalDevice;
        this->logicalDevice = logicalDevice;
        this->queues = queues;
        this->surface = &surface;
        this->renderPass = renderPass;

        // Declared for logging convenience
        std::stringstream toLog;
        std::stringstream* msg = &toLog;

        VkSemaphoreCreateInfo semInfo = {
            VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
            nullptr,
            0
        };

        toLog<<"Creating new semaphore for swapchain. Result: ";
        toLog<<SBE::VkResultLookup(vkCreateSemaphore(
            logicalDevice->getSelf(),
            &semInfo,
            nullptr,
            &imageAvailability
        ));
        SBE::log->debug(msg);

        // Get the capabilities of our presentation surface
        VkSurfaceCapabilitiesKHR surfaceCapabilities;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice->getDevice(), surface, &surfaceCapabilities);

        // Get the number of available formats for the surface
        uint32_t formatCount = 0;
        toLog<<"Getting format count for surface formats supported by physical device. Result: ";toLog<<SBE::VkResultLookup(
            vkGetPhysicalDeviceSurfaceFormatsKHR(
                physicalDevice->getDevice(), 
                surface, 
                &formatCount, 
                nullptr
            ));
        toLog<<"\tCount: "<<formatCount;
        SBE::log->debug(msg);

        // Get the array of available formats for the surface
        std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
        toLog<<"Getting formats supported by physical device for surface. Result: ";
        toLog<<SBE::VkResultLookup(
            vkGetPhysicalDeviceSurfaceFormatsKHR(
                physicalDevice->getDevice(), 
                surface, 
                &formatCount, 
                surfaceFormats.data()
            ));
        SBE::log->debug(msg);

        // Get the number of presentation modes available for the surface
        uint32_t presentModeCount = 0;
        toLog<<"Getting presentation mode count for surface. Result: ";
        toLog<<SBE::VkResultLookup(
            vkGetPhysicalDeviceSurfacePresentModesKHR(
                physicalDevice->getDevice(), 
                surface, 
                &presentModeCount, 
                nullptr
            ));
        SBE::log->debug(msg);

        // Get the array of available presentation modes for the surface
        std::vector<VkPresentModeKHR> presentModes(presentModeCount);
        toLog<<"Getting presentation modes supported by surface. Result: ";
        toLog<<SBE::VkResultLookup(
            vkGetPhysicalDeviceSurfacePresentModesKHR(
                physicalDevice->getDevice(), 
                surface, 
                &presentModeCount, 
                presentModes.data()
            ));
        SBE::log->debug(msg);

        // Verify valid surface format exists for swapchain
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
                SBE::log->critical("Unable to find compatible surface format.");
                abort();
            }
        }

        // The default presentation mode is First In First Out
        this->swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;
        // Unless mailbox presentation mode is supported
        for (const auto& mode : presentModes) {
            if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
                this->swapchainPresentMode = mode;
                break;
            }
        }

        // Get the extent of the swapchain
        // IF THE RENDERABLE AREA CHANGES, THE EXTENT WILL TOO!
        // WE WILL NEED TO RECREATE THE SWAPCHAIN WITH A NEW EXTENT IF THAT OCCURS!
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

        // IF WE'RE RESIZING, I'M PRETTY SURE WE'LL WANT THIS TO BE DIFFERENT
        swapchainInfo.oldSwapchain = VK_NULL_HANDLE;

        // Create the swapchain
        toLog<<"Attempting to create swapchain. Result: ";
        toLog<<SBE::VkResultLookup(vkCreateSwapchainKHR(logicalDevice->getSelf(), &swapchainInfo, nullptr, &swapchain));
        SBE::log->debug(msg);

        // Query the number of images we should have in the swapchain
        uint32_t imageCount = 0;
        toLog<< "Attempting to get swapchain image count: ";
        toLog<<SBE::VkResultLookup(
            vkGetSwapchainImagesKHR(
                logicalDevice->getSelf(), 
                swapchain, 
                &imageCount, 
                nullptr
            ));
        toLog<<"\tSwapchain image count: "<<imageCount;
        SBE::log->debug(msg);

        // Resize the swapchainImages array to match the queried size
        // And then attempt to get the actual swapchain images
        swapchainImages.resize(imageCount);
        toLog<< "Attempting to get swapchain images: ";
        toLog<<SBE::VkResultLookup(
            vkGetSwapchainImagesKHR(
                logicalDevice->getSelf(),
                swapchain,
                &imageCount,
                swapchainImages.data()
            ));
        SBE::log->debug(&toLog);

        // Now that we have the images, we need to get the VkImageViews
        // So resize the image view array...
        // And then, per each image, create a view.
        swapchainImageViews.resize(imageCount);
        for(unsigned int i = 0; i < imageCount; i++)
        {
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

            toLog<<"Creating VkImageView for swapchain image #"<<i<<". Result: ";toLog<<SBE::VkResultLookup(
                vkCreateImageView(
                    logicalDevice->getSelf(), 
                    &viewCreateInfo, 
                    nullptr, 
                    &swapchainImageViews.data()[i]
                ));
            SBE::log->debug(msg);
        }

        // Do the same for framebuffers now
        swapchainFramebuffers.resize(imageCount);
        for(unsigned int i = 0; i < imageCount; i++)
        {
            VkFramebufferCreateInfo framebufferCreateInfo = {};
            framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferCreateInfo.renderPass = *renderPass;
            framebufferCreateInfo.attachmentCount = 1;
            framebufferCreateInfo.pAttachments = &swapchainImageViews.data()[i];
            framebufferCreateInfo.width = swapchainExtent.width;
            framebufferCreateInfo.height = swapchainExtent.height;
            framebufferCreateInfo.layers = 1;

            toLog<<"Creating VkFramebuffer for swapchain image #"<<i<<": ";
            toLog<<SBE::VkResultLookup(
                vkCreateFramebuffer(
                    logicalDevice->getSelf(),
                    &framebufferCreateInfo,
                    nullptr,
                    &swapchainFramebuffers.data()[i]
                ));
            SBE::log->debug(msg);
        }
    }

    VkSwapchainKHR* Swapchain::getSwapchain()
    {
        return &swapchain;
    }

    VkExtent2D Swapchain::getExtent()
    {
        return swapchainInfo.imageExtent;
    }

    VkSemaphore* Swapchain::getSemaphore()
    {
        return &imageAvailability;
    }


    VkPresentModeKHR Swapchain::getPresentMode()
    {
        return swapchainPresentMode;
    }

    std::tuple<uint32_t, VkImage, VkImageView, VkFramebuffer> Swapchain::getFramedata()
    {
        std::stringstream toLog;
        
        // Get the index for the available frame data.
        uint32_t imageIndex = 0;
        toLog<< "Querying index of next free framedata set. Result: ";
        toLog<<SBE::VkResultLookup(
            vkAcquireNextImageKHR(
                logicalDevice->getSelf(), 
                swapchain, 
                UINT64_MAX, 
                imageAvailability, 
                VK_NULL_HANDLE, 
                &imageIndex
            ));
        toLog<<"\tReturned index: "<<imageIndex;
        SBE::log->debug(&toLog);

        // Return all the information we could possibly want for the actual rendering
        return std::tuple<uint32_t, VkImage, VkImageView, VkFramebuffer>
        (
            imageIndex, 
            swapchainImages[imageIndex], 
            swapchainImageViews[imageIndex], 
            swapchainFramebuffers[imageIndex]
        );
    }

    void Swapchain::setPresentMode(VkPresentModeKHR newPresentMode)
    {
        SBE::log->critical("NEED TO IMPLEMENT: Swapchain::setPresentMode");
    }

    void Swapchain::recreate()
    {
        SBE::log->critical("NEED TO IMPLEMENT: Swapchain::recreate");
    }
};