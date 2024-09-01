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
#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H

#include <vector>

#include "vulkan.hpp"

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
    class Swapchain
    {
        private:
                    VkSwapchainKHR swapchain;
                    VkPresentModeKHR swapchainPresentMode;
                    VkSwapchainCreateInfoKHR swapchainInfo;
                    VkSurfaceFormatKHR swapchainFormat;

                    VkSemaphore imageAvailability;
                    std::vector<VkImage> swapchainImages;
                    std::vector<VkImageView> swapchainImageViews;
                    std::vector<VkFramebuffer> swapchainFramebuffers;
        
                    SBE::PhysicalDevice* parent;

                    SBE::LogicalDevice* logicalDevice;

                    SBE::QueueCollection* queues;
                    VkSurfaceKHR* surface;
                    VkRenderPass* renderPass;

        public:
            Swapchain(SBE::PhysicalDevice* physicalDevice, SBE::LogicalDevice* logicalDevice, QueueCollection* queues, VkSurfaceKHR surface, VkRenderPass* renderPass);

            VkExtent2D getExtent();
            VkPresentModeKHR getPresentMode();

            VkSwapchainKHR* getSwapchain();
            VkSemaphore* getSemaphore();
            std::tuple<uint32_t, VkImage, VkImageView, VkFramebuffer> getFramedata();

            void setPresentMode(VkPresentModeKHR newPresentMode);

            void recreate();
    };
};
#endif