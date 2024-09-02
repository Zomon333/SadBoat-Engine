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
#ifndef RENDERPASS_H
#define RENDERPASS_H

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
    class Renderpass
    {
        private:
            VkRenderPass renderPass;
            
        public:
            Renderpass(SBE::LogicalDevice* logicalDevice, VkRenderPassCreateInfo* creationInfo = nullptr);
            Renderpass(SBE::LogicalDevice* logicalDevice, std::vector<VkAttachmentDescription> attachments, std::vector<VkSubpassDescription> subpasses, std::vector<VkSubpassDependency> dependencies);

            VkRenderPass* getRenderpass();
    };
};
#endif