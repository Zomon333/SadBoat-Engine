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

#include "vulkan/renderpass.hpp"
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
    Renderpass::Renderpass(SBE::LogicalDevice* logicalDevice, VkRenderPassCreateInfo* creationInfo)
    {
        SBE::log->debug("Beginning renderpass creation.");
        if(creationInfo == nullptr)
        {
            SBE::log->debug("Using default renderpass parameters.");

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
            
            
            std::stringstream toLog;
            toLog<<"Attempting renderpass creation: "<<SBE::VkResultLookup(vkCreateRenderPass(logicalDevice->getSelf(), &renderPassInfo, nullptr, &renderPass));
                    SBE::log->debug(&toLog);
        }
        else
        {
            std::stringstream toLog;
            toLog<<"Attempting renderpass creation: "<<SBE::VkResultLookup(vkCreateRenderPass(logicalDevice->getSelf(), creationInfo, nullptr, &renderPass));
                    SBE::log->debug(&toLog);
        }
        
    }

    Renderpass::Renderpass(SBE::LogicalDevice* logicalDevice, std::vector<VkAttachmentDescription> attachments, std::vector<VkSubpassDescription> subpasses, std::vector<VkSubpassDependency> dependencies)
    {
        VkRenderPassCreateInfo creationInfo = {};
        creationInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        creationInfo.attachmentCount = attachments.size(); // Number of attachments
        creationInfo.pAttachments = attachments.data();
        creationInfo.subpassCount = subpasses.size(); // Number of subpasses
        creationInfo.pSubpasses = subpasses.data();
        creationInfo.dependencyCount = dependencies.size(); // Number of dependencies
        creationInfo.pDependencies = dependencies.data();
        
        std::cout<<"";
        std::stringstream toLog;
        toLog<<"Attempting renderpass creation: "<<SBE::VkResultLookup(vkCreateRenderPass(logicalDevice->getSelf(), &creationInfo, nullptr, &renderPass));
                SBE::log->debug(&toLog);
        std::cout<<"";
    }

    VkRenderPass* Renderpass::getRenderpass()
    {
        std::cout<<"";
        return &renderPass;
    }

}